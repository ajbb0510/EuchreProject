#ifndef P2RANDOM_H
#define P2RANDOM_H

#include <vector>
#include <string>

class P2random {
public:
    static void srand(int seed, int maxDist, int maxSpeed, int maxHealth);
    static uint64_t rand();

    static std::string getNextZombieName();
    static int getNextZombieDistance();
    static int getNextZombieSpeed();
    static int getNextZombieHealth();

    //Don't need to read further than this, unless you want to learn about
    //Mersenne Twister implementation
private:
    static int zombieCounter, maxRandDist, maxRandSpeed, maxRandHealth;
    static std::vector<std::string> ZOMBIE_NAMES;
    static int getNextInt(int);

    /**
    * mt.h: Mersenne Twister header file
    *
    * Jason R. Blevins <jrblevin@sdf.lonestar.org>
    * Durham, March  7, 2007
    */

    /**
    * Mersenne Twister.
    *
    * M. Matsumoto and T. Nishimura, "Mersenne Twister: A
    * 623-dimensionally equidistributed uniform pseudorandom number
    * generator", ACM Trans. on Modeling and Computer Simulation Vol. 8,
    * No. 1, January pp.3-30 (1998).
    *
    * http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html.
    */
    class MersenneTwister {
    public:
        MersenneTwister(void);
        ~MersenneTwister(void);

        void init_genrand(unsigned long s);

        unsigned long genrand_uint64(void);

    private:
        static const int N = 624;
        static const int M = 397;
        // constant vector a
        static const unsigned long MATRIX_A = 0x9908b0dfUL;
        // most significant w-r bits
        static const unsigned long UPPER_MASK = 0x80000000UL;
        // least significant r bits
        static const unsigned long LOWER_MASK = 0x7fffffffUL;

        unsigned long *mt_;                  // the state vector
        int mti_;                            // mti == N+1 means mt not initialized

        unsigned long *init_key_;            // Storage for the seed vector
        int key_length_;                     // Seed vector length
        unsigned long s_;                    // Seed integer
        bool seeded_by_array_;               // Seeded by an array
        bool seeded_by_int_;                 // Seeded by an integer
    };
    static MersenneTwister mt;
};

#endif 
