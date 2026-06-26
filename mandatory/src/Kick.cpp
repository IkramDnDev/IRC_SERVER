 #include "../includes/Utils.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Message.hpp"


/*
syntax : KICK #channel user :Comment

1 -> Authentified & Enough params
2 -> Channel exist
3 -> sender is member
4 -> sender is operator
5 -> target user in the channel
6 -> build kick message  ex: 
    => if there is an comment -> :alice!alice@127.0.0.1 KICK #general bob :Spamming
    => No comment -> :alice!alice@127.0.0.1 KICK #general bob :No reason giving
7 -> broadcast the message to all client
8 -> Remove user


*/


void handle_kick_request(Server &server, Client &client, Message &msg)
{
    if (!client.isAuthenticated())
    {
        client.envoyerMessage(ERR_NOTREGISTERED("*", "You have not registered - PASS/NICK/USER required first"));
        return;
    }

    if (msg.args.size() < 2)
    {
        client.envoyerMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "INVITE"));
        return;
    }
    std::string channel_name = msg.args[0];
    std::vector<std::string> targetsNickLst;
    targetsNickLst = split(msg.args[1]);
    
    std::string comment = "No reason giving";
    
    for (size_t idx = 0; idx < targetsNickLst.size(); ++idx)
    {
        // Store comment
        if (msg.args.size() >= 3)
        {       
            if (msg.args[2][0] == ':')
            {
                comment = msg.args[2].substr(1);
                for (size_t i = 3; i < msg.args.size(); i++)
                    comment += " " + msg.args[i];
            }
            else
                comment = msg.args[2]; 
        }

        // Check channel if exist
        std::map<std::string, Channel> &channels = server.getChannels();
        std::map<std::string, Channel>::iterator it = channels.find(channel_name);
        if (it == channels.end())
        {
            client.envoyerMessage(ERR_NOSUCHCHANNEL(client.getNickname(), channel_name));
            continue;
        }
        Channel& ch = it->second;

        if (!ch.isMember(&client))
        {
            client.envoyerMessage(ERR_NOTONCHANNEL(client.getNickname(), channel_name));
            continue;
        }

        if (!ch.isOperator(&client))
        {
            client.envoyerMessage(ERR_CHANOPRIVSNEEDED(client.getNickname(), channel_name));
            continue;
        }

        Client* targetClient = server.findClientByNickname(targetsNickLst[idx]);
        if (!targetClient)
        {
            client.envoyerMessage(ERR_NOSUCHNICK(client.getNickname(), targetsNickLst[idx]));
            continue;
        }
        if (!ch.isMember(targetClient))
        {
            client.envoyerMessage(ERR_USERNOTINCHANNEL(client.getNickname(), targetClient->getNickname(), channel_name));
            continue;
        }

        // Broadcast KICK to all existing members + client removed also before kicking
        std::string kicknMsg = make_prefix(client) + " KICK " + channel_name + " " + targetClient->getNickname() + " : " + comment + IRC_LINE_END;
        ch.broadcast(kicknMsg, client.getSocket());
        client.envoyerMessage(kicknMsg);

        ch.removeMember(targetClient);
    }
}