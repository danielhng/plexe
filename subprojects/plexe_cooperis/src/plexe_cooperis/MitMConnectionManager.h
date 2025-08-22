
#pragma once

#include "veins/base/connectionManager/BaseConnectionManager.h"
#include "veins/base/connectionManager/NicEntry.h"

namespace plexe {


class MitMConnectionManager : public veins::BaseConnectionManager {
public:
    virtual void initialize(int stage) override;
    MitMConnectionManager() {
        RisHijacking = nullptr;
        RisHijackingTime = SimTime(3);
        usingtwoRis = false;
        MaliciousRIS = false;
        RisHijacked = false;
    }


protected:
    void handleMessage(cMessage* msg) override;

    bool RisHijacked;
    cMessage* RisHijacking;
    SimTime RisHijackingTime;
    bool usingtwoRis;
    bool MaliciousRIS;
    double calcInterfDist() override;
    bool isInRange(veins::BaseConnectionManager::NicEntries::mapped_type pFromNic,
            veins::BaseConnectionManager::NicEntries::mapped_type pToNic) override;

    virtual void handleSelfMsg(cMessage* msg);
};

} // namespace veins
