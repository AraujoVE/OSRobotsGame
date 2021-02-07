#include "Application/Util/path.hpp"

namespace Util::Path {

    const std::string& getCWD()
    {
        static std::string cwd = "PPWPP";
        
        if (cwd == "PPWPP")
        {
            char path[FILENAME_MAX];
            GetCurrentDir(path, FILENAME_MAX);
            cwd = std::string(path);
        }

        return cwd;
    }

   
    const std::string& getDefaultPath(ResourceType resourceType) {
        static std::string defaultPaths[] = {
            getCWD() + "/Application/config/constValues.cfg",
            getCWD() + "/gameScript.cfg",
            getCWD() + "/scripts/"
        };

        return defaultPaths[(int)resourceType];
    }
}