//
// @authors: Enkhtuvshin Janchivnyambuu
//           Henning Puttnies
//           Peter Danielis
//           University of Rostock, Germany
// 

package inet.linklayer.ieee8021as;

module GPtpMaster extends GPtpEndstation
{
    parameters:
        @display("i=device/pc2");
        forwarding = default(false);  // disable routing by default
        networkLayer.proxyARP = default(false);

        gPtp.gPtpNodeType = "MASTER_NODE"; // @enum("gPtpNodeType"): MASTER_NODE, BRIDGE_NODE, SLAVE_NODE
    connections allowunconnected:
}
