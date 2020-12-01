#include "header/RobotsManagement.hpp"
#include "header/Task.hpp"
#include "header/VillageStats.hpp"
#include "header/Task.hpp"
//#include <vector>
//#include <map> 
//#include <ctime>
//#include <iostream>



//Inicialização e destruição de classe
Task::Task(RobotFunctions funct,time_t id,int *efficiency){
    // use current time as seed for random generator
    std::srand(std::time(nullptr));
    initializeParameters(funct,id,efficiency);
}
void Task::initializeParameters(RobotFunctions funct,time_t id,int *newEfficiency){
    type = funct;
    robotsNo = 0;
    initTime = id;
    efficiency = newEfficiency;
    remainingTime = TIME_STEPS * (INIT_TIME_STEP + (std::rand()%9));
    createThread();
}



//Gets de cada um dos parâmetros

RobotFunctions Task::getType(){
    return type;
}

int Task::getRobotsNo(){
    return robotsNo;
}

time_t Task::getInitTime(){
    return initTime;
}

int Task::getRemainingTime(){
    return remainingTime;
}



//Sets de cada um dos parâmetros

void Task::setType(RobotFunctions newType){
    type = newType;
}

void Task::setRobotsNo(int newRobotsNo){
    robotsNo = newRobotsNo;
}

void Task::setInitTime(time_t newInitTime){
    initTime = newInitTime;
}

void Task::setRemainingTime(int newRemainingTime){
    remainingTime = newRemainingTime;
}

//Retorna false para quando o remainingTime da tarefa se torna 0, caso contrário apenas reduz o tempo
bool Task::updateRemainingTime(){
    int timeReduced = (*efficiency) * robotsNo;
    if(timeReduced <= 0){
        remainingTime = 0;
        deleteThread();
        return false;
    }
    remainingTime -= timeReduced;
    return true;
}


//TODO: Implmentar a criação e destruição da thread e como ela funciona

void Task::createThread(){}

void Task::deleteThread(){}