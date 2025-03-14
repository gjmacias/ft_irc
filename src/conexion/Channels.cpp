#include "../../includes/conexion/Channel.hpp"

/*
###############################################################################
#									CONSTRUCTOR								  #
###############################################################################
*/
Channel::Channel()
{
	std::time_t			time = std::time(NULL);
	std::ostringstream	oss;
	char				modes[5] = {'i', 't', 'k', 'o', 'l'};

	oss << time;
	this->_time_creation = oss.str();

	for(int i = 0; i < 5; i++)
		this->_modes.push_back(std::make_pair(modes[i], false));

	this->_name = "";

	this->_key_password = "";
	this->_topic_name = "";
	this->_topic_time = "";
	this->_topic_editor = "";
	this->_limit_numberOfClients = 0;
}
Channel::~Channel(){}

Channel::Channel(Channel const &src){*this = src;}

Channel &Channel::operator=(Channel const &src)
{
	if (this != &src)
	{
		this->_time_creation = src._time_creation;

		this->_modes = src._modes;
		for(size_t i = 0; i < this->_modes.size(); i++)
			this->_modes[i].second = src._modes[i].second;

		this->_name = src._name;

		this->_key_password = src._key_password;		
		this->_topic_name = src._topic_name;
		this->_topic_time = src._topic_time;
		this->_topic_editor = src._topic_editor;
		this->_limit_numberOfClients = src._limit_numberOfClients;

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
void	Channel::SetModesTopicRestriction(bool restricted) { this->_modes[1].second = restricted; }
void	Channel::SetModesChannelKey(bool key){this->_modes[2].second = key;}
void	Channel::SetModesOperatorPrivilege(bool privilege){this->_modes[3].second = privilege;}
void	Channel::SetModesLimit(bool limit){this->_modes[4].second = limit;}

void	Channel::SetName(std::string name){this->_name = name;}

void	Channel::SetPassword(std::string password){this->_key_password = password;}
void	Channel::SetTopicName(std::string topic_name){this->_topic_name = topic_name;}
void	Channel::SetTopicTime(std::string topic_time) { this->_topic_time = topic_time; }
void	Channel::SetTopicEditor(std::string topic_editor) { this->_topic_editor = topic_editor; }
void	Channel::SetLimitNumber(unsigned int number_limit) { this->_limit_numberOfClients = number_limit; }

/*
###############################################################################
#									GETTERS									  #
###############################################################################
*/
std::string		Channel::GetModesTimeCreation(){return this->_time_creation;}

bool			Channel::GetModesInvitOnly(){return this->_modes[0].second;}
bool			Channel::GetModesTopicRestriction(){return this->_modes[1].second;}
bool			Channel::GetModesChannelKey(){return this->_modes[2].second;}
bool			Channel::GetModesOperatorPrivilege(){return this->_modes[3].second;}
bool			Channel::GetModesLimit(){return this->_modes[4].second;}

std::string		Channel::GetName(){return this->_name;}

std::string		Channel::GetPassword(){return this->_key_password;}
std::string		Channel::GetTopicName(){return this->_topic_name;}
std::string		Channel::GetTopicTime() { return this->_topic_time; }
std::string		Channel::GetTopicEditor() { return this->_topic_editor; }
size_t			Channel::GetLimitNumber() { return this->_limit_numberOfClients; }

std::string		Channel::GetModes()
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
Client	*Channel::GetClientByNickname(std::string nickname)
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
Client	*Channel::GetObligatedAdmin() { return this->_clients[0]; }

/*
###############################################################################
#									FUNCTIONS								  #
###############################################################################
*/

size_t	Channel::CountAllClients() { return this->_clients.size() + this->_admins.size(); }
size_t	Channel::CountAdmins() { return this->_admins.size(); }
size_t	Channel::CountClients() { return this->_clients.size(); }

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

