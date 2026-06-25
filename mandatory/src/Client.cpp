#include "../includes/Client.hpp"

//1
Client::Client() : 
    _fd(-1), _ip(""), _host(""), _buffer(""), 
    _nickname(""), _username(""), _realname(""), _servername(IRC_SERVER_PREFIX) ,
    _hasGivenPassword(false), _hasGivenNickname(false), _hasGivenUsername(false), _isAuthenticated(false) 
{}

Client::Client(int fd, const std::string &ip) : 
    _fd(fd), _ip(ip), _host(ip), _buffer(""), 
    _nickname(""), _username(""), _realname(""), _servername(IRC_SERVER_PREFIX) ,
    _hasGivenPassword(false), _hasGivenNickname(false), _hasGivenUsername(false), _isAuthenticated(false) 
{}

Client::~Client() {}

//2 Getters / Setters 
std::string Client::getIp() const { return _ip; }
std::string Client::getHost() const { return _host; }
std::string Client::getBuffer() const { return _buffer; }
std::string Client::getNickname() const { return _nickname; }
std::string Client::getUsername() const { return _username; }
std::string Client::getRealname() const { return _realname; }
std::string Client::getHostName() const { return _hostname; }
std::string Client::getServerName() const { return _servername; }
std::string Client::ClientHostmask() { return (":" + _nickname + "!" + _username + "@" + _ip); }

int Client::getSocket() const { return _fd; }

void Client::appendToBuffer(const std::string& str) { _buffer += str; }
void Client::clearBuffer() { _buffer.clear(); }

void Client::setNickname(const std::string &nick) { _nickname = nick; }
void Client::setUsername(const std::string &user) { _username = user; }
void Client::setRealname(const std::string &real) { _realname = real; }
void Client::setHasGivenPassword(bool value) { _hasGivenPassword = value; }
void Client::setHasGivenNickname(bool value) { _hasGivenNickname = value; }
void Client::setHasGivenUsername(bool value) { _hasGivenUsername = value; }
void Client::setIsAuthenticated(bool value) { _isAuthenticated = value; }
void Client::setHostName(const std::string& hostname) { _hostname = hostname; }
void Client::setServerName(const std::string& servername) { _servername = servername; }

bool Client::hasGivenPassword() const { return _hasGivenPassword; }
bool Client::hasGivenNickname() const { return _hasGivenNickname; }
bool Client::hasGivenUsername() const { return _hasGivenUsername; }
bool Client::isAuthenticated() const { return _isAuthenticated; }
