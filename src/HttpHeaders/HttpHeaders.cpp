#include "HttpHeaders.hpp"

HttpHeaders::HttpHeaders(void) {}
HttpHeaders::HttpHeaders(const HttpHeaders &) {}
HttpHeaders::~HttpHeaders() { this->headers.clear(); }
void HttpHeaders::operator=(const HttpHeaders &) {}

const std::string* HttpHeaders::getHeader(const std::string key) const {
	HeadersMap::const_iterator foundedKey = this->headers.find(key);

	if (foundedKey == this->headers.end())
		return (NULL);

	return (&foundedKey->second);
}

HttpHeaders& HttpHeaders::setHeader(const std::string key, const std::string value) {
	this->headerValidator(key, value);
	if (this->headers.find(Util::toLower(key)) != this->headers.end()) {
		return (*this);
	}
	this->headers[key] = value;

	return (*this);
}

HttpHeaders& HttpHeaders::setHeader(std::string & line) {
	std::string key, value;
	size_t separatorPos;

	Util::trim(line, RootConfigs::Whitespaces);
	separatorPos = line.find(':');

	if (separatorPos == std::string::npos) {
		if (line.find_first_of(RootConfigs::Whitespaces) != std::string::npos) {
			ExceptionHandler::WrongHeader();
		}
		key = line;
		value = "";
	} else {
		key = line.substr(0, separatorPos);
		value = line.substr(separatorPos + 1, line.length());
	}

	return (this->setHeader(key, value));
}

void HttpHeaders::headerValidator(const std::string & key, const std::string & value) const {

	if (key.find_first_of(RootConfigs::InvalidHeaderKeys) != std::string::npos) {
		ExceptionHandler::InvalidHeaderKey();
	}

	(void)value;
}

std::string HttpHeaders::toString(void) {
	HeadersMap::iterator currentIt = this->headers.begin();
	HeadersMap::const_iterator endIt = this->headers.end();
	std::string result = "";

	while (currentIt != endIt) {
		result += currentIt->first + ": " + currentIt->second + "\r\n";
		currentIt++;
	}

	return (result);
}

// ------- Static Methods
std::map<std::string, std::string> HttpHeaders::getContentDispositionData(std::string input) {

	std::map<std::string, std::string> Data;
	SplitPair splittedInput = Util::split(input, ';');
	bool isFoundName = false, isFoundFileName = false;

	std::string *tempStrPtr = NULL;
	std::string tempStr = "";
	std::string *splittedInputArr = splittedInput.first;
	size_t splittedInputSize = splittedInput.second;

	while (splittedInputSize > 0) {
		splittedInputSize--;

		if (!isFoundFileName) {
			tempStrPtr = HttpHeaders::parseDispositionDataLinePart(splittedInputArr[splittedInputSize], "filename");
			if (tempStrPtr) {
				isFoundFileName = true;
				tempStr = *tempStrPtr;
				delete tempStrPtr;
				Data["filename"] = tempStr;
			}
			continue ;
		}

		if (!isFoundName) {
			tempStrPtr = HttpHeaders::parseDispositionDataLinePart(splittedInputArr[splittedInputSize], "name");
			if (tempStrPtr) {
				isFoundName = true;
				tempStr = *tempStrPtr;
				delete tempStrPtr;
				Data["name"] = tempStr;
			}
		}

	}

	if (!isFoundFileName || !isFoundName)
		throw std::runtime_error("Invalid parsing");

	return (Data);
}

// ---------- Static methods helpers
std::string* HttpHeaders::parseDispositionDataLinePart(std::string haystack, std::string needle) {
	size_t doubleQuoteStartPos = 0, doubleQuoteEndPos = 0;

	doubleQuoteStartPos = haystack.find(needle + "=\"");
	if (doubleQuoteEndPos != std::string::npos) {
		doubleQuoteStartPos += std::string(needle + "\"").length();
		doubleQuoteEndPos = haystack.find("\"", doubleQuoteStartPos + 1);

		if (doubleQuoteEndPos != std::string::npos) {
			return (new std::string(haystack.substr(doubleQuoteStartPos + 1, doubleQuoteEndPos - doubleQuoteStartPos - 1)));
		}
	}

	return (NULL);
}