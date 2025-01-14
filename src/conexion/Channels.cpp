#include "Channel.hpp"

/*
###############################################################################
#									CONSTRUCTOR								  #
###############################################################################
*/
Channel::Channel()
{
	this->_invit_only = 0;
	this->_topic = 0;
	this->_key = 0;
	this->_limit = 0;
	this->_topic_restriction = false;
	this->_name = "";
	this->_topic_name = "";
	char charaters[5] = {'i', 't', 'k', 'o', 'l'};

	for(int i = 0; i < 5; i++)
		modes.push_back(std::make_pair(charaters[i],false));

	this->_created_at = "";
}
Channel::~Channel(){}

Channel::Channel(Channel const &src){*this = src;}

Channel &Channel::operator=(Channel const &src)
{
	if (this != &src)
	{
		this->_invit_only = src._invit_only;
		this->_topic = src._topic;
		this->_key = src._key;
		this->_limit = src._limit;
		this->_topic_restriction = src._topic_restriction;
		this->_name = src._name;
		this->_password = src._password;
		this->_created_at = src._created_at;
		this->_topic_name = src._topic_name;
		this->_clients = src._clients;
		this->_admins = src._admins;
		this->_modes = src._modes;
	}
	return *this;
}

/*
###############################################################################
#									SETTERS									  #
###############################################################################
*/

/*
###############################################################################
#									GETTERS									  #
###############################################################################
*/