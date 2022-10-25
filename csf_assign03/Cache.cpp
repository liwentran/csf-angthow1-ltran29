#include "Cache.h"
#include "Set.h"
#include <iostream>

using std::cout;
using std::endl;

// Constructor
Cache::Cache(int cache_size, int set_size, int block_size, bool write_allocate, write_type write_t, evict_type evict_t) : 
cache_size(cache_size), set_size(set_size), block_size(block_size), write_allocate(write_allocate), write_t(write_t), evict_t(evict_t), cycles(0), access_counter(0) {
    //Create all sets
    for(int i = 0; i < cache_size; i++){
        Set s = Set(set_size, block_size);
        sets.push_back(s);
    }
}

unsigned long Cache::get_cycles(){
    return cycles;
}

int Cache::log2(int memory){
    int result = 0;
    while (memory >>= 1) result++; 
    return result;
}


int Cache::load(uint32_t address, bool is_dirty) {
    //need to check for 0?
    uint32_t index_size = log2(set_size);
    uint32_t offset_size = log2(block_size);
    uint32_t tag_size = 32 - index_size - offset_size;

    //get index: clear tag
    uint32_t index = address << tag_size;
    //clear offset
    index >>= (tag_size + offset_size);

    //get tag
    uint32_t tag = address >> (index_size + offset_size);

    //check if tag exists in index, use map (maps tag to index)
    Set &s = sets[log2(index)];
    auto i = s.slots_map.find(tag);
    if (i != s.slots_map.end()) { // hit
        int index_of_slot = i->second;
        //update data
        s.slots[index_of_slot].mapped_memory = address;
        //update access timestamp
        s.slots[index_of_slot].access_ts = ++access_counter;
        return 1;

    } else { // miss
        //remove logic based on LRU
        Slot lru = s.slots[0];
        int rm_idx = 0;

        //if first slot is invalid then no need to run
        if(lru.valid){
            for(unsigned i = 1; i < s.slots.size(); i++){
                if(!s.slots[i].valid){ //if slot is invalid use it
                    rm_idx = i;
                    break;
                }
                if(s.slots[i].access_ts < s.slots[rm_idx].access_ts){
                    rm_idx = i;
                }
            }
        }
        //update map 
        //if valid
        if(s.slots[rm_idx].valid){
            s.slots_map.erase(s.slots[rm_idx].tag);
            if(s.slots[rm_idx].dirty){
                cycles += block_size / 4 * 100;
            }
        }
        s.slots_map[tag] = rm_idx;

        //update data and cycles
        cycles += block_size / 4 * 100;
        s.slots[rm_idx].valid = true;
        if(is_dirty){
            s.slots[rm_idx].dirty = true;
         }
        s.slots[rm_idx].tag = tag;
        s.slots[rm_idx].mapped_memory = address;
        s.slots[rm_idx].access_ts = ++access_counter;
        return -1;
    }
}


int Cache::store(uint32_t address) {

    //checking if cache has address
    uint32_t index_size = log2(set_size);
    uint32_t offset_size = log2(block_size);
    uint32_t tag_size = 32 - index_size - offset_size;

    //get index: clear tag
    uint32_t index = address << tag_size;
    //clear offset
    index >>= (tag_size + offset_size);
    cout << "index: " << index << '\n';

    //get tag
    uint32_t tag = address >> (index_size + offset_size);
    cout << "tag: " << tag << '\n'; 

    //check if tag exists in index, use map (maps tag to index)
    Set &s = sets[log2(index)];
    auto i = s.slots_map.find(tag);
    if (i != s.slots_map.end()) { // hit
        if(write_t == write_through){ //write through
            load(address, false);
            cycles += block_size / 4 * 100;
        }
        else{ //write back
            load(address, true);
        }
        return 1;

    } else { // miss
        //remove logic based on LRU
        if(write_allocate){
            load(address, false);
            //load into cache
        }
        if(write_t == write_through){ //write through
            load(address, false);
            cycles += block_size / 4 * 100; //store in memory
        }
        else{ //write back
            load(address, true);
        }
        return -1;
    }



}





// Outputs the design parameters
void Cache::print_design() {
    cout << "Cache size: " << cache_size << endl 
        << "Set size: " << set_size << endl
        << "Block size: " << block_size << endl
        << "Write allocate?: " << write_allocate << endl 
        << "Write type: " << (write_t==write_through ? "write-through" : "write_back") << endl
        << "Evict type: " << (evict_t==lru ? "lru" : "fifo") << endl;
}