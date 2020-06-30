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

#include <string>
#include <functional>

#include "veins/veins.h"
#include "veins/modules/mobility/traci/TraCIBuffer.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"
#include "veins/modules/mobility/traci/TraCIConnection.h"
#include "veins/base/utils/Coord.h"

using veins::TraCIBuffer;
using veins::TraCICoord;

namespace veins {
namespace TraCISubscriptionManagement {

enum access_t{
    off, readWriteDynamic, subscribe
};

enum rsoId_t{
    globalVar, objectVar, idListVar
};

struct TraCICmdTypes {
    uint8_t subscriptionVarCMD;
    uint8_t subscriptionVarRET;
    uint8_t subscriptionCtxCMD;
    uint8_t subscriptionCtxRET;
    uint8_t getCMD;
    uint8_t getRET;
    uint8_t setCMD;
};

class IMobileAgent;

class RemoteSimulationObject {
public:
    typedef void (RemoteSimulationObject::*vF_t)(void);
      struct variableData_t{
          access_t activeFlage;
          uint8_t varType;
          rsoId_t rsoId;
          vF_t getVar;
          vF_t SetVar;
      };

    RemoteSimulationObject();
    RemoteSimulationObject(std::shared_ptr<TraCIConnection> connection, std::shared_ptr<TraCICommandInterface> commandInterface, std::string remoteSimulationObjectType, TraCICmdTypes cmdTypes);
    virtual ~RemoteSimulationObject();

    const static std::map<std::string, uint8_t> varLookup;

    template<typename T>
    T getVar(uint8_t varId){
        auto mIter = variableMap.find(varId);
        if (mIter == variableMap.end())
            throw cRuntimeError("No variable defined for 0x%02X", varId);
        variableData_t s = mIter->second;

        auto f = reinterpret_cast<T(RemoteSimulationObject::*)(void)>(s.getVar);
        return (this->*f)();
    }

    std::set<std::string> getIdList();
    void setVar(int varId, TraCIBuffer& buffer);

    virtual void clearRSO(){};//do nothing (default)

    bool isActive(int varId) const;
    bool isGlobal() const;
    void ensureActive(int varId) const;
    void addGetter(int varId, uint8_t varType, access_t activeFlag, vF_t getFunc, vF_t setFunc);
    void addGetter(int varId, uint8_t varType, access_t activeFlag, rsoId_t rsoId, vF_t getFunc, vF_t setFunc);

    bool isIdListSubscribed() const;
    std::vector<uint8_t> getSubscribedGlobalVars() const;
    std::vector<uint8_t> getSubscribedObjectVars() const;
    const TraCICmdTypes getCmdTypes() const;

    void setId(std::string id);
    virtual const std::string& getId() const;
    const std::string& getRemoteSimulationObjectType() const;
    virtual void setTime(simtime_t time);
    virtual simtime_t getTime() const;

protected:
   virtual void initVariableMap() = 0;
   std::shared_ptr<TraCIConnection> getConnection() const;
   std::shared_ptr<TraCICommandInterface> getCommandInterface() const;
   std::vector<std::string> genericParseStringList(TraCIBuffer& buffer);
   std::set<std::string> stringListAsSet(TraCIBuffer& buffer);

   std::map<int, variableData_t> variableMap;
private:

   /**
    * Stores the connection to TraCI server.
    */
   std::shared_ptr<TraCIConnection> connection;

   /**
    * Stores the command interface to TraCI server.
    */
   std::shared_ptr<TraCICommandInterface> commandInterface;

   std::string remoteSimulationObjectType;

   const TraCICmdTypes cmdTypes;

   std::string id = "-1"; //not set

   simtime_t time; //time step to which the data belongs.

};


class IMobileAgent {
public:
    virtual ~IMobileAgent() = default;

    virtual const std::string& getId() const = 0;
    virtual simtime_t getTime() const = 0;
    virtual const Coord& getPosition() const = 0;
    virtual double getX() const = 0;
    virtual double getY() const = 0;
    virtual double getSpeed() const = 0;
    virtual double getAngel() const = 0;
    virtual Heading getHeading() const = 0;
    virtual  const std::string& getTypeId() const = 0;

    virtual void setTypeId(TraCIBuffer& buffer) = 0;
    virtual void setPosition(TraCIBuffer& buffer) = 0;

    template<typename T>
    static std::shared_ptr<T>get(std::shared_ptr<IMobileAgent> rso){
        std::shared_ptr<T> ret = std::dynamic_pointer_cast<T>(rso);
        if (!ret){
            throw cRuntimeError("Cannot cast RemoteSimulationObject.");
        }
        return ret;
    }
};


template <class T>
class RSOFactory {
public:
    RSOFactory() : connection(nullptr), commandInterface(nullptr), subscribeList() {}
    // RSOFactory(std::vector<const std::pair<uint8_t, access_t>> subscribeMap) : connection(nullptr), commandInterface(nullptr), subscribeList(subscribeMap){}
    RSOFactory(std::vector<uint8_t> subscribeMap) : RSOFactory()
    {
        subscribeList.clear();
        for (const uint8_t& var : subscribeMap){
            subscribeList.push_back(std::pair<int, access_t>(var, access_t::subscribe));
        }
    }

    ~RSOFactory() = default;

    std::shared_ptr<T> create(){
        if (!connection || !commandInterface)
            throw cRuntimeError("connection or commandInterface not initialized in RSOFactory");
        std::shared_ptr<T> o (new T(connection, commandInterface));

        for (auto const &subItem : subscribeList){
            auto it = o->variableMap.find(subItem.first);
            if (it == o->variableMap.end())
                throw cRuntimeError("Variable 0x%02X does not exist for SumoVehicleType", subItem.first);
            // set access value defined by Factory method for new object.
            it->second.activeFlage = subItem.second;
        }
        return o;
    }

    std::shared_ptr<T> createGlobalRSO(){
            if (!connection || !commandInterface)
                throw cRuntimeError("connection or commandInterface not initialized in RSOFactory");
            std::shared_ptr<T> o (new T(connection, commandInterface));

            for (auto const &subItem : subscribeList){
                auto it = o->variableMap.find(subItem.first);
                if (it == o->variableMap.end())
                    throw cRuntimeError("Variable 0x%02X does not exist for SumoVehicleType", subItem.first);
                // set access value defined by Factory method for new object.
                it->second.activeFlage = subItem.second;
            }
            o->setId("");
            return o;
        }



    std::shared_ptr<TraCICommandInterface>& getCommandInterface() {
        return commandInterface;
    }

    std::shared_ptr<TraCIConnection>& getConnection()  {
        return connection;
    }

    void setCommandInterface(
            const std::shared_ptr<TraCICommandInterface>& commandInterface) {
        this->commandInterface = commandInterface;
    }

    void setConnection(const std::shared_ptr<TraCIConnection>& connection) {
        this->connection = connection;
    }

private:
    /**
     * Stores the connection to TraCI server.
     */
    std::shared_ptr<TraCIConnection> connection;

    /**
     * Stores the command interface to TraCI server.
     */
    std::shared_ptr<TraCICommandInterface> commandInterface;

    std::list<std::pair<uint8_t, access_t>> subscribeList;
};

} // end namespace TraCISubscriptionManagement
} // namespace veins
