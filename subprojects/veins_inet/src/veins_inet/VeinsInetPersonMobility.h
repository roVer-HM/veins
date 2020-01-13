/*
 * VeinsInetPersonMobility.h
 *
 *  Created on: Oct 2, 2019
 *      Author: stsc
 */

#pragma once

#include "inet/mobility/base/MobilityBase.h"
#include "veins_inet/VeinsInetMobilityBase.h"

#include "veins_inet/veins_inet.h"

namespace veins {


class VeinsInetPersonMobility : public veins::VeinsInetMobilityBase {
public:
    VeinsInetPersonMobility();
    virtual ~VeinsInetPersonMobility();


    /** @brief called by class VeinsInetManager */
    virtual void preInitialize(std::shared_ptr<IMobileAgent> mobileAgent) override;

    /** @brief called by class VeinsInetManager */
    virtual void nextPosition(std::shared_ptr<IMobileAgent> mobileAgent) override;

    virtual TraCICommandInterface::Person* getPersonCommandInterface() const;

protected:
    mutable TraCICommandInterface::Person* personCommandInterface = nullptr; /**< cached value */
};

} /* namespace veins */

