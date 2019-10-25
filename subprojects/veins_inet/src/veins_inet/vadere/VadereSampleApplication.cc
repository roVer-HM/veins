//
// Copyright (C) 2018 Christoph Sommer <sommer@ccs-labs.org>
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

#include "inet/common/ModuleAccess.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/TagBase_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"

#include "veins_inet/VeinsInetSampleMessage_m.h"
#include "VadereSampleMessage_m.h"
#include "VadereSampleApplication.h"

using namespace inet;

Define_Module(VadereSampleApplication);

VadereSampleApplication::VadereSampleApplication()
{
}

bool VadereSampleApplication::startApplication()
{
    // host[0] should stop at t=20s
    if (getParentModule()->getIndex() == 6) {
        auto callback = [this]() {
            getParentModule()->getDisplayString().setTagArg("i", 1, "red");

            std::vector<std::string> targetLists = traciPerson->getAllTargetIDs();
            if (targetLists.size() > 1){

                auto payload = makeShared<VadereSampleMessage>();
                timestampPayload(payload);
                payload->setChunkLength(B(300));
                payload->setBlocked_target(targetLists[0].c_str());
                payload->setAlternative_targetsArraySize(1);
                payload->setAlternative_targets(0,  targetLists[1].c_str());

                auto packet = createPacket("subway_entrance_blocked");
                packet->insertAtBack(payload);
                sendPacket(std::move(packet));
                std::cout << "subway entrance " << targetLists[0] << " blocked";
            }
        };
        timerManager.create(veins::TimerSpecification(callback).oneshotAt(SimTime(28, SIMTIME_S)));
    }

    return true;
}

bool VadereSampleApplication::stopApplication()
{
    return true;
}

VadereSampleApplication::~VadereSampleApplication()
{
}

void VadereSampleApplication::processPacket(std::shared_ptr<inet::Packet> pk)
{
    auto payload = pk->peekAtFront<VadereSampleMessage>();

    EV_INFO << "Received packet: " << payload << endl;

    getParentModule()->getDisplayString().setTagArg("i", 1, "green");

    std::string blocked = payload->getBlocked_target();
    std::string alternative = payload->getAlternative_targets(0);
    if (!blocked.empty() && !alternative.empty()){
        std::vector<std::string> targetLists = traciPerson->getTargetList();
        std::vector<std::string> newTargetList;
        for (auto &target : targetLists){
            if (target.compare(blocked) == 0){
                newTargetList.push_back(alternative);
            } else {
                newTargetList.push_back(target);
            }
        }
        traciPerson->setTargetList(newTargetList);

    }


    if (haveForwarded) return;

    auto packet = createPacket("relay");
    packet->insertAtBack(payload);
    sendPacket(std::move(packet));

    haveForwarded = true;
}
