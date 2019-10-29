//
// Copyright (C) 2011 David Eckhoff <eckhoff@cs.fau.de>
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

#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"

using namespace veins;

void DemoBaseApplLayer::initialize(int stage)
{
    DemoBaseApplLayerGeneric::initialize(stage);

}

void DemoBaseApplLayer::initializeFromMobilityModule(){
    // initialize pointers to other modules
    if (FindModule<TraCIMobility*>::findSubModule(getParentModule())) {
        mobility = TraCIMobilityAccess().get(getParentModule());
        traci = mobility->getCommandInterface();
        traciVehicle = mobility->getVehicleCommandInterface();
    }
    else {
        traci = nullptr;
        mobility = nullptr;
        traciVehicle = nullptr;
    }
}

void DemoBaseApplLayer::subscribeToSignals(){
    findHost()->subscribe(BaseMobility::mobilityStateChangedSignal, this);
    findHost()->subscribe(TraCIMobility::parkingStateChangedSignal, this);
}

void DemoBaseApplLayer::unsubscribeFromSignals() {
    findHost()->unsubscribe(BaseMobility::mobilityStateChangedSignal, this);
}



void DemoBaseApplLayer::receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj, cObject* details)
{
    Enter_Method_Silent();
    if (signalID == BaseMobility::mobilityStateChangedSignal) {
        handlePositionUpdate(obj);
    }
    else if (signalID == TraCIMobility::parkingStateChangedSignal) {
        handleParkingUpdate(obj);
    }
}


void DemoBaseApplLayer::handleParkingUpdate(cObject* obj)
{
    isParked = mobility->getParkingState();
}



DemoBaseApplLayer::~DemoBaseApplLayer()
{
    cancelAndDelete(sendBeaconEvt);
    cancelAndDelete(sendWSAEvt);
    unsubscribeFromSignals();
}

