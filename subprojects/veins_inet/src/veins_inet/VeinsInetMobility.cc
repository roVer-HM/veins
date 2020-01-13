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

#include "veins_inet/VeinsInetMobility.h"

#include "inet/common/INETMath.h"
#include "inet/common/Units.h"
#include "inet/common/geometry/common/GeographicCoordinateSystem.h"

#include "veins/modules/mobility/traci/subscriptionManagement/RemoteSimulationObject.h"
#include "veins/modules/mobility/traci/subscriptionManagement/SumoVehicle.h"

using namespace veins::TraCISubscriptionManagement;

namespace veins {

using namespace inet::units::values;

Register_Class(VeinsInetMobility);

VeinsInetMobility::VeinsInetMobility()
{
}

VeinsInetMobility::~VeinsInetMobility()
{
    delete vehicleCommandInterface;
}

void VeinsInetMobility::preInitialize(std::shared_ptr<IMobileAgent> mobileAgent)
{
    std::shared_ptr<SumoVehicle> v = IMobileAgent::get<SumoVehicle>(mobileAgent);
    preInitialize(v->getId(), inet::Coord(v->getPosition().x, v->getPosition().y) , v->getRoadId(), v->getSpeed(), v->getHeading().getRad());
}

void VeinsInetMobility::preInitialize(std::string external_id, const inet::Coord& position, std::string road_id, double speed, double angle)
{
    Enter_Method_Silent();
    this->external_id = external_id;
    lastPosition = position;
    lastVelocity = inet::Coord(cos(angle), -sin(angle)) * speed;
    lastOrientation = inet::Quaternion(inet::EulerAngles(rad(-angle), rad(0.0), rad(0.0)));
}

void VeinsInetMobility::nextPosition(std::shared_ptr<IMobileAgent> mobileAgent)
{
    std::shared_ptr<SumoVehicle> v = IMobileAgent::get<SumoVehicle>(mobileAgent);
    nextPosition(inet::Coord(v->getPosition().x, v->getPosition().y), v->getRoadId(), v->getSpeed(), v->getHeading().getRad());
}

void VeinsInetMobility::nextPosition(const inet::Coord& position, std::string road_id, double speed, double angle)
{
    Enter_Method_Silent();

    lastPosition = position;
    lastVelocity = inet::Coord(cos(angle), -sin(angle)) * speed;
    lastOrientation = inet::Quaternion(inet::EulerAngles(rad(-angle), rad(0.0), rad(0.0)));

    // Update display string to show node is getting updates
    auto hostMod = getParentModule();
    if (std::string(hostMod->getDisplayString().getTagArg("veins", 0)) == ". ") {
        hostMod->getDisplayString().setTagArg("veins", 0, " .");
    }
    else {
        hostMod->getDisplayString().setTagArg("veins", 0, ". ");
    }

    emitMobilityStateChangedSignal();
}

TraCICommandInterface::Vehicle* VeinsInetMobility::getVehicleCommandInterface() const
{
    if (!vehicleCommandInterface) vehicleCommandInterface = new TraCICommandInterface::Vehicle(getCommandInterface()->vehicle(getExternalId()));
    return vehicleCommandInterface;
}

} // namespace veins
