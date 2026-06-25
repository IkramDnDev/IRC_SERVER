#include "../includes/Client.hpp"
#include "../includes/Server.hpp"
#include "../includes/Message.hpp"
#include "../includes/Utils.hpp"
#include "../includes/IrcReply.hpp"


void verify_password(const std::string password, Message msg, Client &client)
{
    if (msg.args.empty())
    {
        client.envoyerMessage(ERR_NEEDMOREPARAMS("*" ,"PASS"));
        return ;
    }
    if (msg.args[0] == password)
        client._hasGivenPassword = true;
    else
    {
        client._hasGivenPassword = false;
        client.envoyerMessage(ERR_PASSWDMISMATCH("*"));
    }
}

bool NickNameValide(const Message & msg)
{
    if (msg.args.size() < 1 || msg.args[0].empty())
        return false;

    const std::string& nick = msg.args[0];
    char first = nick[0];

    if (!std::isalpha(first) &&
        first != '[' && first != ']' &&
        first != '\\' && first != '`' &&
        first != '^' && first != '{' &&
        first != '}')
        return false;
    
    for (size_t i = 1; i < nick.length(); i++)
    {
        char c = nick[i];
        if (std::isalnum(c))
            continue;
        if (c == '-' || c == '[' || c == ']' || c == '\\' ||
            c == '`' || c == '^' || c == '{' || c == '}')
            continue;
        return false;
    }
    return true;
}

bool check_Dupplicated(Message msg, std::map<int, Client>& clients, int currentClientSocket)
{
    std::map<int, Client>::iterator it = clients.begin();
    while (it != clients.end())
    {
        //verifie si un autre client mais pas le meme client ok !
        if (it->second.getSocket() != currentClientSocket && it->second.getNickname() == msg.args[0])
        {
            return true;
        }
        ++it;
    }
    return false;
}

void verify_names (Client& c, Message msg, std::map<int, Client>& allClients)
{
    if (msg.command == "NICK")
    {
        if (msg.args.size() < 1 || msg.args[0].empty())
        {
            c.envoyerMessage(ERR_NONICKNAMEGIVEN("*"));
            return ;
        }
        if (!NickNameValide(msg))
        {
            c.envoyerMessage(ERR_ERRONEUSNICKNAME(msg.args[0]));
            return ;
        }
        if (check_Dupplicated(msg, allClients, c.getSocket()))
        {
            c.envoyerMessage(ERR_NICKNAMEINUSE(msg.args[0]));
            return ;
        }
        c.setNickname(msg.args[0]);
        c._hasGivenNickname = true;
    }
    //USER <username> <hostname> <servername> :<realname>
    else if (msg.command == "USER")
    {
        if (c._hasGivenUsername)
        {
            c.envoyerMessage(ERR_ALREADYREGISTRED(c.getNickname()));
            return ;
        }
        if (msg.args.size() < 4)
        {
            c.envoyerMessage(ERR_NEEDMOREPARAMS("*", msg.command));
            return ;
        }
        c.setUsername(msg.args[0]);
        /*
            c.setServerName(msg.args[1]); -> C'est deja fait avec l'IP (_host) !
            c.setServerName(msg.args[2]); -> C'est deja fait avec _servername !
        */
        std::string realname;
        if (msg.args[3][0] == ':')
        {
            realname = msg.args[3].substr(1);
            for (size_t j = 4; j < msg.args.size(); j++)
            {
                realname += " " + msg.args[j];
            }
        }
        else
        {
            realname = msg.args[3];
        }
        c.setRealname(realname);
        c._hasGivenUsername = true;
    }
}

void UpdateNickname(Client &client, Server &server, Message msg, std::map<int, Client>& clients)
{
    if (msg.args.empty() || msg.args[0].empty())
    {
        client.envoyerMessage(ERR_NONICKNAMEGIVEN(client.getNickname()));
        return ;
    }
    if (!NickNameValide(msg))
    {
        client.envoyerMessage(ERR_ERRONEUSNICKNAME(msg.args[0]));
        return ;
    }
    if (check_Dupplicated(msg, clients, client.getSocket()))
    {
        client.envoyerMessage(ERR_NICKNAMEINUSE(msg.args[0]));
        return ;
    }
    std::string oldNickname = client.getNickname();
    client.setNickname(msg.args[0]);
    client.envoyerMessage(RPL_NICK(oldNickname, client.getUsername(), client.getHostName(), client.getNickname()));

    server.prodcastNickUpdated(client, oldNickname);
}


int handle_authentication(Client &client, Message msg, Server& server)
{
    if (client.isAuthenticated() == true)
    {
        if (msg.command == "PASS" || msg.command == "USER")
        {
            client.envoyerMessage(ERR_ALREADYREGISTRED(client.getNickname()));
            return 1;
        }
        else if (msg.command == "NICK")
        {
            UpdateNickname(client, server, msg, server.getAllClients());
            return 1;
        }
        return 0; // Passe le relais aux autres commandes de parse_message (JOIN, PRIVMSG...)
    }
    if (!client.hasGivenPassword() && msg.command != "PASS")
    {
        client.envoyerMessage(ERR_NOTREGISTERED("*", "You have not registered - PASS required first"));
        return (1);
    }
    if (msg.command == "PASS")
    {
        verify_password(server.getPassword(), msg, client);
    }
    if (client.hasGivenPassword())
    {
        if (msg.command == "NICK" || msg.command == "USER")
        {
            verify_names(client, msg, server.getAllClients());
        }
        //! je dois faire une test des commande incorect apres tap pass
    }
    if (!client.isAuthenticated() && client.hasGivenPassword() &&
        client.hasGivenNickname() && client.hasGivenUsername())
    {
        client.setIsAuthenticated(true);
        std::string message = RPL_WELCOME(client.getNickname(), " :Welcome to the 1337 Internet Relay Chat Network ");
        client.envoyerMessage(message);
    }
    return (1);
}
