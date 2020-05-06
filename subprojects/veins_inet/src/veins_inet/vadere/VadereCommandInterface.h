//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: GPL-2.0-or-later
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
#include <veins/modules/mobility/traci/TraCICommandInterface.h>

namespace veins {


class VaderePersonItfc : public TraCICommandInterface::TraCIObjectIterface {
public:
    VaderePersonItfc(TraCICommandInterface* traci, std::string nodeId)
        : TraCICommandInterface::TraCIObjectIterface(traci)
        , nodeId(nodeId){

        personCmdItfc = new TraCICommandInterface::Person(traci, nodeId);
    }

    std::string getTypeId() ; // delegate to personCmdItfc
    int getCount() ; // delegate to personCmdItfc
    std::list<std::string> getIdList() ; // delegate to personCmdItfc
    std::vector<std::string> getTargetList() ;
    std::vector<std::string> getAllTargetIDs() ;
    void setTargetList(std::vector<std::string> targetList);
    void setInformation(simtime_t start, simtime_t obsolete_at, std::string information);
    friend TraCICommandInterface;

private:
    std::string nodeId;
    mutable TraCICommandInterface::Person* personCmdItfc = nullptr; /**< cached value */
};


class VadereSimulationItfc : public TraCICommandInterface::TraCIObjectIterface {
public:
    VadereSimulationItfc(TraCICommandInterface* traci)
        : TraCICommandInterface::TraCIObjectIterface(traci),
          nodeId("-1"){

    }

    std::string getHash(std::string scenario);
    struct SimCfg {
        std::string oppConfigName, oppExperiment, oppDateTime, oppResultRootDir,
        oppIterationVariables, oppRepetition, oppOutputScalarFile, oppOutputVecFile;
        int seed;
        bool useVadereSeed;
    };
    void sendSimulationConfig(SimCfg simCfg);
private:
    std::string nodeId;

};

} /* namespace veins */
