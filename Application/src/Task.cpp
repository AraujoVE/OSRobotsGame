#include "header/RobotsManagement.hpp"
#include "header/Task.hpp"
#include "header/VillageStats.hpp"
#include "header/Task.hpp"

#include <random>
//#include <vector>
//#include <map> 
#include <ctime>
//#include <iostream>


int Task::lastId = -1;
//Inicialização e destruição de classe
Task::Task(RobotFunctions::RobotFunction funct){
    // use current time as seed for random generator
    std::srand(std::time(nullptr));
    initializeParameters(funct);
}

Task::~Task() {}

void Task::initializeParameters(RobotFunctions::RobotFunction funct){
    id = ++lastId; 
    type = funct;
    robotsNo = 0;
    lastUpdateTime = time(0);
    initTime = time(0);
    efficiency = 1;
    timeUnits = TIME_STEP * (INIT_TIME_STEP + (std::rand()%9));
    predictedTime = 0;
    createThread();
}



//Gets de cada um dos parâmetros

RobotFunctions::RobotFunction Task::getType() const{
    return type;
}

int Task::getRobotsNo() const {
    return robotsNo;
}

int Task::getPredictedTime() const {
    return predictedTime;
}
int Task::getId() const {
    return id;
}


time_t Task::getInitTime() const {
    return initTime;
}


//Sets de cada um dos parâmetros

void Task::setType(RobotFunctions::RobotFunction newType){
    type = newType;
}

void Task::setPredictedTime(int newPredictedTime){
    predictedTime = newPredictedTime;
}

void Task::setRobotsNo(int newRobotsNo){
    updatePredictedTime(efficiency,newRobotsNo);
    robotsNo = newRobotsNo;
}


void Task::efficiencyUpdate(int newEfficiency){
    updatePredictedTime(newEfficiency,robotsNo);
}



//Retorna false para quando o PredictedTime da tarefa se torna 0, caso contrário apenas muda os atributos necessários
bool Task::updatePredictedTime(int newEfficiency,int newRobotsNo){
    time_t curTime = time(0);
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
