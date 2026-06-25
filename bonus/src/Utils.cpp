#include "../includes/Utils.hpp"

bool isValidPort(const std::string& portStr)
{
    if (portStr.empty())
        return (false);
    for (size_t i = 0; i < portStr.length(); i++)
    {
        if (!std::isdigit(portStr[i]))
            return (false);
    }
    char* endptr;
    long port = std::strtol(portStr.c_str(), &endptr, 10);
    if (*endptr != '\0' || port < 1 || port > 65535)
        return (false);
    return (true);
}

bool parseBotArguments(int argc, char **argv)
{
    if (argc != 4)
    {
        std::cerr << RED << "Usage: ./ircbot <host> <port> <password>" << RESET << std::endl;
        return (false);
    }
    
    std::string host = argv[1];
    std::string portStr = argv[2];
    std::string password = argv[3];

    if (host.empty())
    {
        std::cerr << RED << "Error: host cannot be empty" << RESET << std::endl;
        return (false);
    }
    if (!isValidPort(portStr))
    {
        std::cerr << RED << "Error: invalid port (1 - 65535)" << RESET << std::endl;
        return (false);
    }
    if (password.empty())
    {
        std::cerr << RED << "Error: password cannot be empty" << RESET << std::endl;
        return (false);
    }
    return (true);
}