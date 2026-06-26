#ifndef                                                         IRCREPLY_HPP
#define                                                         IRCREPLY_HPP

#define                                                         IRC_SERVER_PREFIX std::string("IRCServer")
#define                                                         IRC_LINE_END "\r\n"

// Le format IRC correct => :prefix CODE target :message

//Errors :
#define RPL_WHOISUSER(requester, nick, user, host, realname)    ":" + IRC_SERVER_PREFIX + " 311 " + requester + " " + nick + " " + user + " " + host + " * :" + realname +  + IRC_LINE_END
#define RPL_ENDOFWHOIS(requester, nick)                         ":" + IRC_SERVER_PREFIX + " 318 " + requester + " " + nick + " :End of /WHOIS list " + IRC_LINE_END
#define RPL_WHOISCHANNELS(target, nick, channels)               ":" + IRC_SERVER_PREFIX + " 319 " + target + " " + nick + " :" + channels + IRC_LINE_END
#define ERR_NOSUCHNICK(client, nick)                            ":" + IRC_SERVER_PREFIX + " 401 " + client + " " + nick + " :No such nick/channel " + IRC_LINE_END
#define ERR_UNKNOWNCOMMAND(target, cmd)                         ":" + IRC_SERVER_PREFIX + " 421 " + target + " " + cmd + " :Unknown command " + IRC_LINE_END
#define ERR_NONICKNAMEGIVEN(target)                             ":" + IRC_SERVER_PREFIX + " 431 " + target + " :No nickname given" + IRC_LINE_END
#define ERR_ERRONEUSNICKNAME(target)                            ":" + IRC_SERVER_PREFIX + " 432 " + target + " :Erroneous nickname " + IRC_LINE_END
#define ERR_NICKNAMEINUSE(target)                               ":" + IRC_SERVER_PREFIX + " 433 " + target + " :Nickname is already in use " + IRC_LINE_END
#define ERR_NOTREGISTERED(target, msg)                          ":" + IRC_SERVER_PREFIX + " 451 " + target + " :" + msg + IRC_LINE_END
#define ERR_NEEDMOREPARAMS(target, command)                     ":" + IRC_SERVER_PREFIX + " 461 " + target + " " + command + " :Not enough parameters " + IRC_LINE_END
#define ERR_ALREADYREGISTRED(target)                            ":" + IRC_SERVER_PREFIX + " 462 " + target + " :You may not reregister " + IRC_LINE_END
#define ERR_PASSWDMISMATCH(target)                              ":" + IRC_SERVER_PREFIX + " 464 " + target + " :Password incorrect " + IRC_LINE_END

//Replays :
#define RPL_WELCOME(target, msg)                                ":" + IRC_SERVER_PREFIX + " 001 " + target + " :" + msg + IRC_LINE_END
#define RPL_NICK(oldnick, username, userhost, newnick)          ":" + oldnick + "!" + username + "@" + userhost + " NICK :" + newnick + IRC_LINE_END


//
#define RPL_TOPIC(client, channel, topic)                       ":" + IRC_SERVER_PREFIX + " 332 " + client + " " + channel + " :" + topic + IRC_LINE_END
#define RPL_NOTOPIC(client, channel)                            ":" + IRC_SERVER_PREFIX + " 331 " + client + " " + channel + " :No topic is set" + IRC_LINE_END
#define RPL_ENDOFNAMES(client, channel)                         ":" + IRC_SERVER_PREFIX + " 366 " + client + " " + channel + " :End of /NAMES list" + IRC_LINE_END
#define RPL_NAMREPLY(client, channel, names)                    ":" + IRC_SERVER_PREFIX + " 353 " + client + " = " + channel + " :" + names + IRC_LINE_END

#define ERR_BADCHANMASK(client, channel)                        ":" + IRC_SERVER_PREFIX + " 476 " + client + " " + channel + " :Bad Channel Mask" + IRC_LINE_END
#define ERR_INVITEONLYCHAN(client, channel)                     ":" + IRC_SERVER_PREFIX + " 473 " + client + " " + channel + " :Cannot join channel (+i)" + IRC_LINE_END
#define ERR_CHANNELISFULL(client, channel)                      ":" + IRC_SERVER_PREFIX + " 471 " + client + " " + channel + " :Channel is full (+l)" + IRC_LINE_END
#define ERR_BADCHANNELKEY(client, channel)                      ":" + IRC_SERVER_PREFIX + " 475 " + client + " " + channel + " :Cannot join channel (bad key)" + IRC_LINE_END
#define ERR_NOSUCHCHANNEL(client, channel)                      ":" + IRC_SERVER_PREFIX + " 403 " + client + " " + channel + " :No such channel" + IRC_LINE_END
#define ERR_NOTONCHANNEL(client, channel)                       ":" + IRC_SERVER_PREFIX + " 442 " + client + " " + channel + " :You're not on that channel" + IRC_LINE_END
#define ERR_CHANOPRIVSNEEDED(client, channel)                   ":" + IRC_SERVER_PREFIX + " 482 " + client + " " + channel + " :You're not channel operator" + IRC_LINE_END
#define ERR_USERONCHANNEL(client, channel)                      ":" + IRC_SERVER_PREFIX + " 443 " + client + " " + channel + " :is already on channel" + IRC_LINE_END
#define ERR_USERNOTINCHANNEL(client, nick, channel)             ":" + IRC_SERVER_PREFIX + " 441 " + client + " " + nick + " " + channel + " :They aren't on that channel " + IRC_LINE_END
#define RPL_INVITING(client, nick, channel)                     ":" + IRC_SERVER_PREFIX + " 341 " + client + " " + nick + " " + channel + IRC_LINE_END
//


#endif
