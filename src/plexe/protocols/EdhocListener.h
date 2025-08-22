
#ifndef SRC_PLEXE_PROTOCOLS_EDHOCPROTOCOL_H_
#define SRC_PLEXE_PROTOCOLS_EDHOCPROTOCOL_H_

#include "plexe/messages/edhocmsg_m.h"
#include "BaseProtocol.h"

namespace plexe {
class EdhocListener: public BaseProtocol {
public:
    EdhocListener();
    virtual ~EdhocListener();
    simsignal_t receivedBitsSignal;
private:
  cMessage* edhocTimer;
  int edhocState;
  int myIndex;

protected:
  virtual void initialize(int stage) override;
  virtual void handleSelfMsg(cMessage* msg) override;
  virtual void handleLowerMsg(cMessage* msg) override;

};

}  // namespace plexe

#endif /* SRC_PLEXE_PROTOCOLS_EDHOCPROTOCOL_H_ */
