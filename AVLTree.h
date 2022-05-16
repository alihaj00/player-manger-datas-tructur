#ifndef UNTITLED5_AVLTREE_H
#define UNTITLED5_AVLTREE_H
#include <iostream>
#include <stdlib.h>
#include <exception>
#include <assert.h>
using std::cin;
using std::cout;
using std::endl;
template <typename T, typename S>
class AVLTree{

    struct TreeNode{
        TreeNode *right , *left , *father;
        S* info;
        T key;
        int height;


    public:
        TreeNode(T key , S& info , TreeNode* father = nullptr , TreeNode* left = nullptr , TreeNode* right = nullptr , int height = 0)
        {
            this->key = key;
            this->info = &info;
            this->father = father;
            this->left = left;
            this->right = right;
            this->height = 0;
        }
        ~TreeNode()= default;
        // We start writing the rotation from here :
        void RRrotation() //when we have RR case it means that the right side has more height than left side (by 2) so we have to do a left left rotation.
        {
            if(!this)
                return;
            TreeNode* grandfather = this->father; //We need the father of this in order to rotate
            if(grandfather) //making sure it isn't NULL ( if it is then we're talking about the root)
            {
                if(grandfather->left==this)
                {
                    grandfather->left = this->right;
                }
                else
                    grandfather->right = this->right;
            }
            TreeNode *Temp = this->right;
            Temp->father = this->father;
            this->right = Temp->left;
            this->father = Temp;
            if (Temp->left) {
                Temp->left->father = this;
            }
            Temp->left=this;
            this->heightUpdate();
            this->father->heightUpdate();
        }


        void LLrotation() //In this case it means that the left side has more height than the right side so we have to apply a right right rotation
        {
            if(!this)
                return;
            TreeNode* grandfather = this->father;
            if(grandfather)
            {
                if(grandfather->right == this)
                    grandfather->right = this->left;
                else
                    grandfather->left = this->left;

            }
            TreeNode* Temp = this->left;
            Temp->father = this->father;
            this->left=Temp->right;
            this->father=Temp;
            if (Temp->right)
                Temp->right->father = this;

            Temp->right=this;
            this->heightUpdate();
            this->father->heightUpdate();
        }

        void LRrotation()
        {
            if(!this)
                return;
            this->left->RRrotation();
            this->LLrotation();
        }

        void RLrotation()
        {
            if(!this)
                return;
            this->right->LLrotation();
            this->RRrotation();
        }

        int max(int height1 , int height2) //We use this function in order to compare heights
        {
            return (height1>height2)?height1:height2;
        }


        int getHeightDiff() //Checking the balance of the node (to make sure that out tree is balanced and doesn't need to get rotation)
        {
            int rightH=this->right?this->right->height+1:0;
            int leftH =this->left?this->left->height+1:0;
            return (rightH-leftH);
        }

        //Defining some operators because we're in generic functions
        bool operator>(TreeNode node)
        {
            return this->key>node.key;
        }

        TreeNode operator=(const TreeNode& node)=delete;

        //Other functions that we need for our class
        void heightUpdate() //Calculating the height of the node and updating it , function needed for insert/remove or after rotations
        {
            if(!this)
                return;
            if(this->left==nullptr && this->right==nullptr)
                this->height = 0;
            else if(this->left!=nullptr && this->right==nullptr)
                this->height = 1+(this->left->height);
            else if(this->left==nullptr && this->right!=nullptr)
                this->height = 1+(this->right->height);
            else
                this->height= 1+ max(this->left->height , this->right->height);
        }

        bool correct(TreeNode* root) //we check if the balance is good , if it isn't we give the suitable rotation
        {
            this->heightUpdate();
            int factor=this->getHeightDiff();
            if(factor==-2){
                if(this->left->getHeightDiff()<=-1)
                    this->LLrotation();
                else
                    this->LRrotation();
            }
            else if(factor==2){
                if(this->right->getHeightDiff()<=-1)
                    this->RLrotation();
                else
                    this->RRrotation();
            }
            return this == root;

        }
    };
    TreeNode* root;
    TreeNode* Min;
    TreeNode* Max;
    TreeNode* Next;
    TreeNode* Pre;

public:
    AVLTree()
    {
        this->root = nullptr;
        this->Min = nullptr;
        this->Max = nullptr;
        this->Next = nullptr;
        this->Pre = nullptr ;
    }

    ~AVLTree()
    {
        deleteTree(this->root);

    }

    void deleteTree(TreeNode* node){
        if (!node)
            return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
    void deleteTreeWithKey(TreeNode* node){
        if (!node)
            return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete &(node->key);
        delete node;
    }
    void FindNewMin()
    {
        if(!this->root)
            this->Min=nullptr;
        TreeNode* temp = this->root;
        while(temp->left)
            temp = temp->left;
        this->Min = temp;
    }
    void FindNewMax()
    {
        if(!this->root)
            this->Max=nullptr;
        TreeNode* temp = this->root;
        while(temp->right)
            temp = temp->right;
        this->Max = temp;
    }


    //Finding a node
    TreeNode* searchForNode(T key)  //Finding a node with the given key , could be used to see if a node already exists .
    {
        TreeNode* node = root;
        T Temp;
        while(node!=nullptr)
        {
            Temp = node->key;
            if(key==Temp)
                return node;
            else if(Temp>key)
            {
                node = node->left;
            }
            else
                node = node->right;
        }
        return nullptr;
    }

    //Needed functions in order to insert or remove a node
    TreeNode* findLinker(T key)  //This function is used to find where to insert the new node and returns the father for an easy linking
    {
        TreeNode* temp = this->root;
        TreeNode* dad=nullptr;
        while(temp!=nullptr)
        {
            if(key>temp->key)
            {
                dad = temp;
                temp = temp->right;
            }
            else if(temp->key>key)
            {
                dad = temp;
                temp = temp->left;
            }
            else
                return nullptr; //it means that a node with the given key already exists
        }
        return dad;
    }

    void Rebalance(TreeNode* newNode)
    {
        TreeNode* temp = newNode;
        while(temp!=nullptr)
        {
            if(temp->correct(this->root) && this->root->father!=nullptr)
            {
                this->root=this->root->father;
            }
            temp = temp->father;
        }
    }

    TreeNode* replace(TreeNode* node)  //We look for a replacement for a node that we're going to remove , so this function is built mainly for removing process
    {
        TreeNode* replacement=node->left;
        if(!replacement){
            return nullptr;
        }
        TreeNode* next=replacement->right;
        while(next!= nullptr){
            replacement=replacement->right;
            next=replacement->right;
        }
        return replacement;
    }

    void deleteRoot()
    {
        TreeNode* replacement = replace(this->root);
        if(!replacement && !this->root->right) //in this case it means our Tree has a root only , so we simply delete it and update our parameters such as min and max without worrying about balance
        {
            delete this->root;
            this->root = nullptr;
            this->Max = nullptr;
            this->Min=nullptr;
            return;
        }
        else if(!replacement) //This case means that it has a right son but not a left son
        {
            replacement=this->root->right;
            replacement->father=nullptr;
            delete this->root;
            this->root=replacement;
            Rebalance(replacement);
            FindNewMin();
            FindNewMax();
            return;
        }
        else if(replacement->father==this->root){
            replacement->father=nullptr;
            replacement->right=this->root->right;
            if(this->root->right) {
                this->root->right->father=replacement;
            }
            delete this->root;
            this->root=replacement;
            Rebalance(replacement);
            FindNewMax();
            FindNewMin();
            return;
        }
        else{
            TreeNode* temp=replacement->father;
            temp->right=(replacement->left);
            if(replacement->left)
                replacement->left->father=temp;
            replacement->father=nullptr;
            replacement->left=this->root->left;
            replacement->right=this->root->right;
            if(this->root->right)
                this->root->right->father=replacement;
            if(this->root->left)
                this->root->left->father=replacement;
            if(this->root->right) {
                this->root->right->father=replacement;
            }
            delete this->root;
            this->root= replacement;
            replacement->heightUpdate();
            Rebalance(temp);
            FindNewMin();
            FindNewMax();
            return;
        }
        return;
    }
    void FirstCaseRemove(TreeNode* dad , TreeNode* replacement , TreeNode* node) //if there isn't a left son we replace it with the right one
    //in case it doesn't exist we simply delete the node
    {
        if(dad->left==node){
            if(!node->right){
                delete node;
                dad->left=nullptr;
                Rebalance(dad);
                return;
            }
            dad->left=node->right;
            node->right->father=dad;
            delete node;
        }
        else if(dad->right==node){
            if(!node->right){
                delete node;
                dad->right=nullptr;
                Rebalance(dad);
                return;
            }
            dad->right=node->right;
            node->right->father=dad;
            delete node;
        }
        Rebalance(dad);
    }

    void SecondCaseRemove(TreeNode* dad , TreeNode* replacement , TreeNode* node) //if the left son of the node we want to delete doesn't have a right son
    // we replace it with its left son
    {
        if(node==dad->left){
            dad->left=replacement;
        }
        if(node==dad->right){
            dad->right=replacement;
        }
        replacement->father=dad;
        replacement->right=node->right;
        if(node->right) {
            node->right->father=replacement;
        }
        delete node;
        replacement->heightUpdate();
        Rebalance(dad);
    }

    void LastCaseRemove(TreeNode* dad , TreeNode* replacement , TreeNode* node) //if the right son of the left son exists we replace the left son with the right son
    {
        TreeNode* replacementDad=replacement->father;
        if(node==dad->left){
            dad->left=replacement;
        }
        if(node==dad->right){
            dad->right=replacement;
        }
        replacement->father=dad;
        replacementDad->right=(replacement->left);
        if(replacement->left)
            replacement->left->father=replacementDad;
        replacement->left=node->left;
        replacement->right=node->right;
        if(replacement->right) {
            node->right->father=replacement;
        }
        node->left->father=replacement;
        delete node;
        replacement->heightUpdate();
        Rebalance(replacementDad);
    }

    //Inserting a node
    void insertNode(T newKey , S* newInfo)
    {
        if(this->root == nullptr)
        {
            auto newNode = new TreeNode(newKey,*newInfo,nullptr,nullptr,nullptr,0);
            this->root = newNode;
            this->Min = newNode;
            this->Max = newNode;
            return;
        }
        if(searchForNode(newKey))  //It means the node already exists
            return;
        auto newNode = new TreeNode(newKey,*newInfo,nullptr,nullptr,nullptr,0);
        TreeNode* temp = findLinker(newKey);
        if(temp->key>newKey) {
            temp->left = newNode;
            newNode->father = temp;
        }
        else if(temp->key<newKey) {
            temp->right = newNode;
            newNode->father = temp;
        }
        Rebalance(newNode);
        if(newKey>this->Max->key)
            this->Max = newNode;
        if(newKey<this->Min->key)
            this->Min = newNode;
    }

    //Removing a node
    void removeNode(T key) //We have 4 cases , so we made 4 functions that deal with each case
    {
        if(!this->root)
            return;
        if(this->root->key == key) {
            deleteRoot();
            return;
        }
        TreeNode* node = searchForNode(key);
        if(!node)
            return;
        TreeNode* replacement = replace(node);
        TreeNode* dad = node->father;
        if(!replacement)
        {
            FirstCaseRemove(dad ,replacement , node);
            FindNewMax();
            FindNewMin();
            return ;
        }
        else if(replacement->father==node)
        {
            SecondCaseRemove(dad,replacement,node);
            FindNewMin();
            FindNewMax();
            return;
        }
        else{
            LastCaseRemove(dad,replacement,node);
            FindNewMin();
            FindNewMax();
            return;
        }
    }

    //Print function

    void InOrder()
    {
        printFromCertainNode(this->root);

    }

    void printFromCertainNode(TreeNode* node)
    {
        if(!node)
            return;
        printFromCertainNode(node->left);
        std::cout<< node->key <<std::endl;
        printFromCertainNode(node->right);
    }

    //Additional Functions that help us

    bool doesItExist(T key)
    {
        TreeNode* node = searchForNode(key);
        if(!node)
            return 0;
        return 1;
    }

    S* getNodeInfo(T key)
    {
        TreeNode* node=searchForNode(key);
        if(!node)
            return nullptr;
        return node->info;
    }

    S* getMaxInfo()
    {
        if(!this->Max)
            return nullptr;
        return this->Max->info;
    }

    S* getMinInfo()
    {
        if(!this->Min)
            return nullptr;
        return this->Min->info;
    }

    TreeNode* getRoot()
    {
        if(this->root)
            return nullptr;
        return this->root;
    }

    bool  is_empty(){
        if(!this->root)return true ;
        return false ;
    }

    S** getInfoInOrder(int numOfClasses , int count)
    {
        // must we delete the array ? after finish ?
        // must init the count at start ? ==>
        count = 0 ;

        if(numOfClasses<=0)
            return nullptr;
        S** array = new S*[numOfClasses];
        for(int k=0;k<numOfClasses;k++)
            array[k] = new S;
        int i=0;
        TreeNode* current = this->Min;
        TreeNode* previous = current->father;
        while(current!=this->Max)
        {
            array[i] = (current->info);
            S* x=array[i];
            i++;
            (count)++;
            x=array[1];
            if(count == numOfClasses)
                return array;
            // if you check all situations ?
            if(!(current->left) && !(current->right) && previous->left == current) //We are at the left son , we go to its father
            {
                current = previous;
                previous = previous->father;
            }
            else if(!(current->right) && previous->right==current) //We are at the right son , so we move to the grandparent
            {
                current = previous;
                previous = current->father;
                while(previous->right==current)
                {
                    current = previous;
                    previous = previous ->father;
                }
                current = previous;
            }
            else if(current->right) //if we are at a parent , it means we came from the left so we move to the smallest leaf in the right side
            {
                current = current->right;
                previous = current->father;
                while(current->left)
                {
                    current = current->left;
                }
                previous = current ->father;
            }else if(current->left&& !current->right && previous->left==current)
            {
                current = previous;
                previous = current->father;
            }
            previous = current->father;

        }
        array[i] = current->info;
        (count)++;
        return array;
    }

    void resetNext() {
        this->Next = this->Min;
    }
    S* getNextInfo() // we start from minimum and save our current place in Next
    {
        if(!this->Next)return nullptr;
        if(this->Next == this->Max)
            return nullptr;
        TreeNode* current = this->Next;
        TreeNode* previous = current->father;
        if(!(current->left) && !(current->right) && previous->left == current) //We are at the left son , we go to its father
        {
            current = previous;
            previous = previous->father;
        }
        else if(!(current->right) && previous->right==current) //We are at the right son , so we move to the grandparent
        {
            current = previous;
            previous = current->father;
            while(previous->right==current)
            {
                current = previous;
                previous = previous ->father;
            }
            current = previous;
        }
        else if(current->right) //if we are at a parent , it means we came from the left so we move to the smallest leaf in the right side
        {
            current = current->right;
            previous = current->father;
            while(current->left)
            {
                current = current->left;
            }
            previous = current ->father;
        }
        else if(current->left&& !current->right && previous->left==current)
        {
            current = previous;
            previous = current->father;
        }
        this->Next = current;
        return current->info;
    }

    TreeNode* arrayToAVLHelp(S* arr[],int start, int end,int max,T* keys)
    {

        if (start > end)
            return NULL;
        int mid = (start + end)/2;
        auto newroot = new TreeNode(keys[mid],**(arr+mid), nullptr,nullptr,nullptr,0);
        if(mid==0)
            this->Min = newroot;
        if(mid== max)
            this->Max = newroot;
        newroot->left = arrayToAVLHelp(arr, start,mid - 1,max,keys);
        newroot->right = arrayToAVLHelp(arr, mid + 1, end,max,keys);
        if(newroot->left){
            newroot->left->father = newroot;
        }
        if(newroot->right){
            newroot->right->father = newroot;
        }

        return newroot;
    }

    void arrayToAVL(S* arr[], int size,T* keys) //we make an array into an AVL tree with O(numOfClasses)
    {
        this->root = arrayToAVLHelp(arr,0,size-1,size-1,keys);
        this->resetNext();
        this->resetPre();
    }

    void resetPre()
    {
        this->Pre = this->Max;
    }
    S* getPreviousInfo()  //We start from Max and walk to Min
    {
        if(!this->Pre)return nullptr;
        if(this->Pre == this->Min)
            return nullptr;
        TreeNode* current = this->Pre;
        TreeNode* previous = current->father;
        if(!(current->left) && !(current->right) && previous->right == current) //if we're right son we go to father
        {
            current = previous;
            previous = previous->father;
        }
        else if(!(current->left)  && previous->left==current) //if we're a dad of one(from right) or less we move up until we are no more in the left side
        {
            current = previous;
            previous = current->father;
            while(previous->left==current)
            {
                current = previous;
                previous = previous ->father;
            }
            current = previous;
        }
        else if(current->left) //if we are at a parent , it means we came from the right so we move to the biggest leaf in the left side
        {
            current = current->left;
            previous = current->father;
            while(current->right)
            {
                current = current->right;
            }
            previous = current ->father;
        }
        else if(current->right && !current->left && previous->right==current)
        {
            current = previous;
        }
        this->Pre = current;
        return current->info;
    }

};
#endif //UNTITLED5_AVLTREE_H
