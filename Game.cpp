//
//  game.cpp
//  eecs281project2
//
//  Created by Andrew Bennett-Belch on 5/15/17.
//  Copyright © 2017 Andrew Bennett-Belch. All rights reserved.
//


#include "Game.h"
#include "P2random.h"
#include <string>
#include <sstream>
using namespace std;
Game::Game() {
    Configuration();
}

Zombie::Zombie() {
    name = " ";
    distance = 0;
    speed = 0;
    health = 0;
    roundsactive = 0;
}

Configuration::Configuration() {
    quiverCapacity = 0;
    yourHealth = 0;
    randomSeed = 0;
    maxDistance = 0;
    maxSpeed = 0;
    maxHealth = 0;
}


void Game::playGame(std::ostringstream &os, Eecs281PQ<Zombie *, Comparator> *pq,
                    Eecs281PQ<Zombie*,MostActive> *pq2,
                    Eecs281PQ<Zombie*,LeastActive> *pq3,
                    bool vpresent) {
    
    //input junk
    string comment = " ";
    
    
    int round = 0;
    int nextaddround = 0;
    size_t currentsize = Spawnorder.size();
    
    //number of random and named zombies to create
    int numrandom = 0;
    int numnamed = 0;
    int numarrows = 0;
    
    int zombieshuffle = 0;
    int movement = 0;
    size_t indexeaten = -1;
    Zombie *lastzombiekilled = nullptr;
    
    
    P2random::srand(config.randomSeed, config.maxDistance, config.maxSpeed, config.maxHealth);
    
    //read in until all zombies are dead
    while (true) {
        round += 1;
        
        if ((round > nextaddround)) {
            if (!(cin >> comment >> comment >> nextaddround)) {
                if (pq->empty()) {
                    round--;
                    break;
                }
                else {
                    nextaddround = 0;
                    numrandom = 0;
                    numnamed = 0;
                }
            }
            else if (nextaddround == round) {
                cin >> comment >> numrandom >> comment >> numnamed;
            }
            else {
                numrandom = 0;
                numnamed = 0;
            }
        }
        else if (round == nextaddround) {
            cin >> comment >> numrandom >> comment >> numnamed;
        }
        
        else {
            numrandom = 0;
            numnamed = 0;
        }
        
        
        
        
        
        
        
        if (vpresent) {
                os << "Round: " << round << "\n";
        }
        
        numarrows = config.quiverCapacity;
        //update zombies
        currentsize = Spawnorder.size();
        for (size_t i = 0; i < currentsize; i++) {
            if (Spawnorder[i]->health > 0) {
                zombieshuffle = (int)((P2random::rand() % Spawnorder[i]->speed) / 2);
                movement = Spawnorder[i]->speed + zombieshuffle;
                Spawnorder[i]->distance = max(0, Spawnorder[i]->distance - movement);
                if (vpresent) {
                    os << "Moved: " << Spawnorder[i]->name << " (distance: " << Spawnorder[i]->distance << ", speed: " << Spawnorder[i]->speed << ", health: " << Spawnorder[i]->health << ")\n";
                }
                if ((Spawnorder[i]->distance == 0) && (config.yourHealth != 0)) {
                    config.yourHealth--;
                    if (config.yourHealth == 0) {
                        indexeaten = i;
                    }
                }
                
            }
        }
        //create random zombies
        for (size_t i = 0; i < (size_t)numrandom; i++) {
            currentsize = Spawnorder.size();
            Zombie *z = new Zombie;
            z->name = P2random::getNextZombieName();
            z->distance = P2random::getNextZombieDistance();
            z->speed = P2random::getNextZombieSpeed();
            z->health = P2random::getNextZombieHealth();
            Spawnorder.push_back(z);
            pq->push(z);
            pq2->push(z);
            pq3->push(z);
            if (vpresent) {
                os << "Created: " << z->name << " (distance: "
                    << z->distance << ", speed: " << z->speed << ", health: " << z->health << ")\n";
            }
        }
        //create named zombies
        for (size_t i = 0; i < (size_t)numnamed; i++) {
            currentsize = Spawnorder.size();
            Zombie *z = new Zombie;
            cin >> z->name >> comment >> z->distance >> comment >> z->speed >> comment >> z->health;
            Spawnorder.push_back(z);
            pq->push(z);
            pq2->push(z);
            pq3->push(z);
            if (vpresent) {
                os << "Created: " << z->name << " (distance: "
                    << z->distance << ", speed: " << z->speed << ", health: "
                    << z->health << ")\n";
            }
        }
        
        for (size_t i = 0; i < Spawnorder.size(); i++) {
            if (Spawnorder[i]->health > 0) {
                Spawnorder[i]->roundsactive++;
            }
        }
        
        //if you died break
        if (config.yourHealth == 0) {
            break;
        }
        
        pq->updatePriorities();
        pq2->updatePriorities();
        pq3->updatePriorities();

        //shoot zombies until you run out of arrows
        while ((numarrows > 0) && (!pq->empty())) {
            Zombie *z = pq->top();
            while ((numarrows > 0) && z->health > 0) {
                z->health--;
                numarrows--;
            }
            if (z->health == 0) {
                pq->pop();
                Killorder.push_back(z);
                lastzombiekilled = z;
            }
        }
        
    }
    pq2->updatePriorities();
    pq3->updatePriorities();
    if (config.yourHealth == 0) {
        os << "DEFEAT IN ROUND " << round << "! " << Spawnorder[indexeaten]->name << " ate your brains!\n";
    }
    else {
        os << "VICTORY IN ROUND " << round << "! " <<
            lastzombiekilled->name << " was the last zombie. You survived with "
            << config.yourHealth << " health left.\n";
    }
    
    return;
}


void Game::statistics(ostringstream &os, Eecs281PQ<Zombie *, Comparator> *pq,
                      Eecs281PQ<Zombie*,MostActive> *pq2, Eecs281PQ<Zombie*,LeastActive> *pq3,
                      int sargg) {
    
    os << "Zombies still active: " << pq->size() << "\n";
    
    size_t numkilled = Killorder.size();
    
    size_t m = min(numkilled, (size_t)sargg);
    os << "First zombies killed:\n";
    //print out first n zombies killed
    for (size_t i = 0; i < m; i++) {
        Zombie* debug = Killorder[i];
        os << debug->name << " " << (i + 1) << "\n";
    }
    size_t counter = m;
    os << "Last zombies killed:\n";
    //print out last m zombies killed
    for (size_t i = numkilled; i > numkilled - m; i--) {
        Zombie *debug2 = Killorder[i - 1];
        os << debug2->name << " " << counter << "\n";
        counter--;
    }
    
    os << "Most active zombies:\n";
    int m2 = min((int)pq2->size(), sargg);
    //print most active zombies
    for (int i = 0; i < m2; i++) {
        Zombie *debug3 = pq2->top();
        if (debug3->roundsactive > 0) {
            os << debug3->name << " " << debug3->roundsactive << "\n";
        }
        pq2->pop();
    }
    os << "Least active zombies:\n";
    //print least active zombies
    for (int i = 0; i < m2; i++) {
        Zombie *debug4 = pq3->top();
        if (debug4->roundsactive > 0) {
            os << debug4->name << " " << debug4->roundsactive << "\n";
        }
        pq3->pop();
    }

    return;
}

