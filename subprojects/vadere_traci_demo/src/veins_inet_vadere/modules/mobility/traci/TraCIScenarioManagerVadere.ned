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

package org.car2x.veins.subprojects.veins_inet_vadere.modules.mobility.traci;

// Based on veins::TraCIScenarioManagerLauchd.ned This Manager connects to the Vadere Pedestrian
// Simulation Framework
//
// @author Stefan Schuhaeck
//

import org.car2x.veins.modules.mobility.traci.TraCIScenarioManager;

simple TraCIScenarioManagerVadere extends TraCIScenarioManager
{
    parameters:
        @class(veins::TraCIScenarioManagerVadere);
        string scenarioFilePath; // launch configuration to send to Vadere
        string cache_config = default ("cache.config");
        string cache_suffixes = default (".ffcace, .csv"); // order defines priority
}