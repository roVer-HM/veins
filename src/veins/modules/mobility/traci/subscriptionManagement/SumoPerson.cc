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

#include <veins/modules/mobility/traci/subscriptionManagement/SumoPerson.h>
#include "veins/modules/mobility/traci/TraCIConstants.h"

using namespace veins::TraCIConstants;

namespace veins {
namespace TraCISubscriptionManagement {
SumoPerson::SumoPerson() : RemoteSimulationObject() {}
SumoPerson::SumoPerson(std::shared_ptr<TraCIConnection> connection, std::shared_ptr<TraCICommandInterface> commandInterface)
    : RemoteSimulationObject(connection, commandInterface, "SumoPerson", TraCICmdTypes{CMD_SUBSCRIBE_PERSON_VARIABLE,
                                                                            RESPONSE_SUBSCRIBE_PERSON_VARIABLE,
                                                                            0,
                                                                            0,
                                                                            CMD_GET_PERSON_VARIABLE,
                                                                            RESPONSE_GET_PERSON_VARIABLE,
                                                                            CMD_SET_PERSON_VARIABLE}) {
    initVariableMap();
}

SumoPerson::~SumoPerson() {
    // TODO Auto-generated destructor stub
}

int SumoPerson::idCount = 0;
std::set<std::string> SumoPerson::idList{};


void SumoPerson::initVariableMap(){
    addGetter(ID_LIST, TYPE_STRINGLIST, access_t::off, rsoId_t::globalVar, (vF_t)&SumoPerson::getIdList, (vF_t)&SumoPerson::setIdList);
    addGetter(ID_COUNT, TYPE_INTEGER, access_t::off, rsoId_t::globalVar, (vF_t)&SumoPerson::getIdCount, (vF_t)&SumoPerson::setIdCount);
    addGetter(VAR_ANGLE, TYPE_DOUBLE, access_t::off, (vF_t)&SumoPerson::getAngel, (vF_t)&SumoPerson::setAngel);
    addGetter(VAR_ROAD_ID, TYPE_STRING, access_t::off, (vF_t)&SumoPerson::getRoadId, (vF_t)&SumoPerson::setRoadId);
    addGetter(VAR_POSITION, POSITION_2D, access_t::off, (vF_t)&SumoPerson::getPosition, (vF_t)&SumoPerson::setPosition);
    addGetter(VAR_SPEED, TYPE_DOUBLE, access_t::off, (vF_t)&SumoPerson::getSpeed, (vF_t)&SumoPerson::setSpeed);
    addGetter(VAR_TYPE, TYPE_STRING, access_t::off, (vF_t)&SumoPerson::getTypeId, (vF_t)&SumoPerson::setTypeId);
}


std::set<std::string> SumoPerson::getIdList(){
    ensureActive(ID_LIST);
    return idList;
}

int SumoPerson::getIdCount(){
    ensureActive(ID_COUNT);
    return idCount;
}

void SumoPerson::setIdList(TraCIBuffer& buffer){
   idList = stringListAsSet(buffer);
}

void SumoPerson::setIdCount(TraCIBuffer& buffer){
    buffer >> idCount;
}

const std::string& SumoPerson::getId() const{
    return RemoteSimulationObject::getId();
}

double SumoPerson::getAngel() const {
    ensureActive(VAR_ANGLE);
    return angel;
}

const std::string& SumoPerson::getRoadId() const {
    ensureActive(VAR_ROAD_ID);
    return roadId;
}


const Coord& SumoPerson::getPosition() const {
    ensureActive(VAR_POSITION);
    return position;
}


double SumoPerson::getSpeed() const {
    ensureActive(VAR_SPEED);
    return speed;
}

const std::string& SumoPerson::getTypeId() const {
    ensureActive(VAR_TYPE);
    return typeID;
}

Heading SumoPerson::getHeading() const {
    ensureActive(VAR_ANGLE);
    return heading;
}

double SumoPerson::getX() const {
    return x;
}

double SumoPerson::getY() const {
    return y;
}



void SumoPerson::setAngel(TraCIBuffer& buffer) {
    buffer >> angel;
    heading = getConnection()->traci2omnetHeading(angel);
}

void SumoPerson::setRoadId( TraCIBuffer& buffer) {
    buffer >> roadId;
}


void SumoPerson::setPosition(TraCIBuffer& buffer) {
    buffer >> x;
    buffer >> y;
    position = getConnection()->traci2omnet(TraCICoord(x, y));
}

void SumoPerson::setSpeed(TraCIBuffer& buffer) {
    buffer >> speed;
}

void SumoPerson::setTypeId(TraCIBuffer& buffer) {
    buffer >> typeID;
}


} // end namespace TraCISubscriptionManagement
} // namespace veins
