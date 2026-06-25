#ifndef                     CLIENT_HPP
#define                     CLIENT_HPP

#include                    "../includes/Utils.hpp"
#include                    "../includes/IrcReply.hpp"

class Client
{
    private :
    //1
        int                 _fd;
        std::string         _ip;
        std::string         _host;
        std::string         _buffer;

    public :
    //2
        std::string         _nickname;
        std::string         _username;
        std::string         _realname;
        std::string         _hostname;
        std::string         _servername;

        bool                _hasGivenPassword;
        bool                _hasGivenNickname;
        bool                _hasGivenUsername;
        bool                _isAuthenticated;
    //3
        Client              ();
        Client              (int fd, const std::string &ip);
        ~Client             ();


    //4

        int                 getSocket() const;
        std::string         getIp() const;
        std::string         getHost() const;
        std::string         getBuffer() const;
        std::string         ClientHostmask();//NICK!USERNAME@HOSTNAME.
        
        void                appendToBuffer(const std::string& str);
        void                clearBuffer();
        void                envoyerMessage(const std::string &message);
        

    //5
        void                setNickname(const std::string &nick);
        std::string         getNickname() const;

        void                setUsername(const std::string &user);
        std::string         getUsername() const;

        void                setRealname(const std::string &real);
        std::string         getRealname() const;

        void                setHostName(const std::string& hostname);
        std::string         getHostName() const;

        void                setServerName(const std::string& servername);
        std::string         getServerName() const;

    //6
        void                setHasGivenPassword(bool value);
        bool                hasGivenPassword() const;

        void                setHasGivenNickname(bool value);
        bool                hasGivenNickname() const;

        void                setHasGivenUsername(bool value);
        bool                hasGivenUsername() const;

        void                setIsAuthenticated(bool value);
        bool                isAuthenticated() const;
};


#endif