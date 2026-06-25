#include "../includes/Channel.hpp"

Channel::Channel() : _nameChannel("")
{
    _inviteOnly = false;
    _password = "";
    _hasPassword = false;
    _userLimit = -1;
    _hasTopic = false;
    _topic = "";
}

Channel::Channel(std::string name) : _nameChannel(name)
{
    _inviteOnly = false;
    _password = "";
    _hasPassword = false;
    _userLimit = -1;
    _hasTopic = false;
    _topic = "";
}

Channel::~Channel() {}

std::string Channel::getName() const { return this->_nameChannel; }

void Channel::setName(std::string name) { _nameChannel = name; }

const std::vector<Client*>& Channel::getMembers() const { return this->_members; }

// Ajoute un client au canal s'il n'y est pas deja
void Channel::addMember(Client* client)
{
    if (client && !isMember(client))
        this->_members.push_back(client);
}

// Supprime un client du canal (utile si un client se deconnecte)
void Channel::removeMember(Client* client)
{
    if (!client)
        return;
    std::vector<Client*>::iterator it;
    for (it = this->_members.begin(); it != this->_members.end(); ++it)
    {
        if (*it == client)
        {
            this->_members.erase(it);
            break;
        }
    }
    //
    for (std::vector<Client*>::iterator itOp = _operators.begin(); itOp != _operators.end(); ++itOp)
    {
        if (*itOp == client)
        {
            _operators.erase(itOp);
            break;
        }
    }
    removeInvitedUser(client);
}

// Verifie si un client est present dans ce canal
bool Channel::isMember(Client* client)
{
    if (!client)
        return false;
    std::vector<Client*>::iterator it;
    for (it = this->_members.begin(); it != this->_members.end(); ++it)
    {
        if (*it == client)
            return true;
    }
    return false;
}

// Envoie le message a tous les membres du canal (sauf a senderFd)
void Channel::broadcast(const std::string &message, int senderFd)
{
    std::vector<Client*>::iterator it;
    for (it = this->_members.begin(); it != this->_members.end(); ++it)
    {
        if ((*it)->getSocket() != senderFd)
            (*it)->envoyerMessage(message);
    }
}

//je doi faire channel pour prowdacst de changemnt de nick
//je dois faire une boucle dans le whois.cpp pour channels


//Operators
const std::vector<Client*>& Channel::getOperators() const { return _operators; }

bool Channel::isOperator(Client* client) const
{
    if (!client) return false;
    for (size_t i = 0; i < _operators.size(); ++i)
        if (_operators[i] == client) return true;
    return false;
}

void Channel::addOperator(Client* client)
{
    if (!client) return;
    for (size_t i = 0; i < _operators.size(); ++i)
        if (_operators[i] == client) return;
    _operators.push_back(client);
}

// Password
void Channel::setPassword(const std::string& pass)
{
    _password = pass;
    _hasPassword = true;
}
bool Channel::hasPassword() const { return _hasPassword; }

bool Channel::checkPassword(const std::string& pass) const
{
    if (!_hasPassword) return true;
    return _password == pass;
}

// Limit user
void Channel::setUserLimit(int limit) { _userLimit = limit; }
int Channel::getUserLimit() const { return _userLimit; }

// Invite
void Channel::setInviteOnly(bool value) { _inviteOnly = value; }
bool Channel::isInviteOnly() const { return _inviteOnly; }

void Channel::addInvitedUser(Client* client)
{
    if (!client) return;
    for (size_t i = 0; i < _invitedUsers.size(); ++i)
        if (_invitedUsers[i] == client) return;
    _invitedUsers.push_back(client);
}

void Channel::removeInvitedUser(Client* client)
{
    if (!client) return;
    for (std::vector<Client*>::iterator it = _invitedUsers.begin(); it != _invitedUsers.end(); ++it)
    {
        if (*it == client)
        {
            _invitedUsers.erase(it);
            break;
        }
    }
}

bool Channel::isInvited(Client* client) const
{
    if (!client) return false;
    for (size_t i = 0; i < _invitedUsers.size(); ++i)
        if (_invitedUsers[i] == client) return true;
    return false;
}

// Topic
void Channel::setTopic(const std::string& topic)
{
    _topic = topic;
    _hasTopic = true;
}
bool Channel::hasTopic() const { return _hasTopic; }
std::string Channel::getTopic() const { return _topic; }