//
// Copyright (C) 2006-2017 Nico Dassler <dassler@hm.edu>
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
#include "veins/veins.h"
#include "veins/base/utils/Coord.h"
#include "veins/base/utils/Heading.h"

namespace veins {
namespace TraCISubscriptionManagement {

/**
 * @struct Vehicle
 *
 * Extends the TrafficParticipant definition by a couple of properties
 * of the vehicle like length, height and width.
 *
 * @author Nico Dassler <dassler@hm.edu>
 */

class SumoVehicle : public RemoteSimulationObject, public IMobileAgent {
public:
    SumoVehicle(std::shared_ptr<TraCIConnection> connection, std::shared_ptr<TraCICommandInterface> commandInterface);
    virtual ~SumoVehicle();
    template<typename T>
    friend class RSOFactory;

    std::set<std::string> getIdList();
    int getIdCount();
    const std::string& getId() const override; // RemoteSimulationObject|IMobileAgent
    double getAngel() const;
    const std::string& getRoadId() const;
    Heading getHeading() const;
    double getHeight() const;
    double getLength() const;
    const Coord& getPosition() const override; // IMobileAgent

    int getSignals() const;
    double getSpeed() const;
    const std::string& getTypeId() const override; // IMobileAgent
    double getWidth() const;
    double getX() const override;    // IMobileAgent
    double getY() const override;   // IMobileAgent

protected:
    void initVariableMap() override;

    void setIdList(TraCIBuffer& buffer);
    void setIdCount(TraCIBuffer& buffer);
    void setHeight(TraCIBuffer& buffer);
    void setAngel(TraCIBuffer& buffer);
    void setRoadId(TraCIBuffer& buffer);
    void setLength(TraCIBuffer& buffer);
    void setSignals(TraCIBuffer& buffer);
    void setTypeId(TraCIBuffer& buffer) override; // IMobileAgent
    void setWidth(TraCIBuffer& buffer);
    void setSpeed(TraCIBuffer& buffer);
    void setPosition(TraCIBuffer& buffer) override; // IMobileAgent

private:

    static std::set<std::string> idList;
    static int idCount;

    Coord position;
    double x;
    double y;
    std::string typeID;
    std::string roadId;
    double speed;
    double angel;
    int signals;
    double length;
    double height;
    double width;
    Heading heading;
};

} // end namespace TraCISubscriptionManagement
} // namespace veins
