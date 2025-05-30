#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lte-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace std;
using namespace ns3;

int main(int argc, char *argv[])
{
    int nUE = 2;
    CommandLine cmd(__FILE__);
    cmd.AddValue("nUE", "Number of users", nUE);
    cmd.Parse(argc, argv);
    
    Ptr<LteHelper> lte = CreateObject<LteHelper>();
    NodeContainer enbNodes;
    enbNodes.Create(1);
    NodeContainer ueNodes;
    ueNodes.Create(nUE);


    MobilityHelper mobility;
    
    mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX",
                                  DoubleValue(0.0),
                                  "MinY",
                                  DoubleValue(0.0),
                                  "DeltaX",
                                  DoubleValue(5.0),
                                  "DeltaY",
                                  DoubleValue(10.0),
                                  "GridWidth",
                                  UintegerValue(3),
                                  "LayoutType",
                                  StringValue("RowFirst"));
    mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
                              "Bounds",
                              RectangleValue(Rectangle(-50, 50, -50, 50)));
    mobility.Install(ueNodes);


    Ptr<ListPositionAllocator> ENBposition = CreateObject<ListPositionAllocator> ();
    ENBposition->Add(Vector (0.0,0.0,0.0));
    mobility.SetPositionAllocator(ENBposition);
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(enbNodes);


    lte->SetSchedulerType("ns3::PfFfMacScheduler");


    NetDeviceContainer enbDevices;
    enbDevices = lte->InstallEnbDevice(enbNodes);
    NetDeviceContainer ueDevices;
    ueDevices = lte->InstallUeDevice(ueNodes);


    lte->Attach(ueDevices, enbDevices.Get(0));
    enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
    EpsBearer bearer(q);
    lte->ActivateDataRadioBearer(ueDevices, bearer);

 
    lte->EnableMacTraces();
    lte->EnableRlcTraces();
  
    Simulator::Stop(Seconds(10));
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}