#include "../../../includes/conexion/Server.hpp"

/*
###############################################################################
#									SENDERS									  #
###############################################################################
*/
void	Server::SendResponse(std::string response, int fd)
{
	if(send(fd, response.c_str(), response.size(), 0) == -1)
		std::cerr << "Response send() faild" << std::endl;
}

	//	-----	Send Errors		-----
void	Server::SendError(int code, int fd, std::string clientname, std::string msg)
{
	std::stringstream	ss;
	std::string			response;

	ss << ":localhost " << code << " " << clientname << msg;
	response = ss.str();
	if(send(fd, response.c_str(), response.size(), 0) == -1)
		std::cerr << "send() faild" << std::endl;
}

void	Server::SendErrorV2(int code, int fd, std::string clientname, std::string channelname, std::string msg)
{
	std::stringstream	ss;
	std::string			response;

	ss << ":localhost " << code << " " << clientname << " " << channelname << msg;
	response = ss.str();
	if(send(fd, response.c_str(), response.size(), 0) == -1)
		std::cerr << "send() faild" << std::endl;
}

