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

namespace veins {

class VEINS_API TraCIGenericScenarioManager : public cSimpleModule {
public:
    static const simsignal_t traciInitializedSignal;
    static const simsignal_t traciModuleAddedSignal;
    static const simsignal_t traciModuleRemovedSignal;
    static const simsignal_t traciTimestepBeginSignal;
    static const simsignal_t traciTimestepEndSignal;

    TraCIGenericScenarioManager();
    virtual ~TraCIGenericScenarioManager();

    void initialize(int stage) override;

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
    // maps from vehicle type to moduleType, moduleName, and moduleDisplayString
    std::string host;
    int port;

    typedef std::map<std::string, std::string> TypeMapping;
    TypeMapping moduleType; /**< module type to be used in the simulation for each managed vehicle */
    TypeMapping moduleName; /**< module name to be used in the simulation for each managed vehicle */
    TypeMapping moduleDisplayString; /**< module displayString to be used in the simulation for each managed vehicle */

    bool autoShutdown; /**< Shutdown module as soon as no more vehicles are in the simulation */
    bool autoShutdownTriggered;

    double penetrationRate;
    bool ignoreGuiCommands; /**< whether to ignore all TraCI commands that only make sense when the server has a graphical user interface */



    std::unique_ptr<TraCIConnection> connection;
    std::unique_ptr<TraCICommandInterface> commandIfc;

    cMessage* connectAndStartTrigger; /**< self-message scheduled for when to connect to TraCI server and start running */
    cMessage* executeOneTimestepTrigger; /**< self-message scheduled for when to next call executeOneTimestep */

    virtual void init_traci();
    virtual void processSubcriptionResult(TraCIBuffer& buf);

    void executeOneTimestep(); /**< read and execute all commands for the next timestep */

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

