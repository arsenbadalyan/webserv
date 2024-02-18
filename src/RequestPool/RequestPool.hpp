#ifndef __REQUEST_POOL__
#define __REQUEST_POOL__

class RequestPool {

    public:
        static void newRequest(int clientSocket);

    private:
        RequestPool(void);
        RequestPool(const RequestPool &);
        ~RequestPool();
        RequestPool& operator=(const RequestPool &);

};

#endif // !__REQUEST_POOL__