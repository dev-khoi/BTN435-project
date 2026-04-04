#include "arp_handler.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

// ArpHandler stores an IP-to-MAC table with interface and status fields. It is protected by tableMutex 
ArpHandler::ArpHandler()
{
    std::lock_guard<std::mutex> lock(tableMutex);
    table.clear();

    // Preload resolved entries to simulate ARP table population at startup.
    table["192.168.1.10"] = {"192.168.1.10", "AA:BB:CC:DD:EE:01", "eth0", "Resolved"};
    table["192.168.2.5"] = {"192.168.2.5", "AA:BB:CC:DD:EE:02", "eth1", "Resolved"};
    table["192.168.3.15"] = {"192.168.3.15", "AA:BB:CC:DD:EE:03", "eth1", "Resolved"};
}

// addOrUpdate() and resolve() support dynamic lookup/update.
void ArpHandler::addOrUpdate(const std::string &ip, const std::string &mac, const std::string &interfaceName,
                             const std::string &status)
{
    // Thread-safe insert/update for dynamic ARP changes.
    std::lock_guard<std::mutex> lock(tableMutex);
    table[ip] = {ip, mac, interfaceName, status};
}
std::string ArpHandler::resolve(const std::string &ip) const
{
    // Thread-safe lookup: resolve IP to MAC.
    std::lock_guard<std::mutex> lock(tableMutex);
    auto it = table.find(ip);
    if (it == table.end())
    {
        return "";
    }
    return it->second.mac;
}

std::vector<ArpEntry> ArpHandler::getEntries() const
{
    std::lock_guard<std::mutex> lock(tableMutex);
    std::vector<ArpEntry> entries;
    entries.reserve(table.size());

    for (const auto &kv : table)
    {
        entries.push_back(kv.second);
    }

    std::sort(entries.begin(), entries.end(), [](const ArpEntry &a, const ArpEntry &b)
              { return a.ip < b.ip; });
    return entries;
}

// formatTable() generates aligned terminal output for GET /network/arp requests 
std::string ArpHandler::formatTable() const
{
    const std::vector<ArpEntry> entries = getEntries();

    std::ostringstream out;
    // Generate aligned output used by GET /network/arp responses.
    out << "ARP Table:\n";
    out << std::left
        << std::setw(17) << "IP Address"
        << std::setw(22) << "MAC Address"
        << std::setw(12) << "Interface"
        << "Status\n";

    for (const ArpEntry &entry : entries)
    {
        out << std::left
            << std::setw(17) << entry.ip
            << std::setw(22) << entry.mac
            << std::setw(12) << entry.interfaceName
            << entry.status << "\n";
    }

    return out.str();
}
