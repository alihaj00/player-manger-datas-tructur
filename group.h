
#ifndef UNTITLED20_GROUP_H
#define UNTITLED20_GROUP_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library1.h"
#include <iostream>
#include "AVLTree.h"
#include "player.h"

class player;
class player_in_group;
class playerKey;
class group;
class group{
public:
    AVLTree<playerKey,player_in_group> *players;
    int group_id;
    int players_num;

    group(int id):group_id(id),players_num(0){
        this->players=new AVLTree<playerKey,player_in_group>();
    }
    group(int id, int num,AVLTree<playerKey,player_in_group> *p):group_id(id),players_num(num){
        (this->players)=p;
    }
    group():group_id(0),players_num(0){
        this->players= nullptr;
    }/*
    group(const group& g){
        this->group_id=g.group_id;
        this->players_num=g.players_num;
        AVLTree<playerKey,player_in_group> *p=new AVLTree<playerKey,player_in_group>();
        p=

    }*/
    ~group(){

        players->deleteTree(players->getRoot());
        delete players;
    }



};

class full_group {
public:
    group* g;
    full_group(group* p= nullptr)
    {
        this->g = p;
    }
    ~full_group()= default;

};
#endif //UNTITLED20_GROUP_H
