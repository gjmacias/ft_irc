#include "../../../includes/conexion/Server.hpp"

/*
###############################################################################
#									SENDERS									  #
###############################################################################
*/

void	Server::SendError(int code, int fd, std::string clientname, std::string msg)
{
	std::stringstream	ss;
	std::string			response;

	ss << ":irserv " << code << " " << clientname << msg;
	response = ss.str();
    SendResponse(response, fd);
}

void	Server::SendErrorV2(int code, int fd, std::string clientname, std::string channelname, std::string msg)
{
	std::stringstream	ss;
	std::string			response;

	ss << ":irserv " << code << " " << clientname << " " << channelname << msg;
	response = ss.str();
    SendResponse(response, fd);
}

