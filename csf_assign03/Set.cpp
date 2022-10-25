#include "Set.h"
#include <iostream>

using std::cout;
using std::endl;

// Constructor
Set::Set(int set_size, int block_size) : set_size(set_size), block_size(block_size) { 
    //add slots based on set_size, do not map yet
    for(int i = 0; i < set_size; i++){
        Slot s = Slot(false, false, 0);
            slots.push_back(s);
    }

}