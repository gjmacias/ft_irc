#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
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

std::vector<std::string>	split_cmd(std::string &cmd)
{
	std::vector<std::string>	splited_cmd;
	std::istringstream			stm(cmd);
	std::string					temp;

	std::cout << YELLOW << "[DEBUG] Empieza el spliteo! " << cmd << WHITE << std::endl;
	while(stm >> temp)
	{
		splited_cmd.push_back(temp);
		temp.clear();
	}
	std::cout << YELLOW << "[DEBUG] Acaba el Spliteo! " << cmd << WHITE << std::endl;
	return splited_cmd;
}

