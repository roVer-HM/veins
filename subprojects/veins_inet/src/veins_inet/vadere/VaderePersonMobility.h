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

#pragma once

#include "inet/mobility/base/MobilityBase.h"
#include "veins_inet/VeinsInetMobilityBase.h"
#include "veins_inet/veins_inet.h"
#include "VadereCommandInterface.h"

namespace veins {

/**
 * Use positions provided by vadere (mobility provider). Use same procedure as LiniarMobiliy to interpolate between
 * two updates to smooth movement. Between two updates the velocity and orientation is constant
 * and calculated only once.
 * To allow interpolation the mobility provider must return the next position at t_1 = t_0 + dt.
 * With each query to getPostion the current position (lastPostion) is updated based on a linear
 * movement towards P_1.
 */
class VaderePersonMobility : public VeinsInetMobilityBase{
public:
    VaderePersonMobility();
    virtual ~VaderePersonMobility();

    /** @brief called by class VeinsInetManager */
    virtual void preInitialize(std::shared_ptr<IMobileAgent> mobileAgent) override;

    /** @brief called by class VeinsInetManager */
    virtual void nextPosition(std::shared_ptr<IMobileAgent> mobileAgent) override;

    virtual VaderePersonItfc*  getPersonCommandInterface() const;

    virtual const inet::Coord& getCurrentPosition() override; //MobilityBase
    virtual const inet::Coord& getCurrentVelocity() override; //MobilityBase
    virtual const inet::Coord& getCurrentAcceleration() override; //MobilityBase

    virtual const inet::Quaternion& getCurrentAngularPosition() override; //MobilityBase
    virtual const inet::Quaternion& getCurrentAngularVelocity() override; //MobilityBase
    virtual const inet::Quaternion& getCurrentAngularAcceleration() override; //MobilityBase

    void setInitialPosition() override;


    virtual void move();

    virtual void moveAndUpdate();
protected:
    mutable VaderePersonItfc* personCommandInterface = nullptr; /**< cached value */

    simtime_t lastUpdate;

    simtime_t nextPosTime;
    inet::Coord nextPos;


};

} /* namespace veins */

