#include "header/RobotsManagement.hpp"
#include "header/Task.hpp"
#include "header/VillageStats.hpp"
#include <vector>
#include <map> 
#include <ctime>
#include <iostream>

//Inicialização e destruição da classe

RobotsManagement::RobotsManagement(){
    initializeStats();
}

void RobotsManagement::initializeStats(){
    totRobots = 1;
    freeRobots = 1;
    efficiency = 1;
    prodCost = 1;
    villageStats = NULL;
    tasks = NULL;
}

RobotsManagement::~RobotsManagement(){}



//Gets de cada um dos parâmetros

int RobotsManagement::getTotRobots(){
    return totRobots;
}

int RobotsManagement::getFreeRobots(){
    return freeRobots;
}

int RobotsManagement::getEfficiency(){
    return efficiency;
}

int RobotsManagement::getProdCost(){
    return prodCost;
}


//Sets de cada um dos parâmetros

void RobotsManagement::setTotRobots(int newTotRobots){
    totRobots = newTotRobots;
}

void RobotsManagement::setFreeRobots(int newFreeRobots){
    freeRobots = newFreeRobots;
}

void RobotsManagement::setEfficiency(int newEfficiency){
    efficiency = newEfficiency;
}

void RobotsManagement::setProdCost(int newProdCost){
    prodCost = newProdCost;
}

void RobotsManagement::setVillageStats(VillageStats *newVillageStats){
    villageStats = newVillageStats;
}

void RobotsManagement::setTasks(std::vector<std::map<time_t,Task>> *newTasks){
    tasks = newTasks;
}



bool RobotsManagement::createRobot(){
    int curResources = villageStats->getResources();
    
    //Calcula se existe dinheiro o suficiente para criar um robô
    if(curResources >= prodCost){
        //Decrementa dos recursos o custo de produção do robô
        villageStats->setResources(curResources - prodCost);
        //Incrementa o número total e o número de robôs livres    
        totRobots++;
        freeRobots++;
        return true;
    }
    return false;
}

bool RobotsManagement::destroyRobot(){
    //Verifica se é possível destruir robôs
    if(totRobots && freeRobots){
        //Decrementa o número total e o número de robôs livres    
        totRobots--;
        freeRobots--;
        return true;
    }
    return false;
}

bool RobotsManagement::createTask(RobotFunctions funct){
    time_t curTime = time(0);

    //Verifica se ainda não foi adicionado nenhuma chave com o valor de time(0) no map da RobotFunction funct
    if(tasks->at(funct).find(curTime) == tasks->at(funct).end()){
        //Caso não tenha sido, adiciona uma Task vazia no map
        Task newTask(funct,curTime,&efficiency);
        tasks->at(funct).insert({curTime,newTask});
        return true;
    } 
    return false;
}

bool RobotsManagement::moveRobot(RobotFunctions funct,time_t id,int robotNo){
    if(!robotNo) return true; //Mover 0 robos para a task não muda nada
    if(robotNo > 0 && freeRobots - robotNo <= 0) return false; // Não é possível adicionar mais robôs do que se tem livre
    else if(robotNo < 0 && tasks->at(funct).at(id).getRobotsNo() + robotNo < 0) return false; //Não é possível remover mais robos de uma task do que ela possui
    else if(tasks->at(funct).find(id) == tasks->at(funct).end()) return false; //Não é possível mover robôs para uma task inexistente

    //Decrementa os robôs livres
    freeRobots -= robotNo;

    //Move ou remove robotNo robôs da task
    int oldRobotsNo = tasks->at(funct).at(id).getRobotsNo();
    tasks->at(funct).at(id).setRobotsNo(oldRobotsNo + robotNo);

    return true;
}