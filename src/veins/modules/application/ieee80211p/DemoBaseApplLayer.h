//
// Copyright (C) 2016 David Eckhoff <eckhoff@cs.fau.de>
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

#pragma once

#include <map>

#include "veins/base/modules/BaseApplLayer.h"
#include "veins/modules/utility/Consts80211p.h"
#include "veins/modules/messages/BaseFrame1609_4_m.h"
#include "veins/modules/messages/DemoServiceAdvertisement_m.h"
#include "veins/modules/messages/DemoSafetyMessage_m.h"
#include "veins/base/connectionManager/ChannelAccess.h"
#include "veins/modules/mac/ieee80211p/DemoBaseApplLayerToMac1609_4Interface.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"
#include "veins/modules/application/ieee80211p/DemoBaseApplLayerGeneric.h"

namespace veins {

using veins::AnnotationManager;
using veins::AnnotationManagerAccess;
using veins::TraCICommandInterface;
using veins::TraCIMobility;
using veins::TraCIMobilityAccess;

/**
 * @brief
 * Demo application layer base class.
 *
 * @author David Eckhoff
 *
 * @ingroup applLayer
 *
 * @see DemoBaseApplLayer
 * @see Mac1609_4
 * @see PhyLayer80211p
 * @see Decider80211p
 */
class VEINS_API DemoBaseApplLayer : public DemoBaseApplLayerGeneric {

public:
    ~DemoBaseApplLayer() override;
    void initialize(int stage) override;

    void receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj, cObject* details) override;

protected:

    /** @brief this function is called every time the vehicle parks or starts moving again */
    virtual void handleParkingUpdate(cObject* obj);

    virtual void initializeFromMobilityModule() override;
    virtual void subscribeToSignals() override;
    virtual void unsubscribeFromSignals() override;

    /* pointers will be set when used with TraCIMobility */
    TraCIMobility* mobility;
    TraCICommandInterface* traci;
    TraCICommandInterface::Vehicle* traciVehicle;

    /* support for parking currently only works with TraCI */
    bool isParked;

};

} // namespace veins
