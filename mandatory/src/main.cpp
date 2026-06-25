#include <iostream>
#include "../includes/Utils.hpp"
#include "../includes/Server.hpp"

void process_signal()
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, Server::handle_signal);
    signal(SIGQUIT, Server::handle_signal);
}

int main(int argc, char **argv)
{
    if (!parseArguments(argc, argv))
        return (1);
    Server server(std::atoi(argv[1]), std::string(argv[2]));
    try
    {
        process_signal();
        server.initServer();
        server.run();
        server.closeSockets();
    }
    catch(const std::exception &e)
    {
        server.closeSockets();
        std::cerr << e.what() << "\n";
    }
    return (0);
}