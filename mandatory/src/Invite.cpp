#include "../includes/Utils.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Message.hpp"

void handle_invite_request(Server &server, Client &client, Message &msg)
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

    std::string targetNick = msg.args[0];
    std::string channel_name = msg.args[1];

    Client* target = server.findClientByNickname(targetNick);
    if (!target)
    {
        client.envoyerMessage(ERR_NOSUCHNICK(client.getNickname(), targetNick));
        return;
    }

    // Check channel if exist
    std::map<std::string, Channel> &channels = server.getChannels();
    std::map<std::string, Channel>::iterator it = channels.find(channel_name);
    if (it == channels.end())
    {
        client.envoyerMessage(ERR_NOSUCHCHANNEL(client.getNickname(), channel_name));
        return;
    }
    Channel& ch = it->second;
    
    if (!ch.isMember(&client))
    {
        client.envoyerMessage(ERR_NOTONCHANNEL(client.getNickname(), channel_name));
        return;
    }

    if (!ch.isOperator(&client))
    {
        client.envoyerMessage(ERR_CHANOPRIVSNEEDED(client.getNickname(), channel_name));
        return;
    }
    
    if (ch.isMember(target))
    {
        client.envoyerMessage(ERR_USERONCHANNEL(client.getNickname(), channel_name));
        return;
    }

    ch.addInvitedUser(target);
    
    // Reply to inviter + INVITE message to target
    client.envoyerMessage(RPL_INVITING(client.getNickname(), targetNick, channel_name));
    target->envoyerMessage(make_prefix(client) + " INVITE " + targetNick + " " + channel_name + IRC_LINE_END);
}
