#ifndef                             BOT_HPP
#define                             BOT_HPP

#include                            "../includes/Utils.hpp"

class Bot
{
    public:
        Bot                         (std::string s_host, std::string s_port, std::string s_pass);
        ~Bot                        ();

        void                        connectToServer();
        void                        printBanner();
        void                        execLoop();

    private:
        void                        sendHandshake();
        void                        verifyAuthStatus(const std::string &srvMsg);
        void                        handleCommand(std::string fullMsg);
        std::string                 parseInput(std::string srcMsg);
        std::string                 removeSpaces(const std::string &rawStr);

        int                         _socketId;
        std::string                 _serverHost;
        std::string                 _serverPort;
        std::string                 _serverPass;
        bool                        _isAuthDone;

        static const std::string    wisdomQuotes[15];
};

#endif