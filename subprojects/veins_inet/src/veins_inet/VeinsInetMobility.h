//
// Copyright (C) 2006-2018 Christoph Sommer <sommer@ccs-labs.org>
//
// Documentation for these modules is at http://veins.car2x.org/
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

//
// Veins Mobility module for the INET Framework
// Based on inet::MovingMobilityBase of INET Framework v4.0.0
//

#pragma once

namespace omnetpp {
}
using namespace omnetpp;

#include "veins_inet/veins_inet.h"
#include "veins_inet/VeinsInetMobilityBase.h"

#include "veins/modules/mobility/traci/TraCIScenarioManager.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"
#include "veins/modules/mobility/traci/subscriptionManagement/RemoteSimulationObject.h"

using veins::TraCISubscriptionManagement::IMobileAgent;
namespace veins {

class VEINS_INET_API VeinsInetMobility : public VeinsInetMobilityBase {
public:
    VeinsInetMobility();

    virtual ~VeinsInetMobility();

    /** @brief called by class VeinsInetManager */
    virtual void preInitialize(std::shared_ptr<IMobileAgent> mobileAgent) override;
    virtual void preInitialize(std::string external_id, const inet::Coord& position, std::string road_id, double speed, double angle);

    /** @brief called by class VeinsInetManager */
    virtual void nextPosition(std::shared_ptr<IMobileAgent> mobileAgent) override;
    virtual void nextPosition(const inet::Coord& position, std::string road_id, double speed, double angle);


    virtual TraCICommandInterface::Vehicle* getVehicleCommandInterface() const;

protected:
    mutable TraCICommandInterface::Vehicle* vehicleCommandInterface = nullptr; /**< cached value */

};

} // namespace veins

namespace veins {
class VEINS_INET_API VeinsInetMobilityAccess {
public:
    template<typename T>
    T get(cModule* host)
    {
        T m = FindModule<T>::findSubModule(host);
        ASSERT(m);
        return m;
    };
};
} // namespace veins

