#include "../../../../includes/conexion/Server.hpp"

void	Server::CapCommand(std::vector<std::string> &splited_cmd, int fd)
{
	if (splited_cmd[1] == "LS")
		SendResponse("CAP * LS :0\r\n", fd);
}

void	Server::PingCommand(std::vector<std::string> &splited_cmd, int fd)
{
	std::string	tmp = "\0";
	std::string	pong = "PONG ";

	if (splited_cmd.size() > 1)
		tmp = splited_cmd[1].append(ENDLINE);
	SendResponse(pong.append(tmp) + "\r\n", fd);
}

void	Server::WhoisCommand(std::vector<std::string> &splited_cmd, int fd)
{
	std::string	tmp = "\0";
	std::string	whois = "WHOIS ";

	if (splited_cmd.size() > 1)
		tmp = splited_cmd[1].append(ENDLINE);
	SendResponse(whois.append(tmp) + "\r\n", fd);
}