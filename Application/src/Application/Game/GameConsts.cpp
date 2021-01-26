#include <vector>
#include <unordered_map>
#include "Application/Game/GameConsts.hpp"
#include "Application/Events/EventListener/EventListener.hpp"
#include "Application/Events/EventHandler/DefaultHandlers.hpp"

#include "mypch.hpp"

#include "DampEngine/Threads/Mutex.hpp"

namespace Application
{

    const ParameterApplier PredefinedAppliers::defaultApplier = [](GameConsts *, float receivedValue, std::vector<std::string>) {
        return receivedValue;
    };
    const ParameterApplier PredefinedAppliers::divParameters = [](GameConsts *gameConsts, float receivedValue, std::vector<std::string> paramsList) {
        return gameConsts->GetRawValue(paramsList.at(0)) / receivedValue;
    };

    GameConsts::GameConsts()
        : m_ConstsMap({}),
          m_EventListener(new EventListener()),
          m_MapMutex()
    {

        m_ConstsMap.insert({DA_DECL_PARAM_BASIC(ON_ATTACK_MULTIPLIER),
                            DA_DECL_PARAM_BASIC(POP_INCREASE_TAX),
                            DA_DECL_PARAM_BASIC(POP_PER_CONSTRUCTION),
                            DA_DECL_PARAM_BASIC(INIT_POP_VALUE),
                            DA_DECL_PARAM_BASIC(INIT_STAT_VALUE),
                            DA_DECL_PARAM_BASIC(ON_ATTACK_DECAY_TAX),
                            DA_DECL_PARAM_APPLY(NORMAL_DECAY_TAX, divParameters, ON_ATTACK_DECAY_TAX),
                            DA_DECL_PARAM_BASIC(ATTACK_FREQUENCY),
                            DA_DECL_PARAM_BASIC(INIT_RESOURCES_VALUE),
                            DA_DECL_PARAM_BASIC(TAX_REDUCT),
                            DA_DECL_PARAM_APPLY(MIN_LOSS_0, divParameters, MAX_LOSS_0),
                            DA_DECL_PARAM_APPLY(MIN_LOSS_1, divParameters, MAX_LOSS_1),
                            DA_DECL_PARAM_APPLY(MIN_LOSS_2, divParameters, MAX_LOSS_2),
                            DA_DECL_PARAM_APPLY(MIN_LOSS_3, divParameters, MAX_LOSS_3),
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
                            DA_DECL_PARAM_BASIC(FAILURE_TAX)});
    }

    GameConsts::~GameConsts() { delete m_EventListener; }

    DampEngine::Mutex GameConsts::s_FileMutex;
    void GameConsts::LoadFromFile(const std::string &srcFile)
    {
        DE_DEBUG("GameConsts::LoadFromFile START");

        s_FileMutex.Lock();
        m_MapMutex.Lock();
        {
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
        m_MapMutex.Unlock();
        s_FileMutex.Unlock();

        for (auto &pairIt : m_ConstsMap)
            pairIt.second.Apply(this);

        DE_DEBUG("GameConsts::LoadFromFile SUCCESS");

        m_EventListener->On<EH_GameConstsChanged>();
    }

    void GameConsts::LoadFromCromossome(const std::vector<double> &cromossome)
    {
        m_MapMutex.Lock();
        {
            DE_ASSERT(cromossome.size() == 27, "(GameConsts::LoadFromCromossome) WRONG CROMOSSOME INFORMED")
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
            SetValue("MIN_LOSS_0", (float)cromossome.at(i++));
            SetValue("MIN_LOSS_1", (float)cromossome.at(i++));
            SetValue("MIN_LOSS_2", (float)cromossome.at(i++));
            SetValue("MIN_LOSS_3", (float)cromossome.at(i++));
            SetValue("MAX_LOSS_0", (float)cromossome.at(i++));
            SetValue("MAX_LOSS_1", (float)cromossome.at(i++));
            SetValue("MAX_LOSS_2", (float)cromossome.at(i++));
            SetValue("MAX_LOSS_3", (float)cromossome.at(i++));
            SetValue("TOT_ROBOTS_INI", (float)cromossome.at(i++));
            SetValue("PROD_COST_INI", (float)cromossome.at(i++));
            SetValue("PROD_COST_INCREASE_TAX", (float)cromossome.at(i++));
            SetValue("TIME_STEP", (float)cromossome.at(i++));
            SetValue("INIT_TIME_STEP", (float)cromossome.at(i++));
            SetValue("MAX_TIME_STEPS", (float)cromossome.at(i++));
            SetValue("MIN_REWARD", (float)cromossome.at(i++));
            SetValue("REWARD_RANGE", (float)cromossome.at(i++));
            SetValue("FAILURE_TAX", (float)cromossome.at(i++));
        }
        m_MapMutex.Unlock();

        for (auto &pairIt : m_ConstsMap)
            pairIt.second.Apply(this);

        m_EventListener->On<EH_GameConstsChanged>();
    }

    std::vector<double> GameConsts::SaveToCromossome() {
        const static size_t geneCount = 27;
        const static std::string cromossomeOrder[] = { "ON_ATTACK_MULTIPLIER", "POP_INCREASE_TAX", "POP_PER_CONSTRUCTION", "INIT_POP_VALUE", "INIT_STAT_VALUE", "ON_ATTACK_DECAY_TAX", "NORMAL_DECAY_TAX", "ATTACK_FREQUENCY", "INIT_RESOURCES_VALUE", "TAX_REDUCT", "MIN_LOSS_0", "MIN_LOSS_1", "MIN_LOSS_2", "MIN_LOSS_3", "MAX_LOSS_0", "MAX_LOSS_1", "MAX_LOSS_2", "MAX_LOSS_3", "TOT_ROBOTS_INI", "PROD_COST_INI", "PROD_COST_INCREASE_TAX", "TIME_STEP", "INIT_TIME_STEP", "MAX_TIME_STEPS", "MIN_REWARD", "REWARD_RANGE", "FAILURE_TAX", };
        DE_ASSERT(sizeof(cromossomeOrder)/sizeof(std::string) == geneCount)

        std::vector<double> cromossome;
        for (unsigned int i = 0; i < geneCount; i++) {
            cromossome.push_back(GetRawValue(cromossomeOrder[i]));
        }

        return cromossome;
    }

    float GameConsts::GetRawValue(const std::string &key) const
    {
        m_MapMutex.Lock();
        auto findIt = m_ConstsMap.find(key);
        DE_ASSERT(findIt != m_ConstsMap.end(), "(CONSTSMAP) MISSING KEY: '" + key + "'");
        float val = findIt->second.CapturedValue;
        m_MapMutex.Unlock();
        return val;
    }

    float GameConsts::GetValue(const std::string &key) const
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

    void GameConsts::RegisterOnValueChanged(EH_GameConstsChanged *eHandler) const { m_EventListener->Register(eHandler);}
    void GameConsts::UnregisterOnValueChanged(EH_GameConstsChanged *eHandler) const { m_EventListener->Unregister(eHandler);}

    void GameConsts::SetTickDelay(uint32_t newTickDelay)
    {
        TICK_DELAY_MICRO = newTickDelay;
        m_EventListener->On<EH_GameConstsChanged>();
    }

    GameConstsCache::GameConstsCache(GameConsts &gameConsts)
        : m_GameConsts(gameConsts)
    {
        std::function<void()> handler(std::bind(&GameConstsCache::UpdateAll, this));
        
        m_GameConsts.RegisterOnValueChanged(new EH_GameConstsChanged([handler]() {
            handler();
            return false;
        }));

        UpdateAll();
    }

    GameConstsCache::~GameConstsCache() {
    }


} // namespace Application
