//
// @authors: Enkhtuvshin Janchivnyambuu
//           Henning Puttnies
//           Peter Danielis
//           University of Rostock, Germany
// 

package inet.linklayer.ieee8021as;

import inet.node.ethernet.EthernetSwitch;


module GPtpBridge extends EthernetSwitch
{
    parameters:
        clock.typename = default("SettableClock");
        gPtp.gPtpNodeType = default("BRIDGE_NODE"); // @enum("gPtpNodeType"): MASTER_NODE, BRIDGE_NODE, SLAVE_NODE
        relayUnit.localDeliveryMacAddresses = default(hasStp ? ["01:80:C2:00:00:00", "01:80:C2:00:00:0E"] : ["01:80:C2:00:00:0E"]);
    submodules:
        gPtp: GPtp {
            @display("p=700,100");
        }
    connections:
        gPtp.socketOut --> nr.in++;
        nr.out++ --> gPtp.socketIn;
}

