

#ifndef SRC_PLEXE_PROTOCOLS_EDHOCPROTOCOL_H_
#define SRC_PLEXE_PROTOCOLS_EDHOCPROTOCOL_H_

#include "plexe/messages/edhocmsg_m.h"
#include "BaseProtocol.h"

namespace plexe {
class EdhocProtocol: public BaseProtocol {
public:
    EdhocProtocol() {
        iniNode1 = false;
    }
    virtual ~EdhocProtocol();
    simsignal_t receivedBitsSignal;
private:
  cMessage* edhocTimer;
  int edhocState;
  int myIndex;
protected:
  virtual void initialize(int stage) override;
  virtual void handleSelfMsg(cMessage* msg) override;
  virtual void handleLowerMsg(cMessage* msg) override;
  void sendEdhocMessage(int nb);
  bool iniNode1;


};

}  // namespace plexe

#endif /* SRC_PLEXE_PROTOCOLS_EDHOCPROTOCOL_H_ */
