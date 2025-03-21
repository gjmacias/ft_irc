#ifndef CLIENT_HPP_
# define CLIENT_HPP_

#include "Server.hpp"
#include "Channel.hpp"

class Client
{
private:
	int							_fd;
	bool						_isRegistered;		//When the password is correct
	bool						_isLogedInServer;	//When is Registered AND have: name and nick
	std::string					_IPaddress;
	std::string					_nickname;
	std::string					_username;
	std::string					_buffer;
	std::vector<std::string>	_myInviteChannels;
public:
//	##################		CHANNEL.CPP		##################
		//		CONSTRUCTOR
	Client();
	Client(std::string nickname, std::string username, int fd);
	~Client();
	Client(Client const &src);
	Client &operator=(Client const &src);

		//		SETTERS
	void		SetFd(int fd);
	void		SetIsRegistered(bool selector);
	void		SetIsLogedInServer(bool selector);
	void		SetIPaddress(std::string ipadd);
	void		SetNickname(std::string nickname);
	void		SetUsername(std::string username);
	void		SetBuffer(std::string recived);

		//		GETTERS
	int			GetFd();
	bool		GetIsRegistered();
	bool		GetIsLogedInServer();
	std::string	GetIPaddress();
	std::string	GetNickname();
	std::string	GetUsername();
	std::string	GetBuffer();
	std::string	GetHostname();

		// MAIN FUNCTIONS
	void		ClearBuffer();
	void		ClearUsedBuffer();
//	##########################################################

//	##################	CLIENT_FUNCTIONS	##################
		//		Add Client
	void		AddToMyInvitedChannels(std::string channel_name);

		//		Removers Client
	void		RmFromMyInvitedChannels(std::string channel_name);

		//		Validations Client
	bool		ImInvitedInChannel(std::string channel_name);
//	##########################################################
};

#endif

