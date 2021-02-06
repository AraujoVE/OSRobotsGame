# OSRobotsGame

This project is an application of concepts from Operating Systems and Evolutive Systems disciplines at USP - São Carlos.
It consists of a multi-threaded game that can be run with and *evolutionary algorithm* to balance it's constants.

# Installation

Currently the game is only supported for linux. (Tested with Ubuntu 18.04)

The installation steps are pretty simple.
Since this repository uses git submodules, it needs to be cloned recursively: \
`git clone --recursive https://github.com/AraujoVE/OSRobotsGame`

Needed system dependencies: `g++-9, make` and basic opengl drivers (depending on your graphics card)

### Extra dependencies
There are some dependencies that need to be installed separately:
- [Armadillo](http://arma.sourceforge.net/download.html) (Tested with version 10.2.0-1) ([Turorial](https://github.com/masumhabib/quest/wiki/How-to-Install-Armadillo))
- [Boost](https://www.boost.org/users/download) (Tested with version 1.75.0-2) (On Ubuntu: `apt-get install -y libboost-all-dev`)

All other depencencies, such as `GLFW, Glad, ImGui, spdlog and premake5` are already linked as submodules.

# Compilation/Execution

We are using premake5 to generate makefiles and solution files.
To generate those makefiles and execute the Application, one can use our custom script with: `./dev.bash`, located at the repository root

Alternatively, one can do mannually all gthe steps:
1. Generating Makefiles for linux (gmake2): `premake5 gmake2`
2. Compiling project: `make all -j <NUMBER_OF_THREADS>` (You will want to use more than one thread)
3. Executing project: `./bin/Debug-linux-x86_64/Application/Application`
