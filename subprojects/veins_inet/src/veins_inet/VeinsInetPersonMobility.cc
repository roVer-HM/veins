/*
 * inetMobilityBase.cc
 *
 *  Created on: Oct 2, 2019
 *      Author: stsc
 */

#include "VeinsInetPersonMobility.h"

#include "inet/common/INETMath.h"
#include "inet/common/Units.h"
#include "inet/common/geometry/common/GeographicCoordinateSystem.h"

namespace veins {

using namespace inet::units::values;

Register_Class(VeinsInetPersonMobility);

VeinsInetPersonMobility::VeinsInetPersonMobility()
{
}

VeinsInetPersonMobility::~VeinsInetPersonMobility()
{
    delete personCommandInterface;
}

void VeinsInetPersonMobility::preInitialize(std::shared_ptr<IMobileAgent> mobileAgent)
{
    Enter_Method_Silent();
    this->external_id = mobileAgent->getId();
    inet::Coord coord = inet::Coord(mobileAgent->getPosition().x, mobileAgent->getPosition().y);
    this->lastPosition = coord;
    double angle = mobileAgent->getAngel();
    this->lastVelocity = inet::Coord(cos(angle), -sin(angle)) * mobileAgent->getSpeed();
    this->lastOrientation = inet::Quaternion(inet::EulerAngles(rad(-angle), rad(0.0), rad(0.0)));
}

void VeinsInetPersonMobility::nextPosition(std::shared_ptr<IMobileAgent> mobileAgent)
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

TraCICommandInterface::Person* VeinsInetPersonMobility::getPersonCommandInterface() const
{
    if (!personCommandInterface) personCommandInterface = new TraCICommandInterface::Person(getCommandInterface()->person(getExternalId()));
    return personCommandInterface;
}

} /* namespace veins */
