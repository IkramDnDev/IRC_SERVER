#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Message.hpp"
#include "../includes/Channel.hpp"

//////////

bool Server::signal_flag = false;
void Server::handle_signal(int signo)
{
    (void)signo;
    Server::signal_flag = true;
}

//////////

Server::Server() {}

Server::~Server() {} 

Server::Server(int port, std::string password)
{
    this->_port = port;
    this->_password = password;
}

int Server::getPort() const { return _port; }

const std::string& Server::getPassword() const { return _password; }

int Server::getServerSocket() const { return _serverSocket; }

std::map<std::string, Channel>& Server::getChannels()  { return this->_channels; }

std::map<int , Client> & Server::getAllClients() { return (_clients); }

//////////

void Server::closeSockets()
{
	for (size_t index = 0; index < _clients.size(); index++)
	{
		int clientFd = _clients[index].getSocket();

		if (clientFd != -1)
			close(clientFd);
	}

	if (_serverSocket != -1)
		close(_serverSocket);
}

void Client::envoyerMessage(const std::string &message)
{
	if (send(this->_fd, message.c_str(), message.size(), 0) < 0)
	{
		std::cerr << "Error sending message" << this->_fd << std::endl;
	}
}

void Server::removeClient(int index)
{
    std::map<int, Client> &clients = this->getAllClients();
    Client *client = NULL;
    for(std::map<int, Client>::iterator it = clients.begin();
        it != clients.end();
        it++)
    {
        if (it->first == index)
        {
            client = &it->second;
            clients.erase(it);
            break ;
        }
    }
    if (!client)
		return ;
    /*
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        !!   remove from all channels   !!
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    */
    std::map<std::string, Channel> &channels = this->getChannels();

    for (std::map<std::string, Channel>::iterator it = channels.begin();
         it != channels.end();
         ++it)
    {
        it->second.removeMember(client);
    }
}

void Server::disconnectClient(size_t index)
{
    int client_fd = _fds[index].fd;

    close(client_fd);

    _fds.erase(_fds.begin() + index);

    _clients.erase(client_fd);
}

//////////

void Server::receiveData(size_t index)
{
    char buffer[1024];
    int clientFd = _fds[index].fd;

    ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead < 0)
    {
        std::cerr << "● [ERROR] recv() failed for fd " << clientFd << std::endl;
        return ;
    }
    if (bytesRead == 0)
    {
        std::cout << " ● " << RED "[INFO] Client 👤 disconnected on fd " <<  RESET << clientFd << std::endl;
        this->removeClient(index);
        this->disconnectClient(index);
        return ;
    }
    buffer[bytesRead] = '\0';
    std::string msg(buffer);
    if (msg.empty())
        return ;
    _clients[clientFd].appendToBuffer(msg);
    std::string bufferData = _clients[clientFd].getBuffer();
    size_t pos = 0;
    while ((pos = bufferData.find('\n')) != std::string::npos)
    {
        std::string line = bufferData.substr(0, pos + 1);
        std::cout << "   ✶ Client " << clientFd
                << " sent: " << line;
        parse_message(line, _clients[clientFd], *this);
        bufferData.erase(0, pos + 1);
    }
    _clients[clientFd].clearBuffer();

    if (!bufferData.empty())
    {
        _clients[clientFd].appendToBuffer(bufferData);
    }
}

//////////

void Server::acceptNewClient()
{
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    int clientSocket = accept(_serverSocket, (struct sockaddr *)&clientAddr, &clientLen);
    if (clientSocket == -1)
    {
        std::cerr << "[ERROR] accept() failed" << std::endl;
        return;
    }

    if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1)
    {
        close(clientSocket);
        std::cerr << "[ERROR] fcntl() failed on client" << std::endl;
        return;
    }

    std::string clientIp = inet_ntoa(clientAddr.sin_addr);

    struct pollfd clientPollFd;
    clientPollFd.fd = clientSocket;
    clientPollFd.events = POLLIN;
    clientPollFd.revents = 0;
    _fds.push_back(clientPollFd);

    _clients[clientSocket] = Client(clientSocket, clientIp);

    std::cout << " ● " << YELLOW "[INFO] New Client 👤 connected from IP: "<< clientIp << " on fd " << clientSocket << RESET << std::endl;
}

//////////

void Server::initServer()
{
    struct sockaddr_in addr;

    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket == -1)
        throw std::runtime_error("Failed to create socket");

    int enable = 1;
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1)
        throw std::runtime_error("Failed to set socket options");
    
    if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("Failed to set non-blocking mode");
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; // Ecoute sur toutes les interfaces
    addr.sin_port = htons(_port);      // Conversion en format reseau (Big Endian)
    if (bind(_serverSocket, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        throw std::runtime_error("Failed to bind socket");
    
    if (listen(_serverSocket, SOMAXCONN) == -1)
        throw std::runtime_error("Failed to listen");

    struct pollfd serverPollFd;
    serverPollFd.fd = _serverSocket;
    serverPollFd.events = POLLIN;
    serverPollFd.revents = 0;
    _fds.push_back(serverPollFd);
}

//////////

void Server::printBanner(void)
{
    std::cout << BLUE;
    std::cout << "  ════════════════════════════════════════════════\n";
    std::cout << "\n";
    std::cout << CYAN;
    std::cout << "  ██████╗ ███████╗██████╗ ██╗   ██╗███████╗██████╗\n";
    std::cout << "  ██╔════╝██╔════╝██╔══██╗██║   ██║██╔════╝██╔══██╗\n";
    std::cout << "  ███████╗█████╗  ██████╔╝██║   ██║█████╗  ██████╔╝\n";
    std::cout << "  ╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝██╔══╝  ██╔══██╗\n";
    std::cout << "  ███████║███████╗██║  ██║ ╚████╔╝ ███████╗██║  ██║\n";
    std::cout << "  ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝  ╚══════╝╚═╝  ╚═╝\n";
    std::cout << RESET;
    std::cout << GRAY;
    std::cout << "\n  · · · · · · · · · · · · · · · · · · · · · · · ·\n";
    std::cout << GREEN;
    std::cout << "       ⚡  High-Performance C++ IRC Server  ⚡\n";
    std::cout << GRAY;
    std::cout << "  · · · · · · · · · · · · · · · · · · · · · · · ·\n\n";
    std::cout << GRAY  << "  Port      " << WHITE << this->_port   << "\n";
    std::cout << GRAY  << "  Password  " << WHITE << "********"    << "\n";
    std::cout << GRAY  << "  Mode      " << GREEN << "RUNNING"     << "\n";
    std::cout << RESET   << "\n";
    std::cout << BLUE  << "  ════════════════════════════════════════════════\n";
    std::cout << GREEN << "  ✔  IRC Server started successfully!\n";
    std::cout << BLUE  << "  ●  Waiting for clients...\n";
    std::cout << GRAY  << "  Press Ctrl+C to stop\n";
    std::cout << BLUE  << "  ════════════════════════════════════════════════\n";
    std::cout << RESET   << "\n";
}

Client* Server::findClientByNickname(const std::string& nickname)
{
    std::map<int, Client>::iterator it;
    
    for (it = this->_clients.begin(); it != this->_clients.end(); ++it)
    {
        if (it->second.getNickname() == nickname)
            return &(it->second);
    }
    return NULL;
}

void Server::prodcastNickUpdated(Client &client, const std::string &oldNickname)
{
    std::string message = ":" + oldNickname + " NICK " + client.getNickname() + "\r\n";

    std::map<std::string, Channel>::iterator it;
    for (it = this->_channels.begin(); it != this->_channels.end(); ++it)
    {   
        if (it->second.isMember(&client))
            it->second.broadcast(message, client.getSocket());
    }
}

/////

void Server::run()
{
    this->printBanner();

    while (Server::signal_flag == false)
    {
        // (Bloquant, 0% CPU)
        int pollCount = poll(&_fds[0], _fds.size(), -1);
        if (pollCount < 0 && Server::signal_flag == false)
            throw std::runtime_error("poll() failed");
        if (pollCount < 0)
            continue;
        for (size_t i = 0; i < _fds.size(); i++)
        {
            if (_fds[i].revents & POLLIN)
            {
                if (_fds[i].fd == _serverSocket)
                    acceptNewClient();
                else
                {
                    size_t old_size = _fds.size();
                    receiveData(i);
                    if (_fds.size() < old_size)
                        i--;
                }
            }
        }
    }
    std::cout << std::endl << BLACK << "[INFO] Server shutting down..." << RESET << std::endl;
}