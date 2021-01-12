#include <DampEngine.hpp>

#include "imgui.h"
#include "imgui_internal.h"

#include "Application/header/RobotsManagement.hpp"
#include "Application/header/Task.hpp"
#include "Application/header/VillageStats.hpp"
#include "Application/header/RobotFunctions.hpp"
#include "Application/header/GameSave.hpp"
#include "Application/header/ConstsMap.hpp"
#include "Application/header/EAScript.hpp"
#include "Application/ImGui/Window/RobotCreationWindow.hpp"
#include "Application/ImGui/Window/TaskWindow.hpp"



#include <vector>
#include <algorithm>
#include <map>
#include <ctime>
#include <iostream>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <cstdlib>
#include <stdlib.h>
#include <random>
#include <sstream>
#include <memory>

#include <unistd.h>