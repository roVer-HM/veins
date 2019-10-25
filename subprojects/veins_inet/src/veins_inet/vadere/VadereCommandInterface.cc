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


#include "VadereCommandInterface.h"
#include "veins/modules/mobility/traci/TraCIConstants.h"
#include "TraCIConstants.h"

using namespace veins::TraCIConstants;

namespace veins {


std::string VaderePersonItfc::getTypeId()
{
    return personCmdItfc->getTypeId();
}
int VaderePersonItfc::getCount()
{
    return personCmdItfc->getCount();
}
std::list<std::string> VaderePersonItfc::getIdList()
{
    return personCmdItfc->getIdList();
}
std::vector<std::string> VaderePersonItfc::getTargetList()
{
    std::list<std::string> ret = traci->genericGetStringList(CMD_GET_PERSON_VARIABLE, nodeId, VAR_TARGET_LIST, RESPONSE_GET_PERSON_VARIABLE);
    return std::vector<std::string> (ret.begin(), ret.end());
}

std::vector<std::string> VaderePersonItfc::getAllTargetIDs()
{
    std::list<std::string> ret = traci->genericGetStringList(CMD_GET_PERSON_VARIABLE, "-1", VAR_TARGET_LIST, RESPONSE_GET_PERSON_VARIABLE);
    return std::vector<std::string> (ret.begin(), ret.end());
}

void VaderePersonItfc::setTargetList(std::vector<std::string> targetList)
{
    uint8_t variableId = VAR_TARGET_LIST;
    uint8_t variableType = TYPE_STRINGLIST;
    int32_t numElem = targetList.size();
    TraCIBuffer buf;
    buf << variableId << nodeId << variableType << numElem;
    for (auto &item : targetList){
        buf << static_cast<std::string>(item);
    }
    TraCIBuffer obuf = connection->query(CMD_SET_PERSON_VARIABLE, buf);
    ASSERT(obuf.eof());

}
} /* namespace veins */

