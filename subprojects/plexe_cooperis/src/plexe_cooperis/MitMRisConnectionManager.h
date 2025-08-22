
#pragma once

#include "veins/base/connectionManager/BaseConnectionManager.h"

namespace plexe {

/**
 * @brief BaseConnectionManager implementation which only defines a
 * specific max interference distance.
 *
 * Calculates the maximum interference distance based on the transmitter
 * power, wavelength, pathloss coefficient and a threshold for the
 * minimal receive Power.
 *
 * @ingroup connectionManager
 */
class MitMRisConnectionManager : public veins::BaseConnectionManager {
public:
    virtual void initialize(int stage) override;
    MitMRisConnectionManager() {
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
    bool MaliciousRIS;
    bool usingtwoRis;
    virtual double calcInterfDist();
    bool isInRange(veins::BaseConnectionManager::NicEntries::mapped_type pFromNic,
                veins::BaseConnectionManager::NicEntries::mapped_type pToNic) override;

    virtual void handleSelfMsg(cMessage* msg);

    void forceReevaluation();
};

} // namespace plexe
