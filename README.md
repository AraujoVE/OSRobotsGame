# OSRobotsGame

This project is divided in two development stages, each one for a different discipline at **USP - São Carlos**. The developer team is also slightly different between the stages.

The first stage introduces a resource management game which uses various *threads* to operate it's mechanics, developed for **Operating Systems I - SSC0140** by:
- [Marcus Vinicius C. B. Martins](https://github.com/marcuscastelo);
- [Vinícius Eduardo de Araújo](https://github.com/AraujoVE);
- [Lucas de Medeiros França Romero](https://github.com/LMFRomero);
- [Pedro Guerra Lourenço](https://github.com/Haltz01).

The second stage adds, on top of the game, an *Evolutionary Algorithm* that aims to solve *game design* problems we had at the first stage of this project: the game always ended up being either too hard or too easy. So, our idea is to use an EA (*Evolutionary Algorithm*) to tune game constants in a way that we are satisfied with the difficulty of the game. This second stage was developed for **Evolutionary systems applied to robotics - SCC0713** by:

- [Marcus Vinicius C. B. Martins](https://github.com/marcuscastelo);
- [Vinícius Eduardo de Araújo](https://github.com/AraujoVE);
- [Pedro Guerra Lourenço](https://github.com/Haltz01);
- [Lucas Yuji Matubara](https://github.com/YujiMatubara).

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

Alternatively, one can do mannually all the steps:
1. Generating Makefiles for linux (gmake2): `premake5 gmake2`
2. Compiling project: `make all -j <NUMBER_OF_THREADS>` (You will want to use more than one thread)
3. Executing project: `./bin/Debug-linux-x86_64/Application/Application`
