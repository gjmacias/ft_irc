#include "../../includes/conexion/Client.hpp"

/*
###############################################################################
#									CONSTRUCTOR								  #
###############################################################################
*/
Client::Client()
{
	this->_fd = -1;
	this->_isRegistered = false;
	this->_isLogedInServer = false;
	this->_IPaddress = "";
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
		this->_isRegistered = src._isRegistered;
		this->_isLogedInServer = src._isLogedInServer;
		this->_IPaddress = src._IPaddress;
		this->_nickname = src._nickname;
		this->_username = src._username;
		this->_buffer = src._buffer;
		this->_myInviteChannels = src._myInviteChannels;
	}
	return *this;
}
/*
###############################################################################
#									SETTERS									  #
###############################################################################
*/
void Client::SetFd(int fd){this->_fd = fd;}
void Client::SetIsRegistered(bool selector){this->_isRegistered = selector;}
void Client::SetIsLogedInServer(bool selector){this->_isLogedInServer = selector;}
void Client::SetIPaddress(std::string IPaddress){this->_IPaddress = IPaddress;}
void Client::SetNickname(std::string nickname){this->_nickname = nickname;}
void Client::SetUsername(std::string username){this->_username = username;}
void Client::SetBuffer(std::string recived){this->_buffer += recived;}

/*
###############################################################################
#									GETTERS									  #
###############################################################################
*/
int			Client::GetFd(){return this->_fd;}
bool		Client::GetIsRegistered(){return this->_isRegistered;}
bool		Client::GetIsLogedInServer(){return this->_isLogedInServer;}
std::string Client::GetIPaddress(){return this->_IPaddress;}
std::string Client::GetNickname(){return this->_nickname;}
std::string Client::GetUsername(){return this->_username;}
std::string Client::GetBuffer(){return this->_buffer;}
std::string Client::GetHostname()
{
	std::string _hostname = this->GetNickname() + "!" + this->GetUsername();
	return _hostname;
}

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

