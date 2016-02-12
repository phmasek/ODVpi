/**
 * ODVpi - Benchmark code extending OpenDaVinci 
 * (https://github.com/se-research/OpenDaVINCI)
 * Copyright (C) 2016 Philip Masek, Magnus Thulin
 *
 * This code is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <stdint.h>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <core/base/Thread.h>
#include "core/data/TimeStamp.h"

#include "MyRealtimeService.hpp"


using namespace std;

RealtimeService::RealtimeService(const uint32_t &periodInMicroseconds) :
    core::base::RealtimeService((PERIOD) (periodInMicroseconds)) {}


void RealtimeService::nextTimeSlice() {

    // Pi algorithm variable are
    // reset after each timeslice.
    long double tempPi;
    long double pi     = 4.0;
    int i              = 1;
    int j              = 3;

    // Initiate timers
    core::data::TimeStamp before;
    core::data::TimeStamp after;
    while (true) {


        // Calculate pi
        tempPi = static_cast<double>(4)/j;
        if (i%2 != 0) {
            pi -= tempPi;
        } else if (i%2 == 0) {
            pi += tempPi;
        }
        i++;
        j+=2;

        
        // How long the [while] has ran.
        // Break if run more than specified
        // occupation % of timeslice.
        after = core::data::TimeStamp();
        if (measureByTime &&
            ((after.toMicroseconds()-before.toMicroseconds()) >= timeslice*((float)occupy/100))) {
            break;
        }else if (!measureByTime && i >= piLimit) {
            break;
        }

    }
    // Add to the timeslice counter
    piTimes++;


    // Calculate the avg. amount of
    // pi digits in one timeslice.
    piDigits=piDigits+(((i-1)-piDigits)/piTimes);


    // Verbose code
    if (verbose==MODE2||(!measureByTime&&verbose!=QUIET)) {
        cout << fixed << setprecision(2) << "Calculated for: " << (after.toMicroseconds()-before.toMicroseconds()) << "us Avg: " << piDuration << "us" << endl;
    } else if (verbose==MODE1) {
        cout << fixed << setprecision(2) << "Pi decimals finished: " << i << " Avg. pi decimals: " << piDigits << endl;
    }

    // Measure avg time used for calculating
    // pi, and how many timeslices were
    // run.
    piDuration = piDuration+(((after.toMicroseconds()-before.toMicroseconds())-piDuration)/piTimes);
}


int32_t main(int32_t argc, char **argv) {
    

    float freq = RealtimeService::TIMESLICE;
    for (int args=0;args<argc;args++){
        if (string(argv[args])=="-f" || string(argv[args])=="--freq") {
            istringstream buffer(string(argv[args+1]));
            buffer >> freq;
            freq = 1000000/freq;
        }
    }

    // Initiate the RT object with the
    // time of one timeslice [TIMESLICE].
    RealtimeService rts((RealtimeService::PERIOD) freq);

    int timer = 1;

    // Reset benchmark variables
    // within the RT object.
    rts.piDigits        = 0;
    rts.piTimes         = 1;
    rts.piDuration      = 0.00;
    rts.timeslice       = freq;

    // Initiate default values 
    // for flag variables.
    rts.measureByTime   = true;
    rts.verbose         = RealtimeService::QUIET;
    rts.piLimit         = 1000;
    rts.occupy          = RealtimeService::OCCUPY;
    rts.runtime         = RealtimeService::RUNTIME;

    for (int args=0;args<argc;args++){
        if (string(argv[args])=="-p" || string(argv[args])=="--pi") {
            rts.measureByTime = false;
            istringstream buffer(string(argv[args+1]));
            buffer >> rts.piLimit;
        } else if (string(argv[args])=="-v" || string(argv[args])=="--verbose") {
            int tmpV;
            istringstream buffer(string(argv[args+1]));
            buffer >> tmpV;
            switch(tmpV) {
                case 1 : rts.verbose = RealtimeService::MODE1; break;
                case 2 : rts.verbose = RealtimeService::MODE2; break;
                case 3 : rts.verbose = RealtimeService::MODE3; break;
                default:  rts.verbose = RealtimeService::MODE1; break;
            }
        } else if (string(argv[args])=="-o" || string(argv[args])=="--occupy") {
            istringstream buffer(string(argv[args+1]));
            buffer >> rts.occupy;
        } else if (string(argv[args])=="-d" || string(argv[args])=="--duration") {
            istringstream buffer(string(argv[args+1]));
            buffer >> rts.runtime;
            rts.runtime = rts.runtime*1000*1000;
        }
    }

    // Print out info before starting
    // execution of timeslices.
    cout << endl;
    cout << "Running at:                            "  << 1000000/rts.timeslice << "hz"                        << endl;
    if (rts.measureByTime)
        cout << "Occupation \% per slice:                " << rts.occupy                                   << "%"          << endl;
    cout << "Duration:                              "  << rts.runtime/1000/1000                        << " second(s)" << endl << endl;


    // Start the RT object and sleep
    // for specified [RUNTIME] time 
    // until stopping.
    rts.start();
    core::base::Thread::usleepFor(rts.runtime);
    rts.stop();


    // Print out results from run
    if (rts.verbose==RealtimeService::MODE3) {
        cout << 1000000/freq << "hz;" << rts.occupy << "%" << endl;
        cout << rts.piDigits << "digits/slice;" << rts.piDuration << "us/slice" << endl;
    } else {
        const char* measured = (rts.measureByTime ? "Occupied " : "Limited pi decimals per slice to ");
        cout << endl << endl;;
        cout << "Measured by:    " << measured << (!rts.measureByTime ? rts.piLimit : rts.occupy) << (!rts.measureByTime ? " digits" : "\% of each timeslice with calculations") << endl;
        cout << "Ran for:                               " << rts.runtime/1000/1000                  << " second(s)"           << endl;
        cout << "Total pi calculations (timeslice(s)):  " << rts.piTimes                            << " calculation(s)"      << endl;
        cout << fixed << setprecision(4) << "Avg. pi digits per slice:              " << rts.piDigits                           << " pi digits/timeslice" << endl;
        cout << fixed << setprecision(4) << "Avg. us spent calculating per slice:   " << rts.piDuration                         << " us/timeslice"        << endl << endl;
    }

}
