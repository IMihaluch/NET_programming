#include "Fileloader.h"


int main(int argc, char** argv){
    if (argc != 6)
    {
        std::cout << "Usage: " << "<host> <port> <user> <password> <file-to-get>\n";
        std::cout << "Or, if u want to close remote server app, issue:\n";
        std::cout << "<host> <port> <user> <password> /@exit@\n";

        return EXIT_FAILURE;
    }

    const RemoteResource remote_resource
        {argv[1], argv[2], argv[3], argv[4], argv[5] };

    Fileloader file_loader{ remote_resource };
    if(file_loader.hasSuccess())
    {
        file_loader.saveFetchedPayload();
    }

    return EXIT_SUCCESS;
}
