/*
 * VeinsInetMobilityBase.cc
 *
 *  Created on: Oct 2, 2019
 *      Author: stsc
 */

#include "VeinsInetMobilityBase.h"

namespace veins {

Register_Abstract_Class(VeinsInetMobilityBase);
const simsignal_t VeinsInetMobilityBase::mobility_pre_ini = registerSignal("veins_inet_mobility_base_mobility_pre_init");


VeinsInetMobilityBase::VeinsInetMobilityBase() {
    // TODO Auto-generated constructor stub

}

VeinsInetMobilityBase::~VeinsInetMobilityBase() {
    // TODO Auto-generated destructor stub
}

void VeinsInetMobilityBase::emmitExternalId(const std::string id)
{
    try {
        double doubleId = std::stod(id);
        emit(mobility_pre_ini, doubleId);
    } catch (std::invalid_argument const &e){
        throw cRuntimeError("Cannot convert given id '%s' to long", id.c_str());
    } catch (std::out_of_range const &e){
        throw cRuntimeError("Given id '%s' out of range", id.c_str());
    }
}

void VeinsInetMobilityBase::initialize(int stage)
{
    MobilityBase::initialize(stage);

    // We patch the OMNeT++ Display String to set the initial position. Make sure this works.
    ASSERT(hasPar("initFromDisplayString") && par("initFromDisplayString"));
}


const inet::Coord& VeinsInetMobilityBase::getCurrentPosition()
{
    return lastPosition;
}

const inet::Coord& VeinsInetMobilityBase::getCurrentVelocity()
{
    return lastVelocity;
}

const inet::Coord& VeinsInetMobilityBase::getCurrentAcceleration()
{
    throw cRuntimeError("Invalid operation");
}

const inet::Quaternion& VeinsInetMobilityBase::getCurrentAngularPosition()
{
    return lastOrientation;
}

const inet::Quaternion& VeinsInetMobilityBase::getCurrentAngularVelocity()
{
    return lastAngularVelocity;
}

const inet::Quaternion& VeinsInetMobilityBase::getCurrentAngularAcceleration()
{
    throw cRuntimeError("Invalid operation");
}

void VeinsInetMobilityBase::setInitialPosition()
{
    subjectModule->getDisplayString().setTagArg("p", 0, lastPosition.x);
    subjectModule->getDisplayString().setTagArg("p", 1, lastPosition.y);
    MobilityBase::setInitialPosition();
}

void VeinsInetMobilityBase::handleSelfMessage(cMessage* message)
{
}

TraCIGenericScenarioManager* VeinsInetMobilityBase::getManager() const
{
    if (!manager) manager = TraCIScenarioManagerAccess().get();
    return manager;
}

TraCICommandInterface* VeinsInetMobilityBase::getCommandInterface() const
{
    if (!commandInterface) commandInterface = getManager()->getCommandInterface();
    return commandInterface;
}

void VeinsInetMobilityBase::setExternalId(std::string id)
{
    external_id = id;
}

std::string VeinsInetMobilityBase::getExternalId() const
{
    if (external_id == "") throw cRuntimeError("TraCIMobility::getExternalId called with no external_id set yet");
    return external_id;
}

} /* namespace veins */
