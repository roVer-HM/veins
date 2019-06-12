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

#include "veins/modules/mobility/traci/TraCIScenarioManagerVadere.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"
#include "veins/modules/mobility/traci/TraCIConstants.h"

#include <sstream>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <fstream>

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

    TraCIScenarioManager::initialize(stage);
}

void TraCIScenarioManagerVadere::finish()
{
    TraCIScenarioManager::finish();
}

void TraCIScenarioManagerVadere::init_traci()
{
    {
        std::pair<uint32_t, std::string> version = getCommandInterface()->getVersion();
        uint32_t apiVersion = version.first;
        std::string serverVersion = version.second;

        if (apiVersion >= 20) {
            EV_DEBUG << "TraCI server \"" << serverVersion << "\" reports API version " << apiVersion << endl;
        }
        else {
            error("TraCI server \"%s\" reports API version %d, which is unsupported. We recommend using the version of sumo-launchd that ships with Veins.", serverVersion.c_str(), apiVersion);
        }
    }

    TraCIBuffer buf;
    buf << scenarioFilePath << scenarioFileContent;
    connection->sendMessage(makeTraCICommand(CMD_FILE_SEND, buf));

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
