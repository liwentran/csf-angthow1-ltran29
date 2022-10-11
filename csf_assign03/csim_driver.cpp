#include <iostream>
#include <sstream>
#include <string>
#include <ctype.h>
#include <string.h>

#include "Csim.h"
#include "Cache.h"


using std::cout;
using std::cin;
using std::cerr;
using std::string;
using std::istringstream;
using std::stoi;
using std::stol;
using std::endl;
using std::isdigit;

bool is_power_of_two(int x);
bool is_valid_int(char* arg);


int main(int argc, char** argv) { 
    /**
     * Reading Cache Parameters:
     * 
     * argv[1]  - number of sets in the cache (a positive power-of-2)
     * argv[2]  - number of blocks in each set (a positive power-of-2)
     * argv[3]  - number of bytes in each block (a positive power-of-2, at least 4)
     * argv[4]  - write-allocate or no-write-allocate
     * argv[5]  - write-through or write-back
     * argv[6]  - lru (least-recently-used) or fifo evictions
     * 
     * example: ./csim 256 4 16 write-allocate write-back lru  < sometracefile
     */

    // if there are not 6 cache design parameters, exit with code 1
    if (argc != 7) {
        cerr << "Invalid arguments" << endl;
        return 1;
    }

    
    char* arg;
    // Cache size validation: must be a positive power of 2
    arg = argv[1];
    if (!is_valid_int(arg)) {
        cerr << "Number of sets in a cache must be a positive power-of-2" << endl;
        return 1;
    }
    int cache_size = stoi(arg);
    if (!is_power_of_two(cache_size) || cache_size <= 0) {
        cerr << "Number of sets in a cache must be a positive power-of-2" << endl;
        return 1;
    }

    // Set size validation: must be a positive power-of-2
    arg = argv[2];
    if (!is_valid_int(arg)) {
        cerr << "Number of blocks in a each set must be a positive power-of-2" << endl;
        return 1;
    }
    int set_size = stoi(arg);
    if (!is_power_of_two(set_size) || set_size <= 0) {
        cerr << "Number of blocks in a each set must be a positive power-of-2" << endl;
        return 1;
    }

    // Block size validation: must be a positive power-of-2 and at least 4
    arg = argv[3];
    if (!is_valid_int(arg)) {
        cerr << "Number of bytes in each block must be a positive power-of-2" << endl;
        return 1;
    }
    int block_size = stoi(arg);
    if (!is_power_of_two(block_size) || block_size <= 0) {
        cerr << "Number of bytes in each block must be a positive power-of-2" << endl;
        return 1;
    }
    if (block_size < 4) {
        cerr << "Block size must be at least four" << endl;
        return 1;
    }

    // Write allocate validation
    arg = argv[4];
    bool write_allocate;
    if (strcmp(arg, "write-allocate") == 0) {
        write_allocate = true;
    } else if (strcmp(arg, "no-write-allocate") == 0) {
        write_allocate = false;
    } else {
        cerr << "4th parameter must be write-allocate or no-write-allocate" << endl;
        return 1;
    }
    
    // Write type validation
    arg = argv[5];
    write_type write_t;
    if (strcmp(arg, "write-through") == 0) {
        write_t = write_through;
    } else if (strcmp(arg, "write-back") == 0) {
        write_t = write_back;
    } else {
        cerr << "5th parameter must be write_through or write-back" << endl;
        return 1;
    }

    // Evict type validation
    arg = argv[6];
    evict_type evict_t;
    if (strcmp(arg, "lru") == 0) {
        evict_t = lru;
    } else if (strcmp(arg, "fifo") == 0) {
        evict_t = fifo;
    } else {
        cerr << "6th parameter must be lru or fifo" << endl;
        return 1;
    }

    // no-write-allocate cannot be combined with write-back
    if (!write_allocate && write_t == write_back) {
        cerr << "write-back and no-write-allocate cannot both be specified" << endl;
        return 1;
    }

    // Create the Cache object
    Cache c = Cache(cache_size, set_size, block_size, write_allocate, write_t, evict_t);

    c.print_design();

    // read each line of the trace file
    string line;
    Csim simulator;
    while (getline(cin, line)) {
        istringstream is(line);
        
        // read the first two fields (ignoring the third)
        char command;       // either 'l' (loading) or 's' (storing)
        string address_hex; // 32-bit memory address given in hexadecimal
        // int32_t address;    // 32-bit memory address in int

        is >> command;
        is >> address_hex;
        uint32_t address = stol(address_hex, 0, 16);
        cout << "Command: " << command << ", Address: " << address_hex << ", Numeric address: " << address << endl;
        
        // Process line
        simulator.process(command, address);

    }

    // Output summary
    simulator.summary();
    return 0;
}

bool is_power_of_two(int x) {
    return (x != 0) && ((x & (x - 1)) == 0);
}

bool is_valid_int(char* str) {
    while (*str) {
        if (!isdigit(*str)) {
            return false;
        }
        str++;
    }
    return true;
}