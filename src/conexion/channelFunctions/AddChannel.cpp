#include "Channel.hpp"

/*
###############################################################################
#									ADDERS									  #
###############################################################################
*/

void Channel::add_client(Client newClient){clients.push_back(newClient);}

void Channel::add_admin(Client newClient){admins.push_back(newClient);}