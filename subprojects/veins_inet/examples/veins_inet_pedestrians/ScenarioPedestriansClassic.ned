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
import inet.physicallayer.ieee80211.packetlevel.Ieee80211DimensionalRadioMedium;
import inet.physicallayer.unitdisk.UnitDiskRadioMedium;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.icn.ICNPublisherHost;
import inet.node.icn.ICNAccessPoint;
import inet.node.inet.StandardHost;
import inet.node.wireless.AccessPoint;
import ned.DatarateChannel;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import org.car2x.veins.subprojects.veins_inet.VeinsInetCar;
import org.car2x.veins.subprojects.veins_inet.VeinsInetPerson;
import org.car2x.veins.subprojects.veins_inet.VeinsInetPersonClassic;
import org.car2x.veins.subprojects.veins_inet.VeinsInetManager;
import inet.node.inet.Router;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;

network ScenarioPedestriansClassic
{
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
        personNode[0]: VeinsInetPersonClassic {
            @display("p=757.016,901.592");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=82.327995,64.256");
        }
        publisher: StandardHost {
            parameters:
                numWlanInterfaces = 0;
                // exactly the same position as in icn config
                @display("p=684.45,699.66");
        }
        //        gateway: Router {
        //            @display("p=483,515");
        //        }
        //        dhcpServer: StandardHost {
        //            @display("p=410,276");
        //            numApps = 1;
        //            app[0].typename = "DhcpServer";
        //            app[0].numReservedAddresses = 3;
        //            app[0].maxNumClients = 1000;
        //            app[0].gateway = "192.168.0.1";
        //            app[0].leaseTime = 500s;
        //        }
        accessPoint2: AccessPoint {
            parameters:
                // exactly the same position as in icn config
                @display("p=290.68,346.44998");
        }
        visualizer: IntegratedCanvasVisualizer;
    connections:
        publisher.ethg++ <--> C <--> accessPoint2.ethg++;
}
