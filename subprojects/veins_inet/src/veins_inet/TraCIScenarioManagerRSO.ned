//
// Copyright (C) 2006-2012 Christoph Sommer <christoph.sommer@uibk.ac.at>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: GPL-2.0-or-later
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

package org.car2x.veins.subprojects.veins_inet;

import org.car2x.veins.modules.mobility.traci.TraCIGenericScenarioManager;

//
// RemoteSimulationObject based ScenarioManager which allows dynamic selection of variables which will be subscribed. 
//
// See the Veins website <a href="http://veins.car2x.org/"> for a tutorial, documentation, and publications </a>.
//
// @author Stefan Schuhbäck
//
// @see TraCIMobility
// @see TraCIGenericScenarioManager
//

simple TraCIScenarioManagerRSO extends TraCIGenericScenarioManager
{
    parameters:
        @class(veins::TraCIScenarioManagerRSO);
        // name: Is a key which is used to create the correct SubscriptionManager for the type (e.g.SubscriptionManager<SumoVehicle>)
        // vars: List of varibales which shold be subscribed to. (Names correspond to TraCIConstant::)
        string VehicleRSO = default("name=SumoVehicle vars=ID_LIST,VAR_POSITION,VAR_ROAD_ID,VAR_SPEED,VAR_ANGLE,VAR_SIGNALS,VAR_LENGTH,VAR_HEIGHT,VAR_WIDTH,VAR_TYPE");
        string SimulationRSO = default("name=SumoSimulation vars=VAR_TIME");
        string TrafficLightRSO = default("");
        string PedestrianRSO = default("");
        string moduleAPI = default("node=SumoVehicle");
        xml launchConfig; // launch configuration to send to sumo-launchd.py
}

