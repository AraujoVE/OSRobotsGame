#pragma once

//#include <pthread.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

namespace EAAlgorithm
{
    class ConvertScripts
    {
    private:
        const static int COMMANDS_SIZE = 8;
        std::string commands[8] = {"task++", "task--", "move++", "move--", "robots++", "robots--", "wait", "end"};
        std::unordered_map<std::string, std::string> functToIntMap = {{"pro", "0"}, {"hun", "1"}, {"med", "2"}, {"con", "3"}, {"res", "4"}};
        std::vector<std::string> gameplay;
        std::string filePath;

    public:
        ConvertScripts(std::string);
        ~ConvertScripts();

        void removeSpaces(std::string &desiredStr);
        void removeParenthesis(std::string &desiredStr);
        std::vector<std::string> splitString(std::string desiredStr, std::string key);
        std::vector<std::string> splitText(std::string desiredStr);

        void commandFunct0(std::string, int);
        void commandFunct1(std::string, int);
        void commandFunct2(std::string, int);
        void commandFunct3(std::string, int);
        void commandFunct4(std::string, int);
        void commandFunct5(std::string, int);
        void commandFunct6(std::string, int);
        void commandFunct7(std::string, int);

        void (ConvertScripts::*commandFuncts[COMMANDS_SIZE])(std::string, int) = {
            &ConvertScripts::commandFunct0,
            &ConvertScripts::commandFunct1,
            &ConvertScripts::commandFunct2,
            &ConvertScripts::commandFunct3,
            &ConvertScripts::commandFunct4,
            &ConvertScripts::commandFunct5,
            &ConvertScripts::commandFunct6,
            &ConvertScripts::commandFunct7};
    };
} // namespace EAAlgorithm