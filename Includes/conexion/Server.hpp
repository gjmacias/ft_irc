#ifndef SERVER_HPP_
# define SERVER_HPP_

// Estandar Libraries
# include <iostream>
# include <vector>

// System Libraries
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <poll.h>
# include <csignal>

// Proyect Libraries
#include "Client.hpp"

class Server
{
private:
	static bool					_signal;
	int							_port;
	int							_mainSocketFd;
	std::string					_password;
	std::vector<Client>			_clients;
	std::vector<struct pollfd>	_pollSocketFds;
public:
	Server();
	~Server();
	Server(Server const &src);
	Server &operator=(Server const &src);

		//		FUNCTIONS
	void		ServerInit();
	void		ServerLoop();
	void		AcceptNewClient();
	void		ReceiveNewData(int fd); 
	void		CloseFds();
	void		ClearClients(int fd);

	static void	SignalHandler(int signum);

		//		Authentificiation
	void		ClientAuthentification(int fd, std::string cmd)
		//		INVITE
	void	Invite(std::string &cmd, int &fd);
		//		JOIN
	void	Invite(std::string &cmd, int &fd);
		//		KICK
	void	Invite(std::string &cmd, int &fd);
		//		MODE
	void	Invite(std::string &cmd, int &fd);
		//		PART
	void	Invite(std::string &cmd, int &fd);
		//		PRIVATE_MESSAGE
	void	Invite(std::string &cmd, int &fd);
		//		QUIT
	void	Invite(std::string &cmd, int &fd);
		//		TOPIC
	void	Invite(std::string &cmd, int &fd);
};
#endif