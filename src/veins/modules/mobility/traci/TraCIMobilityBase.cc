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

#include <limits>
#include <iostream>
#include <sstream>

#include "veins/modules/mobility/traci/TraCIMobilityBase.h"

using namespace veins;
using namespace veins::TraCISubscriptionManagement;

using veins::TraCIMobilityBase;

Define_Module(veins::TraCIMobilityBase);

void TraCIMobilityBase::initialize(int stage)
{
    if (stage == 0) {
        BaseMobility::initialize(stage);

        hostPositionOffset = par("hostPositionOffset");

        ASSERT(isPreInitialized);
        isPreInitialized = false;

        Coord nextPos = calculateHostPosition(roadPosition);
        nextPos.z = move.getStartPosition().z;

        move.setStart(nextPos);
        move.setDirectionByVector(heading.toCoord());
        move.setOrientationByVector(heading.toCoord());
        if (this->setHostSpeed) {
            move.setSpeed(speed);
        }


        manager = nullptr;
        last_speed = -1;

    }
    else if (stage == 1) {
        // don't call BaseMobility::initialize(stage) -- our parent will take care to call changePosition later
    }
    else {
        BaseMobility::initialize(stage);
    }
}

void TraCIMobilityBase::finish()
{
    isPreInitialized = false;
}

void TraCIMobilityBase::handleSelfMsg(cMessage* msg)
{
    EV << "TraCIMobilityBase: handleSelfMsg not redefined; delete msg\n";
    delete msg;

}

void TraCIMobilityBase::preInitialize(std::shared_ptr<IMobileAgent> mobileAgent){
    this->external_id = mobileAgent->getId();
    this->lastUpdate = 0;
    this->roadPosition = mobileAgent->getPosition();
    this->speed = mobileAgent->getSpeed();
    this->heading = mobileAgent->getHeading();
    this->hostPositionOffset = par("hostPositionOffset");

    Coord nextPos = calculateHostPosition(roadPosition);
    nextPos.z = move.getStartPosition().z;

    move.setStart(nextPos);
    move.setDirectionByVector(heading.toCoord());
    move.setOrientationByVector(heading.toCoord());
    if (this->setHostSpeed) {
        move.setSpeed(speed);
    }

    isPreInitialized = true;
}

void TraCIMobilityBase::nextPosition(std::shared_ptr<IMobileAgent> mobileAgent)
{
    isPreInitialized = false;
    this->roadPosition = mobileAgent->getPosition();
    this->speed = mobileAgent->getSpeed();
    this->heading = mobileAgent->getHeading();
    EV_DEBUG << "nextPosition " << roadPosition.x << " " << roadPosition.y << " " << " " << speed << " " << heading << std::endl;

    changePosition();
    ASSERT(getCurrentDirection() == heading.toCoord() and getCurrentDirection() == getCurrentOrientation());
}

void TraCIMobilityBase::changePosition()
{
    // ensure we're not called twice in one time step
    ASSERT(lastUpdate != simTime());

    Coord nextPos = calculateHostPosition(roadPosition);
    nextPos.z = move.getStartPosition().z;


    this->lastUpdate = simTime();

    // Update display string to show node is getting updates
    auto hostMod = getParentModule();
    if (std::string(hostMod->getDisplayString().getTagArg("veins", 0)) == ". ") {
        hostMod->getDisplayString().setTagArg("veins", 0, " .");
    }
    else {
        hostMod->getDisplayString().setTagArg("veins", 0, ". ");
    }

    move.setStart(Coord(nextPos.x, nextPos.y, move.getStartPosition().z)); // keep z position
    move.setDirectionByVector(heading.toCoord());
    move.setOrientationByVector(heading.toCoord());
    if (this->setHostSpeed) {
        move.setSpeed(speed);
    }
    fixIfHostGetsOutside();
    updatePosition();
}

void TraCIMobilityBase::fixIfHostGetsOutside()
{
    Coord pos = move.getStartPos();
    Coord dummy = Coord::ZERO;
    double dum;

    bool outsideX = (pos.x < 0) || (pos.x >= playgroundSizeX());
    bool outsideY = (pos.y < 0) || (pos.y >= playgroundSizeY());
    bool outsideZ = (!world->use2D()) && ((pos.z < 0) || (pos.z >= playgroundSizeZ()));
    if (outsideX || outsideY || outsideZ) {
        throw cRuntimeError("Tried moving host to (%f, %f) which is outside the playground", pos.x, pos.y);
    }

    handleIfOutside(RAISEERROR, pos, dummy, dummy, dum);
}

Coord TraCIMobilityBase::calculateHostPosition(const Coord& vehiclePos) const
{
    Coord corPos;
    if (hostPositionOffset >= 0.001) {
        // calculate antenna position of vehicle according to antenna offset
        corPos = vehiclePos - (heading.toCoord() * hostPositionOffset);
    }
    else {
        corPos = vehiclePos;
    }
    return corPos;
}
