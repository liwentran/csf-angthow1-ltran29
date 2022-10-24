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
    int tag;
    Slot(bool valid, bool dirty, int mm) : valid(valid), dirty(dirty), mapped_memory(mm), load_ts(0), access_ts(0) {}
};

// TODO: Create set.cpp and make the constructor
class Set {   
    public:

    // Constructor
    Set(int set_size, int block_size);
    map <int, int> slots_map; 
    vector<Slot> slots;

    private:
    int set_size; //use to determine index: log2 of this
    int block_size; //use to determine offset: log 2 of this
};

#endif