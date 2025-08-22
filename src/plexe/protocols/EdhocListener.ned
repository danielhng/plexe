package org.car2x.plexe.protocols;

import org.car2x.plexe.protocols.BBaseProtocol;

simple EdhocListener extends BBaseProtocol
{
    parameters:
        @display("i=block/network2");
        @class(plexe::EdhocListener);

}