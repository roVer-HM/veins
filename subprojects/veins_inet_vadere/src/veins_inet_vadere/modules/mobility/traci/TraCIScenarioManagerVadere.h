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

#pragma once

#include "veins/veins.h"

#include "veins/modules/mobility/traci/TraCIScenarioManager.h"

namespace veins {

/**
 * @brief
 * Extends the TraCIScenarioManager for use with sumo-launchd.py and SUMO.
 *
 * Connects to a running instance of the sumo-launchd.py script
 * to automatically launch/kill SUMO when the simulation starts/ends.
 *
 * All other functionality is provided by the TraCIScenarioManager.
 *
 * See the Veins website <a href="http://veins.car2x.org/"> for a tutorial, documentation, and publications </a>.
 *
 * @author Christoph Sommer, David Eckhoff
 *
 * @see TraCIMobility
 * @see TraCIScenarioManager
 *
 */
class VEINS_API TraCIScenarioManagerVadere : public TraCIScenarioManager {
public:
    ~TraCIScenarioManagerVadere() override;
    void initialize(int stage) override;
    void finish() override;

protected:
    std::string scenarioFilePath; /**< path to scenario file used to setup Vadere */

    void init_traci() override;

private:
    std::string scenarioFileContent; /**< content of scenario file (Json based description) */
};

class VEINS_API TraCIScenarioManagerVadereAccess {
public:
    TraCIScenarioManagerVadere* get()
    {
        return FindModule<TraCIScenarioManagerVadere*>::findGlobalModule();
    };
};

} // namespace veins
