#include <vector>
#include <unordered_map>
#include "header/ConstsMap.hpp"

#include "mypch.hpp"


namespace Application
{

    std::unordered_map<std::string,float> ConstsMap::constsMap = {};
    void ConstsMap::initMapFromFile(const std::string &srcFile)
    {
        constsMap.clear();

        std::fstream myFile;
        int tokenPos;
        float val;
        myFile.open(srcFile, std::ios::in);
        std::string line, name;
        while (std::getline(myFile, line))
        {
            if(line[0] < 'A' || line[0] > 'z') continue;
            tokenPos = line.find("=");
            name = line.substr(0, tokenPos - 1);
            val = stof(line.substr(tokenPos + 2, line.length() - tokenPos - 1));
            constsMap.insert({name, val});
        }
        return;
    }

    float ConstsMap::getValue(const std::string &key){
        return constsMap[key];
    }

} // namespace Application
