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
	HttpRequest *request = NULL;
	RequestPoolMap::iterator requestIt = this->requestPool.find(clientSocket);

	if (requestIt == this->requestPool.end()) {
		request = new HttpRequest(&server, clientSocket);
		this->requestPool[clientSocket] = request;
	} else {
		request = requestIt->second;
		request->prepareRead(clientSocket);
	}

	if (!request)
		ExceptionHandler::UnexpectedSendRequest();

	return (request->hasFinishedReceivingRequest());
}

bool RequestPool::getResponse(int clientSocket) {
	bool isFinishedWrite;

	if (this->responsePool.find(clientSocket) == this->responsePool.end()) {

		RequestPoolMap::iterator targetRequest = this->requestPool.find(clientSocket);

		if (targetRequest == this->requestPool.end())
			ExceptionHandler::CannotGetRequestToSendResponse();

		HttpResponse *response = new HttpResponse(targetRequest->second, targetRequest->first);

		this->responsePool[clientSocket] = response;
		isFinishedWrite = this->responsePool[clientSocket]->getResponse();
	} else {
		isFinishedWrite = this->responsePool[clientSocket]->sendResponse();
	}

	if (isFinishedWrite) {
		delete this->responsePool[clientSocket];
		this->responsePool.erase(clientSocket);
	}

	return (isFinishedWrite);
	
}

void RequestPool::destroyRequest(int clientSocket) {
	RequestPoolMap::iterator targetRequest = this->requestPool.find(clientSocket);

	if (targetRequest == this->requestPool.end())
		ExceptionHandler::CannotDestroyRequest();

	delete targetRequest->second;

	this->requestPool.erase(clientSocket);
}