#ifndef                             CHANNEL_HPP
#define                             CHANNEL_HPP

#include                            "../includes/Utils.hpp"
#include                            "../includes/Client.hpp"

class Channel
{
    private :
    //1
        std::string                 _nameChannel;
        std::vector<Client*>        _members;

        //-----
        std::vector<Client*>        _operators;
        std::vector<Client*>        _invitedUsers;

        // MODE state (needed for JOIN)
        bool                        _inviteOnly;   // +i
        std::string                 _password;     // +k
        bool                        _hasPassword;  // true when +k is enabled
        int                         _userLimit;    // +l, -1 means no limit

        // TOPIC (needed for JOIN)
        bool                        _hasTopic;
        std::string                 _topic;

    public :
    //2
        Channel                     (std::string name);
        Channel                     ();
        ~Channel                    ();
    //3
        std::string                 getName() const;
        const std::vector<Client*>& getMembers() const;
        void                        setName(std::string name);
    //4
        void                        addMember(Client* client);
        void                        removeMember(Client* client);
        void                        broadcast(const std::string &message, int senderFd);
    //5
        bool                        isMember(Client* client);

        //-----
        const std::vector<Client*>& getOperators() const;
        void                        addOperator(Client* client);
        bool                        isOperator(Client* client) const;
        
        void                        setInviteOnly(bool value);
        bool                        isInviteOnly() const;

        void                        setPassword(const std::string& pass);
        bool                        hasPassword() const;
        bool                        checkPassword(const std::string& pass) const;

        void                        setUserLimit(int limit);
        int                         getUserLimit() const;

        void                        addInvitedUser(Client* client);
        void                        removeInvitedUser(Client* client);
        bool                        isInvited(Client* client) const;

        void                        setTopic(const std::string& topic);
        bool                        hasTopic() const;
        std::string                 getTopic() const;


};

#endif