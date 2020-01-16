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

#include "NodeMappingDistribution.h"

Define_Module(veins::NodeMappingDistribution);
namespace veins {

NodeMappingDistribution::NodeMappingDistribution() {
}

NodeMappingDistribution::~NodeMappingDistribution() {
}

void NodeMappingDistribution::handleMessage(cMessage *msg){
    delete msg;
}

void NodeMappingDistribution::initialize() {

    this->mappingKey = par("mapping_root").stdstringValue();
    std::string mapping_moduleType = par("mapping_moduleType").stdstringValue();
    std::string mapping_moduleName = par("mapping_moduleName").stdstringValue();
    std::string mapping_displayString = par("mapping_displayString").stdstringValue();
    std::string mapping_distribution = par("mapping_distribution").stdstringValue();

    MappingParser parser{};
    mModuleType = parser.parseMappings(mapping_moduleType, "mapping_moduleType", true);
    mModuleName = parser.parseMappings(mapping_moduleName, "mapping_moduleName", true);
    mModuleDispalyString = parser.parseMappings(mapping_displayString, "mapping_displayString", true);
    MappingParser::TypeMapping mdistribution = parser.parseMappings(mapping_distribution, "mapping_distribution", false);

    double start = 0.0;
    double end = 0.0;
    for(auto const& item: mdistribution){
        double val;
        try{
            val = std::stod(item.second);
        }catch (const std::invalid_argument& ia){
            throw cRuntimeError("invalid distribution parameter \"%s\"", mapping_distribution.c_str());
        }
        start = end;
        end += val;
        distributions[item.first] = DistInterval{start, end};
        if (end > 1.0){
            throw cRuntimeError("The sum of discrete variables greater than 1 \"%s\"", mapping_distribution.c_str());
        }
    }
}

bool NodeMappingDistribution::managesKey(const std::string key){
    return mappingKey.compare(key) == 0;
}

MappingParser::TypeMappingTripel NodeMappingDistribution::applyMapping(std::string key, MappingParser::TypeMappingTripel mappingTripel){
    // return standard mapping. This NodeMappingDistribution does not manges given key
    if (! managesKey(key))
        return mappingTripel;

    double rndVal = uniform(0, 1.0);

    std::string subKey;
    for(auto const& dist : distributions){
        std::string k = dist.first;
        DistInterval interval = dist.second;
        if (interval.left <= rndVal && interval.right > rndVal){
            subKey = k;
            break;
        }
    }

    auto valType = mModuleType.find(subKey);
    if(valType != mModuleType.end())
        mappingTripel.mType = valType->second;

    auto valName = mModuleName.find(subKey);
    if(valName != mModuleName.end())
        mappingTripel.mName = valName->second;

    auto valDisplay = mModuleDispalyString.find(subKey);
    if(valDisplay != mModuleDispalyString.end())
        mappingTripel.mDisplayString = valDisplay->second;

    return mappingTripel;
}
} // namespace veins

