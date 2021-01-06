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

#include "VaderePersonMobility.h"

#include "inet/common/INETMath.h"
#include "inet/common/Units.h"
#include "inet/common/geometry/common/GeographicCoordinateSystem.h"

namespace veins {

using namespace inet::units::values;

Register_Class(VaderePersonMobility);

VaderePersonMobility::VaderePersonMobility() {}

VaderePersonMobility::~VaderePersonMobility() {
    if (personCommandInterface)
        delete personCommandInterface;
}

void VaderePersonMobility::setInitialPosition(){
    // lastPosition correctly set during preInitialize
    // using the display string to set initialPosition will cast
    // the positions from int to double thus loosing significant position
    // accuracy of +/- 0.5 m. This is to much for pedestrians.
    // todo: FIX ME
    EV_DEBUG << "do not use DisplayString" << endl;
    if (par("updateDisplayString"))
        updateDisplayStringFromMobilityState();
}

void VaderePersonMobility::preInitialize(std::shared_ptr<IMobileAgent> mobileAgent)
{
    Enter_Method_Silent();
    this->external_id = mobileAgent->getId();
    inet::Coord coord = inet::Coord(mobileAgent->getPosition().x, mobileAgent->getPosition().y);

    this->nextPosTime = mobileAgent->getTime();
    this->nextPos = coord;

    this->lastUpdate = simTime(); //setup set last and next position to same value.
    this->lastPosition = coord;
    this->lastVelocity = inet::Coord(); // velocity 0 when initialized.
    this->lastOrientation = inet::Quaternion();
    emmitExternalId(mobileAgent->getId());
}

void VaderePersonMobility::nextPosition(std::shared_ptr<IMobileAgent> mobileAgent)
{
    Enter_Method_Silent();

    inet::Coord coord = inet::Coord(mobileAgent->getPosition().x, mobileAgent->getPosition().y);

    //
    ASSERT(this->nextPosTime == simTime());
    ASSERT(simTime() < mobileAgent->getTime());

    this->lastPosition = this->nextPos;
    this->lastUpdate = this->nextPosTime;

    this->nextPos = coord;
    this->nextPosTime = mobileAgent->getTime();

    this->lastVelocity = (nextPos - lastPosition) / (nextPosTime - lastUpdate).dbl();
    inet::Coord direction = this->lastVelocity;
    direction.normalize();
    auto alpha = rad(atan2(direction.y, direction.x));
    auto beta = rad(-asin(direction.z));
    auto gamma = rad(0.0);
    this->lastOrientation = inet::Quaternion(inet::EulerAngles(alpha, beta, gamma));

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

VaderePersonItfc* VaderePersonMobility::getPersonCommandInterface() const
{
    if (!personCommandInterface) personCommandInterface = new VaderePersonItfc(getCommandInterface(), getExternalId());
    return personCommandInterface;
}

const inet::Coord& VaderePersonMobility::getCurrentPosition()
{
    moveAndUpdate();
    return lastPosition;
}

const inet::Coord& VaderePersonMobility::getCurrentVelocity()
{
    moveAndUpdate();
    return lastVelocity;
}

const inet::Coord& VaderePersonMobility::getCurrentAcceleration()
{

    throw cRuntimeError("Invalid operation");
}

const inet::Quaternion& VaderePersonMobility::getCurrentAngularPosition()
{
    moveAndUpdate();
    return lastOrientation;
}

const inet::Quaternion& VaderePersonMobility::getCurrentAngularVelocity()
{
    moveAndUpdate();
    return lastAngularVelocity;
}

const inet::Quaternion& VaderePersonMobility::getCurrentAngularAcceleration()
{
    throw cRuntimeError("Invalid operation");
}

void VaderePersonMobility::moveAndUpdate()
{
    simtime_t now = simTime();
    if (lastUpdate != now) {
        move();
        lastUpdate = simTime();
        emitMobilityStateChangedSignal();
    }
}

void VaderePersonMobility::move(){
    double elapsedTime = (simTime() - lastUpdate).dbl();
    lastPosition += lastVelocity * elapsedTime;
}



} /* namespace veins */
