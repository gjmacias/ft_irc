#ifndef SERVER_HPP_
# define SERVER_HPP_

// Librerías estándar
# include <iostream>
# include <vector>

// Librerías del sistema
# include <sys/socket.h>
// #include <sys/types.h>
// #include <netinet/in.h>
# include <fcntl.h>
# include <unistd.h>
// #include <arpa/inet.h>
# include <poll.h>
# include <csignal>

class Server
{
private:
	int							_port;
	int							_mainSocketFd;
	static bool					_signal;
	std::vector<Client>			_clients;
	std::vector<struct pollfd>	_pollSocketFds;
public:
	Server();

	void		ServerInit();
	void		ServerLoop();
	void		AcceptNewClient();
	void		ReceiveNewData(int fd); 
	void		CloseFds();
	void		ClearClients(int fd);

	static void	SignalHandler(int signum);
};
#endif