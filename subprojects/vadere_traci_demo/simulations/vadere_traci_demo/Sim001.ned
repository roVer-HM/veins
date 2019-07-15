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

package veins_inet_vadere.simulations.vadere_traci_demo;

import org.car2x.veins.subprojects.veins_inet_vadere.VadereScenario;
import org.car2x.veins.subprojects.veins_inet_vadere.VadereRSU;

network VadereTraCIDemo extends VadereScenario
{
    
    submodules:
        vadere[1]: VadereRSU {
        	@display("i=veins/sign/yellowdiamond;is=s");
        }
    
}