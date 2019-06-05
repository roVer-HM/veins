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

package org.car2x.veins.subprojects.veins_inet.example;

import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.physicallayer.unitdisk.UnitDiskRadioMedium;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.icn.ICNPublisherHost;
import inet.node.icn.ICNAccessPoint;
import ned.DatarateChannel;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import org.car2x.veins.subprojects.veins_inet.VeinsInetCar;
import org.car2x.veins.subprojects.veins_inet.VeinsInetPerson;
import org.car2x.veins.subprojects.veins_inet.VeinsInetManager;


network ScenarioPedestrians
{
    //    parameters:
    //        @display("bgb=1684,885;bgi=veins/background/hochschule_muenchenBGI");
    types:
        channel C extends DatarateChannel
        {
            datarate = 100Mbps;
            delay = 0.1us;
        }
    submodules:
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=580,275");
        }
        manager: VeinsInetManager {
            @display("p=244.976,64.256");
        }
        carNode[0]: VeinsInetCar {
            @display("p=192.65999,839.93");
        }
        personNode[0]: VeinsInetPerson {
            @display("p=96.329994,839.93");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=82.327995,64.256");
        }
        publisher: ICNPublisherHost {
            parameters:
                numWlanInterfaces = 0;
                // exactly the same position as in classic config
                @display("p=684.45,699.66");
        }
        accessPoint2: ICNAccessPoint {
            parameters:
                // exactly the same position as in classic config
                @display("p=290.68,346.44998");
        }
    connections:
        publisher.pppg++ <--> C <--> accessPoint2.pppg++;
}
