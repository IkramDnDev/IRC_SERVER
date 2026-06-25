#ifndef                                     SERVER_HPP
#define                                     SERVER_HPP

#include                                    "../includes/Utils.hpp"
#include                                    "../includes/Client.hpp"
#include                                    "../includes/Channel.hpp"
#include                                    "../includes/IrcReply.hpp"


class Server
{
    private:
    //1
        int                                 _port;
        std::string                         _password;
        int                                 _serverSocket;
        std::vector<struct pollfd>          _fds;
    //2
        std::map<int , Client>              _clients;
        std::map<std::string, Channel>      _channels; // pour JOIN 

    public:
    //3
        Server                              ();
        Server                              (int port,std::string password);
        ~Server                             ();
    //4
        static void                         handle_signal(int signo);
        static bool                         signal_flag;
    //5
        void                                initServer();
        void                                run();
        void                                acceptNewClient();
        void                                receiveData(size_t clientFd);
        void                                closeConnection(int fd);
        void                                disconnectClient(size_t index);
        void                                removeClient(int index);
        void                                printBanner(void);
        void                                prodcastNickUpdated(Client &client, const std::string &oldNickname);
        void                                closeSockets();
        Client*                             findClientByNickname(const std::string& nickname);
    //6
        std::map<int , Client> &            getAllClients();
        int                                 getPort() const;
        const std::string&                  getPassword() const;
        int                                 getServerSocket() const;
        std::map<std::string, Channel>&     getChannels();
};

#endif