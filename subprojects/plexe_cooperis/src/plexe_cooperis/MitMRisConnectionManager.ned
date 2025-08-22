
package org.car2x.plexe.subprojects.plexe_cooperis;

simple MitMRisConnectionManager
{
    parameters:
        @class(plexe::MitMRisConnectionManager);
        // debug switch for core framework
        bool coreDebug = default(false);
        // send directly to the node or create separate gates for every connection
        bool sendDirect = default(true);
        // name of the radioIn gate to be searched for
        string radioInGate = default("radioIn");
        // minimum carrier frequency of the channel [Hz]
//        double carrierFrequency @unit(Hz);
        // should the maximum interference distance be displayed for each node?
        bool drawMaxIntfDist = default(false);
        bool usingtwoRis = default(false);
        bool MaliciousRIS = default(false);
        
        @display("i=abstract/multicast");
}

