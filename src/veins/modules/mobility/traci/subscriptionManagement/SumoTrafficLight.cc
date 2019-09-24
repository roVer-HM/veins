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

#include <veins/modules/mobility/traci/subscriptionManagement/SumoTrafficLight.h>

using namespace veins::TraCIConstants;
namespace veins {
namespace TraCISubscriptionManagement {

SumoTrafficLight::SumoTrafficLight(std::shared_ptr<TraCIConnection> connection, std::shared_ptr<TraCICommandInterface> commandInterface)
    : RemoteSimulationObject(connection, commandInterface, "SumoTrafficLight", TraCICmdTypes{CMD_SUBSCRIBE_TL_VARIABLE,
                                                                         RESPONSE_SUBSCRIBE_TL_VARIABLE,
                                                                         CMD_SUBSCRIBE_TL_CONTEXT,
                                                                         RESPONSE_SUBSCRIBE_TL_CONTEXT,
                                                                         CMD_GET_TL_VARIABLE,
                                                                         RESPONSE_GET_TL_VARIABLE,
                                                                         CMD_SET_TL_VARIABLE}) {
    initVariableMap();
}

SumoTrafficLight::~SumoTrafficLight() {
    // TODO Auto-generated destructor stub
}

int SumoTrafficLight::idCount = 0;
std::set<std::string> SumoTrafficLight::idList{};

void SumoTrafficLight::initVariableMap(){
    addGetter(ID_LIST, TYPE_STRINGLIST, access_t::off, rsoId_t::globalVar, (vF_t)&SumoTrafficLight::getIdList, (vF_t)&SumoTrafficLight::setIdList);
    addGetter(ID_COUNT, TYPE_INTEGER, access_t::off, rsoId_t::globalVar, (vF_t)&SumoTrafficLight::getIdCount, (vF_t)&SumoTrafficLight::setIdCount);
    addGetter(TL_CURRENT_PHASE, TYPE_INTEGER, access_t::off, (vF_t)&SumoTrafficLight::getCurrentPhase, (vF_t)&SumoTrafficLight::setCurrentPhase);
    addGetter(TL_CURRENT_PROGRAM, TYPE_STRING, access_t::off, (vF_t)&SumoTrafficLight::getCurrentProgram, (vF_t)&SumoTrafficLight::setCurrentProgram);
    addGetter(TL_NEXT_SWITCH, getCommandInterface()->getTimeType(), access_t::off, (vF_t)&SumoTrafficLight::getNextSwitch, (vF_t)&SumoTrafficLight::setNextSwitch);
    addGetter(TL_RED_YELLOW_GREEN_STATE, TYPE_STRING, access_t::off, (vF_t)&SumoTrafficLight::getRedYellowGreenState, (vF_t)&SumoTrafficLight::setRedYellowGreenState);
}

std::set<std::string> SumoTrafficLight::getIdList(){
    ensureActive(ID_LIST);
    return idList;
}

int SumoTrafficLight::getIdCount(){
    ensureActive(ID_COUNT);
    return idCount;
}

void SumoTrafficLight::setIdList(TraCIBuffer& buffer){
   idList = stringListAsSet(buffer);
}

void SumoTrafficLight::setIdCount(TraCIBuffer& buffer){
    buffer >> idCount;
}


void SumoTrafficLight::setCurrentPhase(
        TraCIBuffer& buffer) {
    currentPhase = buffer.readTypeChecked<int32_t>(TraCIConstants::TYPE_INTEGER);;
}

void SumoTrafficLight::setCurrentProgram(
        TraCIBuffer& buffer) {
    currentProgram = buffer.readTypeChecked<std::string>(TraCIConstants::TYPE_STRING);;
}


void SumoTrafficLight::setNextSwitch(
        TraCIBuffer& buffer) {
    nextSwitch = buffer.readTypeChecked<simtime_t>(getCommandInterface()->getTimeType());
}

void SumoTrafficLight::setRedYellowGreenState(
        TraCIBuffer& buffer) {
    redYellowGreenState = buffer.readTypeChecked<std::string>(TraCIConstants::TYPE_STRING);;
}

uint8_t SumoTrafficLight::getCurrentPhase() const {
    return currentPhase;
}

const std::string& SumoTrafficLight::getCurrentProgram() const {
    return currentProgram;
}

const simtime_t& SumoTrafficLight::getNextSwitch() const {
    return nextSwitch;
}

const std::string& SumoTrafficLight::getRedYellowGreenState() const {
    return redYellowGreenState;
}

} /* namespace TraCISubscriptionManagement */
} /* namespace veins */


