#include "header/RobotsManagement.hpp"
#include "header/Task.hpp"
#include "header/VillageStats.hpp"
#include <vector>
#include <map> 
#include <ctime>
#include <iostream>

tm getUTCTime(){
    // current date/time based on current system
    time_t now = time(0);

    // convert now to string form
    std::string dt(ctime(&now));
    // convert now to tm struct for UTC
    tm *gmtm = gmtime(&now);
    //To convert to string
    //dt = asctime(gmtm);
    return *gmtm;
}

int getTotRobots(){
    
}
int getFreeRobots(){
    
}
int getEfficiency(){
    
}
int getProdCost(){
    
}

void setTotRobots(){
    
}
void setFreeRobots(){
    
}
void setEfficiency(){
    
}
void setProdCost(){
    
}



bool RobotsManagement::createRobot(VillageStats vila){
    int curResources = vila.getResources();
    //Calcula se existe dinheiro o suficiente para criar um robô
    if(curResources >= prodCost){
        //Decrementa dos recursos o custo de produção do robô
        vila.setResources(curResources - prodCost);
    }
    else return false;

    //Incrementa o número total e o número de robôs livres    
    totRobots++;
    freeRobots++;
    return true;
}
bool RobotsManagement::destroyRobot(){
    //Vê se é possível destruir robôs
    if(!totRobots || !freeRobots) return false;

    //Decrementa o número total e o número de robôs livres    
    totRobots--;
    freeRobots--;
    return false;
}
bool RobotsManagement::sendRobot(std::vector<std::map<tm,Task>> tasks,RobotFunctions funct,int robotNo){
    if(freeRobots - robotNo <= 0) return false;
    freeRobots -= robotNo;
    tm dateTime = getUTCTime();
    Task newTask(funct,robotNo,dateTime);
    if(tasks.at(funct).find(dateTime) == tasks.at(funct).end()){
        return false;
    }
    tasks.at(funct).insert({getUTCTime(),newTask});
    return true;
}