
#include "plexe_cooperis/MitMRisConnectionManager.h"

#include <cmath>

#include "veins/base/modules/BaseWorldUtility.h"

Define_Module(plexe::MitMRisConnectionManager);

namespace plexe {

void MitMRisConnectionManager::initialize(int stage) {
    veins::BaseConnectionManager::initialize(stage);
    if (stage == 0) {
        EV << "ConnectionManager instance: NED=" << getNedTypeName()
                        << "  C++=" << getClassName() << "  path=" << getFullPath() << endl;

        usingtwoRis = par("usingtwoRis").boolValue();
        MaliciousRIS = par("MaliciousRIS").boolValue();


        RisHijacking = new cMessage("");
        scheduleAt(RisHijackingTime, RisHijacking);
    }
}

double MitMRisConnectionManager::calcInterfDist()
{
    return 1000;
}

void MitMRisConnectionManager::handleMessage(cMessage* msg) {
    if (msg->isSelfMessage()) {
        handleSelfMsg(msg);
        return;
    }

    delete msg;
}

void MitMRisConnectionManager::handleSelfMsg(cMessage* msg)
{

    if (msg == RisHijacking) {
       RisHijacked = true;
       for (int i = 0; i < 2; i++) {
                   cModule* ris = getParentModule()->getSubmodule("ris", i);
                   if (!ris) continue;
                   cModule* nic = ris->getSubmodule("nic", 0);
                   if (nic) {
                       unregisterNic(nic);
                   }
               }
    }
}

bool MitMRisConnectionManager::isInRange(veins::BaseConnectionManager::NicEntries::mapped_type pFromNic,
            veins::BaseConnectionManager::NicEntries::mapped_type pToNic) {
    EV << "[MitM Ris Connection Manager] Entered isInRange " << endl;
    // Quick helpers

    auto fullFrom = pFromNic->nicPtr->getFullPath();
    auto fullTo   = pToNic->nicPtr->getFullPath();

    auto isRIS = [](const std::string& s) {
        return s.find("ris[") != std::string::npos || s.find(".ris[") != std::string::npos;
    };
    auto isNode = [](const std::string& s) {
        return s.find("node[") != std::string::npos || s.find(".node[") != std::string::npos;
    };

    // never connect node<->node
    if (isNode(fullFrom) && isNode(fullTo)) return false;

    if (usingtwoRis) {
        EV << "[MitMRisConnectionManager] Entered usingtwoRis = true" << endl;
        auto allowPair = [&](const std::string& a, const std::string& b) {
            if (a.find("node[1]") != std::string::npos && b.find("ris[0]") != std::string::npos) return true;
            if (a.find("node[1]") != std::string::npos && b.find("ris[1]") != std::string::npos) return true;
            if (a.find("node[0]") != std::string::npos && b.find("ris[0]") != std::string::npos) return true;
            if (a.find("node[2]") != std::string::npos && b.find("ris[1]") != std::string::npos) return true;
            return false;
        };
        // symmetric check
        if (allowPair(fullFrom, fullTo) || allowPair(fullTo, fullFrom)) {
            // also respect the original distance check
            return veins::BaseConnectionManager::isInRange(pFromNic, pToNic);
        }
        return false;
    }

    if (MaliciousRIS) {
            EV << "[MitMConnectionManager] Entered MaliciousRIS = true" << endl;

            if (RisHijacked) {
                        auto allowPair = [&](const std::string& a, const std::string& b) {
                                           if (a.find("node[1]") != std::string::npos && b.find("ris[0]") != std::string::npos) return false;
                                          // if (a.find("node[1]") != std::string::npos && b.find("ris[1]") != std::string::npos) return false;
                                           if (a.find("node[0]") != std::string::npos && b.find("ris[0]") != std::string::npos) return true;
                                          // if (a.find("node[0]") != std::string::npos && b.find("ris[1]") != std::string::npos) return false;
                                          // if (a.find("node[2]") != std::string::npos && b.find("ris[1]") != std::string::npos) return true;
                                           if (a.find("node[2]") != std::string::npos && b.find("ris[0]") != std::string::npos) return true;
                                          // if (a.find("ris[1]") != std::string::npos && b.find("ris[0]") != std::string::npos) return true;

                                           return false;
                                       };
                                       if (allowPair(fullFrom, fullTo) || allowPair(fullTo, fullFrom)) {
                                           // also respect the original distance check
                                           return veins::BaseConnectionManager::isInRange(pFromNic, pToNic);
                                       }
                                       return false;
                    } else {

                        auto allowPair = [&](const std::string& a, const std::string& b) {
                                   if (a.find("node[1]") != std::string::npos && b.find("ris[0]") != std::string::npos) return true;
                                  // if (a.find("node[1]") != std::string::npos && b.find("ris[1]") != std::string::npos) return false;
                                   if (a.find("node[0]") != std::string::npos && b.find("ris[0]") != std::string::npos) return true;
                                  // if (a.find("node[0]") != std::string::npos && b.find("ris[1]") != std::string::npos) return false;
                                  // if (a.find("node[2]") != std::string::npos && b.find("ris[1]") != std::string::npos) return false;
                                   if (a.find("node[2]") != std::string::npos && b.find("ris[0]") != std::string::npos) return false;
                                  // if (a.find("ris[1]") != std::string::npos && b.find("ris[0]") != std::string::npos) return true;

                                   return false;
                               };
                       if (allowPair(fullFrom, fullTo) || allowPair(fullTo, fullFrom)) {
                           // also respect the original distance check
                           return veins::BaseConnectionManager::isInRange(pFromNic, pToNic);
                       }
                       return false;
            }
        }
    //default: let Veins decide
    return veins::BaseConnectionManager::isInRange(pFromNic, pToNic);
}

}

