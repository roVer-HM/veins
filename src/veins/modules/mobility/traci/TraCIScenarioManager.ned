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

package org.car2x.veins.modules.mobility.traci;

//
// Base class for a TraCIScenarioManager.
// To manually launch a TraCI server, run "sumo -c <my.sumo.cfg> --remote-port 9999
//
// See the Veins website <a href="http://veins.car2x.org/"> for a tutorial, documentation, and publications </a>.
//
// @author Christoph Sommer, David Eckhoff
//
// @see TraCIMobility
// @see TraCIScenarioManager
//

simple TraCIScenarioManager extends TraCIGenericScenarioManager
{
    parameters:
        @display("i=block/network2");
        @class(veins::TraCIScenarioManager);

        string trafficLightModuleType = default("");  // module type to be used in the simulation for each managed traffic light
        string trafficLightModuleName = default("tls");  // module name to be used in the simulation for each managed traffic light
        string trafficLightFilter = default("");  // filter string to select which tls shall be subscribed, list sumo IDs separated by spaces
        string trafficLightModuleDisplayString = default("i=misc/node2;is=vs;r=0,,#707070,1");  // module displayString to be used in the simulation for each managed traffic light

        string roiRoads = default("");  // which roads (e.g. "hwy1 hwy2") are considered to consitute the region of interest, if not empty
        string roiRects = default("");  // which rectangles (e.g. "0,0-10,10 20,20-30,30) are considered to consitute the region of interest, if not empty. Note that these rectangles have to use TraCI (SUMO) coordinates and not OMNeT++. They can be easily read from sumo-gui.
        double penetrationRate = default(1); //the probability of a vehicle being equipped with Car2X technology
        
}

