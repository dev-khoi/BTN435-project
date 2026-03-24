#include <iostream>
#include <mutex>
#include <string>
#include <unordered_map>

struct ArpEntry
{
    std::string ip;
    std::string mac;
};

class ArpHandler
{
    std::unordered_map<std::string, std::string> table;
    std::mutex tableMutex;

public:
    ArpHandler()
    {
        preloadTable();
    }

    void preloadTable()
    {
        std::lock_guard<std::mutex> lock(tableMutex);

        table["192.168.10.10"] = "AA:BB:CC:10:10:10";  // Lighting node
        table["192.168.10.20"] = "AA:BB:CC:10:20:20";  // Lighting node
        table["192.168.10.70"] = "AA:BB:CC:20:70:70";  // Thermostat node
        table["192.168.10.100"] = "AA:BB:CC:30:10:00"; // Security camera

        std::cout << "table size: " << table.size() << "\n";
    }

    void addOrUpdate(const std::string &ip, const std::string &mac)
    {
        std::lock_guard<std::mutex> lock(tableMutex);
        table[ip] = mac;
    }

    std::string resolve(const std::string &ip)
    {
        std::lock_guard<std::mutex> lock(tableMutex);
        auto it = table.find(ip);
        if (it == table.end())
        {
            return "";
        }
        return it->second;
    }

    void printTable()
    {
        std::lock_guard<std::mutex> lock(tableMutex);
        std::cout << "\n[ARP] Current Table\n";
        std::cout << "IP Address        -> MAC Address\n";
        std::cout << "--------------------------------------\n";
        for (const auto &pair : table)
        {
            std::cout << pair.first << " -> " << pair.second << "\n";
        }
    }
};

std::string resolveIpToMac(const std::string &ip)
{
    static ArpHandler arp;
    return arp.resolve(ip);
}
