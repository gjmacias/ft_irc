#ifndef CLIENT_HPP_
# define CLIENT_HPP_

#include "irc.h"

class Client
{
private:
	int			_fd;
	std::string	_IPadd;
public:
	Client();

	int		GetFd();
	void	SetFd(int fd);
	void	setIpAdd(std::string ipadd)ipadd;
};
#endif