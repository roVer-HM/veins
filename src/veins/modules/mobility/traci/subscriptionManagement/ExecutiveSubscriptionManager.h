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

#pragma once

#include <memory>
#include <veins/modules/mobility/traci/subscriptionManagement/SumoVehicle.h>
#include "veins/veins.h"

#include "veins/modules/mobility/traci/subscriptionManagement/SubscriptionManager.h"
#include "veins/modules/mobility/traci/TraCIBuffer.h"
#include "veins/modules/mobility/traci/TraCIConnection.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"

namespace veins {
namespace TraCISubscriptionManagement {

/**
 * @class ExecutiveSubscriptionManager
 *
 * This manages all the subscriptions that were made and provides an API
 * to the underlying subscription managers and request their results.
 *
 * @author Nico Dassler <dassler@hm.edu>
 */
class ExecutiveSubscriptionManager {

public:
    /**
     * Constructor.
     *
     * @param connection the connection to the traci server to perform subscripitons
     * @param commandInterface the command interface to make requests to the traci server
     * @param explicitUpdateIfSubscriptionUnavailable In case there is no subscription response
     * for a certain type of participant it will be replaced with an explicit request.
     */
    ExecutiveSubscriptionManager(std::shared_ptr<TraCIConnection>& connection, std::shared_ptr<TraCICommandInterface>& commandInterface, bool explicitUpdateIfSubscriptionUnavailable = true);
    virtual ~ExecutiveSubscriptionManager();

    /**
     * Process a subscription result.
     *
     * Do not modify the buffer before inserting it into this method.
     *
     * This updates the internal state of this object and changes
     * responses to other methods of this object.
     *
     * @param buffer TraCIBuffer containing subscriptions.
     */
    void processSubscriptionResult(TraCIBuffer& buffer);



    /**
     * Initialize all managed Subscriptionmanagers.
     */
    void initialize();

    /**
     * Clear API of all SubscriptionManagers
     */
    void clearAPI();

    template<typename T>
    void addSubscriptionManager(uint8_t responseCommandIdentifer, T& manager){
        std::shared_ptr<ISubscriptionManager> tmpMgr = std::dynamic_pointer_cast<ISubscriptionManager>(std::make_shared<T>(manager));
        subscriptionManagers.insert(std::make_pair(responseCommandIdentifer, tmpMgr));
    }
    std::shared_ptr<ISubscriptionManager> getSubscriptionManager(uint8_t responseCode);

    template<typename T>
    std::shared_ptr<T> get(uint8_t retCode){
        std::shared_ptr<T> mgr = std::dynamic_pointer_cast<T>(getSubscriptionManager(retCode));
        if (!mgr){
            throw cRuntimeError("cannot cast");
        }
        return mgr;
    }

private:

    /**
     * The command interface to the TraCI server.
     */
    std::shared_ptr<TraCICommandInterface> commandInterface;


    std::map<uint8_t, std::shared_ptr<ISubscriptionManager>> subscriptionManagers;

    /**
     * Stores if the id lists of participants should be updated with an explicit request
     * if there is no subscription response included.
     */
    bool explicitUpdateIfIdListSubscriptionUnavailable;

};



} // end namespace TraCISubscriptionManagement
} // namespace veins
