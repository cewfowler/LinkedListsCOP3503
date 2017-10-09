//
//  pa2.h
//  pa2
//
//  Created by Christian on 10/20/16.
//  Copyright © 2016 Christian. All rights reserved.
//
#ifndef LinkedList_h
#define LinkedList_h


#ifndef pa2_h
#define pa2_h


#endif /* pa2_h */


//declares node struct to be used by LinkedList
struct node {
    std::string name;
    int size;
    int numPages;
    
    // used to create the link to the next object in the LinkedList
    node * next;
};

class LinkedList {
private:
    //first node and length of LinkedList
    bool bestFit;
    int numPages;
    node * head = new node;
    

public:
    //default constructor to create the head node
    LinkedList(bool fit);
    
    //used to insert a new node
    bool insertNode(std::string name, int size);
    
    //removes a node
    bool removeNode(std::string name);
    
    //prints LinkedList
    void printListOf32Pages();
    
    //methods for calculating position of best and worst fit
    bool useBestFit(node * newNode);
    bool useWorstFit(node * newNode);
    
    //checks if the node can be input at the position
    bool insert(node * newNode, node * oldNode, node * headNode);
    
    node * consolidate(node * beforeQ, node * q);
    
    int numberOfFragments();
    
};



#endif /* LinkedList_h */
