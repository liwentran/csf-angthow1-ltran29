#include <iostream>
#include <sstream>
#include "csim_functions.h"
#include <string>


using std::cout;
using std::cin;
using std::cerr;
using std::string;
using std::istringstream;
using std::stoi;
using std::endl;
using namespace Csim;


int main(int argc, char** argv) { 

    // read each line of the trace file
    string line;
    while (getline(cin, line)) {
        istringstream is(line);
        
        // read the first two fields (ignoring the third)
        char process;       // either 'l' (loading) or 's' (storing)
        string address_hex; // 32-bit memory address given in hexadecimal
        int32_t address;    // 32-bit memory address in int

        is >> process;
        is >> address_hex;
        address = stoi(address_hex, 0, 16);

        // cout << "Process: " << process << ", Address: " << address_hex << ", Numeric address: " << address << endl;
        
        // Process line
    }

    return 0;
}