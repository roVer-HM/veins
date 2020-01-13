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

package org.car2x.veins.subprojects.veins_inet.vadere;

import org.car2x.veins.subprojects.veins_inet.TraCIScenarioManagerRSO;

//
// TODO auto-generated type
//
simple TraCIScenarioManagerVadere extends TraCIScenarioManagerRSO
{
    parameters:
        @class(veins::TraCIScenarioManagerVadere);
        VehicleRSO = "";
        SimulationRSO = "";
        TrafficLightRSO = "";
        PedestrianRSO = default("name=veins::TraCISubscriptionManagement::VaderePerson vars=ID_LIST,VAR_POSITION,VAR_TYPE,VAR_ANGLE,VAR_SPEED");
        string vadereScenarioPath;
        string vadereCachePath = default("");
}
