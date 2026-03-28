#include "routing_handler.h"

#include <algorithm>
#include <iomanip>
#include <limits>
#include <sstream>

RoutingHandler::RoutingHandler()
{
    std::lock_guard<std::mutex> lock(routesMutex);
    routes.clear();

    routes.push_back({"192.168.10.0", 26, "0.0.0.0", "eth0", 1});
    routes.push_back({"192.168.10.64", 27, "0.0.0.0", "eth1", 1});
    routes.push_back({"192.168.10.96", 28, "0.0.0.0", "eth2", 1});
}

std::vector<RouteEntry> RoutingHandler::getRoutes() const
{
    std::lock_guard<std::mutex> lock(routesMutex);
    std::vector<RouteEntry> out = routes;
    std::sort(out.begin(), out.end(), [](const RouteEntry &a, const RouteEntry &b)
              { return a.network < b.network; });
    return out;
}

RouteDecision RoutingHandler::findRoute(const std::string &destinationIp) const
{
    std::uint32_t destination = 0;
    if (!parseIpv4(destinationIp, destination))
    {
        return {false, "", "", "", -1, -1};
    }

    std::lock_guard<std::mutex> lock(routesMutex);

    const RouteEntry *best = nullptr;
    for (const RouteEntry &route : routes)
    {
        std::uint32_t network = 0;
        if (!parseIpv4(route.network, network))
        {
            continue;
        }

        if (!isPrefixMatch(destination, network, route.prefixLength))
        {
            continue;
        }

        if (best == nullptr || route.prefixLength > best->prefixLength ||
            (route.prefixLength == best->prefixLength && route.cost < best->cost))
        {
            best = &route;
        }
    }

    if (best == nullptr)
    {
        return {false, "", "", "", -1, -1};
    }

    return {true, best->nextHop, best->interfaceName, best->network, best->prefixLength, best->cost};
}

std::string RoutingHandler::formatTable() const
{
    const std::vector<RouteEntry> entries = getRoutes();

    std::ostringstream out;
    out << std::left
        << std::setw(17) << "Network"
        << std::setw(16) << "NextHop"
        << std::setw(12) << "Interface"
        << "Cost\n";

    for (const RouteEntry &entry : entries)
    {
        out << std::left
            << std::setw(17) << (entry.network + "/" + std::to_string(entry.prefixLength))
            << std::setw(16) << entry.nextHop
            << std::setw(12) << entry.interfaceName
            << entry.cost << "\n";
    }

    return out.str();
}

bool RoutingHandler::parseIpv4(const std::string &ip, std::uint32_t &out)
{
    std::stringstream ss(ip);
    std::string part;
    std::uint32_t value = 0;
    int count = 0;

    while (std::getline(ss, part, '.'))
    {
        if (part.empty())
        {
            return false;
        }

        int octet = -1;
        try
        {
            std::size_t parsed = 0;
            octet = std::stoi(part, &parsed);
            if (parsed != part.size())
            {
                return false;
            }
        }
        catch (...)
        {
            return false;
        }

        if (octet < 0 || octet > 255)
        {
            return false;
        }

        value = (value << 8) | static_cast<std::uint32_t>(octet);
        ++count;
    }

    if (count != 4)
    {
        return false;
    }

    out = value;
    return true;
}

bool RoutingHandler::isPrefixMatch(std::uint32_t destination, std::uint32_t network, int prefixLength)
{
    if (prefixLength < 0 || prefixLength > 32)
    {
        return false;
    }

    if (prefixLength == 0)
    {
        return true;
    }

    const std::uint32_t mask = 0xFFFFFFFFu << (32 - static_cast<std::uint32_t>(prefixLength));
    return (destination & mask) == (network & mask);
}
