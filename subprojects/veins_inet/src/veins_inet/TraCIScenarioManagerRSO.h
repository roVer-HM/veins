//
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

#pragma once

#include <map>
#include <memory>
#include <list>
#include <queue>

#include "veins/veins.h"

#include "veins/modules/mobility/traci/TraCIGenericScenarioManager.h"
#include "veins/modules/mobility/traci/subscriptionManagement/ExecutiveSubscriptionManager.h"
#include "veins/modules/mobility/traci/subscriptionManagement/SubscriptionManager.h"

namespace veins {
namespace TraCIConstants {

const uint8_t CMD_FILE_SEND = 0x75;

} // namespace TraCIConstants
} // namespace veins

using namespace veins::TraCISubscriptionManagement;


namespace veins {
class TraCIScenarioManagerRSO: public veins::TraCIGenericScenarioManager {
public:
    TraCIScenarioManagerRSO();
    virtual ~TraCIScenarioManagerRSO();

    virtual void initialize(int stage) override;
    virtual void finish() override;
protected:
    virtual void init_traci() override; // override it!
    virtual void executeOneTimestep() override; // override it!
    virtual void processSubcriptionResults();

    virtual void processMobileAgent(std::shared_ptr<IMobileAgent> mobileAgent);
    virtual void addModule(std::string nodeId, std::string type, std::string name, std::string displayString, std::shared_ptr<IMobileAgent> mobileAgent) override;
    virtual void deleteManagedModule(std::string nodeId) override;
    virtual void deleteManagedModule(cModule * mod);

    virtual void preInitializeModule(cModule* mod, std::shared_ptr<IMobileAgent> mobileAgent) override;
    virtual void updateModulePosition(cModule* mod, std::shared_ptr<IMobileAgent> mobileAgent) override;

    virtual cModule* getManagedModule(std::string nodeId) override;
    virtual cModule* getAndRemoveManagedModule(std::string nodeId);
    virtual cModule* getManagedModule(std::string moduleTupe, std::string nodeId);
    virtual void addManagedModule(std::string moduleTupe, std::string nodeId, cModule* mod);

    virtual void registerNetworknodeInVisulizer(cModule* networkNode);
    virtual void unregisterNetworknodeFromVisulizer(const cModule* networkNode);

    int32_t nextVectorIndex(std::string module);
    typedef std::map<std::string, std::map<std::string, cModule*>> ManagedHosts;
    /* todo: the nodeId right now must be unique over all hostTypes. */
    ManagedHosts hosts; /**map containing cModules sorted based on hostType and nodeId (hostType -> nodeId -> cModule*) */
    std::map<std::string, int32_t> moduleVectorIndex; /** (hostType -> nodeVectorSize*) */


    typedef std::map<std::string, std::vector<uint8_t>> SubMgrVarMapping;
    SubMgrVarMapping subscriptionMgrType;
    TypeMapping moduleAPI;
    std::unique_ptr<ExecutiveSubscriptionManager> subscriptionManager;


    cXMLElement* launchConfig; /**< launch configuration to send to sumo-launchd */
    int seed; /**< seed value to set in launch configuration, if missing (-1: current run number) */
    std::string visualizer; /** If set the module name of the visualizer module. This is used to register new elements */
};

} // end namepsace veins
