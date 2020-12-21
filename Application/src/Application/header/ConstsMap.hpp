#ifndef CONSTS_MAP
#define CONSTS_MAP
#include <unordered_map>
#include <string>
namespace Application{
    class ConstsMap{
        private:
            static std::unordered_map<std::string,float> constsMap;
        public:
            static void initMapFromFile(const std::string&);
            static float getValue(const std::string&);
    };

}

#endif