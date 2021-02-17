#pragma once

#include <string>
#include <vector>
namespace EvoAlg
{
    class ScriptRunner;
    class Script final
    {
        std::vector<std::string> m_Instructions;

    public:
        //TODO: implement if needed
        static void SaveTo(const std::string &path) = delete;
        //ASSERT size > 0
        // m_OutputFile << (pos ? "\n#" : "#") << m_Gameplay[0] << std::endl;
        // for (int i = 1; i < (int)m_Gameplay.size(); i++)
        // {
        //     m_OutputFile << m_Gameplay[i];
        //     if (i != (int)(m_Gameplay.size() - 1))
        //         m_OutputFile << std::endl;
        // }


        static Script LoadFrom(const std::string &path) = delete;

    private:
        inline void AddInstruction(const std::string &instruction) { m_Instructions.push_back(instruction); }

    private:
        friend class ScriptRunner;
        friend class ScriptConverter;
    };
} // namespace EvoAlg