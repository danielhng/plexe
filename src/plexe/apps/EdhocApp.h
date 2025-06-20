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

#ifndef __PLEXE_EDHOCAPP_H_
#define __PLEXE_EDHOCAPP_H_

#include "plexe/apps/BaseApp.h"
#include "plexe/apps/EdhocMessage.h"

#include <string>

namespace plexe {


class EdhocApp : public BaseApp {
  private:
    cMessage* edhocTimer;
    int edhocState;
    int myIndex;

  protected:
    virtual void initialize(int stage) override;
    virtual void handleSelfMsg(cMessage* msg) override;
    virtual void handleLowerMsg(cMessage* msg) override;
    void sendEdhocMessage(const std::string& content);

  public:
    virtual ~EdhocApp() override;

};

} // namespace plexe

#endif
