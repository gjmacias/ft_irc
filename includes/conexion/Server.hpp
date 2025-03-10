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
	void		AddChannel(Channel newChannel);
	
			//		Client Register
	void		ClientAuthentification(std::string cmd, int fd);
	void		ClientUsername(std::vector<std::string> &splited_cmd, std::string cmd, int fd);
	void		ClientNickname(std::vector<std::string> &splited_cmd, int fd);

		//		Parse and execute
	void		ParseAndExecute(std::string &cmd, int fd, std::string &name);

		//		Removers Server
	void		RemoveFd(int fd);
	void		RemoveClient(int fd);
	void		RemoveClientFromChannels(int fd);
	void		RemoveChannel(std::string name);

		//		Sender Server
	void		SendResponse(std::string response, int fd);
	void		SendErrorV2(int fd, int code, std::string clientname, std::string channelname, std::string msg);
	void		SendError(int fd, int code, std::string clientname, std::string msg);

		//		Validations Server
	bool		IsRegisteredAndLoged(int fd);
	bool		IsOnlyRegistered(Client *client);
	bool		IsValidChannelname(std::string &channelname);
	bool		IsValidUsername(std::string &username);
	bool		IsValidNickname(std::string &nickname);
	bool		IsNickNameInUse(std::string &nickname);
//	##########################################################



//	##################	SERVER_FUNCTIONS/COMMANDS	##################
		//		INVITE
	void		InviteCommand(std::vector<std::string> &splited_cmd, int &fd);

		//		JOIN
	void		JoinCommand(std::vector<std::string> &splited_cmd, int &fd);
	void		NotExistCh(std::string channelname, std::string password, int fd);
	void		ExistCh(std::string &channelname, std::string &password, int j, int fd);
	bool 		IsInvited(Client *cli, std::string ChName, int flag);
	int			SearchForClients(std::string nickname);

		//		KICK
	void		KickCommand(std::string cmd, int &fd);
	std::string	SplitCmdKick(std::string cmd, std::vector<std::string> &tmp, std::string &user, int fd);
	std::string	SplitCmdK(std::string &cmd, std::vector<std::string> &tmp);
	void 		FindK(std::string cmd, std::string tofind, std::string &str);

		//		MODE
	void		ModeCommand(std::string cmd, int &fd);
	std::string	ModeInviteOnly(Channel *channel, char opera, std::string chain);
	std::string ModeTopicRestriction(Channel *channel ,char opera, std::string chain);
	std::string ModeChannelKey(std::vector<std::string> tokens, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string &arguments);
	std::string ModeOperatorPrivilege(std::vector<std::string> tokens, Channel *channel, size_t& pos, int fd, char opera, std::string chain, std::string& arguments);
	std::string ModeLimit(std::vector<std::string> tokens,  Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string& arguments);
	std::string ModeToAppend(std::string chain, char opera, char mode);
	void		GetCmdArguments(std::string cmd,std::string& name, std::string& modeset ,std::string &params);
	std::vector<std::string>	SplitParameters(std::string parameters);
	bool		IsPasswordValid(std::string password);
	bool		IsLimitValid(std::string& limit);

		//		PART
	void		PartCommand(std::vector<std::string> &splited_cmd, int &fd);

		//		PRIVATE_MESSAGE
	void		PrivateMessageCommand(std::string cmd, int &fd);
	void		CheckForChannels_Clients(std::vector<std::string> &tmp, int fd);
		//		QUIT
	void		QuitCommand(std::vector<std::string> &splited_cmd, int &fd, std::string &name);

		//		TOPIC
	void		TopicCommand(std::vector<std::string> &splited_cmd,std::string &cmd, int &fd);
	std::string TimeTopic();
	std::string GetTopic(std::string &input);
	int			GetPos(std::string &cmd);
//	##########################################################
};

# endif

