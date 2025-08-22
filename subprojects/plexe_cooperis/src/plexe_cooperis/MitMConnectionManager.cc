//
// Copyright (C) 2022-2024 Michele Segata <segata@ccs-labs.org>
//
// SPDX-License-Identifier: GPL-2.0-or-later
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Base files derived from Veins VLC by Agon Memedi and contributors

#include "plexe_cooperis/MitMConnectionManager.h"

#include <cmath>

#include "veins/base/modules/BaseWorldUtility.h"

Define_Module(plexe::MitMConnectionManager);

namespace plexe {

void MitMConnectionManager::initialize(int stage) {
    veins::BaseConnectionManager::initialize(stage);
    if (stage == 0) {
        EV << "ConnectionManager instance: NED=" << getNedTypeName()
                        << "  C++=" << getClassName() << "  path=" << getFullPath() << endl;
        // optional param to toggle your special policy
        usingtwoRis = par("usingtwoRis").boolValue();
        MaliciousRIS = par("MaliciousRIS").boolValue();

        RisHijacking = new cMessage("");
        scheduleAt(RisHijackingTime, RisHijacking);
    }
}

void MitMConnectionManager::handleMessage(cMessage* msg) {
    if (msg->isSelfMessage()) {
        handleSelfMsg(msg);
        return;
    }

    delete msg;
}

double MitMConnectionManager::calcInterfDist()
{
    /* With the introduction of antenna models, calculating the maximum
     * interference distance only based on free space loss doesn't make any sense
     * any more as it could also be much bigger due to positive antenna gains.
     * Therefore, the user has to provide a reasonable maximum interference
     * distance himself. */
    if (hasPar("maxInterfDist")) {
        double interfDistance = par("maxInterfDist").doubleValue();
        EV_INFO << "max interference distance:" << interfDistance << endl;
        return interfDistance;
    }
    else {
        throw cRuntimeError("ConnectionManager: No value for maximum interference distance (maxInterfDist) provided.");
    }
}

void MitMConnectionManager::handleSelfMsg(cMessage* msg)
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

bool MitMConnectionManager::isInRange(veins::BaseConnectionManager::NicEntries::mapped_type pFromNic,
        veins::BaseConnectionManager::NicEntries::mapped_type pToNic) {
    // Quick helpers
    EV << "[MitMConnectionManager] Entered isInRange on MitMConnectionManager" << endl;

    auto fullFrom = pFromNic->nicPtr->getFullPath();
    auto fullTo   = pToNic->nicPtr->getFullPath();

    auto isRIS = [](const std::string& s) {
        return s.find("ris[") != std::string::npos || s.find(".ris[") != std::string::npos;
    };
    auto isNode = [](const std::string& s) {
        return s.find("node[") != std::string::npos || s.find(".node[") != std::string::npos;
    };

    // 1) never connect node<->node
    if (isNode(fullFrom) && isNode(fullTo)) return false;

    if (usingtwoRis) {
        EV << "[MitMConnectionManager] Entered usingtwoRis = true" << endl;
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
                               if (a.find("node[1]") != std::string::npos && b.find("ris[0]") != std::string::npos) return true;
                              // if (a.find("node[1]") != std::string::npos && b.find("ris[1]") != std::string::npos) return false;
                               if (a.find("node[0]") != std::string::npos && b.find("ris[0]") != std::string::npos) return false;
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

    return veins::BaseConnectionManager::isInRange(pFromNic, pToNic);
}

}
