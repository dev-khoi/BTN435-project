#ifndef ROUTING_HANDLER_H
#define ROUTING_HANDLER_H

#include <cstdint>
#include <mutex>
#include <string>
#include <vector>

struct RouteEntry
{
    std::string network;
    int prefixLength;
    std::string nextHop;
    std::string interfaceName;
    int cost;
};

struct RouteDecision
{
    bool found;
    std::string nextHop;
    std::string interfaceName;
    std::string matchedNetwork;
    int prefixLength;
    int cost;
};

class RoutingHandler
{
public:
    RoutingHandler();

    void addRoute(const std::string &network, int prefixLength, const std::string &nextHop,
                  const std::string &interfaceName, int cost);
    std::vector<RouteEntry> getRoutes() const;
    RouteDecision findRoute(const std::string &destinationIp) const;
    std::string formatTable() const;

private:
    std::vector<RouteEntry> routes;
    mutable std::mutex routesMutex;

    static bool parseIpv4(const std::string &ip, std::uint32_t &out);
    static bool isPrefixMatch(std::uint32_t destination, std::uint32_t network, int prefixLength);
};

#endif
