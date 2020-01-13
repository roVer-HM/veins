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

/* VaderePersonItfc */

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
    std::list<std::string> ret = genericGetStringList(CMD_GET_PERSON_VARIABLE, nodeId, VAR_TARGET_LIST, RESPONSE_GET_PERSON_VARIABLE);
    return std::vector<std::string> (ret.begin(), ret.end());
}

std::vector<std::string> VaderePersonItfc::getAllTargetIDs()
{
    std::list<std::string> ret = genericGetStringList(CMD_GET_PERSON_VARIABLE, "-1", VAR_TARGET_LIST, RESPONSE_GET_PERSON_VARIABLE);
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

/* VadereSimulationItfc */

std::string VadereSimulationItfc::getHash(std::string scenario){
    uint8_t variableId = VAR_CACHE_HASH;
    uint8_t variableType = TYPE_STRING;
    TraCIBuffer cmdBuf;
    cmdBuf << variableId << nodeId << variableType << scenario;
    TraCIConnection::Result result;


    TraCIBuffer buf = connection->query(CMD_GET_SIM_VARIABLE, cmdBuf, nullptr);

    uint8_t cmdLength;
    buf >> cmdLength;
    if (cmdLength == 0) {
        uint32_t cmdLengthX;
        buf >> cmdLengthX;
    }
    uint8_t commandId_r;
    buf >> commandId_r;
    ASSERT(commandId_r == RESPONSE_GET_SIM_VARIABLE);
    uint8_t varId;
    buf >> varId;
    ASSERT(varId == variableId);
    std::string objectId_r;
    buf >> objectId_r;
    ASSERT(objectId_r == nodeId);
    uint8_t resType_r;
    buf >> resType_r;
    ASSERT(resType_r == TYPE_STRING);
    std::string res;
    buf >> res;

    ASSERT(buf.eof());

    return res;

}

} /* namespace veins */

