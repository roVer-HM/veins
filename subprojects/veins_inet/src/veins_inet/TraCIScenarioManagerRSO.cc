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

#include <veins_inet/TraCIScenarioManagerRSO.h>
#include "veins/modules/mobility/traci/subscriptionManagement/RemoteSimulationObject.h"
#include "veins/modules/mobility/traci/subscriptionManagement/SumoVehicle.h"
#include "veins/modules/mobility/traci/subscriptionManagement/SumoPerson.h"
#include "veins/modules/mobility/traci/subscriptionManagement/SubscriptionManager.h"
#include "veins/base/connectionManager/ChannelAccess.h"

#include "veins_inet/VeinsInetManager.h"

#include "veins/base/utils/Coord.h"
#include "veins_inet/VeinsInetMobility.h"


using veins::VeinsInetManager;
using namespace veins::TraCISubscriptionManagement;
using namespace veins::TraCIConstants;
using veins::TraCIScenarioManagerRSO;
using veins::TraCIBuffer;

Define_Module(veins::TraCIScenarioManagerRSO);



namespace veins {

TraCIScenarioManagerRSO::TraCIScenarioManagerRSO()
    : TraCIGenericScenarioManager()
    , moduleVectorIndex()
    , subscriptionMgrType()
    , subscriptionManager(nullptr)
{
}

TraCIScenarioManagerRSO::~TraCIScenarioManagerRSO() {

}

std::pair<std::string, std::vector<std::uint8_t>> parseSubscriptionData(std::string data, std::string parameterName, std::string defaultKey){
    if (data.compare("") == 0){
        EV_DEBUG << parameterName << "not used in simulation" << endl;
        return std::pair<std::string, std::vector<uint8_t>>(defaultKey, {});
    }

    cStringTokenizer stk(data.c_str(), " ");
    std::vector<std::string> values = stk.asVector();
    ASSERT(values.size() == 2);
    std::pair<std::string, std::vector<uint8_t>> ret;
    for (std::string val : values){

        if (val.substr(0, 5).compare("name=") == 0){
            ret.first=val.substr(5, std::string::npos );
        } else if(val.substr(0, 5).compare("vars=") == 0){
            cStringTokenizer stk2(val.substr(5, std::string::npos).c_str(), ",");
            std::vector<std::string> varStrings = stk2.asVector();
            std::vector<uint8_t> varCodes;
            for (const auto &var : varStrings ){
                auto iter = TraCISubscriptionManagement::RemoteSimulationObject::varLookup.find(var);
                if (iter == TraCISubscriptionManagement::RemoteSimulationObject::varLookup.end()){
                    throw cRuntimeError("Variable string '%s' from paramter '%s' does not correspond to a known variable", var.c_str(), parameterName.c_str());
                } else {
                    varCodes.push_back(iter->second);
                }
            }
            ret.second=varCodes;
        } else {
            throw cRuntimeError("string must contains be field name= and vars= but it contains: '%s'", data.c_str());
        }
    }

    return ret;
}

void TraCIScenarioManagerRSO::initialize(int stage)
{

    TraCIGenericScenarioManager::initialize(stage);
    if (stage != 1) {
        return;
    }

    for (auto & module : moduleType){
        moduleVectorIndex.insert(std::make_pair(module.second, 0));
    }

    moduleAPI = parseMappings(par("moduleAPI").stdstringValue(), "moduleAPI", false);
    subscriptionMgrType.insert(parseSubscriptionData(par("VehicleRSO").stdstringValue(), "VehicleRSO", "defaultKey"));
    subscriptionMgrType.insert(parseSubscriptionData(par("SimulationRSO").stdstringValue(), "SimulationRSO", "defaultKey"));
    subscriptionMgrType.insert(parseSubscriptionData(par("TrafficLightRSO").stdstringValue(), "TrafficLightRSO", "defaultKey"));
    subscriptionMgrType.insert(parseSubscriptionData(par("PedestrianRSO").stdstringValue(), "PedestrianRSO", "defaultKey"));
    subscriptionMgrType.erase("defaultKey");

    launchConfig = par("launchConfig").xmlValue();
    seed = par("seed");

}

void TraCIScenarioManagerRSO::finish()
{
    for (auto &types : hosts){
        auto &hostsMap = types.second;
        auto hostsIter = types.second.begin();
        while (hostsIter != types.second.end()){
            EV_DEBUG << "Delete module.... type: " << types.first << " moduleId: " << hostsIter->first << endl;
            deleteManagedModule(hostsIter->second);
            hostsIter = hostsMap.erase(hostsIter);
        }
    }
}

void TraCIScenarioManagerRSO::init_traci(){

    {
        cXMLElementList basedir_nodes = launchConfig->getElementsByTagName("basedir");
        if (basedir_nodes.size() == 0) {
            // default basedir is where current network file was loaded from
            std::string basedir = cSimulation::getActiveSimulation()->getEnvir()->getConfig()->getConfigEntry("network").getBaseDirectory();
            cXMLElement* basedir_node = new cXMLElement("basedir", __FILE__, launchConfig);
            basedir_node->setAttribute("path", basedir.c_str());
            launchConfig->appendChild(basedir_node);
        }
        cXMLElementList seed_nodes = launchConfig->getElementsByTagName("seed");
        if (seed_nodes.size() == 0) {
            if (seed == -1) {
                // default seed is current repetition
                const char* seed_s = cSimulation::getActiveSimulation()->getEnvir()->getConfigEx()->getVariable(CFGVAR_RUNNUMBER);
                seed = atoi(seed_s);
            }
            std::stringstream ss;
            ss << seed;
            cXMLElement* seed_node = new cXMLElement("seed", __FILE__, launchConfig);
            seed_node->setAttribute("value", ss.str().c_str());
            launchConfig->appendChild(seed_node);
        }
    }

    {
        std::pair<uint32_t, std::string> version = commandIfc->getVersion();
        uint32_t apiVersion = version.first;
        std::string serverVersion = version.second;

        if (apiVersion == 1) {
            EV_DEBUG << "TraCI server \"" << serverVersion << "\" reports API version " << apiVersion << endl;
        }
        else {
            throw cRuntimeError("TraCI server \"%s\" reports API version %d, which is unsupported. We recommend using the version of sumo-launchd that ships with Veins.", serverVersion.c_str(), apiVersion);
        }
    }

#if OMNETPP_VERSION <= 0x500
    std::string contents = launchConfig->tostr(0);
#else
    std::string contents = launchConfig->getXML();
#endif

    TraCIBuffer buf;
    buf << std::string("sumo-launchd.launch.xml") << contents;
    connection->sendMessage(makeTraCICommand(CMD_FILE_SEND, buf));

    TraCIBuffer obuf(connection->receiveMessage());
    uint8_t cmdLength;
    obuf >> cmdLength;
    uint8_t commandResp;
    obuf >> commandResp;
    if (commandResp != CMD_FILE_SEND) throw cRuntimeError("Expected response to command %d, but got one for command %d", CMD_FILE_SEND, commandResp);
    uint8_t result;
    obuf >> result;
    std::string description;
    obuf >> description;
    if (result != RTYPE_OK) {
        EV << "Warning: Received non-OK response from TraCI server to command " << CMD_FILE_SEND << ":" << description.c_str() << std::endl;
    }

    {
        auto apiVersion = commandIfc->getVersion();
        EV_DEBUG << "TraCI server \"" << apiVersion.second << "\" reports API version " << apiVersion.first << endl;
        commandIfc->setApiVersion(apiVersion.first);
    }

    {
        // query and set road network boundaries
        auto networkBoundaries = commandIfc->initNetworkBoundaries(par("margin"));
        if (world != nullptr && ((connection->traci2omnet(networkBoundaries.second).x > world->getPgs()->x) || (connection->traci2omnet(networkBoundaries.first).y > world->getPgs()->y))) {
            EV_DEBUG << "WARNING: Playground size (" << world->getPgs()->x << ", " << world->getPgs()->y << ") might be too small for vehicle at network bounds (" << connection->traci2omnet(networkBoundaries.second).x << ", " << connection->traci2omnet(networkBoundaries.first).y << ")" << endl;
        }
    }

    // create new executive manager.
    subscriptionManager.reset(new ExecutiveSubscriptionManager(connection, commandIfc, true));
    for (auto & sub : subscriptionMgrType){
        // build subscriptionfactory class name from remote simulation object name given in config.
        std::string cls = sub.first + "SubFactory";
        cObject* obj = createOneIfClassIsKnown(cls.c_str());
        if (!obj){
            throw cRuntimeError("SubscriptionFactory \'%s\' not found for given RemoteSimulationObject \'%s\'", cls.c_str(), sub.first.c_str());
        }
        SubscriptionManagerFactory* f = check_and_cast<SubscriptionManagerFactory*>(obj);
        subscriptionManager->addSubscriptionManager(f->createSubscriptionManager(sub.second, connection, commandIfc));

    }
    subscriptionManager->initialize();

}

void TraCIScenarioManagerRSO::executeOneTimestep()
{

    EV_DEBUG << "Triggering TraCI server simulation advance to t=" << simTime() << endl;

    simtime_t targetTime = simTime();

    emit(traciTimestepBeginSignal, targetTime);

    if (isConnected()) {
        TraCIBuffer buf = connection->query(CMD_SIMSTEP2, TraCIBuffer() << targetTime);

        subscriptionManager->processSubscriptionResultBuffer(buf);
        processSubcriptionResults();

    }

    emit(traciTimestepEndSignal, targetTime);

    if (!autoShutdownTriggered) scheduleAt(simTime() + updateInterval, executeOneTimestepTrigger);
}


void TraCIScenarioManagerRSO::processSubcriptionResults(){

    if (subscriptionManager->has(TraCIConstants::RESPONSE_SUBSCRIBE_VEHICLE_VARIABLE)){
        std::shared_ptr<SubscriptionManager<SumoVehicle>> vMgr = subscriptionManager->get<SubscriptionManager<SumoVehicle>>(TraCIConstants::RESPONSE_SUBSCRIBE_VEHICLE_VARIABLE);
        for (auto const agent : vMgr->getRSOVector()){
            processMobileAgent(agent);
        }
        for (auto const &id : vMgr->getDeletedRSOs()){
            deleteManagedModule(id);
        }
    }

    if (subscriptionManager->has(TraCIConstants::RESPONSE_SUBSCRIBE_PERSON_VARIABLE)){
        std::shared_ptr<SubscriptionManager<SumoPerson>> pMgr = subscriptionManager->get<SubscriptionManager<SumoPerson>>(TraCIConstants::RESPONSE_SUBSCRIBE_PERSON_VARIABLE);
        for (auto const agent : pMgr->getRSOVector()){
            processMobileAgent(agent);
        }
        for (auto const &id : pMgr->getDeletedRSOs()){
            deleteManagedModule(id);
        }
    }

}

cModule* TraCIScenarioManagerRSO::getManagedModule(std::string nodeId)
{
    for (auto const & hostMap : hosts){
        auto const &it = hostMap.second.find(nodeId);
        if (it != hostMap.second.end()){
            EV_DEBUG << "found host in moduleType: " << hostMap.first;
            return it->second;
        }
    }
    return nullptr;
}

cModule* TraCIScenarioManagerRSO::getAndRemoveManagedModule(std::string nodeId)
{
    for (auto  &hostMap : hosts){
        auto it = hostMap.second.find(nodeId);
        if (it != hostMap.second.end()){
            EV_DEBUG << "found host in moduleType: " << hostMap.first;
            cModule* mod = it->second;
            hostMap.second.erase(it);
            return mod;
        }
    }
    return nullptr;
}


cModule* TraCIScenarioManagerRSO::getManagedModule(std::string moduleType, std::string nodeId)
{
    auto const &hostMap = hosts.find(moduleType);
    if (hostMap == hosts.end()){
        throw cRuntimeError("given moduleType '%s' not found in host map. (search nodeId was '%s')", moduleType.c_str(), nodeId.c_str());
    }
    auto const &nodeIter = hostMap->second.find(nodeId);
    if (nodeIter == hostMap->second.end()) return nullptr;
        return nodeIter->second;
}

void TraCIScenarioManagerRSO::addManagedModule(std::string moduleType, std::string nodeId, cModule* mod){
    hosts[moduleType].insert(std::make_pair(nodeId, mod));
}

void TraCIScenarioManagerRSO::processMobileAgent(std::shared_ptr<IMobileAgent> mobileAgent){
   // Find corresponding module
   // ------------------------------
   cModule* mod = getManagedModule(mobileAgent->getId());

   //Check if unequipped
   // ------------------------------
   // todo: add type specifc penetrationRate

   // Update or create module
   // ------------------------------
   if (!mod){

       TypeMappingTripel m = getTypeMapping(mobileAgent->getTypeId());

       if (m.mType != "0") {
           addModule(mobileAgent->getId(), m.mType, m.mName, m.mDisplayString, mobileAgent);
           EV_DEBUG << "Added mobile agent #" << mobileAgent->getId() << endl;
       }
   } else {
       // module exists - update position
       EV_DEBUG << "mobile agent " << mobileAgent->getId() << " moving to " << mobileAgent->getPosition().x << ", " << mobileAgent->getPosition().y << endl;
       updateModulePosition(mod, mobileAgent);
   }
}

int32_t TraCIScenarioManagerRSO::nextVectorIndex(std::string module){
    if (moduleVectorIndex.find(module) == moduleVectorIndex.end())
        throw cRuntimeError("module '%s' not found in VectorIndex Map", module.c_str());

    return moduleVectorIndex[module]++;
}

void TraCIScenarioManagerRSO::addModule(std::string nodeId, std::string type, std::string name, std::string displayString, std::shared_ptr<IMobileAgent> mobileAgent){

    if (hosts.find(nodeId) != hosts.end()) throw cRuntimeError("tried adding duplicate module '%s'", nodeId.c_str());

    // todo: set penetrationRate

    int32_t nodeVectorIndex = nextVectorIndex(type);

    cModule* parentmod = getParentModule();
    if (!parentmod) throw cRuntimeError("Parent Module not found");

    cModuleType* nodeType = cModuleType::get(type.c_str());
    if (!nodeType) throw cRuntimeError("Module Type \"%s\" not found", type.c_str());

    // TODO: this trashes the vectsize member of the cModule, although nobody seems to use it
    cModule* mod = nodeType->create(name.c_str(), parentmod, nodeVectorIndex, nodeVectorIndex);
    mod->finalizeParameters();
    if (displayString.length() > 0) {
        mod->getDisplayString().parse(displayString.c_str());
    }
    mod->buildInside();
    mod->scheduleStart(simTime() + updateInterval);

    preInitializeModule(mod, mobileAgent);

    mod->callInitialize();
    addManagedModule(type, nodeId, mod);

    emit(traciModuleAddedSignal, mod);
}

void TraCIScenarioManagerRSO::deleteManagedModule(std::string nodeId)
{
    cModule* mod = getAndRemoveManagedModule(nodeId);
    if (!mod) throw cRuntimeError("no vehicle with Id \"%s\" found", nodeId.c_str());

    deleteManagedModule(mod);
}

void TraCIScenarioManagerRSO::deleteManagedModule(cModule * mod)
{
    emit(traciModuleRemovedSignal, mod);

    auto cas = getSubmodulesOfType<ChannelAccess>(mod, true);
    for (auto ca : cas) {
        cModule* nic = ca->getParentModule();
        auto connectionManager = ChannelAccess::getConnectionManager(nic);
        connectionManager->unregisterNic(nic);
    }

    mod->callFinish();
    mod->deleteModule();
}


void TraCIScenarioManagerRSO::preInitializeModule(cModule* mod, std::shared_ptr<IMobileAgent> mobileAgent)
{
    // pre-initialize VeinsInetMobility
    auto mobilityModules = getSubmodulesOfType<VeinsInetMobilityBase>(mod);
    for (auto inetmm : mobilityModules) {
        inetmm->preInitialize(mobileAgent);
    }
}

void TraCIScenarioManagerRSO::updateModulePosition(cModule* mod, std::shared_ptr<IMobileAgent> mobileAgent)
{
    // update position in VeinsInetMobility
    auto mobilityModules = getSubmodulesOfType<VeinsInetMobilityBase>(mod);
    for (auto inetmm : mobilityModules) {
        inetmm->nextPosition(mobileAgent);
    }
}


} // end namepsace veins

