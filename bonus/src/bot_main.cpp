#include "../includes/Utils.hpp"
#include "../includes/Bot.hpp"

int main(int argc, char *argv[])
{
    if (!parseBotArguments(argc, argv))
        return (1);

    std::string targetHost(argv[1]);
    std::string targetPort(argv[2]);
    std::string netPassword(argv[3]);

    try
    {
        Bot instanceBot(targetHost, targetPort, netPassword);
        instanceBot.connectToServer();
        instanceBot.execLoop();
    }
    catch (const std::exception &err)
    {
        std::cerr << RED << "Bot stopped working: " << err.what() << RESET << std::endl;
        return (1);
    }
    return (0);
}