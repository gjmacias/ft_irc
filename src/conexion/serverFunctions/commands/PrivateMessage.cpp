#include "../../../../includes/conexion/Server.hpp"

/*
###############################################################################
#								PRIVATE_MESSAGE								  #
###############################################################################
*/

void	Server::PrivateMessageCommand(std::vector<std::string>& splited_cmd, std::string cmd_reason, int fd)
{
	size_t						it = 0;
	size_t						position = 0;
	std::vector<std::string>	list_recivers;
	std::stringstream			ss;

	if (splited_cmd.size() < 2)
	{
		SendResponse(ERR_NOTENOUGHPARAM(GetClient(fd)->GetNickname()), fd);
		return;
	}
	list_recivers = split_delimeter(splited_cmd[1], ',');
	if (list_recivers.size() > 10)
	{
		SendError(407, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), " :Too many recipients\r\n");
		return;
	}
	while (it < 2)
	{
		position = cmd_reason.find(splited_cmd[it]);
		cmd_reason = cmd_reason.substr(position + splited_cmd[it].size());
		position = cmd_reason.find_first_not_of("\t\v ");
		if (position != std::string::npos)
			cmd_reason = cmd_reason.substr(position);
		else
			cmd_reason.clear();
		it++;
	}
	if (cmd_reason.empty())//ERR_NOTEXTTOSEND (412) // if the client doesn't specify the message
	{
		SendError(412, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), " :No text to send\r\n");
		return;
	}
	for (position = 0; position < list_recivers.size(); position++)
	{
		ss.str("");
		ss << ":" << GetClient(fd)->GetHostname() << "@localhost PRIVMSG " << list_recivers[position] << " ";
		if (cmd_reason[0] == ':')
			ss << cmd_reason << "\r\n";
		else
			ss << splited_cmd[2] << "\r\n";
		if (GetChannel(list_recivers[position]) 
			&& GetChannel(list_recivers[position])->IsClientInChannel(GetClient(fd)->GetNickname()))
			(GetChannel(list_recivers[position]))->SendMeToAll(fd, ss.str());
		else if (GetClient_Nickname(list_recivers[position]))
			SendResponse(ss.str(), (GetClient_Nickname(list_recivers[position]))->GetFd());
		else
			SendError(401, fd, list_recivers[position], " :No such Nick/Channel\r\n");
	}
}