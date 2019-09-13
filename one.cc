#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE("First script example");

int main(int argc, char *argv[])
{
	Time::SetResolution(Time::NS);
	LogComponentEnable("UdpEchoClientApplication",LOG_LEVEL_INFO);
	LogComponentEnable("UdpEchoServerApplication",LOG_LEVEL_INFO);
	
	NodeContainer nodes;
	nodes.Create(2);
	NodeContainer nodes1;
	nodes1.Add(nodes.Get(1));
	nodes1.Create(1);
	
	PointToPointHelper pointToPoint;
	pointToPoint.SetDeviceAttribute("DataRate",StringValue("5Mbps"));
	pointToPoint.SetChannelAttribute("Delay",StringValue("2ms"));
	
	PointToPointHelper pointToPoint1;
	pointToPoint1.SetDeviceAttribute("DataRate",StringValue("7Mbps"));
	pointToPoint1.SetChannelAttribute("Delay",StringValue("1ms"));
	
	NetDeviceContainer devices;
	devices=pointToPoint.Install(nodes);
	NetDeviceContainer devices1;
	devices1=pointToPoint1.Install(nodes1);
	
	InternetStackHelper stack;
	stack.Install(nodes);
	stack.Install(nodes1.Get(1));

	Ipv4AddressHelper address, address1;
	address.SetBase("10.1.1.0", "255.255.255.0");
	address1.SetBase("10.1.2.0", "255.255.255.0");
	
	Ipv4InterfaceContainer interfaces=address.Assign(devices);
	Ipv4InterfaceContainer interfaces1=address1.Assign(devices1);
	UdpEchoServerHelper echoServer(9);
	ApplicationContainer serverApps = echoServer.Install(nodes.Get(1));
	serverApps.Start(Seconds(1.0));
	serverApps.Stop(Seconds(10.0));
	
	UdpEchoClientHelper echoClient(interfaces.GetAddress(1),9);
	echoClient.SetAttribute("MaxPackets", UintegerValue(1));
	echoClient.SetAttribute("Interval",TimeValue(Seconds(1.0)));
	echoClient.SetAttribute("PacketSize", UintegerValue(1024));
	
	ApplicationContainer clientApps=echoClient.Install(nodes.Get(0));
	clientApps.Start(Seconds(2.0));
	clientApps.Stop(Seconds(10.0));
	
	UdpEchoServerHelper echoServer1(10);
	ApplicationContainer serverApps1=echoServer1.Install(nodes1.Get(0));
	serverApps1.Start(Seconds(1.0));
	serverApps1.Stop(Seconds(10.0));
	
	UdpEchoClientHelper echoClient1(interfaces1.GetAddress(0),10);
	echoClient1.SetAttribute("MaxPackets", UintegerValue(2));
	echoClient1.SetAttribute("Interval",TimeValue(Seconds(2.0)));
	echoClient1.SetAttribute("PacketSize", UintegerValue(512));
	
	ApplicationContainer clientApps1=echoClient1.Install(nodes1.Get(1));
	clientApps1.Start(Seconds(2.0));
	clientApps1.Stop(Seconds(10.0));
	
	Simulator::Run();
	Simulator::Destroy();
	return 0;
	
	
	
	
	
	
}