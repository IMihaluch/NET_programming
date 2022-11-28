#ifndef SMESSENG_H
#define SMESSENG_H

#include "ResCreator.h"

inline void resource_access_log(const std::string&);

inline void resource_error_log(const std::string&);

class MessengerServer {
private:
    httpserver::webserver ws_;
    ResponsesCreator file_resources_;
public:
    MessengerServer() = delete;
    MessengerServer(const unsigned short, const std::string&, const std::string&);
};


#endif //SMESSENG_H
