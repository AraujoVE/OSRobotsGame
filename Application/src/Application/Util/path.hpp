#pragma once

#include <stdio.h> /* defines FILENAME_MAX */
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

namespace Util::Path
{
    enum class ResourceType : uint8_t
    {
        GAME_CONSTS = 0,
        GAME_SCRIPT_MACHINE,
        GAME_SCRIPT_HUMAN_FOLDER
    };

    const std::string &getDefaultPath(ResourceType resourceType);
    const std::string &getCWD();
} // namespace Util::Path