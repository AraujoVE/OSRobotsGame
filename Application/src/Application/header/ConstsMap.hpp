#ifndef CONSTS_MAP
#define CONSTS_MAP
#include <unordered_map>
#include <string>
#include <functional>
#include <vector>

#include "Application/Events/EventHandler/DefaultHandlers.fwd.hpp"
#include "Application/Events/EventListener/EventListener.fwd.hpp"

namespace Application
{

    class GameConsts final
    {
    private:
        static std::unordered_map<std::string, float> constsMap;
        EventListener *m_EventListener;

    public:

        GameConsts();
        ~GameConsts();
        

        void LoadValuesFromFile(const std::string &path);
        float GetValue(const std::string &key) const;
        float SetValue(const std::string &key, float newValue);

        void SetOnValueChanged(EH_GCValueChanged *eHandler, const std::vector<std::string> &filterKeyVec);
        void SetOnValueChanged(EH_GCValueChanged *eHandler);

        static std::string GetDefaultPath()
        {
            char path[1024];
            getcwd(path, 1024);
            std::string cwd(path);
            return cwd + "/Application/config/constValues.cfg";
        }
    };

    struct GameConstsCache
    {
        int ON_ATTACK_MULTIPLIER;
        float POP_INCREASE_TAX;
        int POP_PER_CONSTRUCTION;
        float INIT_POP_VALUE;
        int INIT_STAT_VALUE;
        float ON_ATTACK_DECAY_TAX;
        float NORMAL_DECAY_TAX;
        int ATTACK_FREQUENCY;
        float INIT_RESOURCES_VALUE;
        float TAX_REDUCT;
        useconds_t DECAY_DELAY_MICRO;
        float MIN_LOSS[4];
        float MAX_LOSS[4];
        float TOT_ROBOTS_INI;
        float FREE_ROBOTS_INI;
        float PROD_COST_INI;
        float PROD_COST_INCREASE_TAX;
        int TIME_STEP;
        int INIT_TIME_STEP;
        int MAX_TIME_STEPS;
        int MIN_REWARD;
        int REWARD_RANGE;
        int FAILURE_TAX;
        int AVG_REWARD;

    private:
        const GameConsts &m_GameConsts;
        std::vector<std::function<void()>> m_AditionalUpdates;
        void UpdateAll()
        {
            ON_ATTACK_MULTIPLIER = m_GameConsts.GetValue("ON_ATTACK_MULTIPLIER");
            POP_INCREASE_TAX = m_GameConsts.GetValue("POP_INCREASE_TAX");
            POP_PER_CONSTRUCTION = m_GameConsts.GetValue("POP_PER_CONSTRUCTION");
            INIT_POP_VALUE = m_GameConsts.GetValue("INIT_POP_VALUE");
            INIT_STAT_VALUE = m_GameConsts.GetValue("INIT_STAT_VALUE");
            ON_ATTACK_DECAY_TAX = m_GameConsts.GetValue("ON_ATTACK_DECAY_TAX");
            NORMAL_DECAY_TAX = m_GameConsts.GetValue("NORMAL_DECAY_TAX");
            ATTACK_FREQUENCY = m_GameConsts.GetValue("ATTACK_FREQUENCY");
            INIT_RESOURCES_VALUE = m_GameConsts.GetValue("INIT_RESOURCES_VALUE");
            TAX_REDUCT = m_GameConsts.GetValue("TAX_REDUCT");
            DECAY_DELAY_MICRO = m_GameConsts.GetValue("DECAY_DELAY_MICRO");
            MIN_LOSS[0] = m_GameConsts.GetValue("MIN_LOSS_0");
            MIN_LOSS[1] = m_GameConsts.GetValue("MIN_LOSS_1");
            MIN_LOSS[2] = m_GameConsts.GetValue("MIN_LOSS_2");
            MIN_LOSS[3] = m_GameConsts.GetValue("MIN_LOSS_3");
            MAX_LOSS[0] = m_GameConsts.GetValue("MAX_LOSS_0");
            MAX_LOSS[1] = m_GameConsts.GetValue("MAX_LOSS_1");
            MAX_LOSS[2] = m_GameConsts.GetValue("MAX_LOSS_2");
            MAX_LOSS[3] = m_GameConsts.GetValue("MAX_LOSS_3");
            TOT_ROBOTS_INI = m_GameConsts.GetValue("TOT_ROBOTS_INI");
            FREE_ROBOTS_INI = m_GameConsts.GetValue("FREE_ROBOTS_INI");
            PROD_COST_INI = m_GameConsts.GetValue("PROD_COST_INI");
            PROD_COST_INCREASE_TAX = m_GameConsts.GetValue("PROD_COST_INCREASE_TAX");
            TIME_STEP = m_GameConsts.GetValue("TIME_STEP");
            INIT_TIME_STEP = m_GameConsts.GetValue("INIT_TIME_STEP");
            MAX_TIME_STEPS = m_GameConsts.GetValue("MAX_TIME_STEPS");
            MIN_REWARD = m_GameConsts.GetValue("MIN_REWARD");
            REWARD_RANGE = m_GameConsts.GetValue("REWARD_RANGE");
            FAILURE_TAX = m_GameConsts.GetValue("FAILURE_TAX");

            AVG_REWARD = (int)(((float)TIME_STEP) * ((float)INIT_TIME_STEP + ((float)MAX_TIME_STEPS - 1.0) / 2.0) * ((float)MIN_REWARD + ((float)REWARD_RANGE - 1.0) / 2.0));
            

            for (auto updateFn : m_AditionalUpdates)
                updateFn();
        }

    public:
        GameConstsCache(GameConsts &gameConsts);

        void AddAditionalUpdateFn(const std::function<void()>& function) {
            m_AditionalUpdates.push_back(function);
        }
    };

} // namespace Application

#endif