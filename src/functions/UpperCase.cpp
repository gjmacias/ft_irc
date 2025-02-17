#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

std::string	UpperCase(const std::string& str)
{
	std::string result = str;

	std::transform(result.begin(), result.end(), result.begin(), ::toupper);
	return (result);
}

