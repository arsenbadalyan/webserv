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

std::string Util::joinSplittedPartsBy(std::string *splittedParts, size_t joinSize, char joinBy) {
	std::string result = "";
	size_t idx = 0;

	while (idx < joinSize)
	{
		if (splittedParts[idx].length()) {
			result += joinBy;
			result += splittedParts[idx];
		}
		idx++;
	}

	return (result);
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

std::string Util::getDateTime(std::time_t &currentTime) 
{
    std::ostringstream oss;
    std::tm* timeinfo = std::localtime(&currentTime);

    const char* weekdays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    oss << weekdays[timeinfo->tm_wday] << ", ";
    oss << std::setfill('0') << std::setw(2) << timeinfo->tm_mday << " ";
    oss << months[timeinfo->tm_mon] << " ";
    oss << 1900 + timeinfo->tm_year << " ";
    oss << std::setfill('0') << std::setw(2) << timeinfo->tm_hour << ":";
    oss << std::setfill('0') << std::setw(2) << timeinfo->tm_min << ":";
    oss << std::setfill('0') << std::setw(2) << timeinfo->tm_sec << " ";
    oss << "GMT";

    return oss.str();
}

std::string Util::getDateTime()
{
    std::time_t currentTime = std::time(NULL);
    return (getDateTime(currentTime));
}

std::string Util::checkFileLastModifyDate(const std::string& filepath)
{
    struct stat fileInfo;

    if (stat(filepath.c_str(), &fileInfo) != 0) 
		return ("");

    std::time_t lastModifiedTime = fileInfo.st_mtime;
    return (getDateTime(lastModifiedTime));

}

bool Util::isDirectory(const char *path) {
	DIR* dir = opendir(path);

	if (dir) {
		closedir(dir);
		return (true);
	}

	return (false);
}

std::string Util::extractFileType(const std::string & path) {
	SplitPair splittedPath = Util::split(path, '.');

	return (splittedPath.first[splittedPath.second - 1]);
}

std::string Util::generateDefaultErrorPage(const int16_t & statusCode) {
	std::ostringstream htmlStream;

    htmlStream << "<!DOCTYPE html>\n";
    htmlStream << "<html lang=\"en\">\n";
    htmlStream << "<head>\n";
    htmlStream << "    <meta charset=\"UTF-8\">\n";
    htmlStream << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    htmlStream << "    <title>Error</title>\n";
    htmlStream << "    <style>\n";
    htmlStream << "        body {\n";
    htmlStream << "            font-family: Arial, sans-serif;\n";
    htmlStream << "            margin: 0;\n";
    htmlStream << "            padding: 0;\n";
    htmlStream << "            display: flex;\n";
    htmlStream << "            justify-content: center;\n";
    htmlStream << "            align-items: center;\n";
    htmlStream << "            height: 100vh;\n";
    htmlStream << "            background-color: #f7f7f7;\n";
    htmlStream << "        }\n";
    htmlStream << "        .container {\n";
    htmlStream << "            text-align: center;\n";
    htmlStream << "        }\n";
    htmlStream << "        h1 {\n";
    htmlStream << "            color: #333;\n";
    htmlStream << "        }\n";
    htmlStream << "        p {\n";
    htmlStream << "            color: #777;\n";
    htmlStream << "            font-size: 18px;\n";
    htmlStream << "        }\n";
    htmlStream << "    </style>\n";
    htmlStream << "</head>\n";
    htmlStream << "<body>\n";
    htmlStream << "    <div class=\"container\">\n";
    htmlStream << "        <h1>" << statusCode << " " << HttpStatusCode::getStatusCode(statusCode) << "</h1>\n";
	htmlStream << "        <hr />\n";
    htmlStream << "        <p>" << RootConfigs::SERVER_NAME << "</p>\n";
    htmlStream << "    </div>\n";
    htmlStream << "</body>\n";
    htmlStream << "</html>\n";

    // Get the resulting HTML string
    std::string html = htmlStream.str();

	return (html);
}

std::string Util::intToString(int value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::string Util::removeAddnSlashes(std::string str) {
	std::string res = "";
	char prevChr = 0;

	for (size_t i = 0; i < str.length(); i++) {
		if (prevChr == '/' && str[i] == '/')
			continue ;
		prevChr = str[i];
		res += str[i];
	}

	return (res);
}