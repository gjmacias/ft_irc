#include "Server.hpp"

/*
###############################################################################
#									PARSE									  #
###############################################################################
*/
void	Server::ParseAndExecute(std::string &cmd, int fd)
{
	if(cmd.empty())
		return ;

	std::vector<std::string>	splited_cmd = split_cmd(cmd);

	if(splited_cmd.size() == 0)
		return ;
	
	if(UpperCase(splited_cmd[0]) == "BONG")
		return;
	else if(UpperCase(splited_cmd[0]) == "PASS")
		ClientAuthentification(cmd, fd);
	else if (UpperCase(splited_cmd[0]) == "NICK")
		ClientNickname(cmd, fd);
	else if(UpperCase(splited_cmd[0]) == "USER")
		ClientUsername(splited_cmd, fd);
	else if (UpperCase(splited_cmd[0]) == "QUIT")
		QuitCommand(splited_cmd, fd);
	else if(IsRegisteredAndLoged(fd))
	{
		if (UpperCase(splited_cmd[0]) == "INVITE")
			InviteCommand(splited_cmd, fd);
		else if (UpperCase(splited_cmd[0]) == "JOIN")
			JoinCommand(splited_cmd, fd);
		else if (UpperCase(splited_cmd[0]) == "KICK")
			KickCommand(cmd, fd);
		else if (UpperCase(splited_cmd[0]) == "MODE")
			ModeCommand(splited_cmd, fd);
		else if (UpperCase(splited_cmd[0]) == "PART")
			PartCommand(splited_cmd, fd);
		else if (UpperCase(splited_cmd[0]) == "PRIVMSG")
			PrivateMessageCommand(cmd, fd);
		else if (UpperCase(splited_cmd[0]) == "TOPIC")
			TopicCommand(splited_cmd, fd);
		else if (splited_cmd.size())
			SendResponse(ERR_CMDNOTFOUND(GetClient(fd)->GetNickname(), splited_cmd[0]), fd);
	}
	else if (!IsRegisteredAndLoged(fd))
		SendResponse(ERR_NOTREGISTERED(std::string("*")), fd);
}
