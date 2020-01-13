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

#include "veins/veins.h"

#include "veins_inet/TraCIScenarioManagerRSO.h"
#include "VadereCommandInterface.h"


namespace veins{

class TraCIScenarioManagerVadere: public TraCIScenarioManagerRSO {
public:
    TraCIScenarioManagerVadere();
    virtual ~TraCIScenarioManagerVadere();

protected:
    virtual void init_traci() override;
//    virtual void init_obstacles() override;

    virtual void processSubcriptionResults() override;

};
} /** end namespace veins */

