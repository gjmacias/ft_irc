#include "AllFunctions.h"

/*
###############################################################################
#									PARSE									  #
###############################################################################
*/

void	parse_and_exec_cmd(std::string &cmd, int fd)
{
	if(cmd.empty())
		return ;

	std::vector<std::string>	splited_cmd = split_cmd(cmd);
	size_t						found = cmd.find_first_not_of(" \t\v");

	if(found != std::string::npos)
		cmd = cmd.substr(found);
	if(splited_cmd.size() && (splited_cmd[0] == "BONG" || splited_cmd[0] == "bong"))
		return;
    if(splited_cmd.size() && (splited_cmd[0] == "PASS" || splited_cmd[0] == "pass"))
        ClientAuthentification(fd, cmd);
	else if (splited_cmd.size() && (splited_cmd[0] == "NICK" || splited_cmd[0] == "nick"))
		set_nickname(cmd,fd);
	else if(splited_cmd.size() && (splited_cmd[0] == "USER" || splited_cmd[0] == "user"))
		set_username(cmd, fd);
	else if (splited_cmd.size() && (splited_cmd[0] == "QUIT" || splited_cmd[0] == "quit"))
		QUIT(cmd,fd);
	else if(notregistered(fd))
	{
		if (splited_cmd.size() && (splited_cmd[0] == "KICK" || splited_cmd[0] == "kick"))
			KICK(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "JOIN" || splited_cmd[0] == "join"))
			JOIN(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "TOPIC" || splited_cmd[0] == "topic"))
			Topic(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "MODE" || splited_cmd[0] == "mode"))
			mode_command(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "PART" || splited_cmd[0] == "part"))
			PART(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "PRIVMSG" || splited_cmd[0] == "privmsg"))
			PRIVMSG(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "INVITE" || splited_cmd[0] == "invite"))
			Invite(cmd,fd);
		else if (splited_cmd.size())
			_sendResponse(ERR_CMDNOTFOUND(GetClient(fd)->GetNickName(),splited_cmd[0]),fd);
	}
	else if (!notregistered(fd))
		_sendResponse(ERR_NOTREGISTERED(std::string("*")),fd);
}
/*
###############################################################################
#									UTILS									  #
###############################################################################
*/

std::vector<std::string>	split_recivedBuffer(std::string str)
{
	std::vector<std::string>	cmd;
	std::istringstream			stm(str);
	std::string					line;

	while(std::getline(stm, line))
	{
		size_t pos = line.find_first_of("\r\n");
		if(pos != std::string::npos)
			line = line.substr(0, pos);
		cmd.push_back(line);
	}
	return cmd;
}

std::vector<std::string>	split_cmd(std::string& cmd)
{
	std::vector<std::string>	splited_cmd;
	std::istringstream			stm(cmd);
	std::string					temp;

	while(stm >> temp)
	{
		splited_cmd.push_back(temp);
		temp.clear();
	}
	return splited_cmd;
}

bool	notregistered(int fd)
{
	if (!GetClient(fd) || GetClient(fd)->GetNickName().empty() 
		|| GetClient(fd)->GetUserName().empty() || GetClient(fd)->GetNickName() == "*"
		|| !GetClient(fd)->GetLogedIn())
		return false;
	return true;
}