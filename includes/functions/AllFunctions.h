#ifndef FUNCTIONS_H_
# define FUNCTIONS_H_

# include <vector>
# include <string>
# include <sstream>

//		Validations
bool 						isPortValid(std::string port);

//		Split
std::vector<std::string>	split_recivedBuffer(std::string str);
std::vector<std::string>	split_cmd(std::string& cmd);
#endif