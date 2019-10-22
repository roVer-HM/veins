/*
 * VeinsInetVehicleApplicationBase.cpp
 *
 *  Created on: Oct 2, 2019
 *      Author: stsc
 */

#include "veins_inet/VeinsInetPersonApplicationBase.h"


namespace veins {

Define_Module(VeinsInetPersonApplicationBase);

VeinsInetPersonApplicationBase::VeinsInetPersonApplicationBase() {}

VeinsInetPersonApplicationBase::~VeinsInetPersonApplicationBase() {}

void VeinsInetPersonApplicationBase::initializeFromMobilityModule(){
    mobility = veins::VeinsInetMobilityAccess().get<VeinsInetPersonMobility*>(getParentModule());
    traci = mobility->getCommandInterface();
    traciPerson = mobility->getPersonCommandInterface();
}

} /* namespace veins */
