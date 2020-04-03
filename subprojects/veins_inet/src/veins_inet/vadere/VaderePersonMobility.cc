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

void VaderePersonMobility::preInitialize(std::shared_ptr<IMobileAgent> mobileAgent)
{
    Enter_Method_Silent();
    this->external_id = mobileAgent->getId();
    inet::Coord coord = inet::Coord(mobileAgent->getPosition().x, mobileAgent->getPosition().y);
    this->lastPosition = coord;
    double angle = mobileAgent->getAngel();
    this->lastVelocity = inet::Coord(cos(angle), -sin(angle)) * mobileAgent->getSpeed();
    this->lastOrientation = inet::Quaternion(inet::EulerAngles(rad(-angle), rad(0.0), rad(0.0)));
    emmitExternalId(mobileAgent->getId());
}

void VaderePersonMobility::nextPosition(std::shared_ptr<IMobileAgent> mobileAgent)
{
    Enter_Method_Silent();

    inet::Coord coord = inet::Coord(mobileAgent->getPosition().x, mobileAgent->getPosition().y);
    double angle = mobileAgent->getAngel();
    this->lastPosition = coord;
    this->lastVelocity = inet::Coord(cos(angle), -sin(angle)) * mobileAgent->getSpeed();
    this->lastOrientation = inet::Quaternion(inet::EulerAngles(rad(-angle), rad(0.0), rad(0.0)));

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


} /* namespace veins */
