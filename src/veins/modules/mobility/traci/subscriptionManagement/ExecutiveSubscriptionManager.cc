//
// Copyright (C) 2006-2017 Nico Dassler <dassler@hm.edu>
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

#include <memory>

#include "veins/modules/mobility/traci/subscriptionManagement/ExecutiveSubscriptionManager.h"
#include "veins/modules/mobility/traci/subscriptionManagement/SubscriptionManager.h"

#include "veins/modules/mobility/traci/TraCIConstants.h"

namespace veins {
namespace TraCISubscriptionManagement {

ExecutiveSubscriptionManager::ExecutiveSubscriptionManager(std::shared_ptr<TraCIConnection>& connection, std::shared_ptr<TraCICommandInterface>& commandInterface, bool explicitUpdateIfIdListSubscriptionUnavailable)
    : commandInterface(commandInterface)
    , explicitUpdateIfIdListSubscriptionUnavailable(explicitUpdateIfIdListSubscriptionUnavailable)
{
}

ExecutiveSubscriptionManager::~ExecutiveSubscriptionManager(){}

void ExecutiveSubscriptionManager::initialize(){
    for (auto const &mgr : subscriptionManagers){
        mgr.second->clearAPI();
        mgr.second->initializeSubscription();
    }
}

void ExecutiveSubscriptionManager::addSubscriptionManager(std::shared_ptr<ISubscriptionManager> mgr){
    for (auto id : mgr->getManagedReturnCodes()){
        subscriptionManagers.insert(std::make_pair(id, mgr));
    }
}

void ExecutiveSubscriptionManager::processSubscriptionResultBuffer(TraCIBuffer& buffer)
{
    bool receivedPersonIDListSubscription = false;
    clearAPI();

    uint32_t subscriptionResultCount;
    buffer >> subscriptionResultCount;
    EV_DEBUG << "Getting " << subscriptionResultCount << " subscription results" << endl;
    for (uint32_t i = 0; i < subscriptionResultCount; ++i) {

        // this should be zero
        uint8_t responseCommandLength;
        buffer >> responseCommandLength;
        ASSERT(responseCommandLength == 0);
        // this is the length of the command
        uint32_t responseCommandLengthExtended;
        buffer >> responseCommandLengthExtended;
        // this is the response command identifier
        // with this we can find out what kind of subscription result
        // we receive (person subscription or vehicle subscription etc.)
        uint8_t responseCommandID;
        buffer >> responseCommandID;


        // select corresponding manager and execute update()
        if (subscriptionManagers.find(responseCommandID) != subscriptionManagers.end()){
            auto mgr = subscriptionManagers.at(responseCommandID);
            bool ret = mgr->update(buffer);

            if (responseCommandID == TraCIConstants::RESPONSE_SUBSCRIBE_PERSON_VARIABLE){
                // workaround due to not receiving person ID_LIST from SUMO vor Person API
                receivedPersonIDListSubscription = ret;
            }
        } else {
            throw cRuntimeError("No Subscription manager registered for received response code 0x%2x", responseCommandID);
        }
    }

    if (!receivedPersonIDListSubscription && explicitUpdateIfIdListSubscriptionUnavailable){
        if (has(TraCIConstants::RESPONSE_SUBSCRIBE_PERSON_VARIABLE)){
            doExplicitUpdateIfIdListSubscriptionUnavailable(subscriptionManagers.at(TraCIConstants::RESPONSE_SUBSCRIBE_PERSON_VARIABLE));
        }
    }


    ASSERT(buffer.eof());
}

void ExecutiveSubscriptionManager::doExplicitUpdateIfIdListSubscriptionUnavailable(std::shared_ptr<ISubscriptionManager> mgr){
//      NOTE: This is only done because I was not able to receive a person ID LIST subscription
//      If it is somehow possible to get a person id list subscription working. This can be removed.

//      if there was no person id list received we perform a manual update
//        if (!receivedPersonIDListSubscription && explicitUpdateIfIdListSubscriptionUnavailable) {
            veins::TraCICommandInterface::Person defaultPerson = commandInterface->person("");
            std::list<std::string> idList = defaultPerson.getIdList();
            for (auto const &id : idList){
                mgr->subscribeToId(id);
            }
//        }

//      if there was no vehicle id list received we perform a manual update
//        if (!receivedVehicleIDListSubscription && explicitUpdateIfIdListSubscriptionUnavailable) {
//            veins::TraCICommandInterface::Vehicle defaultVehicle = commandInterface->vehicle("");
//            std::list<std::string> idList = defaultVehicle.getIdList();
//            vehicleSubscriptionManager.updateWithList(idList);
//        }
}

void ExecutiveSubscriptionManager::clearAPI()
{
    for (auto const &mgr: subscriptionManagers){
        mgr.second->clearAPI();
    }
}

bool ExecutiveSubscriptionManager::has(uint8_t retCode) const {
    return subscriptionManagers.find(retCode) != subscriptionManagers.end();
}

std::shared_ptr<ISubscriptionManager> ExecutiveSubscriptionManager::getSubscriptionManager(uint8_t responseCode){

    auto it = subscriptionManagers.find(responseCode);
    if (it == subscriptionManagers.end()){
        throw cRuntimeError("no SubscriptionManager found for response type 0x%2x", responseCode);
    }
    return it->second;
}


} // end namespace TraCISubscriptionManagement
} // namespace veins
