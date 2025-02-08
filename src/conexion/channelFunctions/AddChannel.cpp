#include "Channel.hpp"

/*
###############################################################################
#									ADDERS									  #
###############################################################################
*/

void Channel::AddClient(Client *newClient){this->_clients.push_back(newClient);}

void Channel::AddAdmin(Client *newClient){this->_admins.push_back(newClient);}
