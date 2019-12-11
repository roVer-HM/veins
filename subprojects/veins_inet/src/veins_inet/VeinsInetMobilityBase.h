/*
 * VeinsInetMobilityBase.h
 *
 *  Created on: Oct 2, 2019
 *      Author: stsc
 */

#pragma once
namespace omnetpp {
}
using namespace omnetpp;

#include "inet/mobility/base/MobilityBase.h"

#include "veins_inet/veins_inet.h"

#include "veins/modules/mobility/traci/TraCIGenericScenarioManager.h"
#include "veins/modules/mobility/traci/TraCIScenarioManager.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"
#include "veins/modules/mobility/traci/subscriptionManagement/RemoteSimulationObject.h"

namespace veins {

class VeinsInetMobilityBase : public inet::MobilityBase {
public:
    VeinsInetMobilityBase();
    virtual ~VeinsInetMobilityBase();

    virtual void initialize(int stage) override; // cModule


    virtual void preInitialize(std::shared_ptr<IMobileAgent> mobileAgent) = 0;
    virtual void nextPosition(std::shared_ptr<IMobileAgent> mobileAgent) = 0;

    virtual inet::Coord getCurrentPosition() override; //MobilityBase
    virtual inet::Coord getCurrentVelocity() override; //MobilityBase
    virtual inet::Coord getCurrentAcceleration() override; //MobilityBase

    virtual inet::Quaternion getCurrentAngularPosition() override; //MobilityBase
    virtual inet::Quaternion getCurrentAngularVelocity() override; //MobilityBase
    virtual inet::Quaternion getCurrentAngularAcceleration() override; //MobilityBase

    virtual TraCIGenericScenarioManager* getManager() const;
    virtual TraCICommandInterface* getCommandInterface() const;
    virtual void setExternalId(std::string id);
    virtual std::string getExternalId() const;

    static const simsignal_t mobility_pre_ini;

protected:

    virtual void handleSelfMessage(cMessage* message) override;
    virtual void setInitialPosition() override;

    /** @brief The last velocity that was set by nextPosition(). */
    inet::Coord lastVelocity;

    /** @brief The last angular velocity that was set by nextPosition(). */
    inet::Quaternion lastAngularVelocity;

    mutable TraCIGenericScenarioManager* manager = nullptr; /**< cached value */
    mutable TraCICommandInterface* commandInterface = nullptr; /**< cached value */
    std::string external_id; /**< identifier used by TraCI server to refer to this node */

};
} /* namespace veins */


