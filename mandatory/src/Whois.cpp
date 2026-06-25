#include "../includes/Client.hpp"
#include "../includes/Server.hpp"
#include "../includes/Message.hpp"
#include "../includes/Utils.hpp"
#include "../includes/IrcReply.hpp"

void    handle_whois_request(Server &server, Client &client, Message &msg)
{
    if (msg.args.empty())
    {
        // ERR_NONICKNAMEGIVEN (431)
        client.envoyerMessage(ERR_NONICKNAMEGIVEN(client.getNickname()));
        return ;  
    }
    std::string target_nick = msg.args[0];
    Client* target_client = server.findClientByNickname(target_nick);
    if (!target_client)
    {
        // ERR_NOSUCHNICK (401)
        client.envoyerMessage(ERR_NOSUCHNICK(client.getNickname(), target_nick));
        return ;
    }

    std::string nickname = target_client->getNickname();
    std::string username = target_client->getUsername();
    std::string hostname = target_client->getHostName();
    std::string realname = target_client->getRealname();

    //:server 311 <requester> <nick> <user> <host> * :<realname>
    client.envoyerMessage(RPL_WHOISUSER(client.getNickname(), nickname, username, hostname, realname));

    std::string channels_list = "";
    std::map<std::string, Channel>::iterator it; 
    for (it = server.getChannels().begin() ; it != server.getChannels().end(); ++it)
    {
        if (it->second.isMember(target_client))
                channels_list += it->second.getName() + " ";
    }

	if (!channels_list.empty())
    {
        if (channels_list.at(channels_list.size() - 1) == ' ')
            channels_list.erase(channels_list.size() - 1);

        //:nom_du_serveur 319 <ton_pseudo> <pseudo_cible> :[list chanels]
        client.envoyerMessage(RPL_WHOISCHANNELS(client.getNickname(), target_nick, channels_list));
    }
    //:server 318 <requester> <nick> :End of /WHOIS list
    client.envoyerMessage(RPL_ENDOFWHOIS(client.getNickname(), target_nick));
}