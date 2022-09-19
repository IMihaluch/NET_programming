#include "client_udp.h"
#include "client.h"

int clientUDP()
{
    imp::Client client;

    client.createUDP(1991);
    client.start();

    return 0;
}
