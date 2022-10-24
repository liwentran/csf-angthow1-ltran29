#ifndef SET_H
#define SET_H

#include <vector>
#include <map>
using std::vector;
using std::map;

// TODO: determine data types
struct Slot {
    bool valid;
    bool dirty;
    int mapped_memory;
    int load_ts;
    int access_ts;
    Slot(bool valid, bool dirty, int mm) : valid(valid), dirty(dirty), mapped_memory(mm), load_ts(0), access_ts(0) {}
};

// TODO: Create set.cpp and make the constructor
class Set {     //should be created up front with index, set_size, block_size
    public:

    // Constructor
    Set(int set_size, int block_size);

    private:
    int set_size; //use to determine index: log2 of this
    int block_size; //use to determine offset: log 2 of this
    //no tag field but yes tag in the constructor will be used for slots map

    //calculate tag by subtracting index and offset from total capacity
    //input all slots in at start with respective tag, make sure valid is false, load 0, access 0, mapped_memory 0
    vector<Slot> slots;
    //map tag to index of slot 
    map <int, int> slots_map; 


    /**
     * 1. Create set
     * 
     * 
     * 
     * 
    */

    //tag can be determined from blcok and set sizes

    //map tag to slot
    //if map contains tag key, it is a a hit
    //search

    //look at address, get index from adress, figure out which set is being accessed, figure out if there is a matching slot in the set with tag. Iterate through set to find slot that is valid and has matching tag
    //to make faster, map index to slot so this is good for hits

    //map index to set

    //cache should have vector of sets

    //offset represents memory position in block. If block size is 128 bytes then offset is 7 because you need 7 bits to describe all the locatoins, 2^7 = 128  
    //index = which set

    //match if tags are same

    //capacity = sets * slots * blocks 

    //count total amount of loads and stores and use that to determine time stamps on access
};

#endif