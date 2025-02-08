#include "Channel.hpp"

/*
###############################################################################
#									CONSTRUCTOR								  #
###############################################################################
*/
Channel::Channel()
{
	char modes[5] = {'i', 't', 'k', 'o', 'l'};
	for(int i = 0; i < 5; i++)
		this->_modes.push_back(std::make_pair(modes[i], false));

	this->_mode_limit_numberOfClients = 0;

	this->_name = "";
	this->_password = "";
	this->_topic_name = "";
}
Channel::~Channel(){}

Channel::Channel(Channel const &src){*this = src;}

Channel &Channel::operator=(Channel const &src)
{
	if (this != &src)
	{
		this->_modes = src._modes;
		for(size_t i = 0; i < this->_modes.size(); i++)
			this->_modes[i].second = src._modes[i].second;

		this->_mode_limit_numberOfClients = src._mode_limit_numberOfClients;

		this->_name = src._name;
		this->_password = src._password;		
		this->_topic_name = src._topic_name;
	
		this->_clients = src._clients;
		this->_admins = src._admins;
	}
	return (*this);
}

/*
###############################################################################
#									SETTERS									  #
###############################################################################
*/
void	Channel::SetModesInvitOnly(bool invit_only){this->_modes[0].second = invit_only;}
void	Channel::SetModesTopicRestriction(bool restricted){this->_modes[1].second = restricted;}
void	Channel::SetModesChannelKey(bool key){this->_modes[2].second = key;}
void	Channel::SetModesOperatorPrivilege(bool privilege){this->_modes[3].second = privilege;}
void	Channel::SetModesLimit(bool limit){this->_modes[4].second = limit;}
void	Channel::SetModesLimitNumber(unsigned int number_limit){this->_mode_limit_numberOfClients = number_limit;}

void	Channel::SetName(std::string name){this->_name = name;}
void	Channel::SetPassword(std::string password){this->_password = password;}
void	Channel::SetTopicName(std::string topic_name){this->_topic_name = topic_name;}

void	Channel::SetModeAtindex(size_t index, bool mode){this->_modes[index].second = mode;}

/*
###############################################################################
#									GETTERS									  #
###############################################################################
*/
bool	Channel::GetModesInvitOnly(){return this->_modes[0].second;}
bool	Channel::GetModesTopicRestriction(){return this->_modes[1].second;}
bool	Channel::GetModesChannelKey(){return this->_modes[2].second;}
bool	Channel::GetModesOperatorPrivilege(){return this->_modes[3].second;}
bool	Channel::GetModesLimit(){return this->_modes[4].second;}
bool	Channel::GetModesLimitNumber(){return this->_mode_limit_numberOfClients;}

std::string	Channel::GetName(){return this->_name;}
std::string	Channel::GetPassword(){return this->_password;}
std::string	Channel::GetTopicName(){return this->_topic_name;}

std::string	Channel::GetModes()
{
	std::string	mode;

	for(size_t i = 0; i < this->_modes.size(); i++)
	{
		if(this->_modes[i].second)
			mode.push_back(this->_modes[i].first);
	}
	if(!mode.empty())
		mode.insert(mode.begin(),'+');
	return (mode);
}
Client	*Channel::GetClient(int fd)
{
	for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if ((*it)->GetFd() == fd)
			return (*it);
	}
	return NULL;
}
Client	*Channel::GetAdmin(int fd)
{
	for (std::vector<Client *>::iterator it = this->_admins.begin(); it != this->_admins.end(); ++it)
	{
		if ((*it)->GetFd() == fd)
			return (*it);
	}
	return NULL;
}
Client* Channel::GetClientByNickname(std::string nickname)
{
	for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it){
		if ((*it)->GetNickname() == nickname)
			return (*it);
	}
	for (std::vector<Client *>::iterator it = this->_admins.begin(); it != this->_admins.end(); ++it){
		if ((*it)->GetNickname() == nickname)
			return (*it);
	}
	return NULL;
}

/*
###############################################################################
#									FUNCTIONS								  #
###############################################################################
*/

int	Channel::CountAllClients(){return this->_clients.size() + this->_admins.size();}

std::string	Channel::ListOfClients()
{
	std::string	list;

	for(size_t i = 0; i < this->_admins.size(); i++)
	{
		list += "@" + this->_admins[i]->GetNickname();
		if((i + 1) < this->_admins.size())
			list += " ";
	}
	if(this->_clients.size())
		list += " ";
	for(size_t i = 0; i < this->_clients.size(); i++)
	{
		list += this->_clients[i]->GetNickname();
		if((i + 1) < this->_clients.size())
			list += " ";
	}
	return (list);
}

