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
#include "veins_inet/VeinsInetPersonMobility.h"

namespace veins {

class VeinsInetPersonApplicationBase : public VeinsInetApplicationBase  {
public:
    VeinsInetPersonApplicationBase();
    virtual ~VeinsInetPersonApplicationBase();

protected:

    virtual void initializeFromMobilityModule() override;

    veins::TraCICommandInterface::Person* traciPerson;
    veins::VeinsInetPersonMobility* mobility;
    veins::TraCICommandInterface* traci;

};

} /* namespace veins */

