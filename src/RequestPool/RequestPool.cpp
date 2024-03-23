#include "RequestPool.hpp"

RequestPool::RequestPool(void) {}
RequestPool::RequestPool(const RequestPool &) {}
RequestPool::~RequestPool() {}
RequestPool& RequestPool::operator=(const RequestPool &) { return (*this); }

void RequestPool::newRequest(int clientSocket) {
    (void)clientSocket;

    
}