#ifndef CSIM_H
#define CSIM_H
#include <stdint.h>


class Csim {
    public:
        // Constructor
        Csim();

        void process(char command, uint32_t address);    // Processes a single trace
        
        // Prints summary information 
        void summary();
        
    private:
        int loads;
        int stores;
        int load_hits;
        int load_misses;
        int store_hits;
        int store_misses;
        int cycles;
};

#endif // CSIM_H