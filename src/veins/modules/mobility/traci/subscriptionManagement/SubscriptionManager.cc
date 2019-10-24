//
// Copyright (C) 2019 Stefan Schuhbaeck <stefan.schuhbaeck@hm.edu>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include <string>
#include <set>
#include <memory>
#include <functional>

#include "veins/veins.h"

#include "veins/modules/mobility/traci/TraCIBuffer.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"
#include "veins/modules/mobility/traci/TraCIConnection.h"
#include "veins/base/utils/Coord.h"
#include "veins/modules/mobility/traci/subscriptionManagement/SubscriptionManager.h"
#include "veins/modules/mobility/traci/subscriptionManagement/SumoVehicle.h"
#include "veins/modules/mobility/traci/subscriptionManagement/SumoPerson.h"
#include "veins/modules/mobility/traci/subscriptionManagement/SumoSimulation.h"
#include "veins/modules/mobility/traci/subscriptionManagement/SumoTrafficLight.h"

using namespace veins::TraCISubscriptionManagement;

namespace veins {
namespace TraCISubscriptionManagement {

Register_Abstract_Class(SubscriptionManagerFactory)



//
//std::shared_ptr<ISubscriptionManager> ISubscriptionManager::create(std::string subscriptionId
//                        , std::vector<std::uint8_t> varCodes
//                        , std::shared_ptr<TraCIConnection>& c
//                        , std::shared_ptr<TraCICommandInterface>& cIfc){
//
//    auto iter = ISubscriptionManager::factoryMap.find(subscriptionId);
//    if (iter == ISubscriptionManager::factoryMap.end()){
//        throw cRuntimeError("No Subscription found for given id: '%s'", subscriptionId.c_str());
//    }
//
//    return iter->second(varCodes, c, cIfc);
//
//}
//
//const std::map<std::string, ISubscriptionManager::SubscriptionManagerFactory_m> ISubscriptionManager::factoryMap = {
//        {"SumoVehicle", [](std::vector<std::uint8_t> varCodes, std::shared_ptr<TraCIConnection>& c, std::shared_ptr<TraCICommandInterface>& cIfc) {
//            return SubscriptionManager<SumoVehicle>::build(varCodes, c, cIfc);
//                } },
//        {"SumoPerson", [](std::vector<std::uint8_t> varCodes, std::shared_ptr<TraCIConnection>& c, std::shared_ptr<TraCICommandInterface>& cIfc) {
//            return SubscriptionManager<SumoPerson>::build(varCodes, c, cIfc);
//            } },
//        {"SumoSimulation", [](std::vector<std::uint8_t> varCodes, std::shared_ptr<TraCIConnection>& c, std::shared_ptr<TraCICommandInterface>& cIfc) {
//            return SubscriptionManager<SumoSimulation>::build(varCodes, c, cIfc);
//            } },
//        {"SumoTrafficLight", [](std::vector<std::uint8_t> varCodes, std::shared_ptr<TraCIConnection>& c, std::shared_ptr<TraCICommandInterface>& cIfc) {
//            return SubscriptionManager<SumoTrafficLight>::build(varCodes, c, cIfc);
//            } },
//};



} /* namespace TraCISubscriptionManagement */
} /* namespace veins */
