

#ifndef UNTITLED20_MY_PLAYERMANAGER_H
#define UNTITLED20_MY_PLAYERMANAGER_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library1.h"
#include <iostream>
#include "AVLTree.h"
#include "player.h"
#include "group.h"



class my_playerManager {
public:
    AVLTree<int, group> *group_tree;
    AVLTree<int, player> *players_tree;
    AVLTree<playerKey, playerKey> *level_tree;
    AVLTree<int, full_group> *full_group_tree;
    int all_players;

    my_playerManager() {
        this->players_tree = new AVLTree<int, player>;
        this->group_tree = new AVLTree<int, group>;
        this->level_tree = new AVLTree<playerKey, playerKey>;
        this->full_group_tree = new AVLTree<int, full_group>;
        this->all_players = 0;

    }

    ~my_playerManager() {
        if (this->group_tree) {
            group *g = this->group_tree->getMinInfo();
            this->group_tree->resetNext();
            while (g) {
                player_in_group *p = g->players->getMinInfo();
                g->players->resetNext();

                while (p) {
                    delete p;
                    p = g->players->getNextInfo();
                }
                g->players->deleteTreeWithKey(g->players->getRoot());
                delete g;
                g = this->group_tree->getNextInfo();
            }
            this->group_tree->deleteTree(this->group_tree->getRoot());
            delete this->group_tree;
        }
        if (this->full_group_tree) {
            full_group *gp = this->full_group_tree->getMinInfo();
            this->full_group_tree->resetNext();
            while (gp) {
                delete gp;
                gp = this->full_group_tree->getNextInfo();
            }
            this->full_group_tree->deleteTree((this->full_group_tree->getRoot()));
            delete this->full_group_tree;
        }
        if (this->players_tree) {
            player *pp = this->players_tree->getMinInfo();
            this->players_tree->resetNext();
            while (pp) {
                delete pp;
                pp = this->players_tree->getNextInfo();
            }
            this->players_tree->deleteTree(this->players_tree->getRoot());
            delete this->players_tree;}
        if (this->level_tree) {
            playerKey *pk = this->level_tree->getMinInfo();
            this->level_tree->resetNext();
            while (pk) {
                delete pk;
                pk = this->level_tree->getNextInfo();
            }
            this->level_tree->deleteTreeWithKey(this->level_tree->getRoot());
            delete this->level_tree;
            //delete this;
        }
    }

    StatusType AddGroup(int GroupID) {
        if (GroupID <= 0)
            return INVALID_INPUT;
        group *new_group = new group(GroupID);
        if (this->group_tree->searchForNode(GroupID)) {
            delete new_group;
            return FAILURE;
        }
        this->group_tree->insertNode(GroupID, new_group);

        return SUCCESS;
    }

    StatusType AddPlayer(int PlayerID, int GroupID, int Level) {
        if (PlayerID <= 0 || GroupID <= 0 || Level < 0)
            return INVALID_INPUT;
        if (!this->group_tree->searchForNode(GroupID))
            return FAILURE;
        if (this->players_tree->searchForNode(PlayerID))
            return FAILURE;
        group *group_info = this->group_tree->getNodeInfo(GroupID);
        playerKey *new_playerKey = new playerKey(Level, PlayerID);
        player_in_group *new_p = new player_in_group(Level, PlayerID, group_info);
        group_info->players->insertNode(*new_playerKey, new_p);
        player *new_player = new player(Level, PlayerID, new_p);
        new_p->in_player_tree=new_player;
        this->players_tree->insertNode(PlayerID, new_player);
        this->level_tree->insertNode(*new_playerKey, new_playerKey);
        group_info->players_num++;
        if(group_info->players_num==1) {
            full_group* gp = new full_group(&(*group_info));
            this->full_group_tree->insertNode(GroupID, gp);
        }
        this->all_players++;
        return SUCCESS;
    }

    StatusType RemovePlayer(int PlayerID) {
        if (PlayerID <= 0)
            return INVALID_INPUT;
        if (!this->players_tree->searchForNode(PlayerID))
            return FAILURE;
        player *to_remove = this->players_tree->getNodeInfo(PlayerID);
        int level = to_remove->level;
        playerKey *key_to_remove = new playerKey(level, PlayerID);
        this->level_tree->removeNode(*key_to_remove);
        to_remove->my_player_place->my_group->players->removeNode(*key_to_remove);
        delete key_to_remove;
        this->players_tree->removeNode(PlayerID);
        to_remove->my_player_place->my_group->players_num--;
        if(to_remove->my_player_place->my_group->players_num==0){
            delete this->full_group_tree->getNodeInfo(to_remove->my_player_place->my_group->group_id);
            this->full_group_tree->removeNode(to_remove->my_player_place->my_group->group_id);
        }
        this->all_players--;
        delete to_remove;
        return SUCCESS;
    }

    StatusType IncreaseLevel(int PlayerID, int LevelIncrease) {
        if (PlayerID <= 0 || LevelIncrease <= 0)
            return INVALID_INPUT;
        if (!this->players_tree->doesItExist(PlayerID))
            return FAILURE;
        player *to_increase = this->players_tree->getNodeInfo(PlayerID);
        int level = to_increase->level;
        playerKey *key_to_increase = new playerKey(level + LevelIncrease, PlayerID);
        playerKey *key_to_remove = new playerKey(level, PlayerID);
        delete this->level_tree->getNodeInfo(*key_to_remove);
        this->level_tree->removeNode(*key_to_remove);
        player_in_group *new_p_to_add= new player_in_group(level+LevelIncrease,PlayerID,(to_increase->my_player_place->my_group));
        new_p_to_add->in_player_tree = to_increase;
        to_increase->my_player_place->my_group->players->removeNode(*key_to_remove);
        delete key_to_remove;
        to_increase->my_player_place->my_group->players->insertNode(*key_to_increase, new_p_to_add);
        this->level_tree->insertNode(*key_to_increase, key_to_increase);
        to_increase->level += LevelIncrease;
        return SUCCESS;

    }

    StatusType GetHighestLevel(int GroupID, int *PlayerID) {
        if (PlayerID == NULL || GroupID == 0)
            return INVALID_INPUT;
        if (GroupID < 0) {
            if (this->level_tree->getMaxInfo() == NULL)
                *PlayerID = -1;
            else
                *PlayerID = this->level_tree->getMaxInfo()->id;
            return SUCCESS;
        }

        if (!this->group_tree->doesItExist(GroupID))
            return FAILURE;
        if(this->group_tree->getNodeInfo(GroupID)->players_num==0)
        {
            *PlayerID = -1;
            return SUCCESS;
        }
        *PlayerID = this->group_tree->getNodeInfo(GroupID)->players->getMaxInfo()->player_id;
        return SUCCESS;


    }

    StatusType GetAllPlayersByLevel(int GroupID, int **Players, int *numOfPlayers) {

        if (GroupID == 0 || Players == NULL || numOfPlayers == NULL)
            return INVALID_INPUT;
        if (GroupID < 0) {
            //Players = (int**) malloc(this->all_players * (sizeof(*Players)));

            *Players =(int*) malloc(sizeof(int)*this->all_players);

            this->level_tree->resetPre();
            playerKey *current = this->level_tree->getMaxInfo();
            *numOfPlayers = -1;
            while (current) {
                (*numOfPlayers)++;
                (*Players)[(*numOfPlayers)] = current->id;
                current = this->level_tree->getPreviousInfo();
            }

            (*numOfPlayers)++;
            return SUCCESS;
        }

        if (!this->group_tree->doesItExist(GroupID))
            return FAILURE;
        AVLTree<playerKey, player_in_group> *p = this->group_tree->getNodeInfo(GroupID)->players;
        *Players =(int*) malloc(sizeof(int)*this->all_players);
        p->resetPre();
        player_in_group *current = p->getMaxInfo();
        *numOfPlayers = -1;
        while (current) {
            (*numOfPlayers)++;
            (*Players)[*numOfPlayers] = current->player_id;
            current = p->getPreviousInfo();
        }
        (*numOfPlayers)++;
        return SUCCESS;

    }

    StatusType GetGroupsHighestLevel(int numOfGroups, int **Players) {
        if (numOfGroups < 1 || Players == NULL)
            return INVALID_INPUT;
        //Players = (int**)malloc(numOfGroups * sizeof(*Players));

        *Players =(int*) malloc(sizeof(int)*numOfGroups);

        this->full_group_tree->resetNext();
        full_group *g = this->full_group_tree->getMinInfo();
        int counter = 0;
        while (g) {

            (*Players)[counter] = g->g->players->getMaxInfo()->player_id;
            counter++;
            if (numOfGroups == counter)
                return SUCCESS;
            g = this->full_group_tree->getNextInfo();
        }
        return FAILURE;

    }

    StatusType ReplaceGroup(int GroupID, int ReplacementID) {
        if (GroupID <= 0 || GroupID == ReplacementID || ReplacementID <= 0)
            return INVALID_INPUT;
        if (!this->group_tree->doesItExist(GroupID) || !this->group_tree->doesItExist(ReplacementID))
            return FAILURE;
        group *g1 = this->group_tree->getNodeInfo(GroupID);
        group *g2 = this->group_tree->getNodeInfo(ReplacementID);
        //  int count;
        if(g1->players_num==0)
        {
            this->group_tree->removeNode(GroupID);
            return SUCCESS;
        }
        player_in_group *players1[g1->players_num];
        players1[0]=g1->players->getMinInfo();
        g1->players->resetNext();
        for(int l=1;l<g1->players_num;l++)
            players1[l]=g1->players->getNextInfo();


        player_in_group *players2[g2->players_num];
        players2[0]=g2->players->getMinInfo();
        g2->players->resetNext();
        for(int l=1;l<g2->players_num;l++)
            players2[l]=g2->players->getNextInfo();

        players1[0]=g1->players->getMinInfo();
        //player_in_group *all = new [g1->players_num + g2->players_num] player_in_group;
        player_in_group *all[g1->players_num + g2->players_num];
        int i = 0, j = 0;
        //playerKey *keys=new[g1->players_num + g2->players_num] playerKey;
        playerKey  *keys =(playerKey *) malloc((g1->players_num + g2->players_num)* sizeof(player_in_group));
        while (1) {
            if (i + j == g1->players_num + g2->players_num)
                break;
            else if (i == g1->players_num) {
                playerKey *pk=new playerKey(players2[j]->level,players2[j]->player_id);
                keys[i+j]=*pk;
                all[i + j] = (players2[j]);
                j++;

            } else if (j == g2->players_num) {
                playerKey *pk=new playerKey(players1[i]->level,players1[i]->player_id);
                keys[i+j]=*pk;
                all[i + j] = (players1[i]);
                i++;

            } else if (*(players1[i] )<*( players2[j])) {
                playerKey *pk=new playerKey(players1[i]->level,players1[i]->player_id);
                keys[i+j]=*pk;
                all[i + j] = (players1[i]);
                i++;

            } else {
                playerKey *pk=new playerKey(players2[j]->level,players2[j]->player_id);
                keys[i+j]=*pk;
                all[i + j] = (players2[j]);
                j++;
            }
            //   player_in_group* x=all[i+j-1];
        }
        for (int k=0;k<i+j;k++){
            all[k]->my_group=g2;
            all[k]->in_player_tree->my_player_place= all[k];
        }
        AVLTree<playerKey,player_in_group> *new_tree=new AVLTree<playerKey,player_in_group>();
        new_tree->arrayToAVL(&(all[0]),j+i,keys);
        g2->players-> deleteTree(g2->players->getRoot());
        g2->players=new_tree;
        if(g2->players_num==0) {
            full_group* gp = new full_group(g2);
            this->full_group_tree->insertNode(ReplacementID, gp);
        }
        g2->players_num+=g1->players_num;
        this->full_group_tree->removeNode(GroupID);
        this->group_tree->removeNode(GroupID);
        return SUCCESS;

    }
};

#endif //UNTITLED20_MY_PLAYERMANAGER_H
