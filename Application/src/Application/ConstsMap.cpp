#include <magic.h>

#include <vector>
#include <unordered_map>
#include "Application/header/ConstsMap.hpp"
#include "Application/Events/EventListener/EventListener.hpp"
#include "Application/Events/EventHandler/DefaultHandlers.hpp"

#include "mypch.hpp"

namespace Application
{

    GameConsts::GameConsts() : m_EventListener(new EventListener()) {}
    GameConsts::~GameConsts() { delete m_EventListener; }

    std::unordered_map<std::string, float> GameConsts::constsMap = {};
    void GameConsts::LoadValuesFromFile(const std::string &srcFile)
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
        return;
    }

    float GameConsts::GetValue(const std::string &key) const
    {
        DE_ASSERT(constsMap.find(key) != constsMap.end(), "(CONSTSMAP) MISSING KEY: '" + key + "'");
        return constsMap[key];
    }

    float GameConsts::SetValue(const std::string &key, float newValue)
    {
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
