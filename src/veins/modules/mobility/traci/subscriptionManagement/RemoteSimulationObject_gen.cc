//
// Copyright (C) 2019 Stefan Schuhbaeck <stefan.schuhbaeck@hm.edu>
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

// This file is generated using `cog` program. Don not modify manually. Use RemoteSimulationObject_gen.cog. 

#include <veins/modules/mobility/traci/subscriptionManagement/RemoteSimulationObject.h>
#include "veins/modules/mobility/traci/TraCIConstants.h"


namespace veins {
namespace TraCISubscriptionManagement {

/*[[[cog
import cog, re
prog=re.compile('^const\\s+uint8_t\\s+(?P<var>(VAR|ID).*)\\s+=\\s+(?P<val>0x[0-9,a-f]{2})\\s*;')
data=[]
with open("../TraCIConstants.h", 'r') as f:
	for line in f:
		ret = prog.match(line)
		if ret:
			data.append(f'\t{{"{ret["var"]}", TraCIConstants::{ret["var"]}}},')
		

cog.outl("const std::map<std::string, uint8_t> RemoteSimulationObject::varLookup = {")
if len(data) > 0:
	for d in data:
		cog.outl(d)
else:
	cog.outl('{"", 0x00},')
cog.outl('};')
]]]*/
const std::map<std::string, uint8_t> RemoteSimulationObject::varLookup = {
	{"ID_COUNT", TraCIConstants::ID_COUNT},
	{"ID_LIST", TraCIConstants::ID_LIST},
	{"VAR_ACCEL", TraCIConstants::VAR_ACCEL},
	{"VAR_ACCELERATION", TraCIConstants::VAR_ACCELERATION},
	{"VAR_ACCUMULATED_WAITING_TIME", TraCIConstants::VAR_ACCUMULATED_WAITING_TIME},
	{"VAR_ACTIONSTEPLENGTH", TraCIConstants::VAR_ACTIONSTEPLENGTH},
	{"VAR_ALLOWED_SPEED", TraCIConstants::VAR_ALLOWED_SPEED},
	{"VAR_ANGLE", TraCIConstants::VAR_ANGLE},
	{"VAR_APPARENT_DECEL", TraCIConstants::VAR_APPARENT_DECEL},
	{"VAR_ARRIVED_VEHICLES_IDS", TraCIConstants::VAR_ARRIVED_VEHICLES_IDS},
	{"VAR_ARRIVED_VEHICLES_NUMBER", TraCIConstants::VAR_ARRIVED_VEHICLES_NUMBER},
	{"VAR_BEST_LANES", TraCIConstants::VAR_BEST_LANES},
	{"VAR_BUS_STOP_WAITING", TraCIConstants::VAR_BUS_STOP_WAITING},
	{"VAR_CO2EMISSION", TraCIConstants::VAR_CO2EMISSION},
	{"VAR_COEMISSION", TraCIConstants::VAR_COEMISSION},
	{"VAR_COLLIDING_VEHICLES_IDS", TraCIConstants::VAR_COLLIDING_VEHICLES_IDS},
	{"VAR_COLLIDING_VEHICLES_NUMBER", TraCIConstants::VAR_COLLIDING_VEHICLES_NUMBER},
	{"VAR_COLOR", TraCIConstants::VAR_COLOR},
	{"VAR_CURRENT_TRAVELTIME", TraCIConstants::VAR_CURRENT_TRAVELTIME},
	{"VAR_DECEL", TraCIConstants::VAR_DECEL},
	{"VAR_DELTA_T", TraCIConstants::VAR_DELTA_T},
	{"VAR_DEPARTED_VEHICLES_IDS", TraCIConstants::VAR_DEPARTED_VEHICLES_IDS},
	{"VAR_DEPARTED_VEHICLES_NUMBER", TraCIConstants::VAR_DEPARTED_VEHICLES_NUMBER},
	{"VAR_DISTANCE", TraCIConstants::VAR_DISTANCE},
	{"VAR_EDGES", TraCIConstants::VAR_EDGES},
	{"VAR_EDGE_EFFORT", TraCIConstants::VAR_EDGE_EFFORT},
	{"VAR_EDGE_TRAVELTIME", TraCIConstants::VAR_EDGE_TRAVELTIME},
	{"VAR_ELECTRICITYCONSUMPTION", TraCIConstants::VAR_ELECTRICITYCONSUMPTION},
	{"VAR_EMERGENCYSTOPPING_VEHICLES_IDS", TraCIConstants::VAR_EMERGENCYSTOPPING_VEHICLES_IDS},
	{"VAR_EMERGENCYSTOPPING_VEHICLES_NUMBER", TraCIConstants::VAR_EMERGENCYSTOPPING_VEHICLES_NUMBER},
	{"VAR_EMERGENCY_DECEL", TraCIConstants::VAR_EMERGENCY_DECEL},
	{"VAR_EMISSIONCLASS", TraCIConstants::VAR_EMISSIONCLASS},
	{"VAR_FILL", TraCIConstants::VAR_FILL},
	{"VAR_FOES", TraCIConstants::VAR_FOES},
	{"VAR_FUELCONSUMPTION", TraCIConstants::VAR_FUELCONSUMPTION},
	{"VAR_HAS_VIEW", TraCIConstants::VAR_HAS_VIEW},
	{"VAR_HCEMISSION", TraCIConstants::VAR_HCEMISSION},
	{"VAR_HEIGHT", TraCIConstants::VAR_HEIGHT},
	{"VAR_IMPERFECTION", TraCIConstants::VAR_IMPERFECTION},
	{"VAR_LANECHANGE_MODE", TraCIConstants::VAR_LANECHANGE_MODE},
	{"VAR_LANEPOSITION", TraCIConstants::VAR_LANEPOSITION},
	{"VAR_LANEPOSITION_LAT", TraCIConstants::VAR_LANEPOSITION_LAT},
	{"VAR_LANE_ID", TraCIConstants::VAR_LANE_ID},
	{"VAR_LANE_INDEX", TraCIConstants::VAR_LANE_INDEX},
	{"VAR_LASTACTIONTIME", TraCIConstants::VAR_LASTACTIONTIME},
	{"VAR_LATALIGNMENT", TraCIConstants::VAR_LATALIGNMENT},
	{"VAR_LEADER", TraCIConstants::VAR_LEADER},
	{"VAR_LENGTH", TraCIConstants::VAR_LENGTH},
	{"VAR_LINE", TraCIConstants::VAR_LINE},
	{"VAR_LOADED_VEHICLES_IDS", TraCIConstants::VAR_LOADED_VEHICLES_IDS},
	{"VAR_LOADED_VEHICLES_NUMBER", TraCIConstants::VAR_LOADED_VEHICLES_NUMBER},
	{"VAR_MAXSPEED", TraCIConstants::VAR_MAXSPEED},
	{"VAR_MAXSPEED_LAT", TraCIConstants::VAR_MAXSPEED_LAT},
	{"VAR_MINGAP", TraCIConstants::VAR_MINGAP},
	{"VAR_MINGAP_LAT", TraCIConstants::VAR_MINGAP_LAT},
	{"VAR_MIN_EXPECTED_VEHICLES", TraCIConstants::VAR_MIN_EXPECTED_VEHICLES},
	{"VAR_MOVE_TO", TraCIConstants::VAR_MOVE_TO},
	{"VAR_MOVE_TO_VTD", TraCIConstants::VAR_MOVE_TO_VTD},
	{"VAR_NAME", TraCIConstants::VAR_NAME},
	{"VAR_NET_BOUNDING_BOX", TraCIConstants::VAR_NET_BOUNDING_BOX},
	{"VAR_NEXT_EDGE", TraCIConstants::VAR_NEXT_EDGE},
	{"VAR_NEXT_STOPS", TraCIConstants::VAR_NEXT_STOPS},
	{"VAR_NEXT_TLS", TraCIConstants::VAR_NEXT_TLS},
	{"VAR_NOISEEMISSION", TraCIConstants::VAR_NOISEEMISSION},
	{"VAR_NOXEMISSION", TraCIConstants::VAR_NOXEMISSION},
	{"VAR_PARAMETER", TraCIConstants::VAR_PARAMETER},
	{"VAR_PARKING_ENDING_VEHICLES_IDS", TraCIConstants::VAR_PARKING_ENDING_VEHICLES_IDS},
	{"VAR_PARKING_ENDING_VEHICLES_NUMBER", TraCIConstants::VAR_PARKING_ENDING_VEHICLES_NUMBER},
	{"VAR_PARKING_STARTING_VEHICLES_IDS", TraCIConstants::VAR_PARKING_STARTING_VEHICLES_IDS},
	{"VAR_PARKING_STARTING_VEHICLES_NUMBER", TraCIConstants::VAR_PARKING_STARTING_VEHICLES_NUMBER},
	{"VAR_PERSON_NUMBER", TraCIConstants::VAR_PERSON_NUMBER},
	{"VAR_PMXEMISSION", TraCIConstants::VAR_PMXEMISSION},
	{"VAR_POSITION", TraCIConstants::VAR_POSITION},
	{"VAR_POSITION3D", TraCIConstants::VAR_POSITION3D},
	{"VAR_ROAD_ID", TraCIConstants::VAR_ROAD_ID},
	{"VAR_ROUTE", TraCIConstants::VAR_ROUTE},
	{"VAR_ROUTE_ID", TraCIConstants::VAR_ROUTE_ID},
	{"VAR_ROUTE_INDEX", TraCIConstants::VAR_ROUTE_INDEX},
	{"VAR_ROUTE_VALID", TraCIConstants::VAR_ROUTE_VALID},
	{"VAR_ROUTING_MODE", TraCIConstants::VAR_ROUTING_MODE},
	{"VAR_SCREENSHOT", TraCIConstants::VAR_SCREENSHOT},
	{"VAR_SHAPE", TraCIConstants::VAR_SHAPE},
	{"VAR_SHAPECLASS", TraCIConstants::VAR_SHAPECLASS},
	{"VAR_SIGNALS", TraCIConstants::VAR_SIGNALS},
	{"VAR_SLOPE", TraCIConstants::VAR_SLOPE},
	{"VAR_SPEED", TraCIConstants::VAR_SPEED},
	{"VAR_SPEEDSETMODE", TraCIConstants::VAR_SPEEDSETMODE},
	{"VAR_SPEED_DEVIATION", TraCIConstants::VAR_SPEED_DEVIATION},
	{"VAR_SPEED_FACTOR", TraCIConstants::VAR_SPEED_FACTOR},
	{"VAR_SPEED_WITHOUT_TRACI", TraCIConstants::VAR_SPEED_WITHOUT_TRACI},
	{"VAR_STAGE", TraCIConstants::VAR_STAGE},
	{"VAR_STAGES_REMAINING", TraCIConstants::VAR_STAGES_REMAINING},
	{"VAR_STOPSTATE", TraCIConstants::VAR_STOPSTATE},
	{"VAR_STOP_ENDING_VEHICLES_IDS", TraCIConstants::VAR_STOP_ENDING_VEHICLES_IDS},
	{"VAR_STOP_ENDING_VEHICLES_NUMBER", TraCIConstants::VAR_STOP_ENDING_VEHICLES_NUMBER},
	{"VAR_STOP_STARTING_VEHICLES_IDS", TraCIConstants::VAR_STOP_STARTING_VEHICLES_IDS},
	{"VAR_STOP_STARTING_VEHICLES_NUMBER", TraCIConstants::VAR_STOP_STARTING_VEHICLES_NUMBER},
	{"VAR_TAU", TraCIConstants::VAR_TAU},
	{"VAR_TELEPORT_ENDING_VEHICLES_IDS", TraCIConstants::VAR_TELEPORT_ENDING_VEHICLES_IDS},
	{"VAR_TELEPORT_ENDING_VEHICLES_NUMBER", TraCIConstants::VAR_TELEPORT_ENDING_VEHICLES_NUMBER},
	{"VAR_TELEPORT_STARTING_VEHICLES_IDS", TraCIConstants::VAR_TELEPORT_STARTING_VEHICLES_IDS},
	{"VAR_TELEPORT_STARTING_VEHICLES_NUMBER", TraCIConstants::VAR_TELEPORT_STARTING_VEHICLES_NUMBER},
	{"VAR_TIME", TraCIConstants::VAR_TIME},
	{"VAR_TIME_STEP", TraCIConstants::VAR_TIME_STEP},
	{"VAR_TRACK_VEHICLE", TraCIConstants::VAR_TRACK_VEHICLE},
	{"VAR_TYPE", TraCIConstants::VAR_TYPE},
	{"VAR_UPDATE_BESTLANES", TraCIConstants::VAR_UPDATE_BESTLANES},
	{"VAR_VEHICLE", TraCIConstants::VAR_VEHICLE},
	{"VAR_VEHICLECLASS", TraCIConstants::VAR_VEHICLECLASS},
	{"VAR_VIA", TraCIConstants::VAR_VIA},
	{"VAR_VIEW_BOUNDARY", TraCIConstants::VAR_VIEW_BOUNDARY},
	{"VAR_VIEW_OFFSET", TraCIConstants::VAR_VIEW_OFFSET},
	{"VAR_VIEW_SCHEMA", TraCIConstants::VAR_VIEW_SCHEMA},
	{"VAR_VIEW_ZOOM", TraCIConstants::VAR_VIEW_ZOOM},
	{"VAR_WAITING_TIME", TraCIConstants::VAR_WAITING_TIME},
	{"VAR_WAITING_TIME_ACCUMULATED", TraCIConstants::VAR_WAITING_TIME_ACCUMULATED},
	{"VAR_WIDTH", TraCIConstants::VAR_WIDTH},
};
//[[[end]]]



} // end namespace TraCISubscriptionManagement
} //