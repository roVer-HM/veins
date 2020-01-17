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

#include "veins/veins.h"
#include "veins/base/utils/MappingParser.h"
#include "veins/modules/mobility/traci/TraCIGenericScenarioManager.h"

namespace veins {

/*
 * A NodeMappingDistribution allows more precise assignment of
 * mobility entities (vehicles, pedestrians) of the same Type
 * to different node vectors or node types.
 *
 * If added to a simulation. It allows assignment of mobile nodes
 * to different module names, with different node types based on
 * a simple uniform distribution.
 *
 * example: Node type received from mobility provider 'fooBar'
 *
 * // default assignment of TraCI manager
 * *.manager.moduleType = "fooBar=org.car2x.veins.subprojects.veins_inet.Car"
 * *.manager.moduleName = "fooBar=ueD2D"
 * *.manager.moduleDisplayString = "DEFAULT_PEDTYPE='i=veins/node/pedestrian;is=vs'"
 *
 * // definition of NodeMappingDistribution using a vector node.
 * *.numMappings = 1
 * *.mappingDistribution[0].mapping_root="fooBar"
 * *.mappingDistribution[0].mapping_Name="x1=ueD2D x2=nodeB"
 * *.mappingDistribution[0].mapping_distribution="x1=0.35 x2=0.65"
 *
 * Without any NodeMappingDistribution set all mobility entities with a
 * type 'fooBar' will be collected in the vector node 'ueD2D' and
 * instantiated as org.car2x.veins.subprojects.veins_inet.Car.
 *
 * With the example above only 35% of nodes with type 'fooBar' will
 * be assigned to the node vector 'ueD2D' and the rest will be
 * assign to 'nodeB'. This allows different application settings
 * for the same type of mobile entity. (e.g. sender, receiver).
 *
 * This definition also simplifies the definition of simulation
 * campaigns using OMNeT++ standard variable loops.
 *
 * example:
 * *.mappingDistribution[0].mapping_distribution="x1=${Dist1=0.20, 0.40, 0.70} x2=${Dist2=0.80, 0.60, 0.30 ! Dist1}"
 *
 * This would create 3 experiments with a assignment ratio for (.2/.4) (.4/.6) and (.7/.3)
 * to nodes ueD2d and nodeB.
 */
class NodeMappingDistribution : public cSimpleModule{
public:
    NodeMappingDistribution();
    virtual ~NodeMappingDistribution();

    bool managesKey(const std::string key);

    MappingParser::TypeMappingTripel applyMapping(std::string key, MappingParser::TypeMappingTripel mappingTripel);

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    std::string mappingKey; // mapping key which will be adapted
    MappingParser::TypeMapping mModuleType;
    MappingParser::TypeMapping mModuleName;
    MappingParser::TypeMapping mModuleDispalyString;

    struct DistInterval{
        double left;
        double right;
    };
    std::map<std::string, DistInterval> distributions;

};
} // namespace
