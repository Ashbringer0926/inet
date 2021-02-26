//
// @authors: Enkhtuvshin Janchivnyambuu
//           Henning Puttnies
//           Peter Danielis
//           University of Rostock, Germany
// 

package inet.linklayer.ieee8021as;

import inet.node.inet.StandardHost;


module GPtpEndstation extends StandardHost
{
    parameters:
        clock.typename = default("SettableClock");
    submodules:
        gPtp: GPtp {
            @display("p=700.11755,75.166");
        }
    connections:
        gPtp.socketOut --> at.in++;
        at.out++ --> gPtp.socketIn;
}

