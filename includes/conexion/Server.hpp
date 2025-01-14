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
# include <cstring>
# include <cerrno>

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
//	##################		SERVER.CPP		##################
		//		CONSTRUCTOR
	Server();
	~Server();
	Server(Server const &src);
	Server &operator=(Server const &src);

		//		SETTERS

		//		GETTERS
	int			GetPort();
	int			GetMainFd();
	std::string	GetPassword();
	Client		*GetClient(int fd);
	Client		*GetClient_Nickame(std::string nickname);

		//		MAIN FUNCTIONS
	static void	SignalHandler(int signum);

	void		ServerInit(int port, std::string password);
	void		ServerLoop();
	void		AcceptNewClient();
	void		ReceiveNewData(int fd); 
	void		CloseFds();
	void		ClearClients(int fd);
//	##########################################################


//	##################	SERVER FUNCTIONS	##################
		//		Parse and execute
	bool		isRegistered(int fd);
	void		parse_and_exec_cmd(std::string &cmd, int fd);
	
		//		Authentificiation
	void		ClientAuthentification(int fd, std::string cmd);
//	##########################################################


//	##################	SERVER FUNCTIONS/COMMANDS	##################
		//		INVITE
	void		InviteCommand(std::string &cmd, int &fd);

		//		JOIN
	void		JoinCommand(std::string &cmd, int &fd);

		//		KICK
	void		KickCommand(std::string &cmd, int &fd);

		//		MODE
	void		ModeCommand(std::string &cmd, int &fd);
	
		//		PART
	void		PartCommand(std::string &cmd, int &fd);

		//		PRIVATE_MESSAGE
	void		PrivateMessageCommand(std::string &cmd, int &fd);

		//		QUIT
	void		QuitCommand(std::string &cmd, int &fd);

		//		TOPIC
	void		TopicCommand(std::string &cmd, int &fd);
//	##########################################################
};

# endif