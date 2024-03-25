#include "RequestPool.hpp"

RequestPool::RequestPool(const RequestPool &) {}
RequestPool& RequestPool::operator=(const RequestPool &) { return (*this); }

RequestPool::RequestPool(void) {}

RequestPool::~RequestPool() {
	RequestPoolMap::iterator currentRequest = this->requestPool.begin();;
	RequestPoolMap::iterator requestsEnd = this->requestPool.end();

	for (; currentRequest != requestsEnd; currentRequest++) {
		delete currentRequest->second;
	}
}

bool RequestPool::sendRequest(int clientSocket, Server & server) {
	std::cout << "SOCKET: " << clientSocket << std::endl;
	HttpRequest *request = NULL;
	RequestPoolMap::iterator requestIt = this->requestPool.find(clientSocket);

	if (requestIt == this->requestPool.end()) {
		request = new HttpRequest(clientSocket);
		this->requestPool[clientSocket] = request;
	} else {
		request = requestIt->second;
		// TODO: continue reading request
	}

	if (!request)
		ExceptionHandler::UnexpectedSendRequest();

	return (request->hasFinishedReceivingRequest());
}

void RequestPool::getResponse(int clientSocket) {
	std::cout << "GET RESPONSE SOCKET: " << clientSocket << std::endl;
	RequestPoolMap::iterator targetRequest = this->requestPool.find(clientSocket);

	if (targetRequest == this->requestPool.end())
		ExceptionHandler::CannotGetRequestToSendResponse();

	HttpResponse::getResponse(*targetRequest->second, targetRequest->first);
}

void RequestPool::destroyRequest(int clientSocket) {
	RequestPoolMap::iterator targetRequest = this->requestPool.find(clientSocket);

	if (targetRequest == this->requestPool.end())
		ExceptionHandler::CannotDestroyRequest();

	delete targetRequest->second;

	this->requestPool.erase(clientSocket);
}