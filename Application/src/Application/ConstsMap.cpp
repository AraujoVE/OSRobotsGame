#include <magic.h>

#include <vector>
#include <unordered_map>
#include "Application/header/ConstsMap.hpp"
#include "Application/Events/EventListener/EventListener.hpp"
#include "Application/Events/EventHandler/DefaultHandlers.hpp"

#include "mypch.hpp"

#include "DampEngine/Threads/Mutex.hpp"

namespace Application
{

    GameConsts::GameConsts() : m_EventListener(new EventListener()) {}
    GameConsts::~GameConsts() { delete m_EventListener; }

    std::unordered_map<std::string, float> GameConsts::constsMap = {};
    void GameConsts::LoadValuesFromFile(const std::string &srcFile)
    {
        //Static to force all instances to wait for this instance to end
        static DampEngine::Mutex fileMutex;

        fileMutex.Lock();
        {
            constsMap.clear();
            std::fstream myFile;
            int tokenPos;
            float val;
            myFile.open(srcFile, std::ios::in);

            DE_ASSERT(!myFile.fail(), "(GameConsts) FAILED TO OPEN FILE  '" + srcFile + "'");

            std::string line, name;
            while (std::getline(myFile, line))
            {
                if (line[0] < 'A' || line[0] > 'z')
                    continue;
                tokenPos = line.find("=");
                name = line.substr(0, tokenPos - 1);
                val = stof(line.substr(tokenPos + 2, line.length() - tokenPos - 1));
                SetValue(name, val);
            }

            myFile.close();
        }
        fileMutex.Unlock();
    }

    void GameConsts::LoadFromCromossome(const std::vector<double> &cromossome)
    {
        int i = 0;
        SetValue("ON_ATTACK_MULTIPLIER", (float)cromossome.at(i++));
        SetValue("POP_INCREASE_TAX", (float)cromossome.at(i++));
        SetValue("POP_PER_CONSTRUCTION", (float)cromossome.at(i++));
        SetValue("INIT_POP_VALUE", (float)cromossome.at(i++));
        SetValue("INIT_STAT_VALUE", (float)cromossome.at(i++));
        SetValue("ON_ATTACK_DECAY_TAX", (float)cromossome.at(i++));
        SetValue("NORMAL_DECAY_TAX", (float)cromossome.at(i++));
        SetValue("ATTACK_FREQUENCY", (float)cromossome.at(i++));
        SetValue("INIT_RESOURCES_VALUE", (float)cromossome.at(i++));
        SetValue("TAX_REDUCT", (float)cromossome.at(i++));
        // SetValue("DECAY_DELAY_MICRO", (float) cromossome.at(i++));
        SetValue("MIN_LOSS_0", (float)cromossome.at(i++));
        SetValue("MIN_LOSS_1", (float)cromossome.at(i++));
        SetValue("MIN_LOSS_2", (float)cromossome.at(i++));
        SetValue("MIN_LOSS_3", (float)cromossome.at(i++));
        SetValue("MAX_LOSS_0", (float)cromossome.at(i++));
        SetValue("MAX_LOSS_1", (float)cromossome.at(i++));
        SetValue("MAX_LOSS_2", (float)cromossome.at(i++));
        SetValue("MAX_LOSS_3", (float)cromossome.at(i++));
        SetValue("TOT_ROBOTS_INI", (float)cromossome.at(i++));
        SetValue("FREE_ROBOTS_INI", (float)cromossome.at(i++));
        SetValue("PROD_COST_INI", (float)cromossome.at(i++));
        SetValue("PROD_COST_INCREASE_TAX", (float)cromossome.at(i++));
        SetValue("TIME_STEP", (float)cromossome.at(i++));
        SetValue("INIT_TIME_STEP", (float)cromossome.at(i++));
        SetValue("MAX_TIME_STEPS", (float)cromossome.at(i++));
        SetValue("MIN_REWARD", (float)cromossome.at(i++));
        SetValue("REWARD_RANGE", (float)cromossome.at(i++));
        SetValue("FAILURE_TAX", (float)cromossome.at(i++));
    }

    float GameConsts::GetValue(const std::string &key) const
    {
        DE_ASSERT(constsMap.find(key) != constsMap.end(), "(CONSTSMAP) MISSING KEY: '" + key + "'");
        return constsMap[key];
    }

    //TODO: avoid invalid params ((e.g. division per zero))
    float GameConsts::SetValue(const std::string &key, float newValue)
    {   
        //TODO: clamp values in another place
        if ( 
            ( 
                key == "MAX_TIME_STEPS" ||
                key == "REWARD_RANGE" 
            )       
            && newValue < 1.0f
        ) 
        newValue = 1.0f; 

        constsMap[key] = newValue;
        m_EventListener->On<EH_GCValueChanged>({key, newValue});
        return newValue;
    }

    void GameConsts::SetOnValueChanged(EH_GCValueChanged *eHandler, const std::vector<std::string> &filterKeyVec)
    {
        SetOnValueChanged(new EH_GCValueChanged([filterKeyVec, eHandler](const std::string &key, float newValue) {
            if (std::find(filterKeyVec.begin(), filterKeyVec.end(), key) != filterKeyVec.end())
                eHandler->m_Handler(key, newValue);
            return false;
        }));
    }

    void GameConsts::SetOnValueChanged(EH_GCValueChanged *eHandler)
    {
        m_EventListener->Register(eHandler);
    }

    GameConstsCache::GameConstsCache(GameConsts &gameConsts)
        : m_GameConsts(gameConsts)
    {
        std::function<void()> handler(std::bind(&GameConstsCache::UpdateAll, this));

        gameConsts.SetOnValueChanged(new EH_GCValueChanged([handler](const std::string &_, float __) {
            handler();
            return false;
        }));

        UpdateAll();
    }

} // namespace Application
