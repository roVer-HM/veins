/*
 * SumoSimulation.h
 *
 *  Created on: Sep 19, 2019
 *      Author: stsc
 */

#pragma once

#include <veins/modules/mobility/traci/subscriptionManagement/RemoteSimulationObject.h>
#include <veins/modules/mobility/traci/subscriptionManagement/SubscriptionManager.h>

namespace veins {
namespace TraCISubscriptionManagement {

class SumoSimulationSubFactory : public SubscriptionManagerFactory {
    virtual std::shared_ptr<ISubscriptionManager> createSubscriptionManager(std::vector<std::uint8_t> varCodes,
                                                                            std::shared_ptr<TraCIConnection>& c,
                                                                            std::shared_ptr<TraCICommandInterface>& cIfc) const override;
};

class SumoSimulation: public RemoteSimulationObject {
public:
    SumoSimulation(std::shared_ptr<TraCIConnection> connection, std::shared_ptr<TraCICommandInterface> commandInterface);
    virtual ~SumoSimulation();

    template<typename T>
    friend class RSOFactory;

    void clearRSO() override;

    const simtime_t& getServerTimestep();
    const std::vector<std::string>& getStartedParking();
    const std::vector<std::string>& getEndedParking();

    const std::vector<std::string>& getStartedTeleporting();
    const std::vector<std::string>& getEndedTeleporting();

    const std::vector<std::string>& getDepartedVehicles();
    const std::vector<std::string>& getArrivedVehicles();

protected:
    void initVariableMap() override;
    void setServerTimestep(TraCIBuffer& buffer);
    void setEndedParking(TraCIBuffer& buffer);
    void setStartedParking(TraCIBuffer& buffer);
    void setStartedTeleporting(TraCIBuffer& buffer);
    void setEndedTeleporting(TraCIBuffer& buffer);
    void setDepartedVehicles(TraCIBuffer& buffer);
    void setArrivedVehicles(TraCIBuffer& buffer);

private:
    static simtime_t serverTimestep;
    static std::vector<std::string> startedTeleporting;
    static std::vector<std::string> endedTeleporting;
    static std::vector<std::string> startedParking;
    static std::vector<std::string> endedParking;
    static std::vector<std::string> departedVehicles;
    static std::vector<std::string> arrivedVehicles;


    uint32_t activeVehicleCount;  /**< number of vehicles, be it parking or driving **/
    uint32_t parkingVehicleCount; /**< number of parking vehicles, derived from parking start/end events */
    uint32_t drivingVehicleCount; /**< number of driving, as reported by sumo */

};

} /* namespace TraCISubscriptionManagement */
} /* namespace veins */

