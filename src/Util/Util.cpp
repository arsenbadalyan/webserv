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