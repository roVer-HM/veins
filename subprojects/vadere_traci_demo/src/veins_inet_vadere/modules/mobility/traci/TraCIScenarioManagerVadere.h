//
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

#include "veins/veins.h"

#include "veins/modules/mobility/traci/TraCIScenarioManager.h"

#include "veins/modules/mobility/traci/TraCICommandInterface.h"
#include "veins/modules/mobility/traci/TraCIConstants.h"

#include <sstream>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <fstream>
#include <string>


namespace veins {

/**
 * @brief
 * Extends the TraCIScenarioManager to connect to Vadere, a microscopic pedestrian dynamics simulation framework
 *
 * Connects to a running Vadere server instance (vadere-server.jar)
 *
 * Based on TraCIScenarioManagerLaunchd implementation with adaption to Vadere.
 * All other functionality is provided by the TraCIScenarioManager.
 *
 * @author Stefan Schuhbaeck
 *
 * @see TraCIMobility
 * @see TraCIScenarioManager
 * @see TraCIScenarioManagerLaunchd
 *
 */

class VEINS_API TraCIScenarioManagerVadere : public TraCIScenarioManager {


public:
    ~TraCIScenarioManagerVadere() override;
    void initialize(int stage) override;
    void finish() override;

protected:
    std::string scenarioFilePath; /**< path to scenario file used to setup Vadere */
    std::string cache_config; /** file containing pairs of cacheIdentifier and paths to cache files. Lines starting with '#' will be ignored */
    std::vector<std::string> cache_suffixes; /** allowed suffixes. Order of this vector defines order of precedence */
    std::map< std::string, std::string > cachePath; /** contains cache identifier and the corresponding byte data of the cache */
    void init_traci() override;

private:
    struct vadereTraCIVersion{
        int traci;      /** Base TraCI version the Vadere API adheres to */
        int major;      /** Major for additional features not included in TraCI*/
        int minor;      /** Minor*/

        friend bool operator==(const vadereTraCIVersion& lhs, const vadereTraCIVersion& rhs)
        {
            return (lhs.traci == rhs.traci) && (lhs.major == rhs.major) && (lhs.minor == rhs.minor);
        }
        friend bool operator>=(const vadereTraCIVersion& lhs, const vadereTraCIVersion& rhs)
        {
            return (lhs.traci >= rhs.traci) && (lhs.major >= rhs.major) && (lhs.minor >= rhs.minor);
        }
    };
    const size_t VADERE_VERSION_PREFIX = 12;
    const vadereTraCIVersion cacheVersion {20,0,1};
    vadereTraCIVersion buildVadereVersion(std::string versionString);
    std::vector<std::string> stringTokens(std::string& data, const char *sep);
    std::string scenarioFileContent; /**< content of scenario file (Json based description) */
    vadereTraCIVersion vadereVersion;
    cMessage* targetTrigger;
    void trigger_set_target();
    void handleSelfMsg(cMessage* msg);
};

class VEINS_API TraCIScenarioManagerVadereAccess {
public:
    TraCIScenarioManagerVadere* get()
    {
        return FindModule<TraCIScenarioManagerVadere*>::findGlobalModule();
    };
};

} // namespace veins
