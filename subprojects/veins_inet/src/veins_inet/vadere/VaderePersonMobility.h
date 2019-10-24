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

#include "inet/mobility/base/MobilityBase.h"
#include "veins_inet/VeinsInetMobilityBase.h"
#include "veins_inet/veins_inet.h"

namespace veins {

class VaderePersonMobility : public VeinsInetMobilityBase{
public:
    VaderePersonMobility();
    virtual ~VaderePersonMobility();

    /** @brief called by class VeinsInetManager */
    virtual void preInitialize(std::shared_ptr<IMobileAgent> mobileAgent) override;

    /** @brief called by class VeinsInetManager */
    virtual void nextPosition(std::shared_ptr<IMobileAgent> mobileAgent) override;

    virtual TraCICommandInterface::Person* getPersonCommandInterface() const;
protected:
    mutable TraCICommandInterface::Person* personCommandInterface = nullptr; /**< cached value */

};

} /* namespace veins */

