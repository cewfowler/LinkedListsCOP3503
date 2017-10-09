//
//  main.cpp
//  pa2
//
//  Created by Christian on 10/20/16.
//  Copyright © 2016 Christian. All rights reserved.
//


#include <iostream>
#include "pa2.h"
#include <math.h>



LinkedList::LinkedList(bool bestFit){
    //initializes a node with blank properties
    head->numPages = 32;
    head->name = "Free";
    head->size = 0;
    head->next = NULL;
    
    
    //declares if it is a best or worst fit algorithm
    this->bestFit = bestFit;
    
}


int LinkedList::numberOfFragments(){
    node * q = head->next;
    int numberFrags = 1;
    
    while (q){
        if (q->size == 0){
            if (q->next != NULL){
                numberFrags++;
            }
        }
        q = q->next;
    }
    
    return numberFrags;
}


bool LinkedList::insertNode(std::string name, int size){
    //creates a new node from user input
    node * newNode = new node;
    newNode->name = name;
    newNode->size = size;
    newNode->numPages = (int)ceil(size / 3.0);
    newNode->next = NULL;
    
    bool didComplete;
    
    //sets location to insert the node
    if (bestFit){
        didComplete = useBestFit(newNode);
    }
    else {
        didComplete = useWorstFit(newNode);
    }
    
    return didComplete;
}


bool LinkedList::removeNode(std::string name){
    node * q = head;

    
    node * beforeQ = head;

    
    while (q){
        if (q->name == name){
            //sets q size to 0, initial value
            q->size = 0;
            q->name = "Free";
            
            //consolidates free space with node in front of q
            if (beforeQ->size == 0 && beforeQ != q){
                node * newQ = consolidate(beforeQ, q);
                
                //consolidates free space with node in back of q if first two were consolidated
                if (newQ->next != NULL){
                    if (newQ->next->size == 0){
                        consolidate(newQ, newQ->next);
                    }
                }
            }
            
            //consolidates free space with node in back of q
            if (q->next != NULL){
                if (q->next->size == 0){
                    consolidate(q, q->next);
                }
            }
            std::cout << "Program " << name << " successfully removed.";
            return true;
        }
        beforeQ = q;
        q = q->next;
    }
    std::cout << "Unable to find the program to remove!\n";
    
    return false;
}

node * LinkedList::consolidate(node * beforeQ, node * q){
    //if node in front of node being deleted is free space, consolidates them

    beforeQ->numPages = beforeQ->numPages + q->numPages;
    beforeQ->next = q->next;
    q->next = NULL;
    
    return beforeQ;
}



void LinkedList::printListOf32Pages() {
    node * q = head;
    int count = 1;
    
    while (q){
        //prints the names of the files
        for (int i = 0; i < q->numPages; i++ ){
            std::cout << q->name << "\t";
            
            //prints new line after 8
            if (count >= 8){
                count = 0;
                std::cout << "\n";
            }
            count++;
        }
        
        q = q->next;
    }
}


bool LinkedList::useBestFit(node * newNode){
    //makes sure list isn't full
    bool full = true;
    
    //will hold numPages of worst fit
    int leastNumPages = 32;
    int currentNumPages = 0;
    
    //start node for worstFit
    node * bestFitHead = head;
    node * bestFitTail = head;
    
    //temp variables to hold possible new worst-fit values
    node * possibleBestFitTail = head;
    node * possibleBestFitHead = head;
    node * q = head;
    
    while (q) {
        //if size is 0, adds numPages to currentNumPages
        if (q->size == 0){
            currentNumPages += q->numPages;
            possibleBestFitTail = q;
            full = false;
            
            //if currentNumPages is less than leastNumPages, sets value to currentNumPages and sets start node to new node
            if (currentNumPages < leastNumPages && currentNumPages > newNode->numPages){
                leastNumPages = currentNumPages;
                bestFitHead = possibleBestFitHead;
                bestFitTail = possibleBestFitTail;
            }
        }
        
        //if there is a next node and its size is not 0, resets hold values
        if (q->next != NULL){
            if (q->next->size != 0){
                currentNumPages = 0;
                possibleBestFitHead = q->next;
            }
        }
        
        q = q->next;
    }
    
    if (!full){
        return insert(newNode, bestFitTail, bestFitHead);
    }
    
    return false;

}


bool LinkedList::useWorstFit(node * newNode){
    //makes sure list isn't full
    bool full = true;
    
    //will hold numPages of worst fit
    int mostNumPages = 0;
    int currentNumPages = 0;
    
    //start node for worstFit
    node * worstFitHead = head;
    node * worstFitTail = head;
    
    //temp variables to hold possible new worst-fit values
    node * possibleWorstFitTail = head;
    node * possibleWorstFitHead = head;
    node * q = head;
    
    while (q) {
        //if size is 0, adds numPages to currentNumPages
        if (q->size == 0){
            currentNumPages += q->numPages;
            possibleWorstFitTail = q;
            full = false;
        }
        
        //if currentNumPages is greater than mostNumPages, sets value to currentNumPages and sets start node to new node
        if (currentNumPages > mostNumPages && currentNumPages > newNode->numPages){
            mostNumPages = currentNumPages;
            worstFitHead = possibleWorstFitHead;
            worstFitTail = possibleWorstFitTail;
        }
        
        //if there is a next node and its size is not 0, resets hold values
        if (q->next != NULL){
            if (q->next->size != 0){
                currentNumPages = 0;
                possibleWorstFitHead = q->next;
            }
        }
        
        q = q->next;
    }
    
    if (!full){
        return insert(newNode, worstFitTail, worstFitHead);
    }
    
    return false;
}


bool LinkedList::insert(node * newNode, node * oldNode, node * headNode){
    
    //returns true if node is successfully added, false otherwise
    //checks if it is being inserted at the head
    if (oldNode == head && newNode->numPages <= oldNode->numPages){
        head = newNode;
        
        //sets the numPages of old node to be the removal of the numPages of the new node
        oldNode->numPages = oldNode->numPages - newNode->numPages;
        
        //if oldNode still has pages, sets it to next node, otherwise, sets oldNode's next to newNode's next
        if (oldNode->numPages == 0){
            head->next = oldNode->next;
        }
        else {
            head->next = oldNode;
        }
        
        std::cout << "Program " << newNode->name << " successfully added: " << newNode->numPages << " page(s) used.\n";
        return true;
    }
    
    //checks if the new node can be entered and then enters it
    else if (newNode->numPages <= oldNode->numPages){
        
        //sets the next node to the new node
        headNode->next = newNode;
        
        //sets the numPages of old node to be the removal of the numPages of the new node
        oldNode->numPages = oldNode->numPages - newNode->numPages;
        
        //if oldNode still has pages, sets it to next node, otherwise, sets oldNode's next to newNode's next
        if (oldNode->numPages == 0){
            newNode->next = oldNode->next;
        }
        else {
            newNode->next = oldNode;
        }
        
        std::cout << "Program " << newNode->name << " successfully added: " << newNode->numPages << " page(s) used.\n";
        return true;
    }
    
    std::cout << "Unable to add program " << newNode->name << ".\n";
    return false;
}




int userInputNumber() {
    //gets number from user input
    double hold;
    int size;
    
    //accepts input
    std::cin >> hold;
    
    //erases any string
    std::cin.clear();
    std::cin.ignore(1000,'\n');
    
    //checks if user entered a decimal
    if (hold != (int)hold){
        printf("You entered a decimal...\n");
        return -1;
    }
    
    size = (int) hold;
    return size;
}


int main(int argc, const char * argv[]) {
    
    std::string bestOrWorst = "worst";
    LinkedList L = LinkedList(false);

    //checks arguments in the command line
    std::string *cppArgs = new std::string[argc];
    for (int i=0; i != argc; ++i) {
        cppArgs[i] = std::string(argv[i]);
    }
    
    if (argc == 2) {
        //checks if user entered "worst" argument into command line
        if (cppArgs[1] == "worst"){
            L = new LinkedList(false);
            bestOrWorst = "worst";
        }
        
        //checks if user entered "best" argument into command line
        else if (cppArgs[1] == "best"){
            L = new LinkedList(true);
            bestOrWorst = "best";
        }
        
        //checks if user entered anything else; returns an error
        else {
            throw std::invalid_argument("That is not a valid argument!");
        }
    }
    
    int choice = 1;
    
    std::cout << "Using " << bestOrWorst << " fit algorithm.\n";
    
    while (choice != 5){
        
        std::cout << "\n1. Add program.\n";
        std::cout << "2. Kill program.\n";
        std::cout << "3. Fragmentation.\n";
        std::cout << "4. Print memory.\n";
        std::cout << "5. Quit.\n";
           
        choice = userInputNumber();
        if (choice > 0 && choice < 6 ){
            std::cout << "Choice = " << choice << "\n";
        }
        
        switch(choice){
                
            case 1: {
                std::string name;
                int size;
                
                std::cout << "Program name: ";
                std::cin >> name;
                std::cout << "Program size (KB): ";
                
                size = userInputNumber();
                if (size > 0 && size <= 96){
                    L.insertNode(name, size);
                }
                else {
                    std::cout << "Invalid size.\n";
                }
                break;
            }
                
            case 2: {
                std::string name;
                
                std::cout << "Program name: ";
                std::cin >> name;
                L.removeNode(name);
                break;
            }
                
            case 3: {
                std::cout << "There is/are " << L.numberOfFragments() << " fragment(s).\n";
                break;
            }
                
            case 4: {
                L.printListOf32Pages();
                break;
            }
                
            case 5: {
                std::cout << "Exiting the program.\n";
                break;
            }
                
            default: {
                std::cout << "Invalid option.\n";
            }
        }

    }

    return 0;
}
