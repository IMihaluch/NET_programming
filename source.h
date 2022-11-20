
#ifndef LIBHTTPSERVER_FILESTORAGE_H
#define LIBHTTPSERVER_FILESTORAGE_H

#include "httpserver.hpp"
#include <thread>
#include <iostream>

const std::string AUTH_FILE{ "auth.txt" };

struct Login{
    std::string user;
    std::string password;

    const bool operator==(const Login &other) const
    {
        return this->user == other.user && this->password == other.password;
    }
};


class source : public httpserver::http_resource{
private:
    std::vector<Login> logins_;

    void inflateLoginBase();
    const bool authenticate(const Login&) const;

public:
    source();

    const std::shared_ptr<httpserver::http_response>
        render(const httpserver::http_request&);
};

#endif //LIBHTTPSERVER_FILESTORAGE_H
