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

#include <veins/modules/mobility/traci/TraCIScenarioManagerRSO.h>

using namespace veins::TraCIConstants;
using veins::TraCIScenarioManagerRSO;
using veins::TraCIBuffer;

Define_Module(veins::TraCIScenarioManagerRSO);

namespace veins {

TraCIScenarioManagerRSO::TraCIScenarioManagerRSO()
    : TraCIGenericScenarioManager()
    , subscriptionMgrType()
{
    // TODO Auto-generated constructor stub

}

TraCIScenarioManagerRSO::~TraCIScenarioManagerRSO() {
    // TODO Auto-generated destructor stub
}

std::pair<std::string, std::vector<std::string>> parseSubscriptionData(std::string data, std::string parameterName, std::string defaultKey){
    if (data.compare("") == 0){
        EV_DEBUG << parameterName << "not used in simulation" << endl;
        return std::pair<std::string, std::vector<std::string>>(defaultKey, {});
    }

    cStringTokenizer stk(data.c_str(), " ");
    std::vector<std::string> values = stk.asVector();
    ASSERT(values.size() == 2);
    std::pair<std::string, std::vector<std::string>> ret;
    for (std::string val : values){

        if (val.substr(0, 5).compare("name=") == 0){
            ret.first=val.substr(5, std::string::npos );
        } else if(val.substr(0, 5).compare("vars=") == 0){
            cStringTokenizer stk2(val.substr(5, std::string::npos).c_str(), ",");
            ret.second=stk2.asVector();
        } else {
            throw cRuntimeError("string must contains be field name= and vars= but it contains: '%s'", data);
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

    moduleAPI = parseMappings(par("moduleAPI").stdstringValue(), "moduleAPI", false);
    subscriptionMgrType.insert(parseSubscriptionData(par("VehicleRSO").stdstringValue(), "VehicleRSO", "defaultKey"));
    subscriptionMgrType.insert(parseSubscriptionData(par("SimulationRSO").stdstringValue(), "VehicleRSO", "defaultKey"));
    subscriptionMgrType.insert(parseSubscriptionData(par("TrafficLightRSO").stdstringValue(), "VehicleRSO", "defaultKey"));
    subscriptionMgrType.insert(parseSubscriptionData(par("PedestrianRSO").stdstringValue(), "VehicleRSO", "defaultKey"));
    subscriptionMgrType.erase("defaultKey");
}

void TraCIScenarioManagerRSO::init_traci(){

}

void TraCIScenarioManagerRSO::processSubcriptionResult(TraCIBuffer& buf){

}

} // end namepsace veins

