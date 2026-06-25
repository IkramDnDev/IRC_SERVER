#include "../includes/Bot.hpp"

const std::string Bot::wisdomQuotes[15] =
{
    "Talk is cheap. Show me the code. - Linus Torvalds",
    "Programs must be written for people to read. - Abelson & Sussman",
    "Simplicity is the soul of efficiency. - Austin Freeman",
    "Make it work, make it right, make it fast. - Kent Beck",
    "Before software can be reusable it first has to be usable. - Ralph Johnson",
    "Strive for excellence, not perfection. - H. Jackson Brown Jr.",
    "Knowledge is power. - Francis Bacon",
    "The mind is not a vessel to be filled, but a fire to be kindled. - Plutarch",
    "An investment in knowledge pays the best interest. - Benjamin Franklin",
    "Patience is bitter, but its fruit is sweet. - Jean-Jacques Rousseau",
    "The roots of education are bitter, but the fruit is sweet. - Aristotle",
    "Consistency is the true foundation of trust. - Unknown",
    "Code never lies, comments sometimes do. - Ron Jeffries",
    "Software is a great combination between artistry and engineering. - Bill Gates",
    "Action is the foundational key to all success. - Pablo Picasso"
};

Bot::Bot(std::string s_host, std::string s_port, std::string s_pass) 
    : _socketId(-1), _serverHost(s_host), _serverPort(s_port), _serverPass(s_pass), _isAuthDone(false)
{
    std::srand(std::time(0));
}

Bot::~Bot()
{
    if (_socketId != -1)
        close(_socketId);
}

void Bot::connectToServer()
{
    printBanner();

    struct addrinfo queryConfig;
    struct addrinfo *lookupResults;

    std::memset(&queryConfig, 0, sizeof(queryConfig));
    queryConfig.ai_family = AF_INET;       // Uniquement IPv4
    queryConfig.ai_socktype = SOCK_STREAM; // Protocole TCP

    if (getaddrinfo(_serverHost.c_str(), _serverPort.c_str(), &queryConfig, &lookupResults) != 0)
    {
        throw std::runtime_error("Fatal: Domain translation failed.");
    }

    _socketId = socket(lookupResults->ai_family, lookupResults->ai_socktype, lookupResults->ai_protocol);
    if (_socketId < 0)
    {
        freeaddrinfo(lookupResults);
        throw std::runtime_error("Fatal: Socket allocation error.");
    }

    if (connect(_socketId, lookupResults->ai_addr, lookupResults->ai_addrlen) < 0)
    {
        // Pas besoin de close(_socketId) deja 3ndra ~Bot()
        freeaddrinfo(lookupResults);
        throw std::runtime_error("Fatal: TCP connection handshake failed.");
    }

    freeaddrinfo(lookupResults);
    std::cout << ">> Target server reached successfully." << std::endl;
}

void Bot::sendHandshake()
{
    std::string message = "PASS " + _serverPass + "\r\n";
    if (send(_socketId, message.c_str(), message.size(), 0) < 0)
        throw std::runtime_error("Fatal: Failed to send PASS command.");

    message = "NICK Bot\r\n"; 
    if (send(_socketId, message.c_str(), message.size(), 0) < 0)
        throw std::runtime_error("Fatal: Failed to send NICK command.");

    message = "USER Bot 0 * :Bot\r\n";
    if (send(_socketId, message.c_str(), message.size(), 0) < 0)
        throw std::runtime_error("Fatal: Failed to send USER command.");
}

void Bot::verifyAuthStatus(const std::string &srvMsg)
{
    if (srvMsg.find("464") != std::string::npos)//aussi deja 3ndra ~Bot()
        throw std::runtime_error("Access Denied: Wrong IRC Password.");
    else if (srvMsg.find("001") != std::string::npos)
    {
       std::cout << ">> Authentication acknowledged by IRC server!" << std::endl;
       _isAuthDone = true;
    }
}

void Bot::execLoop()
{
    sendHandshake();

    char inputBuffer[1024];
    while (true)
    {
        int readBytes = recv(_socketId, inputBuffer, sizeof(inputBuffer) - 1, 0);
        if (readBytes <= 0)
        {
            std::cerr << ">> Communication closed by host." << std::endl;
            break;
        }
        inputBuffer[readBytes] = '\0';
        std::string serverStream(inputBuffer);

        if (!_isAuthDone)
            verifyAuthStatus(serverStream);
        if(_isAuthDone)//on pas faire else pour si 001 et PRIVMSG arrivent ensemble, on rate le PRIVMSG
            handleCommand(serverStream);
    }
}

std::string Bot::removeSpaces(const std::string &rawStr)
{
    size_t startPos = rawStr.find_first_not_of(" \t\r\n\0");
    if (startPos == std::string::npos) return "";
    size_t endPos = rawStr.find_last_not_of(" \t\r\n");
    return rawStr.substr(startPos, endPos - startPos + 1);
}

std::string Bot::parseInput(std::string srcMsg)
{
    size_t delimiterLocation = srcMsg.find(" ");
    if (delimiterLocation == std::string::npos) return "";
    srcMsg = srcMsg.substr(delimiterLocation + 1);
    
    size_t textMarker = srcMsg.find(" :");
    if (textMarker != std::string::npos)
        return srcMsg.substr(textMarker + 2);
    return srcMsg;
}

void Bot::printBanner(void)
{
    std::cout << BLUE;
    std::cout << "  ════════════════════════════════════════════════\n";
    std::cout << "\n";
    std::cout << CYAN;
    std::cout << "  ██╗██████╗  ██████╗    ██████╗  ██████╗ ████████╗\n";
    std::cout << "  ██║██╔══██╗██╔════╝    ██╔══██╗██╔═══██╗╚══██╔══╝\n";
    std::cout << "  ██║██████╔╝██║         ██████╔╝██║   ██║   ██║   \n";
    std::cout << "  ██║██╔══██╗██║         ██╔══██╗██║   ██║   ██║   \n";
    std::cout << "  ██║██║  ██║╚██████╗    ██████╔╝╚██████╔╝   ██║   \n";
    std::cout << "  ╚═╝╚═╝  ╚═╝ ╚═════╝    ╚═════╝  ╚═════╝    ╚═╝   \n";
    std::cout << RESET;
    std::cout << GRAY;
    std::cout << "\n  · · · · · · · · · · · · · · · · · · · · · · · ·\n";
    std::cout << GREEN;
    std::cout << "       🤖  Interactive C++ IRC Service  🤖\n";
    std::cout << GRAY;
    std::cout << "  · · · · · · · · · · · · · · · · · · · · · · · ·\n\n";
    std::cout << GRAY  << "  Target Host  " << WHITE << this->_serverHost  << "\n";
    std::cout << GRAY  << "  Target Port  " << WHITE << this->_serverPort  << "\n";
    std::cout << GRAY  << "  Status       " << GREEN << "CONNECTING"     << "\n";
    std::cout << RESET   << "\n";
    std::cout << BLUE  << "  ════════════════════════════════════════════════\n";
    std::cout << RESET;
}

void Bot::handleCommand(std::string fullMsg)
{
    if (fullMsg.find("PRIVMSG") == std::string::npos)
        return ;

    // FIX : prendre entre ':' et le premier espace
    std::string senderPseudo = "";
    if (fullMsg[0] == ':')
    {
        size_t spacePos = fullMsg.find(' ');
        if (spacePos != std::string::npos)
            senderPseudo = fullMsg.substr(1, spacePos - 1);// "mohcine"
    }

    std::string macroTrigger = parseInput(fullMsg);
    macroTrigger = removeSpaces(macroTrigger);

    std::cout << "sender: [" << senderPseudo << "] command: [" << macroTrigger << "]" << std::endl;

    if (senderPseudo.empty())
        return ;

    std::string res = "";

    if (macroTrigger == "!INFO")
    {
        res = "PRIVMSG " + senderPseudo + " :[System Bot] Commands available: !INFO, !STATUS, !WISDOM\r\n";
    }
    else if (macroTrigger == "!STATUS")
    {
        std::time_t currentTime = std::time(0); 
        std::tm *parts = std::localtime(&currentTime);
        
        std::stringstream ss;
        ss << "PRIVMSG " << senderPseudo << " :[Server Time] " 
           << parts->tm_hour << "h " 
           << parts->tm_min  << "m " 
           << parts->tm_sec  << "s\r\n";
        res = ss.str();
    }
    else if (macroTrigger == "!WISDOM")
    {
        int randomizedPick = std::rand() % 15;
        res = "PRIVMSG " + senderPseudo + " :" + wisdomQuotes[randomizedPick] + "\r\n";
    }
    else
        return ; // commande inconnue

    if (send(_socketId, res.c_str(), res.size(), 0) < 0)
    {
        std::cerr << ">> Error: send() failed for command: " << macroTrigger << std::endl;
        // close(_socketId);//aussi 3ndna ~Bot()
        throw std::runtime_error("Fatal: Connection lost during send.");
    }
}
