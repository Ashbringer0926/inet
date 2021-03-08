//
// @authors: Enkhtuvshin Janchivnyambuu
//           Henning Puttnies
//           Peter Danielis
//           University of Rostock, Germany
// 

package inet.linklayer.ieee8021as;

import inet.node.ethernet.EthernetSwitch;


module GptpBridge extends EthernetSwitch
{
    parameters:
        clock.typename = default("SettableClock");
        gptp.gptpNodeType = default("BRIDGE_NODE"); // @enum("gptpNodeType"): MASTER_NODE, BRIDGE_NODE, SLAVE_NODE
        relayUnit.localDeliveryMacAddresses = default(hasStp ? ["01:80:C2:00:00:00", "01:80:C2:00:00:0E"] : ["01:80:C2:00:00:0E"]);
    submodules:
        gptp: Gptp {
            @display("p=700,100");
        }
    connections:
        gptp.socketOut --> nr.in++;
        nr.out++ --> gptp.socketIn;
}

