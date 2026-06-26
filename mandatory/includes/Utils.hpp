#ifndef         UTILS_HPP
#define         UTILS_HPP

#include        <iostream>
#include        <string>
#include        <vector>
#include        <csignal>
#include        <poll.h>
#include        <sys/socket.h>
#include        <netinet/in.h>
#include        <fcntl.h>
#include        <cstdlib>
#include        <unistd.h> 
#include        <map>
#include        <arpa/inet.h>
#include        <sstream>

class           Server;
class           Client;
class           Message;

#define         RESET   "\033[0m"
#define         BLACK   "\033[30m"
#define         RED     "\033[31m"
#define         GREEN   "\033[32m"
#define         YELLOW  "\033[33m"
#define         BLUE    "\033[34m"
#define         MAGENTA "\033[35m"
#define         CYAN    "\033[36m"
#define         WHITE   "\033[37m"
#define         GRAY    "\033[0;90m"

//1
bool                        parseArguments(int argc, char **argv);
void                        parse_message(const std::string &msg_line, Client &client, Server &server);

//2
int                         handle_authentication(Client &client, Message msg, Server& server);
void                        handle_join_request(Server &server, Client &client, Message &msg);
void                        handle_invite_request(Server &server, Client &client, Message &msg);
void                        handle_topic_request(Server &server, Client &client, Message &msg);
void                        handle_privmsg_request(Server &server, Client &client, Message &msg);
void                        handle_mode_request(Server &server, Client &client, Message &msg);
void                        handle_kick_request(Server &server, Client &client, Message &msg);
void                        handle_whois_request(Server &server, Client &client, Message &msg);

std::string                 make_prefix(Client& client);
std::vector<std::string>    split(const std::string& s);

#endif