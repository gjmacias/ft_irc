#include "Client.hpp"

/*
###############################################################################
#									CONSTRUCTOR								  #
###############################################################################
*/
Client::Client()
{
	this->_fd = -1;
	this->_IPadd = "";
	this->_nickname = "";
	this->_username = "";
	this->_buffer = "";
}
Client::Client(std::string nickname, std::string username, int fd) :_fd(fd), 
	_nickname(nickname), _username(username){}

Client::~Client(){}

Client::Client(Client const &src)
{
	*this = src;
}
Client &Client::operator=(Client const &src)
{
	if (this != &src)
	{
		this->_fd = src._fd;
		this->_IPadd = src._IPadd;
		this->_nickname = src._nickname;
		this->_username = src._username;
		this->_buffer = src._buffer;
	}
	return *this;
}
/*
###############################################################################
#									SETTERS									  #
###############################################################################
*/
void Client::SetFd(int fd){this->_fd = fd;}
void Client::SetIpAdd(std::string ipadd){this->_IPadd = ipadd;}
void Client::SetNickname(std::string& nickName){this->_nickname = nickName;}
void Client::SetUsername(std::string& username){this->_username = username;}
void Client::SetBuffer(std::string recived){this->_buffer += recived;}

/*
###############################################################################
#									GETTERS									  #
###############################################################################
*/
int Client::GetFd(){return this->_fd;}
std::string Client::GetIpAdd(){return _IPadd;}
std::string Client::GetNickName(){return this->_nickname;}
std::string Client::GetUserName(){return this->_username;}
std::string Client::GetBuffer(){return this->_buffer;}

/*
###############################################################################
#								FUNCTIONS									  #
###############################################################################
*/
void Client::ClearBuffer(){this->_buffer.clear();}

void Client::ClearUsedBuffer()
{
    size_t	last_pos = this->_buffer.find_last_of("\r\n");

	if (last_pos != std::string::npos)
        this->_buffer.erase(0, last_pos + 1); 
	else
		this->_buffer.clear();
}
