#include "../includes/Client.hpp"
#include "../includes/Server.hpp"
#include "../includes/Message.hpp"
#include "../includes/Utils.hpp"
#include "../includes/IrcReply.hpp"

std::string cleanAndToUpper(const std::string& str)
{
    std::string result = str;
    if (!result.empty() && result[result.length() - 1] == '\r')
        result.erase(result.length() - 1);
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = std::toupper(result[i]);
    }
    return result;
}

void parse_message(const std::string &msg_line, Client &client, Server &server)
{
    (void)server;
    if (msg_line.empty())
        return;
    Message msg;
    msg.msg_origin = msg_line;

    std::stringstream ss(msg_line);
    std::string word;

    if (ss >> word)
        msg.command = cleanAndToUpper(word);
    else
        return;
    while (ss >> word)
        msg.args.push_back(word);

    if (handle_authentication(client, msg, server))
        return;

    if (client.isAuthenticated() == false)
        return;

    if (msg.command == "JOIN")
        handle_join_request(server, client, msg);
    else if (msg.command == "INVITE")
        handle_invite_request(server, client, msg);
    else if (msg.command == "TOPIC")
        handle_topic_request(server, client, msg);
    else if (msg.command == "PRIVMSG")
        handle_privmsg_request(server, client, msg);
    else if (msg.command == "MODE")
        handle_mode_request(server, client, msg);
    else if (msg.command == "KICK")
        handle_kick_request(server, client, msg);
    else if (msg.command == "WHOIS")
        handle_whois_request(server, client, msg);
    else
        client.envoyerMessage(ERR_UNKNOWNCOMMAND("*", msg.command));
}