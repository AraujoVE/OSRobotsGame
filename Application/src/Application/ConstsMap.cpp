#include <vector>
#include <unordered_map>
#include "Application/header/ConstsMap.hpp"
#include "Application/Events/EventListener/EventListener.hpp"
#include "Application/Events/EventHandler/DefaultHandlers.hpp"

#include "mypch.hpp"

#include "DampEngine/Threads/Mutex.hpp"

namespace Application
{

    const ParameterApplier PredefinedAppliers::defaultApplier = [](GameConsts *, float receivedValue, std::vector<std::string>) {
        return receivedValue;
    };

    GameConsts::GameConsts()
        : m_ConstsMap({}),
          m_EventListener(new EventListener()),
          m_MapMutex()
    {
        m_ConstsMap.insert({
            DA_DECL_PARAM_BASIC(ON_ATTACK_MULTIPLIER),
            DA_DECL_PARAM_BASIC(POP_INCREASE_TAX),
            DA_DECL_PARAM_BASIC(POP_PER_CONSTRUCTION),
            DA_DECL_PARAM_BASIC(INIT_POP_VALUE),
            DA_DECL_PARAM_BASIC(INIT_STAT_VALUE),
            DA_DECL_PARAM_BASIC(ON_ATTACK_DECAY_TAX),
            DA_DECL_PARAM_BASIC(NORMAL_DECAY_TAX),
            DA_DECL_PARAM_BASIC(ATTACK_FREQUENCY),
            DA_DECL_PARAM_BASIC(INIT_RESOURCES_VALUE),
            DA_DECL_PARAM_BASIC(TAX_REDUCT),
            DA_DECL_PARAM_BASIC(DECAY_DELAY_MICRO),
            DA_DECL_PARAM_BASIC(MIN_LOSS_0),
            DA_DECL_PARAM_BASIC(MIN_LOSS_1),
            DA_DECL_PARAM_BASIC(MIN_LOSS_2),
            DA_DECL_PARAM_BASIC(MIN_LOSS_3),
            DA_DECL_PARAM_BASIC(MAX_LOSS_0),
            DA_DECL_PARAM_BASIC(MAX_LOSS_1),
            DA_DECL_PARAM_BASIC(MAX_LOSS_2),
            DA_DECL_PARAM_BASIC(MAX_LOSS_3),
            DA_DECL_PARAM_BASIC(TOT_ROBOTS_INI),
            DA_DECL_PARAM_BASIC(FREE_ROBOTS_INI),
            DA_DECL_PARAM_BASIC(PROD_COST_INI),
            DA_DECL_PARAM_BASIC(PROD_COST_INCREASE_TAX),
            DA_DECL_PARAM_BASIC(TIME_STEP),
            DA_DECL_PARAM_BASIC(INIT_TIME_STEP),
            DA_DECL_PARAM_BASIC(MAX_TIME_STEPS),
            DA_DECL_PARAM_BASIC(MIN_REWARD),
            DA_DECL_PARAM_BASIC(REWARD_RANGE),
            DA_DECL_PARAM_BASIC(FAILURE_TAX)
        });
    }

    GameConsts::~GameConsts() { delete m_EventListener; }

    DampEngine::Mutex GameConsts::s_FileMutex;
    void GameConsts::LoadValuesFromFile(const std::string &srcFile)
    {
        s_FileMutex.Lock();
        m_MapMutex.Lock();
        {
            m_ConstsMap.clear();

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

        for (auto pairIt : m_ConstsMap)
            pairIt.second.Apply(this);

        m_MapMutex.Unlock();
        s_FileMutex.Unlock();

        m_EventListener->On<EH_GameConstsChanged>();
    }

    void GameConsts::LoadFromCromossome(const std::vector<double> &cromossome)
    {
        int i = 0;
        m_MapMutex.Lock();
        {
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

            for (auto pairIt : m_ConstsMap)
                pairIt.second.Apply(this);

        }
        m_MapMutex.Unlock();
        m_EventListener->On<EH_GameConstsChanged>();
    }

    float GameConsts::GetValue(const std::string &key)
    {
        m_MapMutex.Lock();
        auto findIt = m_ConstsMap.find(key);
        DE_ASSERT(findIt != m_ConstsMap.end(), "(CONSTSMAP) MISSING KEY: '" + key + "'");
        float val = findIt->second.AppliedValue;
        m_MapMutex.Unlock();
        return val;
    }

    void GameConsts::SetValue(const std::string &key, float newValue)
    {
        //ASSUMING MUTEX IS LOCKED
        m_ConstsMap[key].Capture(newValue);
    }

    void GameConsts::SetOnValueChanged(EH_GameConstsChanged *eHandler)
    {
        m_EventListener->Register(eHandler);
    }

    GameConstsCache::GameConstsCache(GameConsts &gameConsts)
        : m_GameConsts(gameConsts)
    {
        std::function<void()> handler(std::bind(&GameConstsCache::UpdateAll, this));

        gameConsts.SetOnValueChanged(new EH_GameConstsChanged([handler]() {
            handler();
            return false;
        }));

        UpdateAll();
    }

} // namespace Application
