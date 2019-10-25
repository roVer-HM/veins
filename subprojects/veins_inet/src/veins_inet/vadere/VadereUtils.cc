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


#include "veins_inet/vadere/VadereUtils.h"

#include <glob.h>
#include <sys/stat.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

namespace vadere {

std::vector<VadereCache> getCachePaths(const cXMLElement* xmlConfig, const std::string basedir){

    std::vector<VadereCache> cachePaths;
    std::string cacheLocation;
    cXMLElementList cache = xmlConfig->getElementsByTagName("cache");
    if (cache.size() == 0){
        return cachePaths;
    } else {
        std::string path = cache[0]->getAttribute("path") == nullptr ? std::string() : std::string(cache[0]->getAttribute("path"));
        std::string hash = cache[0]->getAttribute("hash") == nullptr ? std::string() : std::string(cache[0]->getAttribute("hash"));
        assert(!path.empty());
        assert(!hash.empty());
        cacheLocation = basedir + path + hash + "*";
    }

    glob_t glob_res;

    glob(cacheLocation.c_str(), GLOB_TILDE, nullptr, &glob_res);
    for(unsigned int i=0; i < glob_res.gl_pathc; ++i){
        VadereCache c;
        std::string file = std::string(glob_res.gl_pathv[i]);
        size_t p1 = file.rfind('/');
        size_t p2 = file.find('_', p1);
        size_t p3 = file.find('.',p1);
        std::cout << file << std::endl;
        std::string cacheIdentifier = file.substr(p2, p3-p2);
        std::cout << p1 << ',' << p2 << ',' << p3 << std::endl;
        c.first = cacheIdentifier;
        c.second = file;
        cachePaths.push_back(c);
    }
    globfree(&glob_res);
    return cachePaths;
}

VadereScenario getScenarioContent(const cXMLElement* xmlConfig, const std::string basedir)
{
    VadereScenario ret;
    cXMLElementList scenario = xmlConfig->getElementsByTagName("scenario");
    if (scenario.size() == 0){
        throw cRuntimeError("launchConfig does not have scenario element");
    } else {
        std::string path = scenario[0]->getAttribute("path") == nullptr ? std::string() : std::string(scenario[0]->getAttribute("path"));
        assert(!path.empty());
        ret.first = basedir + path;
    }

    std::ifstream file(ret.first.c_str());
    if (file){
        ret.second = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    } else {
        throw cRuntimeError ("Cannot read scenario file at: \"%s\"", ret.first.c_str());
    }
    return ret;
}

}
