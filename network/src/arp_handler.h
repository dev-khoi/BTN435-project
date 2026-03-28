#ifndef ARP_HANDLER_H
#define ARP_HANDLER_H

#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

struct ArpEntry
{
    std::string ip;
    std::string mac;
    std::string interfaceName;
    std::string status;
};

class ArpHandler
{
public:
    ArpHandler();

    void addOrUpdate(const std::string &ip, const std::string &mac, const std::string &interfaceName,
                     const std::string &status = "Resolved");
    std::string resolve(const std::string &ip) const;
    std::vector<ArpEntry> getEntries() const;
    std::string formatTable() const;

private:
    std::unordered_map<std::string, ArpEntry> table;
    mutable std::mutex tableMutex;
};

#endif
