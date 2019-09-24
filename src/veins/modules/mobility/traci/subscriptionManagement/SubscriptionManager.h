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
#pragma once

#include <string>
#include <set>
#include <memory>
#include <functional>

#include "veins/veins.h"

#include "veins/modules/mobility/traci/TraCIBuffer.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"
#include "veins/modules/mobility/traci/TraCIConnection.h"
#include "veins/base/utils/Coord.h"
#include "veins/modules/mobility/traci/subscriptionManagement/RemoteSimulationObject.h"

namespace veins {
namespace TraCISubscriptionManagement {


struct TraCIcmd {
    uint8_t apiCode;
    uint8_t retCode;
    TraCIBuffer buffer;
    bool operator==(const TraCIcmd& rhs) const
    {
        return apiCode == rhs.apiCode && retCode == rhs.retCode && buffer == rhs.buffer;
    }
};



class ISubscriptionManager {
public:
    ISubscriptionManager(){};
    virtual ~ISubscriptionManager(){};

    virtual bool update(TraCIBuffer& bufer) = 0;
    virtual void initializeSubscription() = 0;
    virtual void clearAPI() = 0;

};

template<class T> // T = RSO (e.g. SumoVehicle)
class SubscriptionManager : public ISubscriptionManager {
public:
    SubscriptionManager(std::shared_ptr<TraCIConnection>& connection, std::shared_ptr<TraCICommandInterface>& commandInterface)
        : connection(connection)
        , commandInterface(commandInterface)
        , subscribedRSO()
        , subscriptionInterval(0, SimTime::getMaxTime())
    {}
    ~SubscriptionManager(){}

    void addRSOFactory(RSOFactory<T> factory);

    void initializeSubscription() override;

    bool update(TraCIBuffer& buf) override;

    bool isSubscribed(const std::string& id) const;

    std::map<std::string, std::shared_ptr<T>> getSubscribedRSO() const;
    std::vector<std::shared_ptr<T>> getRSOVector() const;


    void subscribeToId(std::string id);

    std::shared_ptr<T> getGlobalRSO();

    uint8_t getSubscriptionResponseIdentifer() const;
    /**
     * Clears the underlying containers which are made public by the API. Has
     * to be implemented by each manager.
     */
    void clearAPI() override;
protected:
    /**
     * @brief Execute given command and check if response code match cmd.retCode.
     */
    bool executeTraCICmd(TraCIcmd& cmd);

    /**
     * @brief get/set subscriptionInterval. This interval is used during the creation of
     * a TraCI subscription command.
     */
    std::pair<simtime_t, simtime_t> getSubscriptionInterval() const;
    void setSubscriptionInterval(std::pair<simtime_t, simtime_t> interval);
    void setSubscriptionInterval(simtime_t start, simtime_t end);

    /**
    * @brief Create a TraCIcmd struct which defines a subscription to the TraCIConstants::ID_LIST variable.
    * The apiCode and retCode defines the specific api to use. (e.g. Vehicle, Person, .... api's).
    */
    TraCIcmd createIDListSubscription() const;

    TraCIcmd createObjectVarSubscriptionFor(std::shared_ptr<T>& remoteSimulationObject) const;
    TraCIcmd createGlobalVarSubscription() const;

    bool parseGlobalVarSubscription(std::shared_ptr<T>& currRSO ,TraCIBuffer& buffer, uint8_t numberOfResponseVariables);
    void parseObjectVarSubscriptio(std::shared_ptr<T>& currRSO ,TraCIBuffer& buffer, uint8_t numberOfResponseVariables);

    void processIDList(std::set<std::string>& idSet);

    // remote simulation object with id="" (empty string). This object is used to store variables of the RSO-Type (-> independent of the id)
    std::shared_ptr<T> globalRSO;
    RSOFactory<T> remoteSimObjectFactory;
private:

    void unsubscribeFromObjectVariables(std::shared_ptr<T> rso);

    TraCIcmd cmd_empty{0,0,TraCIBuffer{}};

    /**
     * Stores the connection to TraCI server.
     */
    std::shared_ptr<TraCIConnection> connection;

    /**
     * Stores the command interface to TraCI server.
     */
    std::shared_ptr<TraCICommandInterface> commandInterface;


    std::map<std::string, std::shared_ptr<T>> subscribedRSO;
    std::set<std::string> newSubscriptions;

    std::pair<simtime_t, simtime_t> subscriptionInterval;

};

template<class T>
void SubscriptionManager<T>::initializeSubscription(){
    // check for ID List Subscription and subscribe
//    TraCIcmd cmd = createIDListSubscription();
//    executeTraCICmd(cmd);

    // check for global subscriptions and subscribe (e.g. ID_LIST)
    TraCIcmd cmd = createGlobalVarSubscription();
    executeTraCICmd(cmd);
}

template<class T>
void SubscriptionManager<T>::addRSOFactory(RSOFactory<T> factory){
    remoteSimObjectFactory = factory;
    remoteSimObjectFactory.setConnection(connection);
    remoteSimObjectFactory.setCommandInterface(commandInterface);
    globalRSO = remoteSimObjectFactory.createGlobalRSO();
    subscribedRSO.insert(std::make_pair(globalRSO->getId(), globalRSO));
}

template<class T>
std::map<std::string, std::shared_ptr<T>> SubscriptionManager<T>::getSubscribedRSO() const{
    return subscribedRSO;
}

template<class T>
std::vector<std::shared_ptr<T>> SubscriptionManager<T>::getRSOVector() const{
    std::vector<std::shared_ptr<T>> ret;
    for (auto el : subscribedRSO){
        if (el.second == globalRSO){
            continue; // do not return the global RSO (empty string as index)
        }

        ret.push_back(el.second);
    }
    return ret;
}

template<class T>
std::shared_ptr<T> SubscriptionManager<T>::getGlobalRSO(){
    return globalRSO;
}

template<class T>
void SubscriptionManager<T>::clearAPI(){
    newSubscriptions.clear();
}

template<class T>
bool SubscriptionManager<T>::executeTraCICmd(TraCIcmd& cmd){

    if (cmd == cmd_empty) return false;
    // query result.
   TraCIBuffer buffer = connection->query(cmd.apiCode, cmd.buffer);

   // remove unnecessary stuff from buffer
   uint8_t responseCommandLength;
   buffer >> responseCommandLength;
   ASSERT(responseCommandLength == 0);
   // this is the length of the command
   uint32_t responseCommandLengthExtended;
   buffer >> responseCommandLengthExtended;
   uint8_t responseCommandID;
   buffer >> responseCommandID;
   ASSERT(responseCommandID == cmd.retCode);

   update(buffer);
   ASSERT(buffer.eof());
   return true;
}

template<class T>
std::pair<simtime_t, simtime_t> SubscriptionManager<T>::getSubscriptionInterval() const{
    return subscriptionInterval;
}

template<class T>
bool SubscriptionManager<T>::isSubscribed(const std::string& id) const{
    return subscribedRSO.find(id) != subscribedRSO.end();
}

template<class T>
void SubscriptionManager<T>::setSubscriptionInterval(std::pair<simtime_t, simtime_t> interval){
    subscriptionInterval = interval;
}

template<class T>
void SubscriptionManager<T>::setSubscriptionInterval(simtime_t start, simtime_t end){
    setSubscriptionInterval(std::make_pair(start, end));
}

template<class T>
bool SubscriptionManager<T>::update(TraCIBuffer& buf){
    clearAPI();
    bool idListUpdateReceived = false;

    // this is the object id that this subscription result contains
    std::string responseObjectID;
    buf >> responseObjectID;
    std::shared_ptr<T> currRSO = subscribedRSO.at(responseObjectID);
    if (currRSO){
        currRSO->clearRSO(); //clear possible old information
        // the number of response variables that are contained in this buffer
        uint8_t numberOfResponseVariables;
        buf >> numberOfResponseVariables;
        std::vector<uint8_t> expectedVars;
        // check if this is a global subscription (e.g. ID_LIST or any variable which ignores the objectId field)
        if (currRSO == globalRSO){
            // global subscription managed by globalRSO with empty id (empty string)
            expectedVars = currRSO->getSubscribedGlobalVars();
            ASSERT(numberOfResponseVariables == expectedVars.size());
            idListUpdateReceived = parseGlobalVarSubscription(currRSO, buf, numberOfResponseVariables);
            if (idListUpdateReceived){
                std::set<std::string> idList = currRSO->getIdList();
                processIDList(idList);
            }

        } else {
            // object variables for a remote simulation object. (single car, traffic light, ...)
            expectedVars = currRSO->getSubscribedObjectVars();
            ASSERT(numberOfResponseVariables == expectedVars.size());
            parseObjectVarSubscriptio(currRSO, buf, numberOfResponseVariables);
        }

//        if (numberOfResponseVariables == 1){
//            std::set<std::string> ids = parseGlobalVarSubscription(buf);
//            processIDList(ids);
//            idListUpdateReceived = true;
//        } else if (numberOfResponseVariables == expectedVars.size()){
//           if (isSubscribed(responseObjectID)) {
//               std::shared_ptr<T> currRSO = subscribedRSO.at(responseObjectID);
//               parseVariableSubscription(currRSO, buf, numberOfResponseVariables);
//           }
//        }
    } else {
        throw cRuntimeError("Subscription for remote simulation object with Id: '0x%2x' is not tracked by the SubscriptionManager ");
    }


    return idListUpdateReceived;
}

template<class T>
void SubscriptionManager<T>::parseObjectVarSubscriptio(std::shared_ptr<T>& currRSO ,TraCIBuffer& buffer, uint8_t numberOfResponseVariables){
    for (int counter = 0; counter < numberOfResponseVariables; ++counter) {
        // extract a couple of values:
        // - identifies the variable (position, list etc.)
        uint8_t responseVariableID;
        buffer >> responseVariableID;
        // - status of the variable
        uint8_t variableStatus;
        buffer >> variableStatus;
        if (variableStatus == TraCIConstants::RTYPE_OK){
            // use variable map within RemoteSimulationObject to determine the correct setter.
            currRSO->setVar(responseVariableID, buffer);
        } else {
            uint8_t variableType;
             buffer >> variableType;
             // the status of the variable is not ok
             ASSERT(variableType == TraCIConstants::TYPE_STRING);
             std::string errormsg;
             buffer >> errormsg;
             if (isSubscribed(currRSO->getId())) {
                 if (variableStatus == TraCIConstants::RTYPE_NOTIMPLEMENTED) {
                     throw cRuntimeError("TraCI server reported subscribing to %s variable 0x%2x not implemented (\"%s\"). Might need newer version.", currRSO->getRemoteSimulationObjectType(), responseVariableID, errormsg.c_str());
                 }

                 throw cRuntimeError("TraCI server reported error subscribing to %s variable 0x%2x (\"%s\").", currRSO->getRemoteSimulationObjectType(), responseVariableID, errormsg.c_str());
             }
         }
    }
}

template<class T>
bool SubscriptionManager<T>::parseGlobalVarSubscription(std::shared_ptr<T>& currRSO ,TraCIBuffer& buffer, uint8_t numberOfResponseVariables){
    bool idListUpdateReceived = false;
    for (int counter = 0; counter < numberOfResponseVariables; ++counter) {
        uint8_t responseVariableID;
        buffer >> responseVariableID;
        // - status of the variable
        uint8_t variableStatus;
        buffer >> variableStatus;
        if (variableStatus == TraCIConstants::RTYPE_OK){
            // use variable map within RemoteSimulationObject to determine the correct setter.
            currRSO->setVar(responseVariableID, buffer);
            if (responseVariableID == TraCIConstants::ID_LIST){
                idListUpdateReceived = true;
            }
        } else {
            uint8_t variableType;
             buffer >> variableType;
             // the status of the variable is not ok
             ASSERT(variableType == TraCIConstants::TYPE_STRING);
             std::string errormsg;
             buffer >> errormsg;
             if (isSubscribed(currRSO->getId())) {
                 if (variableStatus == TraCIConstants::RTYPE_NOTIMPLEMENTED) {
                     throw cRuntimeError("TraCI server reported subscribing to %s variable 0x%2x not implemented (\"%s\"). Might need newer version.", currRSO->getRemoteSimulationObjectType(), responseVariableID, errormsg.c_str());
                 }

                 throw cRuntimeError("TraCI server reported error subscribing to %s variable 0x%2x (\"%s\").", currRSO->getRemoteSimulationObjectType(), responseVariableID, errormsg.c_str());
             }
         }
    }
    return idListUpdateReceived;
}


template<class T>
void SubscriptionManager<T>::subscribeToId(std::string id){
    if (subscribedRSO.find(id) != subscribedRSO.end()){
        // not subscribed.
        std::shared_ptr<T> newRSO = remoteSimObjectFactory.create();
        newRSO->setId(id);
        subscribedRSO.insert(std::make_pair(id, newRSO));
        newSubscriptions.insert(id);

        //create new variable subscription for newRSO
        TraCIcmd cmd = createObjectVarSubscriptionFor(newRSO);
        executeTraCICmd(cmd); // this will call update.
    }
}

template<class T>
void SubscriptionManager<T>::processIDList(std::set<std::string>& idSet){

    for (auto const& id : idSet){
        auto iter = subscribedRSO.find(id);
        if (iter == subscribedRSO.end()){
            // id does not correspond to existing RSO (RemoteSimulationObject)
            // create new RSO, set id and add to subscribedRSO map.

            std::shared_ptr<T> newRSO = remoteSimObjectFactory.create();
            newRSO->setId(id);
            subscribedRSO.insert(std::make_pair(id, newRSO));
            newSubscriptions.insert(id);

            //create new variable subscription for newRSO
            TraCIcmd cmd = createObjectVarSubscriptionFor(newRSO);
            executeTraCICmd(cmd); // this will call update.
        }
    }

    auto subIter = subscribedRSO.begin();
    while (subIter !=subscribedRSO.end()){
        auto nIt = idSet.find(subIter->first);
        if (nIt == idSet.end() && subIter->first != ""){ // do not remove globalRSO (with empty string as id)
            // existing RSO at subIter not returned remote simulator (remove)
            unsubscribeFromObjectVariables(subIter->second);
            subIter = subscribedRSO.erase(subIter);
        } else {
            subIter++;
        }
    }
}


template<class T>
TraCIcmd SubscriptionManager<T>::createIDListSubscription() const{

    if (!globalRSO->isIdListSubscribed())
        return cmd_empty;

    // subscribe to list of RSO ids
    std::pair<simtime_t, simtime_t> subInterval = getSubscriptionInterval();
    std::string objectId = globalRSO->getId();
    uint8_t variableNumber = 1;
    uint8_t variable1 = TraCIConstants::ID_LIST;
    TraCIBuffer buf = TraCIBuffer() << subInterval.first << subInterval.second << objectId << variableNumber << variable1;

    const TraCICmdTypes cmdTypes = globalRSO->getCmdTypes();

    return TraCIcmd{cmdTypes.subscriptionVarCMD , cmdTypes.subscriptionVarRET, buf};
}

template<class T>
TraCIcmd SubscriptionManager<T>::createObjectVarSubscriptionFor(std::shared_ptr<T>& remoteSimulationObject) const{

    // subscribe to some attributes of the vehicle
    std::pair<simtime_t, simtime_t> subInterval = getSubscriptionInterval();
    std::string objectId = remoteSimulationObject->getId();
    // subscribe to these id's
    const std::vector<uint8_t> variables = remoteSimulationObject->getSubscribedObjectVars();
    uint8_t variableNumber = static_cast<uint8_t>(variables.size());

    TraCIBuffer buf = TraCIBuffer() << subInterval.first << subInterval.second << objectId << variableNumber ;//<< variable1 << variable2 << variable3 << variable4 << variable5 << variable6 << variable7 << variable8 << variable9;
    for (const uint8_t &var : variables){
        buf << var;
    }

    const TraCICmdTypes cmdTypes = remoteSimulationObject->getCmdTypes();
    return TraCIcmd{cmdTypes.subscriptionVarCMD , cmdTypes.subscriptionVarRET, buf};
}

template<class T>
TraCIcmd SubscriptionManager<T>::createGlobalVarSubscription() const{

    // subscribe to these global variables in the RSO-Type
    const std::vector<uint8_t> globalVars = globalRSO->getSubscribedGlobalVars();
    if (globalVars.size() == 0)
        return cmd_empty;

    // subscribe to global globalVars within a RSO Type (e.g. idList count or most variables in the SimulationAPI)
     std::pair<simtime_t, simtime_t> subInterval = getSubscriptionInterval();
     std::string objectId = globalRSO->getId();
     uint8_t variableNumber = static_cast<uint8_t>(globalVars.size());

     TraCIBuffer buf = TraCIBuffer() << subInterval.first << subInterval.second << objectId << variableNumber ;//<< variable1 << variable2 << variable3 << variable4 << variable5 << variable6 << variable7 << variable8 << variable9;
     for (const uint8_t &var : globalVars){
         buf << var;
     }

     const TraCICmdTypes cmdTypes = globalRSO->getCmdTypes();
     return TraCIcmd{cmdTypes.subscriptionVarCMD , cmdTypes.subscriptionVarRET, buf};
}

template<class T>
void SubscriptionManager<T>::unsubscribeFromObjectVariables(std::shared_ptr<T> rso){
    // subscribe to some attributes of the vehicle
    std::pair<simtime_t, simtime_t> subInterval = getSubscriptionInterval();
    std::string objectId = rso->getId();
    uint8_t variableNumber = 0;

    const TraCICmdTypes cmdTypes = rso->getCmdTypes();
    TraCIBuffer buf = connection->query(cmdTypes.subscriptionVarCMD, TraCIBuffer() << subInterval.first << subInterval.second << objectId << variableNumber);
    ASSERT(buf.eof());
}


} /* namespace TraCISubscriptionManagement */
} /* namespace veins */

