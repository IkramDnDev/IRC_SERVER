#ifndef                             MESSAGE_HPP
#define                             MESSAGE_HPP

#include                            <string>
#include                            <vector>

class Message
{
    public :
    //1
        std::string                 msg_origin;
        std::string                 command;
        std::vector<std::string>    args;
    //2
        Message()                   : msg_origin(""), command("") {}
};

#endif