/*
 * VeinsInetVehicleApplicationBase.h
 *
 *  Created on: Oct 2, 2019
 *      Author: stsc
 */

#pragma once

#include <vector>

#include "veins_inet/veins_inet.h"

#include "inet/common/INETDefs.h"
#include "veins_inet/VeinsInetApplicationBase.h"

namespace veins {

class VeinsInetVehicleApplicationBase : public VeinsInetApplicationBase  {
public:
    VeinsInetVehicleApplicationBase();
    virtual ~VeinsInetVehicleApplicationBase();

protected:

    virtual void initializeFromMobilityModule() override;

    veins::TraCICommandInterface::Vehicle* traciVehicle;
    veins::VeinsInetMobility* mobility;
    veins::TraCICommandInterface* traci;
};

} /* namespace veins */

