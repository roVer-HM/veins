//
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

package org.car2x.veins.subprojects.veins_inet;
import inet.mobility.base.MobilityBase;

simple VeinsInetMobilityBase extends MobilityBase
{
    @display("i=block/cogwheel");
    @signal[mobilityStateChanged](type=inet::MobilityBase);
    @signal[veins_inet_mobility_base_mobility_pre_init];
    @statistic[externalId](source=veins_inet_mobility_base_mobility_pre_init; record=last);
    bool initFromDisplayString = default(true); // do not change this to false
}
    
