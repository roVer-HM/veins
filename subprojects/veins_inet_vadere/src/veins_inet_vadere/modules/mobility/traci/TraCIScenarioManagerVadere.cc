//
// Copyright (C) 2006-2012 Christoph Sommer <christoph.sommer@uibk.ac.at>
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

#include "TraCIScenarioManagerVadere.h"

namespace veins {

namespace TraCIConstants {

const uint8_t CMD_FILE_SEND = 0x75;

} // namespace TraCIConstants


} // namespace veins

using namespace veins::TraCIConstants;

using veins::TraCIScenarioManagerVadere;

Define_Module(veins::TraCIScenarioManagerVadere);

TraCIScenarioManagerVadere::~TraCIScenarioManagerVadere()
{
}

void TraCIScenarioManagerVadere::initialize(int stage)
{
    if (stage != 1) {
        TraCIScenarioManager::initialize(stage);
        return;
    }
    scenarioFilePath = par("scenarioFilePath").stdstringValue();

    // default basedir is where current network file was loaded from
    std::string basedir = cSimulation::getActiveSimulation()->getEnvir()->getConfig()->getConfigEntry("network").getBaseDirectory();

    std::string filePath = basedir + scenarioFilePath;
    std::ifstream file(filePath.c_str());
    if (file){
        scenarioFileContent = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    } else {
        error("Cannot read scenario file at: \"%s\"", filePath.c_str());
    }

    cache_config = par("cache_config").stdstringValue();
    std::string cache_suffixes_str = par("cache_suffixes").stdstringValue();
    cache_suffixes = stringTokens(cache_suffixes_str, ",");


    std::string cacheConfigPath = basedir + cache_config;
    std::ifstream cacheConfigFile(cacheConfigPath.c_str());

    if(cacheConfigFile){
        while(cacheConfigFile){
            std::string line;
            std::getline(cacheConfigFile, line);
            // include cache if line does not start with a '#'
            if(line == "")
                continue;
            if(line.substr(0, 1) != "#"){
                std::vector<std::string> cache = stringTokens(line, ",");
                // check if file exists
                std::string fullPath = basedir + cache[1];
                std::ifstream f(fullPath.c_str());
                if(f.good()){
                    cachePath.insert(std::make_pair(cache[0], cache[1]));
                }
            }
        }
    }
    simtime_t firstTarget = 51.6;
    targetTrigger = new cMessage("targetTrigger");
    scheduleAt(firstTarget, targetTrigger);
    TraCIScenarioManager::initialize(stage);
}

void TraCIScenarioManagerVadere::handleSelfMsg(cMessage* msg)
{

    if (msg == targetTrigger){
        simtime_t curr = simTime();
        simtime_t next;
        std::string targetId = "2";
        if(curr == 51.6){
            TraCIBuffer p;
            std::string elId = "33";
            p << static_cast<uint8_t>(0xfe) << elId; //trargetlist + id
            int32_t size = 1;
            p << static_cast<uint8_t>(0x0E) << size;
            p << targetId;
            TraCIBuffer buf = connection->query(0xce, p);
            ASSERT(buf.eof());
            next = 66.0;
            scheduleAt(next, targetTrigger);
        }
        if(curr == 66.0){
            TraCIBuffer p;
            std::string elId = "39";
            p << static_cast<uint8_t>(0xfe) << elId; //trargetlist + id
            int32_t size = 1;
            p << static_cast<uint8_t>(0x0E) << size;
            p << targetId;
            TraCIBuffer buf = connection->query(0xce, p);
            ASSERT(buf.eof());
            next = 69.2;
            scheduleAt(next, targetTrigger);
        }
        if(curr == 69.2){
            TraCIBuffer p;
            std::string elId = "45";
            p << static_cast<uint8_t>(0xfe) << elId; //trargetlist + id
            int32_t size = 1;
            p << static_cast<uint8_t>(0x0E) << size;
            p << targetId;
            TraCIBuffer buf = connection->query(0xce, p);
            ASSERT(buf.eof());
            next = 78.4;
            scheduleAt(next, targetTrigger);
        }
        if(curr == 78.4){
            TraCIBuffer p;
            std::string elId = "47";
            p << static_cast<uint8_t>(0xfe) << elId; //trargetlist + id
            int32_t size = 1;
            p << static_cast<uint8_t>(0x0E) << size;
            p << targetId;
            TraCIBuffer buf = connection->query(0xce, p);
            ASSERT(buf.eof());
            next = 262.0;
            scheduleAt(next, targetTrigger);
        }
        if(curr == 262.0){
            TraCIBuffer p;
            std::string elId = "173";
            p << static_cast<uint8_t>(0xfe) << elId; //trargetlist + id
            int32_t size = 1;
            p << static_cast<uint8_t>(0x0E) << size;
            p << targetId;
            TraCIBuffer buf = connection->query(0xce, p);
            ASSERT(buf.eof());
        }
    } else {
        TraCIScenarioManager::handleSelfMsg(msg);
    }
}

std::vector<std::string> TraCIScenarioManagerVadere::stringTokens(std::string& data, const char *sep)
{
    // remove white spaces see: https://stackoverflow.com/a/83481
    std::string::iterator end_pos = std::remove(data.begin(), data.end(), ' ');
    data.erase(end_pos, data.end());

    cStringTokenizer tokenizer(data.c_str(), sep);
    return tokenizer.asVector();
}

void TraCIScenarioManagerVadere::finish()
{
    if (isConnected()) {
        // send close command to traci server if still connected.
        TraCIBuffer buf = connection->query(CMD_CLOSE, TraCIBuffer());
    }

    TraCIScenarioManager::finish();
}


TraCIScenarioManagerVadere::vadereTraCIVersion TraCIScenarioManagerVadere::buildVadereVersion(std::string versionString)
{
    vadereTraCIVersion version;
    size_t p1 = versionString.find('.');
    if (p1 == std::string::npos){
        error("Expected string of the form \"x.x.x\" but received: \"%s\" ", versionString.c_str());
    }
    size_t p2 = versionString.find('.', p1 +1);
    if (p2 == std::string::npos){
        error("Expected string of the form \"x.x.x\" but received: \"%s\" ", versionString.c_str());
    }
    //20.0.1 p1=2 p2=4 p3=6
    size_t p3 = versionString.length();
    version.traci = std::stoi(versionString.substr(0, p1)); // (2-0 = 2) 0-1
    version.major = std::stoi(versionString.substr(p1 + 1, p2 - p1 + 1)); // (4-3 = 1) 3
    version.minor = std::stoi(versionString.substr(p2 + 1, p3 - p2 + 1)); // (6-5 - 1) 5

    return version;
}

void TraCIScenarioManagerVadere::init_traci()
{
    {
        std::pair<uint32_t, std::string> version = getCommandInterface()->getVersion();
        uint32_t apiVersion = version.first;
        std::string serverVersion = version.second;

        std::size_t firstSpace = serverVersion.find_first_of(" ");
        if (firstSpace != std::string::npos){
            std::string  vadereTraCiVersion = serverVersion.substr(VADERE_VERSION_PREFIX, firstSpace - VADERE_VERSION_PREFIX); // should now be d.d.d
            vadereVersion = buildVadereVersion(vadereTraCiVersion); // save to member to acces later

        } else {
            error("Expected a VaderTraCI-x.x version identifier within the version string. versionString: \"%s\" ", serverVersion.c_str());
        }

        if (apiVersion >= 20) {
            EV_DEBUG << "TraCI server \"" << serverVersion << "\" reports API version " << apiVersion << endl;
        }
        else {
            error("TraCI server \"%s\" reports API version %d, which is unsupported. We recommend using the version of sumo-launchd that ships with Veins.", serverVersion.c_str(), apiVersion);
        }
    }

    TraCIBuffer buf;
    if(vadereVersion >= cacheVersion && !cachePath.empty()){
        buf << scenarioFilePath << scenarioFileContent;


        uint32_t numberOfHashes = static_cast<uint32_t>(cachePath.size());
        // add number of hashes (Integer: 4 byte)
        buf << numberOfHashes;

        for(std::pair<const std::string, const std::string> entry : cachePath ){
            std::ifstream binF(entry.second, std::ifstream::binary);
            if(binF){
                binF.seekg(0, binF.end);
                int len = binF.tellg();
                binF.seekg(0, binF.beg);
                char* cacheBuffer = new char [len];
                binF.read(cacheBuffer, len);

                // add hash to traci buffer
                buf << entry.first << len;
                for(int i=0; i < len; i++){
                    buf << cacheBuffer[i];
                }
                binF.close();
                delete[] cacheBuffer;
            } else {
                error("The cache file should exist. \"%s\"", entry.second.c_str());
            }
        }

        // load filenames (send with complete name including hash)
        // load data into byte[]
        // build command [String:FilnameScenario, String:ScenarioConntent, int:NumberOfCaches, [String:cacheIdentifier, int:numberOfBytes, bytes...] ... []]

        connection->sendMessage(makeTraCICommand(CMD_FILE_SEND, buf));
    } else {
        buf << scenarioFilePath << scenarioFileContent;
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


    TraCIScenarioManager::init_traci();

    subscriptionManager->clearAPI();
}
