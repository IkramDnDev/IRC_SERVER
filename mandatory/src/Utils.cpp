#include "../includes/Utils.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

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

bool parseArguments(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << RED << "Usage: ./ircserv <port> <password>" << std::endl;
        return (false);
    }
    std::string portStr = argv[1];
    std::string password = argv[2];
    if (!isValidPort(portStr))
    {
        std::cerr << RED << "Error: invalid port (1 - 65535)" << std::endl;
        return (false);
    }
    if (password.empty())
    {
        std::cerr << RED << "Error: password cannot be empty" << std::endl;
        return (false);
    }
    return (true);
}
