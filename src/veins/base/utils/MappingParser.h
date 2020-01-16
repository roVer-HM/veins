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

#include "veins/veins.h"

namespace veins {
class MappingParser {
public:
    MappingParser() : mappingsDelimiter(" ")
                    , protectionCharacter("'")
                    , mappingsKeyValueDelimiter("="){};
    virtual ~MappingParser();

    std::vector<std::string> getMapping(std::string el);

    typedef std::map<std::string, std::string> TypeMapping;
    TypeMapping parseMappings(std::string parameter, std::string parameterName, bool allowEmpty);

    struct TypeMappingTripel{
            std::string mType, mName, mDisplayString;
        };

private:
    const char* mappingsDelimiter;
    const char* protectionCharacter;
    const char* mappingsKeyValueDelimiter;
};
} // namespace veins

