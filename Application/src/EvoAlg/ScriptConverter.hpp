#pragma once

//#include <pthread.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

namespace EvoAlg
{
    class Script;
    class ScriptConverter
    {
    private:
        const static int COMMANDS_SIZE = 8;

        //TODO: make const static
        std::string commands[8] = {"task++", "task--", "move++", "move--", "robots++", "robots--", "wait", "end"};

        //TODO: make const static
        std::unordered_map<std::string, std::string> functToIntMap = {{"pro", "0"}, {"hun", "1"}, {"med", "2"}, {"con", "3"}, {"res", "4"}};

        //TODO: use class Script
        std::vector<std::string> m_Gameplay;

        std::fstream m_OutputFile;
        std::string m_HumanScriptsFolderPath;
        size_t m_LastSize = 0;

    public:
        ScriptConverter(const std::string& humanScriptsFolderPath);
        ~ScriptConverter();

        Script *Convert();

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

        void (ScriptConverter::*commandFuncts[COMMANDS_SIZE])(std::string, int) = {
            &ScriptConverter::commandFunct0,
            &ScriptConverter::commandFunct1,
            &ScriptConverter::commandFunct2,
            &ScriptConverter::commandFunct3,
            &ScriptConverter::commandFunct4,
            &ScriptConverter::commandFunct5,
            &ScriptConverter::commandFunct6,
            &ScriptConverter::commandFunct7};
    };
} // namespace EvoAlg