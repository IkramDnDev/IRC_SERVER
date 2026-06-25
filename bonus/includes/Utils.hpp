#ifndef         UTILS_HPP
#define         UTILS_HPP


#include        <iostream>
#include        <string>
#include        <cstdlib>
#include        <cstring>
#include        <cctype>
#include        <unistd.h>
#include        <sys/types.h>
#include        <sys/socket.h>
#include        <netdb.h>
#include        <ctime>
#include        <sstream>

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

bool            parseBotArguments(int argc, char **argv);


#endif
