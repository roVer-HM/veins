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

#pragma once

#include <string>
#include <fstream>
#include <list>
#include <stdexcept>

#include "veins/base/modules/BaseMobility.h"
#include "veins/base/utils/FindModule.h"
#include "veins/modules/mobility/traci/subscriptionManagement/RemoteSimulationObject.h"
#include "veins/modules/mobility/traci/TraCIScenarioManager.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"


using veins::TraCISubscriptionManagement::IMobileAgent;

namespace veins {

/**
 * @brief
 * Used in modules created by the TraCIScenarioManager.
 *
 * This module relies on the TraCIScenarioManager for state updates
 * and can not be used on its own.
 *
 * This is a generic base independent of the actual agent (car, pedestrian)

 * @see TraCIMobility for Sumo vehicle agent.
 *
 * @author Christoph Sommer, David Eckhoff, Luca Bedogni, Bastian Halmos, Stefan Joerer, Stefan Schuhbäck (refactoring)
 *
 * @see TraCIScenarioManager
 * @see TraCIScenarioManagerLaunchd
 *
 * @ingroup mobility
 */
class VEINS_API TraCIMobilityBase : public BaseMobility {
public:


    TraCIMobilityBase()
        : BaseMobility()
        , isPreInitialized(false)
        , manager(nullptr)
        , commandInterface(nullptr)
    {
    }
    ~TraCIMobilityBase() override
    {
    }
    void initialize(int) override;
    void finish() override;

    void handleSelfMsg(cMessage* msg) override;
    virtual void preInitialize(std::shared_ptr<IMobileAgent> mobileAgent);
    virtual void nextPosition(std::shared_ptr<IMobileAgent> mobileAgent);
    virtual void changePosition();
//    virtual void changeParkingState(bool);
    virtual void setExternalId(std::string external_id)
    {
        this->external_id = external_id;
    }
    virtual std::string getExternalId() const
    {
        if (external_id == "") throw cRuntimeError("TraCIMobility::getExternalId called with no external_id set yet");
        return external_id;
    }
    virtual double getHostPositionOffset() const
    {
        return hostPositionOffset;
    }
    virtual double getSpeed() const
    {
        if (speed == -1) throw cRuntimeError("TraCIMobility::getSpeed called with no speed set yet");
        return speed;
    }
    /**
     * returns heading.
     */
    virtual Heading getHeading() const
    {
        if (heading.isNan()) throw cRuntimeError("TraCIMobility::getHeading called with no heading set yet");
        return heading;
    }


    virtual TraCIGenericScenarioManager* getManager() const
    {
        if (!manager) manager = TraCIScenarioManagerAccess().get();
        return manager;
    }
    virtual TraCICommandInterface* getCommandInterface() const
    {
        if (!commandInterface) commandInterface = getManager()->getCommandInterface();
        return commandInterface;
    }

    /**
     * Returns the speed of the host (likely 0 if setHostSpeed==false)
     */
    Coord getHostSpeed() const
    {
        return BaseMobility::getCurrentSpeed();
    }


protected:

    bool isPreInitialized; /**< true if preInitialize() has been called immediately before initialize() */

    std::string external_id; /**< updated by setExternalId() */
    double hostPositionOffset; /**< front offset for the antenna of this agent */
    bool setHostSpeed; /**< whether to update the speed of the host (along with its position)  */

    simtime_t lastUpdate; /**< updated by nextPosition() */
    Coord roadPosition; /**< position of the agent (front), updated by nextPosition() */
    double speed; /**< updated by nextPosition() */
    Heading heading; /**< updated by nextPosition() */

    mutable TraCIGenericScenarioManager* manager;
    mutable TraCICommandInterface* commandInterface;
    double last_speed;


    void fixIfHostGetsOutside() override; /**< called after each read to check for (and handle) invalid positions */


    /**
     * Calculates where the OMNeT++ module position of this agent should be, given its front bumper position
     */
    Coord calculateHostPosition(const Coord& vehiclePos) const;

    /**
     * Calling this method on pointers of type TraCIMobility is deprecated in favor of calling either getHostSpeed or getSpeed.
     */
    Coord getCurrentSpeed() const override
    {
        return BaseMobility::getCurrentSpeed();
    }

};

class VEINS_API TraCIMobilityBaseAccess {
public:
    TraCIMobilityBaseAccess* get(cModule* host)
    {
        TraCIMobilityBaseAccess* traci = FindModule<TraCIMobilityBaseAccess*>::findSubModule(host);
        ASSERT(traci);
        return traci;
    };
};

} // namespace veins
