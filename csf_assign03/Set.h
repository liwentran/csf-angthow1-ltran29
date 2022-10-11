#ifndef SET_H
#define SET_H

#include <vector>
using std::vector;

// TODO: determine data types
struct Slot {
    int index;
    bool valid;
    int tag;
    int mapped_memory;
    Slot(int idx, bool valid, int tag, int mm) : index(idx), valid(valid), tag(tag), mapped_memory(mm) {}
};

// TODO: Create set.cpp and make the constructor
class Set {
    public:

    // Constructor
    Set(int set_size, int block_size);

    private:
    int set_size;
    int block_size;
    vector<Slot> slots;
};

#endif