#include <fstream>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/ipv4-global-routing-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Third script example");

int main(int argc, char *argv[])
{
	double lat=2.0;
	uint64_t rate=5000000;
	double interval=0.05;
	NS_LOG_INFO("Create nodes");
	NodeContainer n;
	n.Create(2);
	
	NS_LOG_INFO("Create channels");
	PointToPointHelper p2p;  
	p2p.SetDeviceAttribute("DataRate",DataRateValue(DataRate(rate)));
	p2p.SetChannelAttribute("Delay",TimeValue(MilliSeconds(lat)));
	p2p.SetDeviceAttribute("Mtu",UintegerValue(1400));
	
	NetDeviceContainer dev=p2p.Install(n.Get(0), n.Get(1));

	InternetStackHelper internet;
	internet.Install(n);
	
	Ipv4AddressHelper ipv4;
	NS_LOG_INFO("Assign IP");
	ipv4.SetBase("10.1.1.0", "255.255.255.0");
	
	Ipv4InterfaceContainer i=ipv4.Assign(dev);
	
	Ipv4GlobalRoutingHelper::PopulateRoutingTables();
	NS_LOG_INFO("Create applications");
	
	uint16_t port1=8000;
	UdpServerHelper server1(port1);
	ApplicationContainer apps = server1.Install(n.Get(1));
	apps.Start(Seconds(1.0));
	apps.Stop(Seconds(10.0));
	
	uint32_t MaxPacketSize=1024;
	Time interPacketInterval=Seconds(interval);
	uint32_t maxPacketCount=320;
	
	UdpClientHelper client1(i.GetAddress(1),port1);
	client1.SetAttribute("MaxPackets", UintegerValue(maxPacketCount));
	client1.SetAttribute("Interval",TimeValue(interPacketInterval));
	client1.SetAttribute("PacketSize", UintegerValue(MaxPacketSize));
	
	apps=client1.Install(n.Get(0));
	apps.Start(Seconds(2.0));
	apps.Stop(Seconds(10.0));
	
	p2p.EnablePcap("lab-3",dev,false);
	
	FlowMonitorHelper flowmon;
	Ptr<FlowMonitor> monitor=flowmon.InstallAll();
	NS_LOG_INFO("Run Simulation");
	Simulator::Stop(Seconds(11.0));
	Simulator::Run();
	monitor->CheckForLostPackets();
	
	 Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ( ));
	 std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
	 for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
		 {
			 Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
			 std::cout<<"  t.sourceAddress="<<t.sourceAddress<<"  t.destinationAddress="<<t.destinationAddress<<" Source port:"<<t.sourcePort<<" " <<"Destination port:"<<t.destinationPort<<"\n";
			 if ((t.sourceAddress=="10.1.1.1" && t.destinationAddress == "10.1.1.2"))
				{
					std::cout << "Flow " << i->first  << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
					std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
					std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
					std::cout << "  Tx Packets: " << i->second.txPackets << "\n";
					std::cout << "  Rx Packets: " << i->second.rxPackets << "\n";
					std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / 9.0 / 1000 / 1000  << " Mbps\n";
				}
         }
  
	monitor->SerializeToXmlFile("lab-1.flowmon", true, true);
	Simulator::Destroy ();
	NS_LOG_INFO ("Done.");
	return 0;
}

/* OUTPUT
   
  t.sourceAddress=10.1.1.1  t.destinationAddress=10.1.1.2 Source port:49153 Destination port:8000
Flow 1 (10.1.1.1 -> 10.1.1.2)
  Tx Bytes:   168320
  Rx Bytes:   168320
  Tx Packets: 160
  Rx Packets: 160
  Throughput: 0.149618 Mbps

*/