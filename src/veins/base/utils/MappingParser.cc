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

#include "veins/base/utils/MappingParser.h"
#include "veins/veins.h"

namespace veins {

MappingParser::~MappingParser() {
}

std::vector<std::string> MappingParser::getMapping(std::string el){
    // search for string protection characters default(')
    size_t first = el.find(protectionCharacter);
    size_t second;
    size_t eq;
    std::string type, value;
    std::vector<std::string> mapping;

    if (first == std::string::npos) {
        // there's no string protection, simply split by '='
        cStringTokenizer stk(el.c_str(), mappingsKeyValueDelimiter);
        mapping = stk.asVector();
    }
    else {
        // if there's string protection, we need to find a matching delimiter
        second = el.find(protectionCharacter, first + 1);
        // ensure that a matching delimiter exists, and that it is at the end
        if (second == std::string::npos || second != el.size() - 1) throw cRuntimeError("invalid syntax for mapping \"%s\"", el.c_str());

        // take the value of the mapping as the text within the quotes
        value = el.substr(first + 1, second - first - 1);

        if (first == 0) {
            // if the string starts with a quote, there's only the value
            mapping.push_back(value);
        }
        else {
            // search for the equal sign
            eq = el.find(mappingsKeyValueDelimiter);
            // this must be the character before the quote
            if (eq == std::string::npos || eq != first - 1) {
                throw cRuntimeError("invalid syntax for mapping \"%s\"", el.c_str());
            }
            else {
                type = el.substr(0, eq);
            }
            mapping.push_back(type);
            mapping.push_back(value);
        }
    }
    return mapping;
}

MappingParser::TypeMapping MappingParser::parseMappings(std::string parameter, std::string parameterName, bool allowEmpty)
{
    /**
     * possible syntaxes
     *
     * "a"          : assign module type "a" to all nodes (for backward compatibility)
     * "a=b"        : assign module type "b" to vehicle type "a". the presence of any other vehicle type in the simulation will cause the simulation to stop
     * "a=b c=d"    : assign module type "b" to vehicle type "a" and "d" to "c". the presence of any other vehicle type in the simulation will cause the simulation to stop
     * "a=b c=d *=e": everything which is not of vehicle type "a" or "b", assign module type "e"
     * "a=b c=0"    : for vehicle type "c" no module should be instantiated
     * "a=b c=d *=0": everything which is not of vehicle type a or c should not be instantiated
     *
     * For display strings key-value pairs needs to be protected with single quotes, as they use an = sign as the type mappings. For example
     * *.manager.moduleDisplayString = "'i=block/process'"
     * *.manager.moduleDisplayString = "a='i=block/process' b='i=misc/sun'"
     *
     * moduleDisplayString can also be left empty:
     * *.manager.moduleDisplayString = ""
     */

    unsigned int i;
    TypeMapping map;

    // tokenizer to split into mappings ("a=b c=d", -> ["a=b", "c=d"])
    cStringTokenizer typesTz(parameter.c_str(), mappingsDelimiter);
    // get all mappings
    std::vector<std::string> typeMappings = typesTz.asVector();
    // and check that there exists at least one
    if (typeMappings.size() == 0) {
        if (!allowEmpty)
            throw cRuntimeError("parameter \"%s\" is empty", parameterName.c_str());
        else
            return map;
    }

    // loop through all mappings
    for (i = 0; i < typeMappings.size(); i++) {

        // tokenizer to find the mapping from vehicle type to module type
        std::string typeMapping = typeMappings[i];

        std::vector<std::string> mapping = getMapping(typeMapping);

        if (mapping.size() == 1) {
            // we are where there is no actual assignment
            // "a": this is good
            // "a b=c": this is not
            if (typeMappings.size() != 1)
                // stop simulation with an error
                throw cRuntimeError("parameter \"%s\" includes multiple mappings, but \"%s\" is not mapped to any vehicle type", parameterName.c_str(), mapping[0].c_str());
            else
                // all vehicle types should be instantiated with this module type
                map["*"] = mapping[0];
        }
        else {

            // check that mapping is valid (a=b and not like a=b=c)
            if (mapping.size() != 2) throw cRuntimeError("invalid syntax for mapping \"%s\" for parameter \"%s\"", typeMapping.c_str(), parameterName.c_str());
            // check that the mapping does not already exist
            if (map.find(mapping[0]) != map.end()) throw cRuntimeError("duplicated mapping for vehicle type \"%s\" for parameter \"%s\"", mapping[0].c_str(), parameterName.c_str());

            // finally save the mapping
            map[mapping[0]] = mapping[1];
        }
    }

    return map;
}
} // namespace veins
