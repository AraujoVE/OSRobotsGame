#include "header/RobotsManagement.hpp"
#include "header/Task.hpp"
#include "header/VillageStats.hpp"
#include "header/Task.hpp"
//#include <vector>
//#include <map> 
#include <ctime>
//#include <iostream>



//Inicialização e destruição de classe
Task::Task(RobotFunctions funct,time_t id){
    // use current time as seed for random generator
    std::srand(std::time(nullptr));
    initializeParameters(funct,id);
}
void Task::initializeParameters(RobotFunctions funct,time_t id){
    type = funct;
    robotsNo = 0;
    initTime = id;
    lastUpdateTime = id;
    efficiency = 1;
    timeUnits = TIME_STEP * (INIT_TIME_STEP + (std::rand()%9));
    predictedTime = 0;
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

int Task::getPredictedTime(){
    return predictedTime;
}



//Sets de cada um dos parâmetros

void Task::setType(RobotFunctions newType){
    type = newType;
}

void Task::setInitTime(time_t newInitTime){
    initTime = newInitTime;
}

void Task::setPredictedTime(int newPredictedTime){
    predictedTime = newPredictedTime;
}

void Task::setRobotsNo(int newRobotsNo){
    updatePredictedTime(time(0),efficiency,newRobotsNo);
    robotsNo = newRobotsNo;
}


void Task::efficiencyUpdate(int newEfficiency){
    updatePredictedTime(time(0),newEfficiency,robotsNo);
}



//Retorna false para quando o PredictedTime da tarefa se torna 0, caso contrário apenas muda os atributos necessários
bool Task::updatePredictedTime(time_t curTime,int newEfficiency,int newRobotsNo){
    //timeVar simboliza teoricamente quantas unidades de progresso foram geradas entre o momento atual e 
    //o momento da última atualização, isso se dá pelo produto dos termos abaixo.
    int timeVar = (efficiency) * (robotsNo) * (curTime - lastUpdateTime);

    //Time units ou tem seu valor reduzido de timeVar ou se fosse dar <=0 vai para 0.
    timeUnits -= (timeVar >= timeUnits ? timeUnits : timeVar);

    //A futura eficiência e o futuro número de robos são utilizados para predizer quando será o time_t em que tal task vai ocorrer
    predictedTime = (timeUnits <= 0 ? 0 : curTime + (time_t)((timeUnits) / ((newEfficiency) * (newRobotsNo))) );
    
    //Ultimo update
    lastUpdateTime = curTime;

    if(timeUnits <= 0 ){
        //Para finalizar a thread dessa task
        deleteThread();
        return false;
    }
    return true;
}


//TODO: Implmentar a criação e destruição da thread e como ela funciona

void Task::createThread(){}

void Task::deleteThread(){}