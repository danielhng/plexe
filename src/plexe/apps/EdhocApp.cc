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
#include "EdhocApp.h"

#include "plexe/apps/BaseApp.h"
#include "veins/modules/messages/BaseFrame1609_4_m.h"
#include "veins/base/utils/FindModule.h"
#include "plexe/protocols/BaseProtocol.h"

using namespace veins;

namespace plexe {

Define_Module(EdhocApp);


void EdhocApp::initialize(int stage) {
        BaseApp::initialize(stage);
        if (stage == 1) {
            myIndex = getParentModule()->getIndex();
            EV << "[TEST]" << myIndex << endl;
            edhocTimer = new cMessage("edhocTimer");
            if (myIndex == 1) edhocState = 0; // initiator
            else edhocState = 1; // responder
            EV << "[TEST] EdhocApp initialized " << endl;
            SimTime rounded = SimTime(5, SIMTIME_S);
            scheduleAt(simTime() + rounded, edhocTimer);
        }
}

void EdhocApp::handleSelfMsg(cMessage* msg) {
    EV << "[EDHOC] Entered handleSelfMsg" << myIndex << endl;
    BaseApp::handleSelfMsg(msg);
    if (msg == edhocTimer) {
        switch (edhocState) {
            case 0:
                EV << "[TEST] Case 0 entered" << myIndex << endl;
                sendEdhocMessage("EDHOC 1");

                edhocState = 2;
                break;
            case 3:
                EV << "[TEST] Case 3 entered" << endl;

                sendEdhocMessage("EDHOC 2");
                edhocState = 5;
                break;
            case 4:
                EV << "[TEST] Case 4 entered" << endl;

                sendEdhocMessage("EDHOC 3");
                edhocState = 6;
                break;
            default:
                break;
        }
    }
    }

void EdhocApp::handleLowerMsg(cMessage* msg) {
    EV << "[TEST] Entered handleLowerMsg on" << myIndex << endl;

    BaseFrame1609_4* frame = check_and_cast<BaseFrame1609_4*>(msg);
    cPacket* payload = frame->decapsulate();

    ASSERT2(payload, "[TEST] Received a BaseFrame1609_4 with nothing inside");

    EV << "[TEST] Entered handleLowerMsg on" << myIndex << "content: " << payload << " source: "<< msg->getSenderModule() << " dest: " << msg->getArrivalModule() << "\n";
    // Try casting to EdhocMessage first
    if (auto* edhoc = dynamic_cast<EdhocMessage*>(payload)) {
        std::string content = edhoc->payload;
        EV << "[TEST] [" << myIndex << "] received: " << content << "\n";

        if (content == "EDHOC 1" && edhocState == 1) {
            edhocState = 3;
            scheduleAt(simTime() + SimTime(0.1, SIMTIME_S), edhocTimer);
        } else if (content == "EDHOC 2" && edhocState == 2) {
            edhocState = 4;
            scheduleAt(simTime() + SimTime(0.1, SIMTIME_S), edhocTimer);
        } else if (content == "EDHOC 3" && edhocState == 5) {
            EV << "[TEST] [" << myIndex << "] EDHOC exchange complete.\n";
        }

        delete edhoc;
    }
    // If not an EDHOC message, fall back to BaseApp behavior
    else if (payload->getKind() == BaseProtocol::BEACON_TYPE) {
        EV << "[TEST] Received Platooning Beacon (name: " << msg->getName() << ", Sender: " << msg->getSenderModuleId() << ")\n";
        onPlatoonBeacon(check_and_cast<PlatooningBeacon*>(payload));
    } else {
        EV_WARN << "[" << myIndex << "] Received unknown packet type\n";
        delete payload;
    }

    delete frame;
    }

void EdhocApp::sendEdhocMessage(const std::string& content) {
    EV << "[TEST] Entered sendEdhocMessage: " << content << endl;

    EdhocMessage* edhoc = new EdhocMessage("edhoc", content);
    edhoc->setByteLength(content.size());
    edhoc->setKind(BaseProtocol::BEACON_TYPE);
    sendFrame(edhoc, LAddress::L2BROADCAST());  // inherited from BaseApplLayer
}

EdhocApp::~EdhocApp() {
        cancelAndDelete(edhocTimer);
}

} // namespaec plexe

// namespace plexe
