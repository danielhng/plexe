//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "EdhocProtocol.h"


#include "veins/modules/mac/ieee80211p/Mac1609_4.h"
#include "veins/base/utils/FindModule.h"
#include "veins/modules/messages/BaseFrame1609_4_m.h"

#include "plexe/PlexeManager.h"
#include "plexe/driver/Veins11pRadioDriver.h"
#include "plexe/messages/PlexeInterfaceControlInfo_m.h"

using namespace veins;

namespace plexe {

Define_Module(EdhocProtocol)

void EdhocProtocol::initialize(int stage) {
    BaseProtocol::initialize(stage);

        if (stage == 1) {
            iniNode1 = par("iniNode1").boolValue();
            receivedBitsSignal = registerSignal("receivedBits");
            myIndex = getParentModule()->getIndex();
            EV << "[TEST]" << myIndex << endl;
            edhocTimer = new cMessage("edhocTimer");
            if (iniNode1) {
                if (myIndex == 1) edhocState = 0; // initiator
                else edhocState = 1; // responder
            } else {
                if (myIndex == 0) edhocState = 0; // initiator
                else edhocState = 1; // responder
            }
            EV << "[TEST] EdhocApp initialized " << endl;
            SimTime rounded = SimTime(1, SIMTIME_S);
            scheduleAt(simTime() + rounded, edhocTimer);
        }
}

void EdhocProtocol::handleSelfMsg(cMessage* msg) {
    EV << "[EDHOC] Entered handleSelfMsg" << myIndex << "Message: " << msg->getName() << endl;
    BaseProtocol::handleSelfMsg(msg);

    SimTime next = SimTime(1, SIMTIME_S);
    if (msg == edhocTimer) {
        switch (edhocState) {
            case 0:
                EV << "[TEST] Case 0 entered" << myIndex << endl;
                sendEdhocMessage(1);
                edhocState = 2;
                break;
            case 3:
                EV << "[TEST] Case 3 entered" << endl;

                sendEdhocMessage(2);
                edhocState = 5;
                break;
            case 4:
                EV << "[TEST] Case 4 entered" << endl;

                sendEdhocMessage(3);
                edhocState = 0;

                scheduleAt(simTime() + next, edhocTimer);
                break;
            default:
                break;
        }
    }
}

void EdhocProtocol::handleLowerMsg(cMessage* msg) {

    BaseFrame1609_4* frame = check_and_cast<BaseFrame1609_4*>(msg);
    ASSERT2(frame, "received a frame not of type BaseFrame1609_4");

    cPacket* enc = frame->getEncapsulatedPacket();
    ASSERT2(enc, "received a BaseFrame1609_4 with nothing inside");

    EV << "[TEST] Entered handleLowerMsg on" << myIndex << "content: " << enc << " source: "<< msg->getSenderModule() << " dest: " << msg->getArrivalModule() << "\n";
    // Try casting to EdhocMessage first
    if (edhocmsg* epkt = dynamic_cast<edhocmsg*>(enc)) {
        EV << "[PACKET SIZE]" << frame->getByteLength() << "* 16" << endl;
        emit(receivedBitsSignal, frame->getByteLength() * 8);
        int nb = epkt->getPacketnb();
        EV << "[TEST] [" << myIndex << "] received: edhoc" << nb << " edhoc state: " << edhocState << "\n";

        if (nb == 1 && edhocState == 1) {
            edhocState = 3;
            scheduleAt(simTime() + SimTime(0.1, SIMTIME_S), edhocTimer);
        } else if (nb = 2 && edhocState == 2) {
            edhocState = 4;
            scheduleAt(simTime() + SimTime(0.1, SIMTIME_S), edhocTimer);
        } else if (nb = 3 && edhocState == 5) {
            EV << "[TEST] [" << myIndex << "] EDHOC exchange complete.\n";
            edhocState = 1;
        }

    } else {
        EV_WARN << "[" << myIndex << "] Received unknown packet type\n";
    }


    delete frame;
    EV << "frame deleted" << endl;

}


void EdhocProtocol::sendEdhocMessage(int nb) {
    EV << "[TEST] Entered sendEdhocMessage: " << nb << endl;

    edhocmsg* edhoc = new edhocmsg();
    edhoc->setPacketnb(nb);
    //edhoc->setKind(BaseProtocol::BEACON_TYPE);

    auto wsm = veins::make_unique<BaseFrame1609_4>("", BaseProtocol::BEACON_TYPE);
    wsm->setRecipientAddress(1);
    wsm->setChannelNumber(static_cast<int>(Channel::cch));
    wsm->setUserPriority(2);
    wsm->encapsulate(edhoc);

    sendTo(wsm.release(), PlexeRadioInterfaces::COOPERIS);  // inherited from BaseProtocol
}


EdhocProtocol::~EdhocProtocol() {
    // TODO Auto-generated destructor stub
}
}
