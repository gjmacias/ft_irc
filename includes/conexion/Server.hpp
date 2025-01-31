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
# include <sstream>

// C Libraries
# include <csignal>
# include <cstdlib>
# include <cstring>
# include <cerrno>

// Proyect Libraries
# include "Client.hpp"
# include "Channel.hpp"
# include "Errors.hpp"
# include "Information.hpp"
# include "AllFunctions.h"

class Channel;

class Client;

class Server
{
private:
	static bool					_signal;
	int							_port;
	int							_mainSocketFd;
	std::string					_password;
	std::vector<Client>			_clients;
	std::vector<Channel>		_channels;
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
	Channel		*GetChannel(std::string name);

		//		MAIN FUNCTIONS
	static void	SignalHandler(int signum);

	void		ServerInit(int port, std::string password);
	void		ServerLoop();
	void		AcceptNewClient();
	void		ReceiveNewData(int fd); 
	void		CloseFds();
//	##########################################################



//	##################	SERVER_FUNCTIONS	##################
		//		Adders Server
	void		AddChannel(Channel newChannel);
	
			//		Client Involved
	void		ClientAuthentification(std::string cmd, int fd);
	void		ClientNickname(std::string cmd, int fd);
	void		ClientUsername(std::vector<std::string> &splited_cmd, int fd);

		//		Parse and execute
	void		ParseAndExecute(std::string &cmd, int fd);

		//		Removers Server
	void		RemoveFd(int fd);
	void		RemoveClient(int fd);
	void		RemoveClientFromChannels(int fd);
	void		RemoveChannel(std::string name);

		//		Sender Server
	void		SendResponse(std::string response, int fd);
	void		SendErrorV2(int code, std::string clientname, std::string channelname, int fd, std::string msg);
	void		SendError(int code, std::string clientname, int fd, std::string msg);

		//		Validations Server
	bool		IsRegisteredAndLoged(int fd);
	bool		IsOnlyRegistered(Client *client);
	bool		IsValidNickname(std::string& nickname);
	bool		IsNickNameInUse(std::string& nickname);
//	##########################################################



//	##################	SERVER_FUNCTIONS/COMMANDS	##################
		//		INVITE
	void		InviteCommand(std::vector<std::string> &splited_cmd, int &fd);

		//		JOIN
	void		JoinCommand(std::vector<std::string> &splited_cmd, int &fd);

		//		KICK
	void		KickCommand(std::vector<std::string> &splited_cmd, int &fd);

		//		MODE
	void		ModeCommand(std::vector<std::string> &splited_cmd, int &fd);
	
		//		PART
	void		PartCommand(std::vector<std::string> &splited_cmd, int &fd);

		//		PRIVATE_MESSAGE
	void		PrivateMessageCommand(std::vector<std::string> &splited_cmd, int &fd);

		//		QUIT
	void		QuitCommand(std::vector<std::string> &splited_cmd, int &fd);

		//		TOPIC
	void		TopicCommand(std::vector<std::string> &splited_cmd, int &fd);
//	##########################################################
};

# endif