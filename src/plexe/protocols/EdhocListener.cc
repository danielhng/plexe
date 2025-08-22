
#include "EdhocListener.h"


#include "veins/modules/mac/ieee80211p/Mac1609_4.h"
#include "veins/base/utils/FindModule.h"
#include "veins/modules/messages/BaseFrame1609_4_m.h"

#include "plexe/PlexeManager.h"
#include "plexe/driver/Veins11pRadioDriver.h"
#include "plexe/messages/PlexeInterfaceControlInfo_m.h"

using namespace veins;

namespace plexe {

Define_Module(EdhocListener)

void EdhocListener::initialize(int stage) {
    BaseProtocol::initialize(stage);

        if (stage == 1) {
            receivedBitsSignal = registerSignal("receivedBits");
            myIndex = getParentModule()->getIndex();
            EV << "[TEST]" << myIndex << endl;
            EV << "[TEST] Edhoc Listener initialized " << endl;
        }
}

void EdhocListener::handleSelfMsg(cMessage* msg) {
    EV << "[EDHOC] Entered handleSelfMsg" << myIndex << "Message: " << msg->getName() << endl;
    BaseProtocol::handleSelfMsg(msg);
}

void EdhocListener::handleLowerMsg(cMessage* msg) {

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
        EV << "[TEST] [" << myIndex << " EDHOC LISTENER] received: edhoc" << nb << " edhoc state: " << edhocState << "\n";
        EV << "[Listener] Listening Completed " << endl;

    } else {
        EV_WARN << "[" << myIndex << "] Received unknown packet type\n";
    }


    delete frame;
    EV << "frame deleted" << endl;

}


EdhocListener::EdhocListener() {
    // TODO Auto-generated constructor stub

}

EdhocListener::~EdhocListener() {
    // TODO Auto-generated destructor stub
}
}
