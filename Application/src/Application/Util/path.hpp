#pragma once

#include <stdio.h>  /* defines FILENAME_MAX */
#include <string>

#if defined(DE_SYSTEM_WINDOWS)
#include <direct.h>
#define GetCurrentDir _getcwd
#elif defined(DE_SYSTEM_LINUX) || true
#include <unistd.h>
#define GetCurrentDir getcwd
#else
#error "unsupported system"
#endif

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

    enum class ResourceType : uint8_t {
        GAME_CONSTS = 0,
        GAME_SCRIPT_MACHINE,
        GAME_SCRIPT_HUMAN_FOLDER
    };

    const std::string& getDefaultPath(ResourceType resourceType) {
        static std::string defaultPaths[] = {
            getCWD() + "/Application/config/constValues.cfg",
            getCWD() + "/gameScript.cfg",
            getCWD() + "/scripts/"
        };

        return defaultPaths[(int)resourceType];
    }
}