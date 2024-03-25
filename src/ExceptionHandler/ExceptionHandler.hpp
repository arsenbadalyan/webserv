#ifndef __EXCEPTION_HANDLER__HPP__
#define __EXCEPTION_HANDLER__HPP__

#include <iostream>

class ExceptionHandler {

    public:
        static void WrongHeader(void);    
        static void InvalidHeaderKey(void);
        static void UnexpectedSendRequest(void);
        static void CannotGetRequestToSendResponse(void);
        static void CannotDestroyRequest(void);

};

#endif // !__EXCEPTION_HANDLER__HPP__