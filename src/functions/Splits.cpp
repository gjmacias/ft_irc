#include "AllFunctions.h"

/*
###############################################################################
#									SPLITS									  #
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