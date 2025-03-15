#ifndef SERVER_HPP_
# define SERVER_HPP_

// Standar Libraries
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

// Project Libraries
# include "Client.hpp"
# include "Channel.hpp"
# include "../comunication/Errors.hpp"
# include "../comunication/Information.hpp"
# include "../functions/AllFunctions.h"
# include "../functions/ColorsDefinitions.h"


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
	Client		*GetClient_Nickname(std::string nickname);
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
	void		AddChannel(Channel &newChannel);
	
			//		Client Register
	void		ClientAuthentification(std::string cmd, int fd);
	void		ClientUsername(std::vector<std::string> &splited_cmd, std::string cmd, int fd);
	void		ClientNickname(std::vector<std::string> &splited_cmd, int fd);

		//		Parse and execute
	void		ParseAndExecute(std::string &cmd, int fd);

		//		Removers Server
	void		RemoveFd(int fd);
	void		RemoveClient(int fd);
	void		RemoveClientFromChannels(int fd);
	void		RemoveChannel(std::string name);

		//		Sender Server
	void		SendErrorV2(int code, int fd, std::string clientname, std::string channelname, std::string msg);
	void		SendError(int code, int fd, std::string clientname, std::string msg);

		//		Validations Server
	bool		IsRegisteredAndLoged(int fd);
	bool		IsOnlyRegistered(Client *client);
	bool		IsValidChannelname(std::string &channelname);
	bool		IsValidUsername(std::string &username);
	bool		IsValidNickname(std::string &nickname);
	bool		IsPasswordValid(std::string password);
	bool		IsLimitValid(std::string& limit);
	bool		IsNickNameInUse(std::string& nickname);

//	##########################################################



//	##################	SERVER_FUNCTIONS/COMMANDS	##################
		//		INVITE
	void		InviteCommand(std::vector<std::string> &splited_cmd, int fd);

		//		OTHERS
	void		CapCommand(std::vector<std::string> &splited_cmd, int fd);
	void		PingCommand(std::vector<std::string> &splited_cmd, int fd);
	void		WhoisCommand(std::vector<std::string> &splited_cmd, int fd);

		//		JOIN
	void		JoinCommand(std::vector<std::string> &splited_cmd, int fd);
	void		NotExistCh(std::string channelname, std::string password, int fd);
	void		ExistCh(std::string &channelname, std::string &password, int j, int fd);
	int			SearchForClients(std::string nickname);

		//		KICK
	void		KickCommand(std::vector<std::string>& splited_cmd, std::string cmd_reason, int fd);

		//		MODE
	void		ModeCommand(std::vector<std::string>& splited_cmd, int fd);
	void		ModeExecute(Channel* channel, std::vector<std::pair<char, bool> > list_modes, std::vector<std::string> list_parameters, int fd);
			//	5 modes
	bool		ModeInviteOnly(Channel* channel, bool flag);
	bool		ModeTopicRestriction(Channel* channel, bool flag);
	bool		ModeChannelKey(Channel* channel, bool flag, std::string& password, int fd);
	bool		ModeOperatorPrivilege(Channel* channel, bool flag, std::string& user, int fd);
	bool		ModeLimit(Channel* channel, bool flag, std::string& limit, int fd);
			//	utils
	void		PrepareModeResponse(std::vector<std::string>& response, std::pair<char, bool> mode, std::string parameter);
	void		SendModeResponse(std::vector<std::string> response, Channel* channel, int fd);
	void		OrderModes(std::vector<std::pair<char, bool> >& list_modes, std::string arraymodes);

		//		PART
	void		PartCommand(std::vector<std::string> &splited_cmd, std::string cmd_reason, int fd);

		//		PRIVATE_MESSAGE
	void		PrivateMessageCommand(std::vector<std::string>& splited_cmd, std::string cmd_reason, int fd);

		//		QUIT
	void		QuitCommand(std::vector<std::string>& splited_cmd, std::string cmd_reason, int fd);

		//		TOPIC
	void		TopicCommand(std::vector<std::string>& splited_cmd, std::string cmd_reason, int fd);
	void		TopicExecute(std::string channel, std::vector<std::string>& splited_cmd, std::string cmd_reason, int fd);
	std::string TopicTime();

//	##########################################################
};

# endif

