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
void Cache::inc_cycles(){
    cycles++;
}

int Cache::log2(int memory){
    int result = 0;
    while (memory >>= 1) result++; 
    return result;
}
void Cache::unwrap_address(uint32_t address, uint32_t &tag, uint32_t &index) {
    tag = address;
    tag >>= log2(block_size); // Get rid of the offset
    index = tag % cache_size; // Get index
    tag >>= log2(cache_size); // Get the rest which is the tag
}

int Cache::find_valid_index(Set &s) {
    int rm_idx = 0;
    for(unsigned i = 1; i < s.slots.size(); i++){
        if(!s.slots[i].valid){ //if slot is invalid use it
            rm_idx = i;
            break;
        }
        if(evict_t == fifo){
            if(s.slots[i].load_ts < s.slots[rm_idx].load_ts){
                rm_idx = i;
            }
        }
        else{
            if(s.slots[i].access_ts < s.slots[rm_idx].access_ts){
                rm_idx = i;
            }
        }
    }
    return rm_idx;
}

int Cache::load(uint32_t address, bool is_dirty) {
    uint32_t tag, index;
    unwrap_address(address, tag, index);

    //check if tag exists in index, use map (maps tag to index)
    Set &s = sets[index];
    auto i = s.slots_map.find(tag);

    if (i != s.slots_map.end() && s.slots[i->second].valid) { // hit
        int index_of_slot = i->second;
        cycles++;
        s.slots[index_of_slot].mapped_memory = address;         // update data
        s.slots[index_of_slot].access_ts = ++access_counter;    // update access timestamp
        return 1;

    } else { // miss
        Slot lru = s.slots[0];         //remove logic based on LRU
        int rm_idx = 0;

        if(lru.valid){         //if first slot is invalid then no need to run
            rm_idx = find_valid_index(s);
        }

        if(s.slots[rm_idx].valid){ //remove from map
            s.slots_map.erase(s.slots[rm_idx].tag);
            if(s.slots[rm_idx].dirty){
                cycles += block_size / 4 * 100;
            }
        }

        s.slots_map[tag] = rm_idx;
        cycles += block_size / 4 * 100;         //update data and cycles
        s.slots[rm_idx].valid = true;

        if(is_dirty){
            s.slots[rm_idx].dirty = true;
         } else{
            s.slots[rm_idx].dirty = false;
        }
        
        s.slots[rm_idx].tag = tag;
        s.slots[rm_idx].mapped_memory = address;
        s.slots[rm_idx].access_ts = ++access_counter;
        s.slots[rm_idx].load_ts = access_counter;
        return -1;
    }
}


int Cache::store(uint32_t address) {
    uint32_t tag, index;
    unwrap_address(address, tag, index);
    //check if tag exists in index, use map (maps tag to index)
    Set &s = sets[index];
    auto i = s.slots_map.find(tag);
    if (i != s.slots_map.end() && s.slots[i->second].valid) { // hit
        if(write_t == write_through){ //write through
            load(address, false);
            cycles += 100;
        }
        else{ //write back
            load(address, true);
        }
        return 1;
    } else { // miss
        if(write_allocate){  //remove logic based on LRU
            if(write_t == write_through){ //write through
                load(address, false);
                cycles += 100; //store in memory
            }
            else{ //write back 
                load(address, true);
            }
        }
        else{
            cycles += 100; //store in memory
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