#include "Server.hpp"

/*
###############################################################################
#									SENDERS									  #
###############################################################################
*/

revisar// void Channel::SendToAll(std::string rpl1)
// {
// 	for(size_t i = 0; i < admins.size(); i++)
// 		if(send(admins[i].GetFd(), rpl1.c_str(), rpl1.size(),0) == -1)
// 			std::cerr << "send() faild" << std::endl;
// 	for(size_t i = 0; i < clients.size(); i++)
// 		if(send(clients[i].GetFd(), rpl1.c_str(), rpl1.size(),0) == -1)
// 			std::cerr << "send() faild" << std::endl;
// }

// void Channel::SendToAll(std::string rpl1, int fd)
// {
// 	for(size_t i = 0; i < admins.size(); i++){
// 		if(admins[i].GetFd() != fd)
// 			if(send(admins[i].GetFd(), rpl1.c_str(), rpl1.size(),0) == -1)
// 				std::cerr << "send() faild" << std::endl;
// 	}
// 	for(size_t i = 0; i < clients.size(); i++){
// 		if(clients[i].GetFd() != fd)
// 			if(send(clients[i].GetFd(), rpl1.c_str(), rpl1.size(),0) == -1)
// 				std::cerr << "send() faild" << std::endl;
// 	}
// }