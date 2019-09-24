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

#pragma once

#include <map>
#include <memory>
#include <list>
#include <queue>

#include "veins/veins.h"

#include "veins/modules/mobility/traci/TraCIGenericScenarioManager.h"

namespace veins {
class TraCIScenarioManagerRSO: public veins::TraCIGenericScenarioManager {
public:
    TraCIScenarioManagerRSO();
    virtual ~TraCIScenarioManagerRSO();

    virtual void initialize(int stage) override;
protected:
    virtual void init_traci() override; // override it!
    virtual void processSubcriptionResult(TraCIBuffer& buf) override; // override it!

    typedef std::map<std::string, std::vector<std::string>> SubMgrVarMapping;
    SubMgrVarMapping subscriptionMgrType;
    TypeMapping moduleAPI;
};

} // end namepsace veins
