#include "../includes/Utils.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Message.hpp"
#include "../includes/Channel.hpp"
#include "../includes/IrcReply.hpp"

std::string make_prefix(Client& client)
{
    return ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIp();
}

static bool is_valid_channel_name(const std::string& name)
{
    if (name.empty() || name[0] != '#')
        return false;

    for (size_t i = 1; i < name.size(); ++i)
    {
        unsigned char c = static_cast<unsigned char>(name[i]);
        if (c <= 32 || c == 127)
            return false;
        if (name[i] == ',')
            return false;
    }
    return true;
}


static std::string build_nick_list_for_channel(Channel& ch)
{
    std::string lst;
    const std::vector<Client*>& members = ch.getMembers();

    for (size_t i = 0; i < members.size(); ++i)
    {
        Client* m = members[i];
        if (!m) continue;

        if (!lst.empty())
            lst += " ";

        if (ch.isOperator(m))
            lst += "@" + m->getNickname();
        else
            lst += m->getNickname();
    }
    return lst;
}

static void sendTopicNamesMsgs(Client &client, Channel& ch, std::string channel_name)
{
    if (ch.hasTopic())
        client.envoyerMessage(RPL_TOPIC(client.getNickname(), channel_name, ch.getTopic()));
    else
        client.envoyerMessage(RPL_NOTOPIC(client.getNickname(), channel_name));
    
    std::string names = build_nick_list_for_channel(ch);
    client.envoyerMessage(RPL_NAMREPLY(client.getNickname(), channel_name, names));
    client.envoyerMessage(RPL_ENDOFNAMES(client.getNickname(), channel_name));
}

std::vector<std::string> split(const std::string& s)
{
    std::vector<std::string> out;
    size_t start = 0;
    for (size_t i = 0; i <= s.size(); ++i)
    {
        if (i == s.size() || s[i] == ',')
        {
            out .push_back(s.substr(start, i - start));
            start = i + 1;
        }
    }
    return out;
}

void handle_join_request(Server &server, Client &client, Message &msg)
{
    (void)server;
    if (!client.isAuthenticated())
    {
        client.envoyerMessage(ERR_NOTREGISTERED("*", "You have not registered - PASS/NICK/USER required first"));
        return;
    }

    if (msg.args.empty())
    {
        client.envoyerMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "JOIN"));
        return;
    }
    std::vector<std::string> channelList = split(msg.args[0]);
    std::vector<std::string> keyList;
    if (msg.args.size() >= 2)
        keyList = split(msg.args[1]);
    for (size_t idx = 0; idx < channelList.size(); ++idx)
    {
        std::string channel_name = channelList[idx];
        
        if (!is_valid_channel_name(channel_name))
        {
            std::string nick = client.getNickname().empty() ? "*" : client.getNickname();
            client.envoyerMessage(ERR_BADCHANMASK(nick, channel_name));
            continue;
        }

        std::string provideKey;
        if (idx < keyList.size())
            provideKey = keyList[idx];
        else
            provideKey.clear();

        Channel& ch = server.getChannels()[channel_name];
        ch.setName(channel_name);

        // if user is already joined the channel -> print topic and list of users
        if (ch.isMember(&client))
        {
            sendTopicNamesMsgs(client, ch, channel_name);
            continue;
        }

        // Password
        if(ch.hasPassword())
        {
            if (!ch.checkPassword(provideKey))
            {
                client.envoyerMessage(ERR_BADCHANNELKEY(client.getNickname(), channel_name));
                continue;
            }
        }

        // channel invite only
        if (ch.isInviteOnly() && !ch.isInvited(&client))
        {
            client.envoyerMessage(ERR_INVITEONLYCHAN(client.getNickname(), channel_name));
            continue;
        }

        // limited channel
        if (ch.getUserLimit() != -1)
        {
            if ((int)ch.getMembers().size() >= ch.getUserLimit())
            {
                client.envoyerMessage(ERR_CHANNELISFULL(client.getNickname(), channel_name));
                continue;
            }
        }
        ch.addMember(&client);

        if (ch.getMembers().size() == 1)
            ch.addOperator(&client);

        if (ch.isInviteOnly())
            ch.removeInvitedUser(&client);

        // Broadcast JOIN to all existing members + joining client too
        std::string joinMsg = make_prefix(client) + " JOIN " + channel_name + IRC_LINE_END;
        ch.broadcast(joinMsg, client.getSocket());
        client.envoyerMessage(joinMsg);

        // Topic & Names
        sendTopicNamesMsgs(client, ch, channel_name);
    }
}
