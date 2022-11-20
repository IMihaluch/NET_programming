#ifndef FILESERVER_H
#define FILESERVER_H

#include "source.h"

inline void resource_access_log(const std::string&);

inline void resource_error_log(const std::string&);

class FileServer {
private:
    httpserver::webserver ws_;
    FileResource file_resources_;
public:
    FileServer() = delete;
    FileServer(const unsigned short, const std::string&, const std::string&);
};

#endif //FILESERVER_H
