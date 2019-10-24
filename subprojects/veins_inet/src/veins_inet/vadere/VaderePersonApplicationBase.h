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

#pragma once

#include <vector>

#include "veins_inet/veins_inet.h"

#include "inet/common/INETDefs.h"
#include "veins_inet/VeinsInetApplicationBase.h"
#include "veins_inet/vadere/VaderePersonMobility.h"

namespace veins {

class VaderePersonApplicationBase : public VeinsInetApplicationBase  {
public:
    VaderePersonApplicationBase();
    virtual ~VaderePersonApplicationBase();

protected:

    virtual void initializeFromMobilityModule() override;

    veins::TraCICommandInterface::Person* traciPerson;
    veins::VaderePersonMobility* mobility;
    veins::TraCICommandInterface* traci;

};

} /* namespace veins */

