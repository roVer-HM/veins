//
// Copyright (C) 2006-2017 Christoph Sommer <sommer@ccs-labs.org>
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

package org.car2x.veins.subprojects.veins_inet.example.veins_inet_vadere;

import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import org.car2x.veins.modules.obstacle.ObstacleControl;
import org.car2x.veins.modules.world.annotations.AnnotationManager;
import org.car2x.veins.subprojects.veins_inet.VeinsInetCar;
import org.car2x.veins.subprojects.veins_inet.VeinsInetPerson;
import org.car2x.veins.subprojects.veins_inet.vadere.TraCIScenarioManagerVadere;


network ScenarioPedestriansVadere
{
    //    parameters:
    //        @display("bgb=1684,885;bgi=veins/background/hochschule_muenchenBGI");
    submodules:
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=580,275");
        }
                
        manager: TraCIScenarioManagerVadere {
            @display("p=244.976,64.256");
        }
        personNode[0]: VeinsInetPerson {
            @display("p=96.329994,839.93");
        }
}
