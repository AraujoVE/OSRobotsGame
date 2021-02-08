#pragma once

#include <unordered_map>
#include <string>
#include <functional>
#include <vector>

#include "Application/Events/EventHandler/DefaultHandlers.fwd.hpp"
#include "Application/Events/EventListener/EventListener.fwd.hpp"

#define HUMAN_TICK_DELAY_MICRO 500e3

namespace Application
{
    class GameConsts;

    using ParameterApplier = std::function<float(GameConsts *, float receivedValue, std::vector<std::string>)>;

    struct PredefinedAppliers
    {
        const static ParameterApplier defaultApplier;
        const static ParameterApplier divParameters;
    };

    struct ParameterData
    {

    private:
        std::mutex m_ValuesMutex;
        ParameterApplier valueApplierFn;
        std::vector<std::string> applierParameters;

    public:
        float CapturedValue, AppliedValue;

        ParameterData(ParameterApplier valueApplierFn, std::vector<std::string> applierParameters)
            : valueApplierFn(valueApplierFn), applierParameters(applierParameters)
        {
            CapturedValue = AppliedValue = 0;
        }

        ParameterData() : ParameterData(PredefinedAppliers::defaultApplier, {})
        {
        }

        void Capture(float capturedValue)
        {
            m_ValuesMutex.lock();
            CapturedValue = capturedValue;
            m_ValuesMutex.unlock();
        }

        void Apply(GameConsts *gameConsts)
        {
            m_ValuesMutex.lock();
            AppliedValue = valueApplierFn(gameConsts, CapturedValue, applierParameters);
            m_ValuesMutex.unlock();
        }
    };

#define DA_DECL_PARAM_BASIC(name) { #name, ParameterData() }
#define DA_DECL_PARAM_APPLY(name, func, ...) { #name, ParameterData(PredefinedAppliers::func, {#__VA_ARGS__}) }

    class GameConsts final
    {
    private:

        std::unordered_map<std::string, ParameterData> m_ConstsMap;
        EventListener * m_EventListener;
        uint32_t TICK_DELAY_MICRO = HUMAN_TICK_DELAY_MICRO;

        //Static to force all instances to open file synchronously
        static std::mutex s_FileMutex;
        mutable std::mutex m_MapMutex;

        void SetValue(const std::string &key, float newValue);


    public:
        GameConsts();
        ~GameConsts();

        void LoadFromFile(const std::string &path);
        void LoadFromCromossome(const std::vector<double> &cromossome);

        std::vector<double> SaveToCromossome();
        // std::vector<double> SaveToFile();

        float GetRawValue(const std::string &key) const;
        float GetValue(const std::string &key) const;

        void RegisterOnValueChanged(EH_GameConstsChanged *eHandler) const;
        void UnregisterOnValueChanged(EH_GameConstsChanged *eHandler) const;


        //Gets/sets the current tick delay in microsseconds
        void SetTickDelay(uint32_t newTickDelay);
        inline uint32_t GetTickDelay() const { return TICK_DELAY_MICRO; }
        
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
        float MIN_LOSS[4];
        float MAX_LOSS[4];
        float TOT_ROBOTS_INI;
        float PROD_COST_INI;
        float PROD_COST_INCREASE_TAX;
        int TIME_STEP;
        int INIT_TIME_STEP;
        int MAX_TIME_STEPS;
        int MIN_REWARD;
        int REWARD_RANGE;
        int FAILURE_TAX;
        int AVG_REWARD;
        uint32_t TICK_DELAY_MICRO;

    private:
        const GameConsts &m_GameConsts;
        void UpdateAll()
        {
            int a = 3;
            a ++;
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
            MIN_LOSS[0] = m_GameConsts.GetValue("MIN_LOSS_0");
            MIN_LOSS[1] = m_GameConsts.GetValue("MIN_LOSS_1");
            MIN_LOSS[2] = m_GameConsts.GetValue("MIN_LOSS_2");
            MIN_LOSS[3] = m_GameConsts.GetValue("MIN_LOSS_3");
            MAX_LOSS[0] = m_GameConsts.GetValue("MAX_LOSS_0");
            MAX_LOSS[1] = m_GameConsts.GetValue("MAX_LOSS_1");
            MAX_LOSS[2] = m_GameConsts.GetValue("MAX_LOSS_2");
            MAX_LOSS[3] = m_GameConsts.GetValue("MAX_LOSS_3");
            TOT_ROBOTS_INI = m_GameConsts.GetValue("TOT_ROBOTS_INI");
            PROD_COST_INI = m_GameConsts.GetValue("PROD_COST_INI");
            PROD_COST_INCREASE_TAX = m_GameConsts.GetValue("PROD_COST_INCREASE_TAX");
            TIME_STEP = m_GameConsts.GetValue("TIME_STEP");
            INIT_TIME_STEP = m_GameConsts.GetValue("INIT_TIME_STEP");
            MAX_TIME_STEPS = m_GameConsts.GetValue("MAX_TIME_STEPS");
            MIN_REWARD = m_GameConsts.GetValue("MIN_REWARD");
            REWARD_RANGE = m_GameConsts.GetValue("REWARD_RANGE");
            FAILURE_TAX = m_GameConsts.GetValue("FAILURE_TAX");

            AVG_REWARD = (int)(((float)TIME_STEP) * ((float)INIT_TIME_STEP + ((float)MAX_TIME_STEPS - 1.0) / 2.0) * ((float)MIN_REWARD + ((float)REWARD_RANGE - 1.0) / 2.0));

            TICK_DELAY_MICRO = m_GameConsts.GetTickDelay();
        }

    public:
        GameConstsCache(GameConsts &gameConsts);
        ~GameConstsCache();
    };

} // namespace Application