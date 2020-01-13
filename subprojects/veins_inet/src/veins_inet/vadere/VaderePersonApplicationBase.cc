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
#include "veins_inet/vadere/VaderePersonApplicationBase.h"


namespace veins {

Define_Module(VaderePersonApplicationBase)

VaderePersonApplicationBase::VaderePersonApplicationBase() {}

VaderePersonApplicationBase::~VaderePersonApplicationBase() {}

void VaderePersonApplicationBase::initializeFromMobilityModule(){

    mobility = veins::VeinsInetMobilityAccess().get<VaderePersonMobility*>(getParentModule());
    traci = mobility->getCommandInterface();
    traciPerson = mobility->getPersonCommandInterface();
}

} /* namespace veins */
