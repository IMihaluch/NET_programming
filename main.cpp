#include <iostream>
#include <iomanip>
#include <PcapLiveDeviceList.h>
#include <unistd.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <IPv4Layer.h>

std::string getProtocolTypeAsString(pcpp::ProtocolType protocolType)
{
    switch (protocolType)
    {
        case pcpp::Ethernet:
            return "Ethernet";
        case pcpp::IPv4:
            return "IPv4";
        case pcpp::TCP:
            return "TCP";
        case pcpp::HTTPRequest:
        case pcpp::HTTPResponse:
            return "HTTP";
        default:
            return "Unknown";
    }
}

static bool onPacketArrivesBlockingMode(pcpp::RawPacket *packet, pcpp::PcapLiveDevice *dev, void *cookie)
{
    pcpp::Packet parsedPacket(packet);

    for (pcpp::Layer *curLayer = parsedPacket.getFirstLayer(); curLayer != NULL; curLayer = curLayer->getNextLayer())
    {
        std::cout << "Тип слоя: " << getProtocolTypeAsString(curLayer->getProtocol()).c_str()
                << "; Общие данные: " << (int) curLayer->getDataLen()
                << " [байт]; Данные слоя: " << (int) curLayer->getHeaderLen()
                << " [байт]; Полезная нагрузка слоя: " << (int) curLayer->getLayerPayloadSize()
                << " [байт]" << std::endl;
    }

    pcpp::IPv4Layer *ipLayer = parsedPacket.getLayerOfType<pcpp::IPv4Layer>();
    if (ipLayer == nullptr)
    {
        std::cerr << "Что-то пошло не так, не удалось найти уровень IPv4" << std::endl;
        exit(4);
    }

    std::cout << "\tIP-адрес источника: " << ipLayer->getSrcIPv4Address().toString() << std::endl
            << "\tIP-адрес назначения: " << ipLayer->getDstIPv4Address().toString() << std::endl
            << "\tIP ID: " << ntohs(ipLayer->getIPv4Header()->ipId) << std::endl
            << "\tTTL: " << static_cast<int>(ipLayer->getIPv4Header()->timeToLive) << std::endl << std::endl;

    return false;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Необходимо указать IP адрес в качестве аргумента" << std::endl;
        exit(1);
    }

    std::string interfaceIPAddr = argv[1];
    pcpp::PcapLiveDevice *dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIp(interfaceIPAddr.c_str());
    if (dev == nullptr)
    {
        std::cerr << "Не удается найти интерфейс с IPv4-адресом " << interfaceIPAddr.c_str() << std::endl;
        exit(2);
    }

    std::cout << "Информация о сетевом интерфейсе: " << std::endl
            << "\tИнтерфейс: " << dev->getName() << std::endl
            << "\tОписание: " << dev->getDesc() << std::endl
            << "\tMAC адрес: " << dev->getMacAddress().toString() << std::endl
            << "\tШлюз: " << dev->getDefaultGateway().toString() << std::endl;
    if (dev->getDnsServers().size() > 0)
        std::cout << "\tDNS сервер: " << dev->getDnsServers().at(0).toString() << std::endl;

    std::cout << std::endl;

    if (!dev->open())
    {
        std::cerr << "Не удается открыть устройство" << std::endl;
        exit(3);
    }

    dev->startCaptureBlockingMode(onPacketArrivesBlockingMode, nullptr, 0);

    return 0;
}
