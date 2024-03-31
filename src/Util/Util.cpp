#include <Util.hpp>

SplitPair Util::split(const std::string& str, char delimiter) {

	std::string token;
	std::stringstream ss(str);
	size_t size = 0, idx = 0;

	while (std::getline(ss, token, delimiter)) {
		size++;
	}

	std::string * dividedList = new std::string[size];

	ss.clear();
	ss.seekg(0);
	while (std::getline(ss, token, delimiter)) {
		dividedList[idx] = token;
		idx++;
	}

	return (SplitPair(dividedList, size));
}

std::string Util::toLower(const std::string& str) {
	std::string lowercasedStr = "";
	size_t idx = 0;

	while (idx < str.size()) {
		lowercasedStr += std::tolower(str[idx]);
		idx++;
	}

	return (lowercasedStr);
}

std::string& Util::trim(std::string& str, const std::string& trimmerStr) {
	size_t startPos = str.find_first_not_of(trimmerStr);
	size_t endPos = str.find_last_not_of(trimmerStr);

	if (startPos != std::string::npos && endPos != std::string::npos) {
		str = str.substr(startPos, endPos - startPos + 1);
	} else {
		str.clear();
	}
	return (str);
}

size_t Util::getFileSize(std::istream& is) {
	size_t size;

	is.seekg(0, std::ios::end);
	size = is.tellg();
	is.seekg(0, std::ios::beg);

	return (size);
}

bool Util::in(const std::string& s1, const std::string& s2)
{
	std::string name	= Util::toLower(s1);
	std::string fi 		= Util::toLower(s2);
	for (size_t i = 0; i <= name.size() - fi.size(); i++)
	{
		size_t j = 0;
		for (; j < fi.size(); ++j)
		{
			if(name[i + j] != fi[j])
			{
				break;
			}
		}
		if (j == fi.size())
			return (true);
	}
	return false;
}
