#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <ctime>

#include "Client.hpp"
#include "Server.hpp"

class Client;

class Channel
{
private:
	std::string							_time_creation;

	std::vector<std::pair<char, bool> >	_modes;

	unsigned int						_mode_limit_numberOfClients;

	std::string							_name;
	std::string							_password;
	std::string 						_topic_name;

	std::vector<Client *>				_clients;
	std::vector<Client *>				_admins;

public:
//	##################		CHANNEL.CPP		##################
		//		CONSTRUCTOR
	Channel();
	~Channel();
	Channel(Channel const &src);
	Channel &operator=(Channel const &src);

		//		SETTERS
	void		SetModesInvitOnly(bool invit_only);
	void		SetModesTopicRestriction(bool restricted);
	void		SetModesChannelKey(bool key);
	void		SetModesOperatorPrivilege(bool privilege);
	void		SetModesLimit(bool limit);
	void		SetModesLimitNumber(unsigned int number_limit);

	void		SetName(std::string name);
	void		SetPassword(std::string password);
	void		SetTopicName(std::string topic_name);

	void		SetModeAtindex(size_t index, bool mode);

		//		GETTERS
	std::string	GetModesTimeCreation();

	bool		GetModesInvitOnly();
	bool		GetModesTopicRestriction();
	bool		GetModesChannelKey();
	bool		GetModesOperatorPrivilege();
	bool		GetModesLimit();
	bool		GetModesLimitNumber();

	std::string GetName();
	std::string GetPassword();
	std::string	GetTopicName();
	
	std::string GetModes();
	Client		*GetClient(int fd);
	Client		*GetAdmin(int fd);
	Client		*GetClientByNickname(std::string nickname);

		// MAIN FUNCTIONS
	int			CountAllClients();
	std::string ListOfClients();
//	##########################################################

//	##################	CHANNEL_FUNCTIONS	##################
		//		Add Channel
	void		AddClient(Client *newClient);
	void		AddAdmin(Client *newClient);

		//		Removers Channel
	void		RemoveClient(int fd);
	void		RemoveAdmin(int fd);

		//		Change Channel
	bool		ChangeClientToAdmin(std::string nickname);
	bool		ChangeAdminToClient(std::string nickname);

		//		Senders Channel
	void		SendEveryone(std::string message);
	void		SendMeToAll(int fd, std::string message);

		//		Validations Channel
	bool		IsModeCharActive(char character);
	bool		IsClientInChannel(std::string nickname);
//	##########################################################
};

#endif

