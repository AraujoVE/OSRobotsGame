#include "ScriptConverter.hpp"

#include "Application/Util/path.hpp"
#include "EvoAlg/Script/Script.hpp"

#include "mypch.hpp"

namespace EvoAlg
{
    void ScriptConverter::removeSpaces(std::string &desiredStr)
    {
        std::string newStr = "";
        for (int i = 0; i < (int)desiredStr.length(); i++)
        {
            if (desiredStr[i] != ' ')
                newStr += desiredStr[i];
        }
        desiredStr = newStr;
    }

    void ScriptConverter::removeParenthesis(std::string &desiredStr)
    {
        desiredStr = desiredStr.substr(desiredStr.find("(") + 1, desiredStr.find(")") - desiredStr.find("(") - 1);
    }

    std::vector<std::string> ScriptConverter::splitString(std::string desiredStr, std::string key)
    {
        std::vector<std::string> splittedStr;
        int tokenPos = desiredStr.find(key);
        while (tokenPos != -1)
        {
            splittedStr.push_back(desiredStr.substr(0, tokenPos));
            desiredStr = desiredStr.substr(tokenPos + 1, desiredStr.length() - (1 + tokenPos));
            tokenPos = desiredStr.find(key);
        }
        splittedStr.push_back(desiredStr);

        return splittedStr;
    }

    std::vector<std::string> ScriptConverter::splitText(std::string desiredStr)
    {
        removeSpaces(desiredStr);
        removeParenthesis(desiredStr);
        return splitString(desiredStr, ",");
    }

    void ScriptConverter::commandFunct0(std::string command, int pos)
    {
        std::vector<std::string> splittedStr = splitText(command);
        // for (int i = 0; i < (int)splittedStr.size(); i++)
            // std::cout << "$" << splittedStr[i] << std::endl;
        m_Gameplay.push_back("0," + functToIntMap[splittedStr[0]]);
    }
    void ScriptConverter::commandFunct1(std::string command, int pos)
    {
        std::vector<std::string> splittedStr = splitText(command);
        // for (int i = 0; i < (int)splittedStr.size(); i++)
        //     std::cout << "$" << splittedStr[i] << std::endl;
        m_Gameplay.push_back("1," + functToIntMap[splittedStr[0]] + "," + splittedStr[1]);
    }

    void ScriptConverter::commandFunct2(std::string command, int pos)
    {
        std::vector<std::string> splittedStr = splitText(command);
        // for (int i = 0; i < (int)splittedStr.size(); i++)
        //     std::cout << "$" << splittedStr[i] << std::endl;
        m_Gameplay.push_back("2," + functToIntMap[splittedStr[0]] + "," + splittedStr[1]);
    }
    void ScriptConverter::commandFunct3(std::string command, int pos)
    {
        std::vector<std::string> splittedStr = splitText(command);
        // for (int i = 0; i < (int)splittedStr.size(); i++)
        //     std::cout << "$" << splittedStr[i] << std::endl;
        m_Gameplay.push_back("3," + functToIntMap[splittedStr[0]] + "," + splittedStr[1]);
    }

    void ScriptConverter::commandFunct4(std::string command, int pos)
    {
        m_Gameplay.push_back("4");
    }
    void ScriptConverter::commandFunct5(std::string command, int pos)
    {
        m_Gameplay.push_back("5");
    }

    void ScriptConverter::commandFunct6(std::string command, int pos)
    {
        std::vector<std::string> splittedStr = splitText(command);
        // for (int i = 0; i < (int)splittedStr.size(); i++)
        //     std::cout << "$" << splittedStr[i] << std::endl;
        m_Gameplay.push_back("6," + splittedStr[0]);
    }
    void ScriptConverter::commandFunct7(std::string command, int pos)
    {
        std::vector<std::string> splittedStr = splitText(command);
        std::fstream myFile;

        auto &targetEndTime = splittedStr[0];

        m_Gameplay.emplace(m_Gameplay.begin() + m_LastSize, "#" + targetEndTime);
        m_LastSize = m_Gameplay.size();
    }

    ScriptConverter::ScriptConverter(const std::string& humanScriptsFolderPath)
    : m_HumanScriptsFolderPath(humanScriptsFolderPath)
    {
    }

    //TODO: fix ml (with reference, e.g. Ref<Script> sc = new Script())
    Script *ScriptConverter::Convert() 
    {
        m_Gameplay.clear();

        Script *convertedScript = new Script();

        std::fstream inputFile;
        inputFile.open(m_HumanScriptsFolderPath + std::to_string(0) + ".txt", std::ios::in);

        int i = 0;
        std::string line;
        while (!inputFile.fail())
        {
            while (std::getline(inputFile, line))
            {
                // std::cout << line << std::endl;
                for (int j = 0; j < 8; j++)
                {
                    if (line.find(commands[j]) != std::string::npos)
                    {
                        (this->*(commandFuncts[j]))(line, i);
                        break;
                    }
                }
            }
            inputFile.close();
            inputFile.open(m_HumanScriptsFolderPath + std::to_string(++i) + ".txt", std::ios::in);
        }

        inputFile.close();

        convertedScript->m_Instructions = m_Gameplay;

        return convertedScript;
    }


    ScriptConverter::~ScriptConverter() {}
} // namespace EvoAlg


//TODO: @AraujoVE, delete this giant commented code if not necessary
  /*
            if(line[0] == '#'){
                gameScript.push_back(std::vector<std::vector<std::string>>());
                curGameplay++;
                curOp = 0;
                gameScript.at(curGameplay).push_back(std::vector<std::string>());
                gameScript.at(curGameplay).at(curOp).push_back(line.substr(1,line.length() - 1));
            }
            else{
                gameScript.at(curGameplay).push_back(std::vector<std::string>());
                curOp++;
                tokenPos = line.find(",");
                while(tokenPos != -1){
                    gameScript.at(curGameplay).at(curOp).push_back(line.substr(0,tokenPos));
                    line = line.substr(tokenPos + 1,line.length() - (1 + tokenPos));
                    tokenPos = line.find(",");
                }
                gameScript.at(curGameplay).at(curOp).push_back(line);
*/

/*
    while (std::getline(myFile, line))
    {
        if(line[0] == '#'){
            gameScript.push_back(std::vector<std::vector<std::string>>());
            curGameplay++;
            curOp = 0;
            gameScript.at(curGameplay).push_back(std::vector<std::string>());
            gameScript.at(curGameplay).at(curOp).push_back(line.substr(1,line.length() - 1));
        }
        else{
            gameScript.at(curGameplay).push_back(std::vector<std::string>());
            curOp++;
            tokenPos = line.find(",");
            while(tokenPos != -1){
                gameScript.at(curGameplay).at(curOp).push_back(line.substr(0,tokenPos));
                line = line.substr(tokenPos + 1,line.length() - (1 + tokenPos));
                tokenPos = line.find(",");
            }
            gameScript.at(curGameplay).at(curOp).push_back(line);
        }
    }
    */










// ----- Script formatting tutorial: -----


/*
task++(par1) //Cria uma nova Task
    //par1 : Tipo de Function da Task{
        pro,
        hun,
        med,
        con,
        res
    }

task--(par1,par2) //Termina uma Task existente
    //par1 : Função da Task
    //par2 : Numero da Task removida

move++(par1,par2) // Move os robos para uma Task
    //par1 : Função da Task afetada
    //par2 : Numero da Task afetada


move--(par1,par2) // Remove os robos de uma Task
    //par1 : Função da Task afetada
    //par2 : Numero da Task afetada


robots++ //Incrementa o número de robôs

robots-- //Destroi um robô

wait(par1) // Espera um certo tempo até a próxima ação
    //par1 : tempo de espera em WAIT_UNITS
*/

/*
//task++(med)
0,2

//task--(pro,5)
1,0,5

//move++(con,6)
2,3,6

//move--(med,9)
3,2,9

//robots++
4

//robots--
5

//wait(3)
6,3
*/

/*
task++(med)
task--(pro,5)
move++(con,6)
move--(med,9)
robots++
robots--
wait(3)
end(182)
*/

/*
task++(med)
task--(pro,5)
move++(con,6)
move--(med,9)
robots++
robots--
wait(10)
end(122)
*/