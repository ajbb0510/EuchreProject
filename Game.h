//
//  game.h
//  eecs281project2
//
//  Created by Andrew Bennett-Belch on 5/15/17.
//  Copyright © 2017 Andrew Bennett-Belch. All rights reserved.
//

#ifndef game_h
#define game_h


#endif /* game_h */
#include <string>
#include <vector>
#include "PoormanPQ.h"
#include "BinaryPQ.h"
#include "SortedPQ.h"
#include "PairingPQ.h"
#include "Eecs281PQ.h"
#include <deque>

class Zombie {
public:
    Zombie();
    std::string name;
    int distance;
    int speed;
    int health;
    int roundsactive;
}; //zombie

class Configuration {
public:
    Configuration();
    int quiverCapacity;
    int yourHealth;
    int randomSeed;
    int maxDistance;
    int maxSpeed;
    int maxHealth;
};//config

struct Comparator {
    bool operator()  (Zombie* lhs, Zombie* rhs) const {
        int ldistance = lhs->distance;
        int rdistance = rhs->distance;
        int lspeed = lhs->speed;
        int rspeed = rhs->speed;
        double lhsETA = ldistance / lspeed;
        double rhsETA = rdistance / rspeed;
        
        
        if (lhsETA != rhsETA) {
            return lhsETA > rhsETA;
        }
        else if (lhs->health != rhs->health) {
            return lhs->health > rhs->health;
        }
        else {
            return lhs->name > rhs->name;
        }
    }
};

struct MostActive {
    bool operator() (Zombie*lhs, Zombie* rhs) const {
        int lhsrounds = lhs->roundsactive;
        int rhsrounds = rhs->roundsactive;
        
        
        if (lhsrounds != rhsrounds) {
            return lhsrounds < rhsrounds;
        }
        else {
            return lhs->name > rhs->name;
        }
    }
};

struct LeastActive {
    bool operator() (Zombie *lhs, Zombie*  rhs) const {
        int lhsrounds = lhs->roundsactive;
        int rhsrounds = rhs->roundsactive;
        
        
        if (lhsrounds != rhsrounds) {
            return lhsrounds > rhsrounds;
        }
        else {
            return lhs->name > rhs->name;
        }
    }
};

class Game {
public:
    Game();
    Configuration config;
    std::vector<Zombie*> Spawnorder;
    std::deque<Zombie*> Killorder;
    void playGame(std::ostringstream &os, Eecs281PQ<Zombie*,Comparator> *pq,
                  Eecs281PQ<Zombie*,MostActive> *pq2,
                  Eecs281PQ<Zombie*,LeastActive> *pq3, bool vpresent);
    void statistics(std::ostringstream &os, Eecs281PQ<Zombie*,Comparator> *pq,
                    Eecs281PQ<Zombie*,MostActive> *pq2,
                    Eecs281PQ<Zombie*,LeastActive> *pq3, int sargg);
};//game





