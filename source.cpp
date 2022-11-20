#include "source.h"
#include <fstream>

source::source(): logins_{} {
    std::cout << "\n********** Started ****************\n" ;

    inflateLoginBase();

    std::cout << "Main listening thread id: " << std::this_thread::get_id() << std::endl;
}

//------------------------------------------------------------------------------

const std::shared_ptr<httpserver::http_response> source::render
        (const httpserver::http_request& req)
{

    std::string client { req.get_requestor() + ":"
        + std::to_string(req.get_requestor_port()) };

    std::string client_request{ req.get_path() };

    std::cout << "\nClient:\n";
    std::cout << "\t" << client << std::endl;

    std::cout << "\nHandling in thread id: ";
    std::cout << std::this_thread::get_id() << std::endl;

    Login login { req.get_user(), req.get_pass() };

    if (client_request == "/@exit@" && authenticate(login))
    {
        std::cout << "Got \'EXIT\' command from client " << client << std::endl;
        std::exit(1);
    }

    if(authenticate(login)) {

        std::ifstream file{ client_request };
        if(client_request != "/" && file.is_open()) {
            file.close();
            std::cout << "Sending file " << client_request << " to "
                      << client << std::endl;
            return std::shared_ptr<httpserver::http_response>
                    (new httpserver::file_response(
                            client_request));
        }
        else
        {
            std::cout << "File " << client_request << " cannot be sent " <<
                "(not found or we do not have read permission).\n";
            return std::shared_ptr<httpserver::string_response>
                    ( new httpserver::string_response
                        ("Not found.",
                         404, "text/plain"));
        }
    }
    else {
        std::cout << "Drop client " << client << std::endl;
        return
                std::shared_ptr<httpserver::basic_auth_fail_response>
                        (new httpserver::basic_auth_fail_response(
                                "FAIL", "supervisor@geekbrains.ru"));
    }
}

void source::inflateLoginBase() {
    std::cout << "Checking logins database (" << AUTH_FILE << ")..";
    std::ifstream auth_file{AUTH_FILE };
    if(!auth_file.is_open())
    {
        std::cout << "Cant open file " << AUTH_FILE
                  << std::endl;
        return;
    }

    std::stringstream file_stream;
    file_stream << auth_file.rdbuf();

    while(file_stream.good())
    {
        std::string username, password;
        file_stream >> username >> password;
        logins_.push_back({username, password});
    }
    std::cout << ".. OK\n";
}

const bool source::authenticate(const Login& login) const {
    if(logins_.empty()) {
        std::cout
         << "This configuration of server doesnt support login authentication.\n";
        return false;
    }

    for (const auto &elem : logins_)
    {
        if (login == elem) {
            std::cout << "Authentication is OK\n";
            return true;
        }
    }

    std::cout << "Authentication failed.\n";
    return false;
}
