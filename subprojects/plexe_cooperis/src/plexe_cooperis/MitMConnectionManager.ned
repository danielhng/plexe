
package org.car2x.plexe.subprojects.plexe_cooperis;

import org.car2x.veins.base.connectionManager.ConnectionManager;

simple MitMConnectionManager extends ConnectionManager
{
    parameters:
        @class(plexe::MitMConnectionManager);
       	bool usingtwoRis = default(false);
        bool MaliciousRIS = default(false);
}

