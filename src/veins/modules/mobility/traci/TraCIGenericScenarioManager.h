/*
 * TraCIAbstractScenarioManager.h
 *
 *  Created on: Sep 23, 2019
 *      Author: stsc
 */

#pragma once

#include <map>
#include <memory>
#include <list>
#include <queue>

#include "veins/veins.h"

#include "veins/base/utils/Coord.h"
#include "veins/base/modules/BaseWorldUtility.h"
#include "veins/base/connectionManager/BaseConnectionManager.h"
#include "veins/base/utils/FindModule.h"
#include "veins/modules/obstacle/ObstacleControl.h"
#include "veins/modules/obstacle/VehicleObstacleControl.h"
#include "veins/modules/mobility/traci/TraCIBuffer.h"
#include "veins/modules/mobility/traci/TraCIColor.h"
#include "veins/modules/mobility/traci/TraCIConnection.h"
#include "veins/modules/mobility/traci/TraCICoord.h"
#include "veins/modules/mobility/traci/VehicleSignal.h"
#include "veins/modules/mobility/traci/TraCIRegionOfInterest.h"
#include "veins/modules/mobility/traci/TraCIGenericScenarioManager.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"
#include "veins/modules/mobility/traci/subscriptionManagement/RemoteSimulationObject.h"

using namespace veins::TraCISubscriptionManagement;
namespace veins {

/**
 * @brief
 * Creates and moves nodes controlled by a TraCI server.
 *
 * Do not use this generic class directly but one of its sub classes.
 *
 * Conventions: a agent is any moving element provided by the server, may it be a Vehicle, Pedestrian or what not.
 *
 * This generic class is agnostic to the type and number of different agents the server provides. The sub classes
 * must provide a way to handle Vehicles or Pedestrian or both.
 */
class VEINS_API TraCIGenericScenarioManager : public cSimpleModule {
public:
    static const simsignal_t traciInitializedSignal;
    static const simsignal_t traciModuleAddedSignal;
    static const simsignal_t traciModuleRemovedSignal;
    static const simsignal_t traciTimestepBeginSignal;
    static const simsignal_t traciTimestepEndSignal;

    TraCIGenericScenarioManager();
    virtual ~TraCIGenericScenarioManager();
    int numInitStages() const override
    {
        return std::max(cSimpleModule::numInitStages(), 2);
    }

    virtual void initialize(int stage) override;

    void handleMessage(cMessage* msg) override;
    virtual void handleSelfMsg(cMessage* msg);

    bool isConnected() const
    {
        return static_cast<bool>(connection);
    }

    TraCICommandInterface* getCommandInterface() const
    {
        return commandIfc.get();
    }

    TraCIConnection* getConnection() const
    {
        return connection.get();
    }

    bool getAutoShutdownTriggered()
    {
        return autoShutdownTriggered;
    }

    /**
     * Predicate indicating a successful connection to the TraCI server.
     *
     * @note Once the connection has been established, this will return true even when the connection has been torn down again.
     */
    bool isUsable() const
    {
        return traciInitialized;
    }

protected:
    bool traciInitialized = false; /**< Flag indicating whether the init_traci routine has been run. Note that it will change to false again once set, even during shutdown. */
    simtime_t connectAt; /**< when to connect to TraCI server (must be the initial timestep of the server) */
    simtime_t firstStepAt; /**< when to start synchronizing with the TraCI server (-1: immediately after connecting) */
    simtime_t updateInterval; /**< time interval of hosts' position updates */
    std::string host;
    int port;

    AnnotationManager* annotations;

    // maps from vehicle type to moduleType, moduleName, and moduleDisplayString
    typedef std::map<std::string, std::string> TypeMapping;
    TypeMapping moduleType; /**< module type to be used in the simulation for each managed vehicle */
    TypeMapping moduleName; /**< module name to be used in the simulation for each managed vehicle */
    TypeMapping moduleDisplayString; /**< module displayString to be used in the simulation for each managed vehicle */

    bool autoShutdown; /**< Shutdown module as soon as no more agents are in the simulation */
    bool autoShutdownTriggered;

    bool ignoreGuiCommands; /**< whether to ignore all TraCI commands that only make sense when the server has a graphical user interface */

    BaseWorldUtility* world;

    std::shared_ptr<TraCIConnection> connection;
    std::shared_ptr<TraCICommandInterface> commandIfc;

    cMessage* connectAndStartTrigger; /**< self-message scheduled for when to connect to TraCI server and start running */
    cMessage* executeOneTimestepTrigger; /**< self-message scheduled for when to next call executeOneTimestep */

    virtual void init_traci(); // override it!
    virtual void init_obstacles(); /** if a ObstacleControl is present initialize it. Must be called from init_traci*/
    virtual void executeOneTimestep(); // override it!

    virtual void preInitializeModule(cModule* mod, std::shared_ptr<IMobileAgent> mobileAgent); // override it!
    virtual void updateModulePosition(cModule* mod, std::shared_ptr<IMobileAgent> mobileAgent); // override it!
    virtual void addModule(std::string nodeId, std::string type, std::string name, std::string displayString, std::shared_ptr<IMobileAgent> mobileAgent);

    virtual cModule* getManagedModule(std::string identifer);
    virtual void deleteManagedModule(std::string identifer);
//    bool isModuleUnequipped(std::string nodeId) const; /**< returns true if this vehicle is Unequipped */


    struct TypeMappingTripel{
        std::string mType, mName, mDisplayString;
    };
    virtual TypeMappingTripel getTypeMapping(std::string typeId);

    /**
     * parses the vector of module types in ini file
     *
     * in case of inconsistencies the function kills the simulation
     */
    void parseModuleTypes();

    /**
     * transforms a list of mappings of an omnetpp.ini parameter in a list
     */
    TypeMapping parseMappings(std::string parameter, std::string parameterName, bool allowEmpty = false);

    virtual int getPortNumber() const;
};

} /* namespace veins */

