

#include "library1.h"
#include <iostream>
#include "AVLTree.h"
#include "player.h"
#include "my_playerManager.h"

void *Init(){
    my_playerManager *DS = new my_playerManager();
    return DS;}

StatusType AddGroup(void *DS, int GroupID) {
    if(!DS)
        return INVALID_INPUT;
    return ((my_playerManager *)DS)->AddGroup(GroupID);
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int Level) {
    if(!DS)
        return INVALID_INPUT;
    return ((my_playerManager *)DS)->AddPlayer(PlayerID,GroupID,Level);
}

StatusType RemovePlayer(void *DS, int PlayerID){
    if(!DS)
        return INVALID_INPUT;
    return ((my_playerManager *)DS)->RemovePlayer(PlayerID);
}

StatusType ReplaceGroup(void *DS, int GroupID, int ReplacementID){
    if(!DS)
        return INVALID_INPUT;
    return ((my_playerManager *)DS)->ReplaceGroup(GroupID,ReplacementID);
}

StatusType IncreaseLevel(void *DS, int PlayerID, int LevelIncrease){
    if(!DS)
        return INVALID_INPUT;
    return ((my_playerManager *)DS)->IncreaseLevel(PlayerID,LevelIncrease);
}


StatusType GetHighestLevel(void *DS, int GroupID, int *PlayerID){
    if(!DS)
        return INVALID_INPUT;
    return ((my_playerManager *)DS)->GetHighestLevel(GroupID,PlayerID);
}

StatusType GetAllPlayersByLevel(void *DS, int GroupID, int **Players, int *numOfPlayers) {
    if(!DS)
        return INVALID_INPUT;
    return ((my_playerManager *)DS)->GetAllPlayersByLevel(GroupID,Players,numOfPlayers) ;
}

StatusType GetGroupsHighestLevel(void *DS, int numOfGroups, int **Players){
    if(!DS)
        return INVALID_INPUT;
    return ((my_playerManager *)DS)->GetGroupsHighestLevel(numOfGroups,Players);
}

void Quit(void** DS) {
    if(DS== nullptr||*DS== nullptr)
        return;
    my_playerManager * tempDS= static_cast<my_playerManager *>(*DS);
    delete(tempDS);
    *DS= nullptr;
}




