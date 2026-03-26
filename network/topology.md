# VLSM Topology

Example base network block: `192.168.10.0/24`

Formula to calculate usable hosts: 2^n - 2

We will split the `/24` into 3 subnets using VLSM (based on needs).

| Subnet | Purpose | Needed Hosts | CIDR | Subnet Mask | Network Address | Usable Host Range | Broadcast |
| --- | --- | --: | --- | --- | --- | --- | --- |
| Lighting | Smart bulbs / light controllers | 40 | `/26` | `255.255.255.192` | `192.168.10.0` | `192.168.10.1 - 192.168.10.62` | `192.168.10.63` |
| Thermostat | Thermostats / HVAC nodes | 20 | `/27` | `255.255.255.224` | `192.168.10.64` | `192.168.10.65 - 192.168.10.94` | `192.168.10.95` |
| Security | Cameras / sensors | 10 | `/28` | `255.255.255.240` | `192.168.10.96` | `192.168.10.97 - 192.168.10.110` | `192.168.10.111` |

Remaining space:

- `192.168.10.112 - 192.168.10.255`

## Gateway Assignment

- Lighting gateway: `192.168.10.1`
- Thermostat gateway: `192.168.10.65`
- Security gateway: `192.168.10.97`

## Logical Topology

- One Linux router or server host with 3 interfaces, one per subnet.
- Client or devices will stay in its own subnet, and send the traffic to their subnet gateway
- Client or devices in different subnet will be able to communicate as the routers or servers are the intermediary between subnets
