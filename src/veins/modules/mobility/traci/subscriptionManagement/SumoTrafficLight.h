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

#pragma once
#include <veins/modules/mobility/traci/subscriptionManagement/RemoteSimulationObject.h>

namespace veins {
namespace TraCISubscriptionManagement {

class SumoTrafficLight : public RemoteSimulationObject  {
public:
    SumoTrafficLight(std::shared_ptr<TraCIConnection> connection, std::shared_ptr<TraCICommandInterface> commandInterface);
    virtual ~SumoTrafficLight();
    template<typename T>
    friend class RSOFactory;

    std::set<std::string> getIdList();
    int getIdCount();
    uint8_t getCurrentPhase() const;
    const std::string& getCurrentProgram() const;
    const simtime_t& getNextSwitch() const;
    const std::string& getRedYellowGreenState() const;

protected:
    void initVariableMap() override;

    void setIdList(TraCIBuffer& buffer);
    void setIdCount(TraCIBuffer& buffer);

    void setCurrentPhase(TraCIBuffer& buffer);
    void setCurrentProgram(TraCIBuffer& buffer);
    void setNextSwitch(TraCIBuffer& buffer);
    void setRedYellowGreenState(TraCIBuffer& buffer);

private:

    static std::set<std::string> idList;
    static int idCount;

    std::string id;
    uint8_t currentPhase;
    std::string currentProgram;
    simtime_t nextSwitch;
    std::string redYellowGreenState;
};

} /* namespace TraCISubscriptionManagement */
} /* namespace veins */

