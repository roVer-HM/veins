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

#include "VaderePerson.h"
#include "veins/modules/mobility/traci/TraCIConstants.h"
#include "TraCIConstants.h"
#include "veins/modules/mobility/traci/subscriptionManagement/SubscriptionManager.h"

using namespace veins::TraCIConstants;

namespace veins {
namespace TraCISubscriptionManagement {

Register_Class(VaderePersonSubFactory)

std::shared_ptr<ISubscriptionManager> VaderePersonSubFactory::createSubscriptionManager(std::vector<std::uint8_t> varCodes,
                                                                            std::shared_ptr<TraCIConnection>& c,
                                                                            std::shared_ptr<TraCICommandInterface>& cIfc) const {
    return SubscriptionManager<VaderePerson>::build(varCodes, c, cIfc);
}




VaderePerson::VaderePerson() : RemoteSimulationObject() { }
VaderePerson::VaderePerson(std::shared_ptr<TraCIConnection> connection, std::shared_ptr<TraCICommandInterface> commandInterface)
    : RemoteSimulationObject(connection, commandInterface, "VaderePerson", TraCICmdTypes{CMD_SUBSCRIBE_PERSON_VARIABLE,
                                                                                RESPONSE_SUBSCRIBE_PERSON_VARIABLE,
                                                                                0,
                                                                                0,
                                                                                CMD_GET_PERSON_VARIABLE,
                                                                                RESPONSE_GET_PERSON_VARIABLE,
                                                                                CMD_SET_PERSON_VARIABLE}) {
    initVariableMap();
}

VaderePerson::~VaderePerson() {
    // TODO Auto-generated destructor stub
}

int VaderePerson::idCount = 0;
std::set<std::string> VaderePerson::idList{};

void VaderePerson::initVariableMap() {
    addGetter(ID_LIST, TYPE_STRINGLIST, access_t::off, rsoId_t::globalVar, (vF_t)&VaderePerson::getIdList, (vF_t)&VaderePerson::setIdList);
    addGetter(ID_COUNT, TYPE_INTEGER, access_t::off, rsoId_t::globalVar, (vF_t)&VaderePerson::getIdCount, (vF_t)&VaderePerson::setIdCount);
    addGetter(VAR_ANGLE, TYPE_DOUBLE, access_t::off, (vF_t)&VaderePerson::getAngel, (vF_t)&VaderePerson::setAngel);
    addGetter(VAR_POSITION, POSITION_2D, access_t::off, (vF_t)&VaderePerson::getPosition, (vF_t)&VaderePerson::setPosition);
    addGetter(VAR_SPEED, TYPE_DOUBLE, access_t::off, (vF_t)&VaderePerson::getSpeed, (vF_t)&VaderePerson::setSpeed);
    addGetter(VAR_TYPE, TYPE_STRING, access_t::off, (vF_t)&VaderePerson::getTypeId, (vF_t)&VaderePerson::setTypeId);
    addGetter(VAR_TARGET_LIST, TYPE_STRINGLIST, access_t::off, (vF_t)&VaderePerson::getTargetList, (vF_t)&VaderePerson::setTargetList);
}

std::set<std::string> VaderePerson::getIdList() {
    ensureActive(ID_LIST);
    return idList;
}

int VaderePerson::getIdCount() {
    ensureActive(ID_COUNT);
    return idCount;
}

const std::string& VaderePerson::getId() const {
    return RemoteSimulationObject::getId();
}

double VaderePerson::getAngel() const {
    ensureActive(VAR_ANGLE);
    return angel;
}

const Coord& VaderePerson::getPosition() const {
    ensureActive(VAR_POSITION);
    return position;
}

double VaderePerson::getSpeed() const {
    ensureActive(VAR_SPEED);
    return speed;
}

const std::string& VaderePerson::getTypeId() const {
    ensureActive(VAR_TYPE);
    return typeID;
}

double VaderePerson::getX() const {
    ensureActive(VAR_POSITION);
    return x;
}

double VaderePerson::getY() const {
    ensureActive(VAR_POSITION);
    return y;
}

std::vector<std::string> VaderePerson::getTargetList() const {
    ensureActive(VAR_TARGET_LIST);
    return targetList;
}



void VaderePerson::setIdList(TraCIBuffer& buffer) {
    idList = stringListAsSet(buffer);
}

void VaderePerson::setIdCount(TraCIBuffer& buffer) {
    buffer >> idCount;
}

void VaderePerson::setAngel(TraCIBuffer& buffer) {
    buffer >> angel;
}

void VaderePerson::setTypeId(TraCIBuffer& buffer) {
    buffer >> typeID;
}

void VaderePerson::setSpeed(TraCIBuffer& buffer) {
    buffer >> speed;
}

void VaderePerson::setPosition(TraCIBuffer& buffer) {
    buffer >> x;
    buffer >> y;
    position = getConnection()->traci2omnet(TraCICoord(x,y));
}

void VaderePerson::setPositionXXX(TraCIBuffer& buffer) {
    buffer >> x;
    buffer >> y;
    position = getConnection()->traci2omnet(TraCICoord(x,y));
}

void VaderePerson::setTargetList(TraCIBuffer& buffer) {
    targetList = genericParseStringList(buffer);
}

} /* namesapce TraCISubscriptionManagement */
} /* namespace veins */
