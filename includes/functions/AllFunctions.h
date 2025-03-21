#ifndef FUNCTIONS_H_
# define FUNCTIONS_H_

# include <vector>
# include <string>
# include <sstream>
# include <cstdlib>

//		sendRespose
void    SendResponse(std::string response, int fd);

//		Validations
bool 						isPortValid(std::string port);

//		Split
std::vector<std::string>	split_recivedBuffer(std::string str);
std::vector<std::string>	split_cmd(std::string& cmd);
std::vector<std::string>	split_delimeter(std::string& cmd, char c);

//		UpperCase
std::string					UpperCase(const std::string& str);

#endif

