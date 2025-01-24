#ifndef CLIENT_HPP_
# define CLIENT_HPP_

#include "Server.hpp"
#include "Channel.hpp"

class Client
{
private:
	bool		_isRegistered;		//When the password is correct
	bool		_isLogedInServer;	//When is Registered AND have: name and nick
	int			_fd;
	std::string	_IPadd;
	std::string	_nickname;
	std::string	_username;
	std::string	_buffer;
public:
	Client();
	Client(std::string nickname, std::string username, int fd);
	~Client();
	Client(Client const &src);
	Client &operator=(Client const &src);

		//		SETTERS
	void		SetFd(int fd);
	void		SetIsRegistered(bool selector);
	void		SetIsLogedInServer(bool selector);
	void		SetIpAdd(std::string ipadd);
	void		SetNickname(std::string nickName);
	void		SetUsername(std::string username);
	void		SetBuffer(std::string recived);

		//		GETTERS
	int			GetFd();
	bool		GetIsRegistered();
	bool		GetIsLogedInServer();
	std::string GetIpAdd();
	std::string	GetNickName();
	std::string	GetUserName();
	std::string	GetBuffer();


		//		FUNCTIONS
	void ClearBuffer();
	void ClearUsedBuffer();
};
#endif