/*
 * SumoSimulation.cpp
 *
 *  Created on: Sep 19, 2019
 *      Author: stsc
 */

#include <veins/modules/mobility/traci/subscriptionManagement/SumoSimulation.h>

using namespace veins::TraCIConstants;
namespace veins {
namespace TraCISubscriptionManagement {

SumoSimulation::SumoSimulation(std::shared_ptr<TraCIConnection> connection,
        std::shared_ptr<TraCICommandInterface> commandInterface)
   : RemoteSimulationObject(connection, commandInterface, "SumoSimulation", TraCICmdTypes{CMD_SUBSCRIBE_SIM_VARIABLE,
                                                                        RESPONSE_SUBSCRIBE_SIM_VARIABLE,
                                                                        CMD_SUBSCRIBE_SIM_CONTEXT,
                                                                        RESPONSE_SUBSCRIBE_SIM_CONTEXT,
                                                                        CMD_GET_SIM_VARIABLE,
                                                                        RESPONSE_GET_SIM_VARIABLE,
                                                                        CMD_SET_SIM_VARIABLE}){
    initVariableMap();
}

SumoSimulation::~SumoSimulation() {
    // TODO Auto-generated destructor stub
}

simtime_t SumoSimulation::serverTimestep{};
std::vector<std::string> SumoSimulation::startedTeleporting{};
std::vector<std::string> SumoSimulation::endedTeleporting{};
std::vector<std::string> SumoSimulation::startedParking{};
std::vector<std::string> SumoSimulation::endedParking{};
std::vector<std::string> SumoSimulation::departedVehicles{};
std::vector<std::string> SumoSimulation::arrivedVehicles{};


void SumoSimulation::initVariableMap(){
    addGetter(getCommandInterface()->getTimeStepCmd(), getCommandInterface()->getTimeType(), access_t::off, rsoId_t::globalVar, (vF_t)&SumoSimulation::getServerTimestep, (vF_t)&SumoSimulation::setServerTimestep);
    addGetter(VAR_TELEPORT_STARTING_VEHICLES_IDS, TYPE_STRINGLIST, access_t::off, rsoId_t::globalVar, (vF_t)&SumoSimulation::getStartedTeleporting, (vF_t)&SumoSimulation::setStartedTeleporting);
    addGetter(VAR_TELEPORT_ENDING_VEHICLES_IDS, TYPE_STRINGLIST, access_t::off, rsoId_t::globalVar, (vF_t)&SumoSimulation::getEndedTeleporting, (vF_t)&SumoSimulation::setEndedTeleporting);
    addGetter(VAR_PARKING_STARTING_VEHICLES_IDS, TYPE_STRINGLIST, access_t::off, rsoId_t::globalVar, (vF_t)&SumoSimulation::getStartedParking, (vF_t)&SumoSimulation::setStartedParking);
    addGetter(VAR_PARKING_ENDING_VEHICLES_IDS, TYPE_STRINGLIST, access_t::off, rsoId_t::globalVar, (vF_t)&SumoSimulation::getEndedParking, (vF_t)&SumoSimulation::setEndedParking);
    addGetter(VAR_DEPARTED_VEHICLES_IDS, TYPE_STRINGLIST, access_t::off, rsoId_t::globalVar, (vF_t)&SumoSimulation::getDepartedVehicles, (vF_t)&SumoSimulation::setDepartedVehicles);
    addGetter(VAR_ARRIVED_VEHICLES_IDS, TYPE_STRINGLIST, access_t::off, rsoId_t::globalVar, (vF_t)&SumoSimulation::getArrivedVehicles, (vF_t)&SumoSimulation::setArrivedVehicles);
}

const std::vector<std::string>& SumoSimulation::getStartedTeleporting() {
    return startedTeleporting;
}

const std::vector<std::string>& SumoSimulation::getEndedTeleporting() {
    return endedTeleporting;
}

const std::vector<std::string>& SumoSimulation::getDepartedVehicles() {
    return departedVehicles;
}

const std::vector<std::string>& SumoSimulation::getArrivedVehicles() {
    return arrivedVehicles;
}

const std::vector<std::string>& SumoSimulation::getStartedParking() {
    return startedParking;
}

const std::vector<std::string>& SumoSimulation::getEndedParking() {
    return endedParking;
}

const simtime_t& SumoSimulation::getServerTimestep() {
    return serverTimestep;
}

void SumoSimulation::clearRSO(){
//    activeVehicleCount = 0;
//    parkingVehicleCount = 0;
//    drivingVehicleCount = 0;
}

void SumoSimulation::setServerTimestep(TraCIBuffer& buffer) {
    buffer >> serverTimestep;
}

void SumoSimulation::setStartedParking(TraCIBuffer& buffer) {
    startedParking = genericParseStringList(buffer);
    parkingVehicleCount += startedParking.size();
    drivingVehicleCount -= startedParking.size();
}

void SumoSimulation::setEndedParking(TraCIBuffer& buffer) {
    endedParking = genericParseStringList(buffer);
    parkingVehicleCount -= endedParking.size();
    drivingVehicleCount += endedParking.size();
}


void SumoSimulation::setStartedTeleporting(TraCIBuffer& buffer) {
    startedTeleporting = genericParseStringList(buffer);
    activeVehicleCount -= startedTeleporting.size();
    drivingVehicleCount -= startedTeleporting.size();
}

void SumoSimulation::setEndedTeleporting(TraCIBuffer& buffer) {
    endedTeleporting = genericParseStringList(buffer);
    activeVehicleCount += endedTeleporting.size();
    drivingVehicleCount += endedTeleporting.size();
}


void SumoSimulation::setDepartedVehicles(TraCIBuffer& buffer) {
    departedVehicles = genericParseStringList(buffer);
    activeVehicleCount += departedVehicles.size();
    drivingVehicleCount += departedVehicles.size();
}

void SumoSimulation::setArrivedVehicles(TraCIBuffer& buffer) {
    arrivedVehicles = genericParseStringList(buffer);
    activeVehicleCount -= arrivedVehicles.size();
    drivingVehicleCount -= arrivedVehicles.size();
}



} /* namespace TraCISubscriptionManagement */
} /* namespace veins */


