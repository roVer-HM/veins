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

#include "veins/veins.h"

/**
 * @brief: util classes an function to setup communication with vadere.
 */
namespace vadere {

    const size_t VADERE_VERSION_PREFIX = 12; // offset in version string to get to 'traci.major.minor' version string.

    /**
     * @brief: version string of TraCI server in Vadere.
     * @parm traci: traci version to which the Vadere server adheres.
     * @param major/minor: Vadere server interval version to inform omnet of additional features specific to Vadere
     */
    struct vadereVersion{
        int traci;
        int major;
        int minor;
        friend bool operator==(const vadereVersion& lhs, const vadereVersion& rhs)
        {
            return (lhs.traci == rhs.traci) && (lhs.major == rhs.major) && (lhs.minor == rhs.minor);
        }
        friend bool operator>=(const vadereVersion& lhs, const vadereVersion& rhs)
        {
            return (lhs.traci >= rhs.traci) && (lhs.major >= rhs.major) && (lhs.minor >= rhs.minor);
        }
        vadereVersion(int traci, int major, int minor):traci(traci),major(major),minor(minor){}
        vadereVersion(std::string versionString){

            std::string  vadereTraCiVersion;
            std::size_t firstSpace = versionString.find_first_of(" ");
            if (firstSpace != std::string::npos){
               vadereTraCiVersion = versionString.substr(VADERE_VERSION_PREFIX, firstSpace - VADERE_VERSION_PREFIX); // should now be d.d.d
            } else {
                throw cRuntimeError ("Server responded with wrong version string \"%s\"", versionString.c_str());
            }

            size_t p1 = vadereTraCiVersion.find('.');
            if (p1 == std::string::npos){
                throw cRuntimeError ("Expected string of the form \"x.x.x\" but received: \"%s\" ", versionString.c_str());
            }
            size_t p2 = vadereTraCiVersion.find('.', p1 +1);
            if (p2 == std::string::npos){
                throw cRuntimeError("Expected string of the form \"x.x.x\" but received: \"%s\" ", versionString.c_str());
            }
            //20.0.1 p1=2 p2=4 p3=6
            size_t p3 = vadereTraCiVersion.length();
            try{
            traci = std::stoi(vadereTraCiVersion.substr(0, p1)); // (2-0 = 2) 0-1
            major = std::stoi(vadereTraCiVersion.substr(p1 + 1, p2 - p1 + 1)); // (4-3 = 1) 3
            minor = std::stoi(vadereTraCiVersion.substr(p2 + 1, p3 - p2 + 1)); // (6-5 - 1) 5
            } catch (const std::invalid_argument &err){
                throw new cRuntimeError("Cannot parse vadere version string \"%s\"", versionString.c_str());
            } catch (const std::out_of_range &err ){
                throw new cRuntimeError("Vadere version string out of range \"%s\"", versionString.c_str());
            }
        }
    };

    const vadereVersion cacheVersion {20,0,1}; // version from which onwards cache data can be send with initial SEND_FILE command

    std::pair<std::string, std::string> typedef VadereCache; // (cacheId / cachePath) map for transferal
    std::pair<std::string, std::string> typedef VadereScenario; // (scenarioPath / scenarioContent)
    // read list of cachId->cachePath pairs from configuration xml.
    std::vector<VadereCache> getCachePaths(const cXMLElement* xmlConfig, const std::string basedir);
    // read scenarioPath->scenarioContent from configuration xml.
    VadereScenario getScenarioContent(const cXMLElement* xmlConfig, const std::string basedir);

}

