/*
 * VeinsInetVehicleApplicationBase.cpp
 *
 *  Created on: Oct 2, 2019
 *      Author: stsc
 */

#include "veins_inet/VeinsInetVehicleApplicationBase.h"

namespace veins {

Define_Module(VeinsInetVehicleApplicationBase);

VeinsInetVehicleApplicationBase::VeinsInetVehicleApplicationBase() {}

VeinsInetVehicleApplicationBase::~VeinsInetVehicleApplicationBase() {}

void VeinsInetVehicleApplicationBase::initializeFromMobilityModule(){
    mobility = veins::VeinsInetMobilityAccess().get<VeinsInetMobility*>(getParentModule());
    traci = mobility->getCommandInterface();
    traciVehicle = mobility->getVehicleCommandInterface();
}

} /* namespace veins */
