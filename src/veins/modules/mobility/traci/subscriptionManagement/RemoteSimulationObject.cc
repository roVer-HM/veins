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

#include <veins/modules/mobility/traci/subscriptionManagement/RemoteSimulationObject.h>
#include "veins/modules/mobility/traci/TraCIConstants.h"


namespace veins {
namespace TraCISubscriptionManagement {

RemoteSimulationObject::RemoteSimulationObject()
    : variableMap()
    , connection(nullptr)
    , commandInterface(nullptr)
    , remoteSimulationObjectType()
    , cmdTypes() {}
RemoteSimulationObject::RemoteSimulationObject(std::shared_ptr<TraCIConnection> connection, std::shared_ptr<TraCICommandInterface> commandInterface, std::string remoteSimulationObjectType, TraCICmdTypes cmdTypes)
    : variableMap()
    , connection(connection)
    , commandInterface(commandInterface)
    , remoteSimulationObjectType(remoteSimulationObjectType)
    , cmdTypes(cmdTypes) {}
RemoteSimulationObject::~RemoteSimulationObject(){}

bool RemoteSimulationObject::isActive(int varId ) const {
    auto it = variableMap.find(varId);
    if (it == variableMap.end()){
        return false;
    } else {
        variableData_t d = it->second;
        return d.activeFlage != access_t::off;
    }
}

bool RemoteSimulationObject::isGlobal() const{
    return getId() == "";
}

void RemoteSimulationObject::setId(std::string id){
    this->id = id;
}

const std::string& RemoteSimulationObject::getId() const{
    return id;
}

void RemoteSimulationObject::setTime(simtime_t time){
    this->time = time;
}

simtime_t RemoteSimulationObject::getTime() const {
    return time;
}

void RemoteSimulationObject::ensureActive(int varId ) const {
    if (!isActive(varId))
        throw cRuntimeError("Variable 0x%02X not active", varId);
}

void RemoteSimulationObject::addGetter(int varId, uint8_t varType, access_t activeFlag, vF_t getFunc, vF_t setFunc){
    variableMap.insert(std::make_pair(varId, variableData_t{activeFlag, varType, rsoId_t::objectVar, getFunc, setFunc}));
}

void RemoteSimulationObject::addGetter(int varId, uint8_t varType, access_t activeFlag, rsoId_t rsoId, vF_t getFunc, vF_t setFunc){
    variableMap.insert(std::make_pair(varId, variableData_t{activeFlag, varType, rsoId, getFunc, setFunc}));
}

void RemoteSimulationObject::setVar(int varId, TraCIBuffer& buffer){
    // - type of the variable
    uint8_t variableType;
    buffer >> variableType;

    auto mIter = variableMap.find(varId);
    if (mIter == variableMap.end())
        throw cRuntimeError("No variable defined for 0x%02X", varId);
    variableData_t varData = mIter->second;
    ASSERT(variableType == varData.varType);
    auto f = reinterpret_cast<void(RemoteSimulationObject::*)(TraCIBuffer&)>(varData.SetVar);
    return (this->*f)(buffer);
}

std::set<std::string> RemoteSimulationObject::getIdList(){
    std::set<std::string> ret = getVar<std::set<std::string>>(TraCIConstants::ID_LIST);
    return ret;
}


std::vector<uint8_t> RemoteSimulationObject::getSubscribedObjectVars() const{
    std::vector<uint8_t> ret{};

    for (auto const &pair : variableMap ){
        if (pair.second.activeFlage == access_t::subscribe && pair.second.rsoId == rsoId_t::objectVar)
            ret.push_back(static_cast<uint8_t>(pair.first));
    }
    return ret;
}

bool RemoteSimulationObject::isIdListSubscribed() const {
    auto iter = variableMap.find(TraCIConstants::ID_LIST);
    if (iter == variableMap.end()) return false;
    return iter->second.activeFlage == access_t::subscribe;
}

std::vector<uint8_t> RemoteSimulationObject::getSubscribedGlobalVars() const {
    std::vector<uint8_t> ret{};

    for (auto const &pair : variableMap ){
        if (pair.second.activeFlage == access_t::subscribe && pair.second.rsoId == rsoId_t::globalVar)
            ret.push_back(static_cast<uint8_t>(pair.first));
    }
    return ret;
}


std::shared_ptr<TraCIConnection> RemoteSimulationObject::getConnection() const
{
    return connection;
}

std::shared_ptr<TraCICommandInterface> RemoteSimulationObject::getCommandInterface() const
{
    return commandInterface;
}

std::vector<std::string> RemoteSimulationObject::genericParseStringList(TraCIBuffer& buffer)
{
    uint32_t count;
    buffer >> count;

    std::vector<std::string> res;
    for(uint32_t i = 0; i < count; ++i){
        std::string str;
        buffer >> str;
        res.push_back(str);
    }
    return res;
}

std::set<std::string> RemoteSimulationObject::stringListAsSet(TraCIBuffer& buffer)
{
    uint32_t count;
    buffer >> count;

    std::set<std::string> res;
    for(uint32_t i = 0; i < count; ++i){
        std::string str;
        buffer >> str;
        res.insert(str);
    }
    return res;
}


const TraCICmdTypes RemoteSimulationObject::getCmdTypes() const {
    return cmdTypes;
}

const std::string& RemoteSimulationObject::getRemoteSimulationObjectType() const {
    return remoteSimulationObjectType;
}

} // end namespace TraCISubscriptionManagement
} // namespace veins


