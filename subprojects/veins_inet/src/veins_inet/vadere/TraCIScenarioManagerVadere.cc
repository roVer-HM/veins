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


#include <string>
#include <fstream>

#include "veins/modules/mobility/traci/subscriptionManagement/RemoteSimulationObject.h"
#include "veins/modules/mobility/traci/subscriptionManagement/SubscriptionManager.h"
#include "VaderePerson.h"

#include "TraCIScenarioManagerVadere.h"
#include "VadereUtils.h"



using namespace veins::TraCIConstants;

Define_Module(veins::TraCIScenarioManagerVadere);

namespace veins{
TraCIScenarioManagerVadere::TraCIScenarioManagerVadere() {}

TraCIScenarioManagerVadere::~TraCIScenarioManagerVadere() {}


void TraCIScenarioManagerVadere::init_traci(){

    // default basedir is where current network file was loaded from
    std::string basedir = cSimulation::getActiveSimulation()->getEnvir()->getConfig()->getConfigEntry("network").getBaseDirectory();

    vadere::VadereScenario scenario = vadere::getScenarioContent(launchConfig, basedir);
    std::vector<vadere::VadereCache> cachePaths;
    cachePaths = vadere::getCachePaths(launchConfig, basedir);


    std::pair<uint32_t, std::string> version = commandIfc->getVersion();
    uint32_t apiVersion = version.first;
    vadere::vadereVersion serverVersion(version.second);

    //todo check vadere version.

    if (apiVersion >= 20) {
        EV_DEBUG << "TraCI server \"" << version.second << "\" reports API version " << apiVersion << endl;
    }
    else {
        throw cRuntimeError("TraCI server \"%s\" reports API version %d, which is unsupported. We recommend using the version of sumo-launchd that ships with Veins.", version.second, apiVersion);
    }

    TraCIBuffer buf;
    // if vadere version supports cache data and cache was found.
    // build command [String:FilnameScenario, String:ScenarioConntent, int:NumberOfCaches, [String:cacheIdentifier, int:numberOfBytes, bytes...] ... []]
    if(serverVersion >= vadere::cacheVersion && !cachePaths.empty()){
        buf << scenario.first << scenario.second;
        uint32_t numberOfHashes = static_cast<uint32_t>(cachePaths.size());
        // add number of hashes (Integer: 4 byte)
        buf << numberOfHashes;

        for(vadere::VadereCache entry : cachePaths ){
            std::ifstream binF(entry.second, std::ifstream::binary);
            if(binF){
                binF.seekg(0, binF.end);
                int len = binF.tellg();
                binF.seekg(0, binF.beg);
                char* cacheBuffer = new char [len];
                binF.read(cacheBuffer, len);

                // add cache identifier to buffer
                buf << entry.first << len;
                // add loaded cache (byte for byte) to buffer
                for(int i=0; i < len; i++){
                    buf << cacheBuffer[i];
                }
                binF.close();
                delete[] cacheBuffer;
            } else {
                error("The cache file should exist. \"%s\"", entry.second.c_str());
            }
        }

        connection->sendMessage(makeTraCICommand(CMD_FILE_SEND, buf));
    }
    // no cache. only send scenario file.
    else {
        buf << scenario.first << scenario.second;
        connection->sendMessage(makeTraCICommand(CMD_FILE_SEND, buf));
    }

    TraCIBuffer obuf(connection->receiveMessage());
    uint8_t cmdLength;
    obuf >> cmdLength;
    uint8_t commandResp;
    obuf >> commandResp;
    if (commandResp != CMD_FILE_SEND) error("Expected response to command %d, but got one for command %d", CMD_FILE_SEND, commandResp);
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
    // add configured RSO SubscriptionManagers

    for (auto & sub : subscriptionMgrType){
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

void TraCIScenarioManagerVadere::processSubcriptionResults(){

    if (subscriptionManager->has(TraCIConstants::RESPONSE_SUBSCRIBE_PERSON_VARIABLE)){
        std::shared_ptr<SubscriptionManager<VaderePerson>> vMgr = subscriptionManager->get<SubscriptionManager<VaderePerson>>(TraCIConstants::RESPONSE_SUBSCRIBE_PERSON_VARIABLE);
        for (auto const agent : vMgr->getRSOVector()){
            processMobileAgent(agent);
        }
        for (auto const &id : vMgr->getDeletedRSOs()){
            deleteManagedModule(id);
        }
    }
}

} /** end namespace veins */

