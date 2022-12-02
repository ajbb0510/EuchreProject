//
//  main.cpp
//  eecs281project2
//
//  Created by Andrew Bennett-Belch on 5/15/17.
//  Copyright © 2017 Andrew Bennett-Belch. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <getopt.h>
#include <sstream>
#include <string>
#include <stdio.h>
#include "Game.h"
#include "P2random.h"
#include "PoormanPQ.h"
#include "BinaryPQ.h"
#include "SortedPQ.h"
#include "PairingPQ.h"
#include "Eecs281PQ.h"


using namespace std;

int main(int argc, char * argv[]) {
    ios_base::sync_with_stdio(false);
    
    
    if (getenv("INPUT")) {
        freopen(getenv("INPUT"), "r", stdin);
        freopen(getenv("OUTPUT"), "w", stdout);
    } // if
    
    
    ostringstream os;
    opterr = true;
    
    struct option longopts[] =
    {
        { "help", no_argument, nullptr, 'h' },
        { "container", required_argument, nullptr, 'c'},
        { "statistics", required_argument, nullptr, 's'},
        { "verbose", no_argument, nullptr, 'v'},
        { nullptr, 0, nullptr, '\0' }
    }; //struct

    int index = 0;
    int c; //single char for option
    bool vpresent = false;
    bool spresent = false;
    bool cpresent = false;
    string carg;
    string sarg;
    
    while ((c = getopt_long(argc, argv, "hc:s:v", longopts, &index)) != -1) {
        switch (c) {
            case 'h':
                os << "Use this program to play a game where you will shoot zombies with arrows and possibly have your brains eaten\n";
                cout << os.str();
                return 0;
            case 'c':
                cpresent = true;
                carg = optarg;
                break;
            case 's':
                spresent = true;
                sarg = optarg;
                break;
            case 'v':
                vpresent = true;
                break;
        }//switch
    }//while
    
    if (!cpresent) {
        os << "You must use c flag for all invocations";
        cout << os.str();
        exit(1);
    }
    
    if ((carg != "POOR_MAN") && (carg != "SORTED")
        && (carg != "BINARY") && (carg != "PAIRING")) {
        
        os << "Argument for c-flag must be POOR_MAN, SORTED, BINARY, or PAIRING";
        cout << os.str();
        exit(1);
    }
    
    if (spresent && sarg == "\0") {
        os << "If using statistics you must specify number of entries";
        cout << os.str();
        exit(1);
    }
    
    Game game;
    
    //read preamble
    //comment line first
    string comment;
    getline(cin,comment);
    cin >> comment >> game.config.quiverCapacity >> comment >>
        game.config.yourHealth >> comment >> game.config.randomSeed >>
        comment >> game.config.maxDistance >> comment >> game.config.maxSpeed
        >> comment >> game.config.maxHealth;
    
    Eecs281PQ<Zombie*,Comparator> *pq;
    Comparator comp;
    
    Eecs281PQ<Zombie*, MostActive> *pq2;
    MostActive mos;
    
    Eecs281PQ<Zombie*, LeastActive> *pq3;
    LeastActive les;
    
    if (carg == "POOR_MAN") {
        pq = new PoormanPQ<Zombie*, Comparator>(comp);
        pq2 = new PoormanPQ<Zombie*, MostActive>(mos);
        pq3 = new PoormanPQ<Zombie*, LeastActive>(les);
    }
    else if (carg == "SORTED") {
        pq = new SortedPQ<Zombie*, Comparator>(comp);
        pq2 = new SortedPQ<Zombie*, MostActive>(mos);
        pq3 = new SortedPQ<Zombie*, LeastActive>(les);
    }
    else if (carg == "PAIRING") {
        pq = new PairingPQ<Zombie*, Comparator>(comp);
        pq2 = new PairingPQ<Zombie*, MostActive>(mos);
        pq3 = new PairingPQ<Zombie*, LeastActive>(les);
    }
    else {
        pq = new BinaryPQ<Zombie*, Comparator>(comp);
        pq2 = new BinaryPQ<Zombie*, MostActive>(mos);
        pq3 = new BinaryPQ<Zombie*, LeastActive>(les);
    }
    
    game.playGame(os, pq, pq2, pq3, vpresent);
    if (spresent) {
        int sargg = stoi(sarg);
        game.statistics(os, pq, pq2, pq3, sargg);
    }
   
    cout << os.str();
    for (auto p: game.Spawnorder) {
        delete p;
    }
    delete pq;
    delete pq2;
    delete pq3;
    return 0;
}
