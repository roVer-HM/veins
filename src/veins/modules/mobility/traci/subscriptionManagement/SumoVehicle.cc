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

#include "veins/modules/mobility/traci/subscriptionManagement/SumoVehicle.h"
#include "veins/modules/mobility/traci/TraCIConstants.h"

using namespace veins::TraCIConstants;

namespace veins {
namespace TraCISubscriptionManagement {

Register_Class(SumoVehicleSubFactory)

std::shared_ptr<ISubscriptionManager> SumoVehicleSubFactory::createSubscriptionManager(std::vector<std::uint8_t> varCodes,
                                                                            std::shared_ptr<TraCIConnection>& c,
                                                                            std::shared_ptr<TraCICommandInterface>& cIfc) const {
    return SubscriptionManager<SumoVehicle>::build(varCodes, c, cIfc);
}

SumoVehicle::SumoVehicle(std::shared_ptr<TraCIConnection> connection,
        std::shared_ptr<TraCICommandInterface> commandInterface)
   : RemoteSimulationObject(connection, commandInterface, "SumoVehicle", TraCICmdTypes{CMD_SUBSCRIBE_VEHICLE_VARIABLE,
                                                                        RESPONSE_SUBSCRIBE_VEHICLE_VARIABLE,
                                                                        CMD_SUBSCRIBE_VEHICLE_CONTEXT,
                                                                        RESPONSE_SUBSCRIBE_VEHICLE_CONTEXT,
                                                                        CMD_GET_VEHICLE_VARIABLE,
                                                                        RESPONSE_GET_VEHICLE_VARIABLE,
                                                                        CMD_SET_VEHICLE_VARIABLE}){
    initVariableMap();
}
SumoVehicle::~SumoVehicle(){

}

int SumoVehicle::idCount = 0;
std::set<std::string> SumoVehicle::idList{};

void SumoVehicle::initVariableMap(){
    addGetter(ID_LIST, TYPE_STRINGLIST, access_t::off, rsoId_t::globalVar, (vF_t)&SumoVehicle::getIdList, (vF_t)&SumoVehicle::setIdList);
    addGetter(ID_COUNT, TYPE_INTEGER, access_t::off, rsoId_t::globalVar, (vF_t)&SumoVehicle::getIdCount, (vF_t)&SumoVehicle::setIdCount);
    addGetter(VAR_ANGLE, TYPE_DOUBLE, access_t::off, (vF_t)&SumoVehicle::getAngel, (vF_t)&SumoVehicle::setAngel);
    addGetter(VAR_ROAD_ID, TYPE_STRING, access_t::off, (vF_t)&SumoVehicle::getRoadId, (vF_t)&SumoVehicle::setRoadId);
    addGetter(VAR_HEIGHT, TYPE_DOUBLE, access_t::off, (vF_t)&SumoVehicle::getHeight, (vF_t)&SumoVehicle::setHeight);
    addGetter(VAR_LENGTH, TYPE_DOUBLE, access_t::off, (vF_t)&SumoVehicle::getLength, (vF_t)&SumoVehicle::setLength);
    addGetter(VAR_POSITION, POSITION_2D, access_t::off, (vF_t)&SumoVehicle::getPosition, (vF_t)&SumoVehicle::setPosition);
    addGetter(VAR_SIGNALS, TYPE_INTEGER, access_t::off, (vF_t)&SumoVehicle::getSignals, (vF_t)&SumoVehicle::setSignals);
    addGetter(VAR_SPEED, TYPE_DOUBLE, access_t::off, (vF_t)&SumoVehicle::getSpeed, (vF_t)&SumoVehicle::setSpeed);
    addGetter(VAR_TYPE, TYPE_STRING, access_t::off, (vF_t)&SumoVehicle::getTypeId, (vF_t)&SumoVehicle::setTypeId);
    addGetter(VAR_WIDTH, TYPE_DOUBLE, access_t::off, (vF_t)&SumoVehicle::getWidth, (vF_t)&SumoVehicle::setWidth);
}


std::set<std::string> SumoVehicle::getIdList(){
    ensureActive(ID_LIST);
    return idList;
}

int SumoVehicle::getIdCount(){
    ensureActive(ID_COUNT);
    return idCount;
}

void SumoVehicle::setIdList(TraCIBuffer& buffer){
   idList = stringListAsSet(buffer);
}

void SumoVehicle::setIdCount(TraCIBuffer& buffer){
    buffer >> idCount;
}

const std::string& SumoVehicle::getId() const{
    return RemoteSimulationObject::getId();
}

double SumoVehicle::getAngel() const {
    ensureActive(VAR_ANGLE);
    return angel;
}

const std::string& SumoVehicle::getRoadId() const {
    ensureActive(VAR_ROAD_ID);
    return roadId;
}

Heading SumoVehicle::getHeading() const {
    ensureActive(VAR_ANGLE);
    return heading;
}

double SumoVehicle::getHeight() const {
    ensureActive(VAR_HEIGHT);
    return height;
}

double SumoVehicle::getLength() const {
    ensureActive(VAR_LENGTH);
    return length;
}

const Coord& SumoVehicle::getPosition() const {
    ensureActive(VAR_POSITION);
    return position;
}

int SumoVehicle::getSignals() const {
    ensureActive(VAR_SIGNALS);
    return signals;
}

double SumoVehicle::getSpeed() const {
    ensureActive(VAR_SPEED);
    return speed;
}

const std::string& SumoVehicle::getTypeId() const {
    ensureActive(VAR_TYPE);
    return typeID;
}

double SumoVehicle::getWidth() const {
    ensureActive(VAR_WIDTH);
    return width;
}

double SumoVehicle::getX() const {
    return x;
}

double SumoVehicle::getY() const {
    return y;
}



void SumoVehicle::setWidth(TraCIBuffer& buffer) {
    buffer >> width;
}

void SumoVehicle::setAngel(TraCIBuffer& buffer) {
    buffer >> angel;
    heading = getConnection()->traci2omnetHeading(angel);
}

void SumoVehicle::setRoadId( TraCIBuffer& buffer) {
    buffer >> roadId;
}

void SumoVehicle::setHeight(TraCIBuffer& data) {
    data >> height;
}

void SumoVehicle::setLength(TraCIBuffer& buffer) {
    buffer >> length;
}

void SumoVehicle::setPosition(TraCIBuffer& buffer) {
    buffer >> x;
    buffer >> y;
    position = getConnection()->traci2omnet(TraCICoord(x, y));
}

void SumoVehicle::setSpeed(TraCIBuffer& buffer) {
    buffer >> speed;

}

void SumoVehicle::setTypeId(TraCIBuffer& buffer) {
    buffer >> typeID;
}

void SumoVehicle::setSignals(TraCIBuffer& buffer) {
    buffer >> signals;
}



} // end namespace TraCISubscriptionManagement
} // namespace veins
