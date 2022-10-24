#include "Csim.h"
#include <iostream>
#include <stdint.h>
#include "Cache.h"

using std::cout;
using std::endl;

Csim::Csim(Cache cache): cache(cache), loads(0), stores(0), load_hits(0), load_misses(0), store_hits(0), store_misses(0), cycles(0) {};

void Csim::process(char command, uint32_t address)  {
    if (command == 'l') {
        loads++;
    }
    if (command == 's') {
        stores++;
    }
    cout << "loads: " << loads << ", stores: " << stores << endl;
}

void Csim::summary() {
    cout << "Total loads: " << loads << endl
        << "Total stores: " << stores << endl
        << "Load hits: " << load_hits << endl
        << "Load misses: " << load_misses << endl
        << "Store hits: " << store_hits << endl
        << "Store misses: " << store_misses << endl
        << "Total cycles: " << cycles << endl;
}