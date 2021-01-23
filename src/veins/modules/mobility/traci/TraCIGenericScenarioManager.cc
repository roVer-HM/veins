/*
 * TraCIAbstractScenarioManager.cc
 *
 *  Created on: Sep 23, 2019
 *      Author: stsc
 */

#include <fstream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <cstdlib>

#include "veins/modules/mobility/traci/TraCIScenarioManager.h"
#include "veins/base/connectionManager/ChannelAccess.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"
#include "veins/modules/mobility/traci/TraCIConstants.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"
#include "veins/modules/obstacle/ObstacleControl.h"
#include "veins/modules/world/traci/trafficLight/TraCITrafficLightInterface.h"

using namespace veins::TraCIConstants;
using veins::TraCIGenericScenarioManager;
using veins::TraCIBuffer;

Define_Module(veins::TraCIGenericScenarioManager);
const simsignal_t TraCIGenericScenarioManager::traciInitializedSignal = registerSignal("org_car2x_veins_modules_mobility_traciInitialized");
const simsignal_t TraCIGenericScenarioManager::traciModulePreInitSignal = registerSignal("org_car2x_veins_modules_mobility_traciModulePreInit");
const simsignal_t TraCIGenericScenarioManager::traciModuleAddedSignal = registerSignal("org_car2x_veins_modules_mobility_traciModuleAdded");
const simsignal_t TraCIGenericScenarioManager::traciModuleRemovedSignal = registerSignal("org_car2x_veins_modules_mobility_traciModuleRemoved");
const simsignal_t TraCIGenericScenarioManager::traciTimestepBeginSignal = registerSignal("org_car2x_veins_modules_mobility_traciTimestepBegin");
const simsignal_t TraCIGenericScenarioManager::traciTimestepEndSignal = registerSignal("org_car2x_veins_modules_mobility_traciTimestepEnd");

namespace veins {

TraCIGenericScenarioManager::TraCIGenericScenarioManager()
    : world(nullptr)
    , connection(nullptr)
    , commandIfc(nullptr)
    , connectAndStartTrigger(nullptr)
    , executeOneTimestepTrigger(nullptr)
{
    // TODO Auto-generated constructor stub

}

TraCIGenericScenarioManager::~TraCIGenericScenarioManager() {
    if (connection) {
        TraCIBuffer buf = connection->query(CMD_CLOSE, TraCIBuffer());
    }
    if (connectAndStartTrigger) {
        cancelAndDelete(connectAndStartTrigger);
        connectAndStartTrigger = nullptr;
    }
    if (executeOneTimestepTrigger) {
        cancelAndDelete(executeOneTimestepTrigger);
        executeOneTimestepTrigger = nullptr;
    }
}

void TraCIGenericScenarioManager::initialize(int stage) {
    cSimpleModule::initialize(stage);
    if (stage != 1) {
            return;
    }
    connectAt = par("connectAt");
    firstStepAt = par("firstStepAt");
    updateInterval = par("updateInterval");
    if (firstStepAt == -1) firstStepAt = connectAt + updateInterval;
    parseModuleTypes();

    ignoreGuiCommands = par("ignoreGuiCommands");
    host = par("host").stdstringValue();
    port = getPortNumber();
    if (port == -1) {
     throw cRuntimeError("TraCI Port autoconfiguration failed, set 'port' != -1 in omnetpp.ini or provide VEINS_TRACI_PORT environment variable.");
    }
    autoShutdown = par("autoShutdown");

    annotations = AnnotationManagerAccess().getIfExists();

    ASSERT(firstStepAt > connectAt);
    connectAndStartTrigger = new cMessage("connect");
    scheduleAt(connectAt, connectAndStartTrigger);
    executeOneTimestepTrigger = new cMessage("step");
    scheduleAt(firstStepAt, executeOneTimestepTrigger);

    EV_DEBUG << "initialized TraCIGenericScenarioManager" << endl;
}

void TraCIGenericScenarioManager::handleMessage(cMessage* msg)
{
    if (msg->isSelfMessage()) {
        handleSelfMsg(msg);
        return;
    }
    throw cRuntimeError("TraCIScenarioManager doesn't handle messages from other modules");
}

void TraCIGenericScenarioManager::handleSelfMsg(cMessage* msg)
{
    if (msg == connectAndStartTrigger) {
        connection.reset(TraCIConnection::connect(this, host.c_str(), port));
        commandIfc.reset(new TraCICommandInterface(this, *connection, ignoreGuiCommands));
        init_traci();
        return;
    }
    if (msg == executeOneTimestepTrigger) {
        executeOneTimestep();
        return;
    }
    throw cRuntimeError("TraCIScenarioManager received unknown self-message");
}

void TraCIGenericScenarioManager::init_obstacles(){
    ObstacleControl* obstacles = ObstacleControlAccess().getIfExists();
    if (obstacles) {
        {
            // get list of polygons
            std::list<std::string> ids = commandIfc->getPolygonIds();
            for (std::list<std::string>::iterator i = ids.begin(); i != ids.end(); ++i) {
                std::string id = *i;
                std::string typeId = commandIfc->polygon(id).getTypeId();
                if (!obstacles->isTypeSupported(typeId)) continue;
                std::list<Coord> coords = commandIfc->polygon(id).getShape();
                std::vector<Coord> shape;
                std::copy(coords.begin(), coords.end(), std::back_inserter(shape));
                obstacles->addFromTypeAndShape(id, typeId, shape);
            }
        }
    }
}

void TraCIGenericScenarioManager::init_traci(){
    throw cRuntimeError("Do not use TraCIGenericScenarioManager directly. Use child class and override this method");
}

void TraCIGenericScenarioManager::executeOneTimestep()
{
    throw cRuntimeError("Do not use TraCIGenericScenarioManager directly. Use child class and override this method");
}

void TraCIGenericScenarioManager::preInitializeModule(cModule* mod, std::shared_ptr<IMobileAgent> mobileAgent)
{
    throw cRuntimeError("Do not use TraCIGenericScenarioManager directly. Use child class and override this method");
}
void TraCIGenericScenarioManager::updateModulePosition(cModule* mod, std::shared_ptr<IMobileAgent> mobileAgent)
{
    throw cRuntimeError("Do not use TraCIGenericScenarioManager directly. Use child class and override this method");
}
void TraCIGenericScenarioManager::addModule(std::string nodeId, std::string type, std::string name, std::string displayString, std::shared_ptr<IMobileAgent> mobileAgent)
{
    throw cRuntimeError("Do not use TraCIGenericScenarioManager directly. Use child class and override this method");
}

cModule* TraCIGenericScenarioManager::getManagedModule(std::string identifer){
    throw cRuntimeError("Do not use TraCIGenericScenarioManager directly. Use child class and override this method");
}

void TraCIGenericScenarioManager::deleteManagedModule(std::string identifer){
    throw cRuntimeError("Do not use TraCIGenericScenarioManager directly. Use child class and override this method");
}

MappingParser::TypeMappingTripel TraCIGenericScenarioManager::getTypeMapping(std::string typeId){

    std::string vType = typeId;
    MappingParser::TypeMappingTripel mapping;

    MappingParser::TypeMapping::iterator iType, iName, iDisplayString;

    MappingParser::TypeMapping::iterator i;
    iType = moduleType.find(vType);
    if (iType == moduleType.end()) {
        iType = moduleType.find("*");
        if (iType == moduleType.end()) throw cRuntimeError("cannot find a module type for mobile agent type \"%s\"", vType.c_str());
    }
    mapping.mType = iType->second;
    // search for module name
    iName = moduleName.find(vType);
    if (iName == moduleName.end()) {
        iName = moduleName.find(std::string("*"));
        if (iName == moduleName.end()) throw cRuntimeError("cannot find a module name for mobile agent type \"%s\"", vType.c_str());
    }
    mapping.mName = iName->second;
    if (moduleDisplayString.size() != 0) {
        iDisplayString = moduleDisplayString.find(vType);
        if (iDisplayString == moduleDisplayString.end()) {
            iDisplayString = moduleDisplayString.find("*");
            if (iDisplayString == moduleDisplayString.end()) throw cRuntimeError("cannot find a module display string for mobile agent type \"%s\"", vType.c_str());
        }
        mapping.mDisplayString = iDisplayString->second;
    }
    else {
        mapping.mDisplayString = "";
    }

    return mapping;
}

void TraCIGenericScenarioManager::parseModuleTypes()
{

    MappingParser::TypeMapping::iterator i;
    std::vector<std::string> typeKeys, nameKeys, displayStringKeys;

    std::string moduleTypes = par("moduleType").stdstringValue();
    std::string moduleNames = par("moduleName").stdstringValue();
    std::string moduleDisplayStrings = par("moduleDisplayString").stdstringValue();

    MappingParser parser{};


    moduleType = parser.parseMappings(moduleTypes, "moduleType", false);
    moduleName = parser.parseMappings(moduleNames, "moduleName", false);
    moduleDisplayString = parser.parseMappings(moduleDisplayStrings, "moduleDisplayString", true);

    // perform consistency check. for each vehicle type in moduleType there must be a vehicle type
    // in moduleName (and in moduleDisplayString if moduleDisplayString is not empty)

    // get all the keys
    for (i = moduleType.begin(); i != moduleType.end(); i++) typeKeys.push_back(i->first);
    for (i = moduleName.begin(); i != moduleName.end(); i++) nameKeys.push_back(i->first);
    for (i = moduleDisplayString.begin(); i != moduleDisplayString.end(); i++) displayStringKeys.push_back(i->first);

    // sort them (needed for intersection)
    std::sort(typeKeys.begin(), typeKeys.end());
    std::sort(nameKeys.begin(), nameKeys.end());
    std::sort(displayStringKeys.begin(), displayStringKeys.end());

    std::vector<std::string> intersection;

    // perform set intersection
    std::set_intersection(typeKeys.begin(), typeKeys.end(), nameKeys.begin(), nameKeys.end(), std::back_inserter(intersection));
    if (intersection.size() != typeKeys.size() || intersection.size() != nameKeys.size()) throw cRuntimeError("keys of mappings of moduleType and moduleName are not the same");

    if (displayStringKeys.size() == 0) return;

    intersection.clear();
    std::set_intersection(typeKeys.begin(), typeKeys.end(), displayStringKeys.begin(), displayStringKeys.end(), std::back_inserter(intersection));
    if (intersection.size() != displayStringKeys.size()) throw cRuntimeError("keys of mappings of moduleType and moduleName are not the same");
}

int TraCIGenericScenarioManager::getPortNumber() const
{
    int port = par("port");
    if (port != -1) {
        return port;
    }

    // search for externally configured traci port
    const char* env_port = std::getenv("VEINS_TRACI_PORT");
    if (env_port != nullptr) {
        port = std::atoi(env_port);
    }

    return port;
}

} /* namespace veins */

