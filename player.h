

#ifndef UNTITLED20_PLAYER_H
#define UNTITLED20_PLAYER_H
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library1.h"
#include <iostream>
#include "AVLTree.h"
#include "group.h"


class player;
class player_in_group;
class playerKey;
class group;



class player {
public:
    int level;
    int player_id;
    player_in_group *my_player_place;

    player(int lvl,int id, player_in_group *p =NULL):level(lvl),player_id(id){
        (this->my_player_place)=p;
    }

    ~player()=default;

};
class player_in_group{
public:
    int level;
    int player_id;
    group *my_group;
    player *in_player_tree;

    player_in_group(int lvl,int id, group *g =NULL,player *p=NULL):level(lvl),player_id(id){
        (this->my_group)=g;
        this->in_player_tree=p;
    }
    player_in_group():level(0),player_id(0){
        my_group=nullptr;
        in_player_tree = nullptr;
    }
    /*
    player_in_group():level(0),player_id(0){
        this->my_group=nullptr;
    }    player_in_group(const player_in_group& p){
        this->level=p.level;
        this->player_id=p.player_id;
        group *new_g= new group()
        *this->my_group=nullptr;
    }
*/

    ~player_in_group()=default;
    bool operator<(player_in_group p){

        if (this->level<p.level)
            return 1;
        else if (this->level>p.level)
            return 0;
        else {
            return this->player_id>p.player_id? 1:0;
        }
    }

    bool operator>(player_in_group p){
        if (this->level<p.level)
            return 0;
        else if (this->level>p.level)
            return 1;
        else {
            return this->player_id>p.player_id? 0:1;
        }

    }


    bool operator==(player_in_group p){
        if ((this->level==p.level)&&(this->player_id==p.player_id))
            return 1;
        return 0;
    }
};



class playerKey{
public:
    int level;
    int id;
    playerKey(int lvl,int id):level(lvl),id(id){}
    playerKey():level(0),id(0){}
    /*
    playerKey(const playerKey& p){
        this->level=p.level;
        this->id=p.id;
    }*/
    ~playerKey()=default;
    bool operator<(playerKey p){
        if (this->level<p.level)
            return 1;
        else if (this->level>p.level)
            return 0;
        else {
            return this->id>p.id? 1:0;
        }
    }
    bool operator>(playerKey p){
        if (this->level<p.level)
            return 0;
        else if (this->level>p.level)
            return 1;
        else {
            return this->id>p.id? 0:1;
        }
    }

    bool operator==(playerKey p){
        if ((this->level==p.level)&&(this->id==p.id))
            return 1;
        return 0;
    }
};
#endif //UNTITLED20_PLAYER_H
