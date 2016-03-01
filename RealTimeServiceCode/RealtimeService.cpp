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

#include <opendavinci/odcore/base/Thread.h>

#include "RealtimeService.h"


using namespace std;

RealtimeService::RealtimeService(const uint32_t &periodInMilliseconds) :
    odcore::base::RealtimeService((PERIOD) (periodInMilliseconds*MICROSECOND)) {}


void RealtimeService::nextTimeSlice() {

    // Pi algorithm variable are
    // reset after each timeslice.
    long double tempPi;
    long double pi     = 4.0;
    int i              = 1;
    int j              = 3;

    // Initiate timers
    odcore::data::TimeStamp before;
    odcore::data::TimeStamp after;
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

        // Calculate the amount of
        // pi digits in one timeslice.
        piDigits++;

        
        // How long the [while] has ran.
        // Break if run more than specified
        // occupation % of timeslice.
        
        after = odcore::data::TimeStamp();
        if (measureByTime &&
            ((after.toNanoseconds()-before.toNanoseconds()) >= timeslice*MICROSECOND*MICROSECOND*((float)occupy/100))) {
            break;
        }else if (!measureByTime && i >= piLimit) {
            break;
        }

    }
    
    if (verbose && !measureByTime) {
        cout << "Duration of 1 timeslice calculation: " << (after.toNanoseconds()-before.toNanoseconds()) << "us" << endl;
    } else if (verbose) {
        cout << "Pi calculations within timeslice: " << i-1 << endl;
    }

    // Measure time used for calculating
    // pi, and how many timeslices were
    // run.
    piDuration += (after.toNanoseconds()-before.toNanoseconds())/MICROSECOND;
    piTimes++;

}


int32_t main(int32_t argc, char **argv) {
    

    int freq = RealtimeService::TIMESLICE;
    for (int args=0;args<argc;args++){
        if (string(argv[args])=="-f" || string(argv[args])=="--freq") {
            istringstream buffer(string(argv[args+1]));
            buffer >> freq;
            freq = 1000/freq;
        }
    }

    // Initiate the RT object with the
    // time of one timeslice [TIMESLICE].
    RealtimeService rts((RealtimeService::PERIOD) freq);

    int timer = 1;

    // Reset benchmark variables
    // within the RT object.
    rts.piDigits        = 0;
    rts.piTimes         = 0;
    rts.piDuration      = 0;
    rts.timeslice       = freq;

    // Initiate default values 
    // for flag variables.
    rts.measureByTime   = true;
    rts.verbose         = false;
    rts.piLimit         = 1000;
    rts.occupy          = RealtimeService::OCCUPY;
    rts.runtime         = RealtimeService::RUNTIME;

    for (int args=0;args<argc;args++){
        if (string(argv[args])=="-p" || string(argv[args])=="--pi") {
            rts.measureByTime = false;
            istringstream buffer(string(argv[args+1]));
            buffer >> rts.piLimit;
        } else if (string(argv[args])=="-v" || string(argv[args])=="--verbose") {
            rts.verbose = true;
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
    cout << "Running at:                            "  << 1000/rts.timeslice << "hz"                        << endl;
    if (rts.measureByTime)
        cout << "Occupation \% per slice:                " << rts.occupy                                   << "%"          << endl;
    cout << "Duration:                              "  << rts.runtime/1000/1000                        << " second(s)" << endl << endl;


    // Start the RT object and sleep
    // for specified [RUNTIME] time 
    // until stopping.
    rts.start();
    while (true) {
        // Run untill pi has been done
        // freq x duration(seconds)
        // Example: 10hz x 5sec = 50 iterations
        if (rts.piTimes == 1000/rts.timeslice*(rts.runtime/1000/1000))
            break;
    }
    rts.stop();

    
    // Print out results from run
    const char* measured = (rts.measureByTime ? "Occupied " : "Limited pi decimals per slice to ");
    cout << endl << endl;;
    cout << "Measured by:    " << measured << (!rts.measureByTime ? rts.piLimit : rts.occupy) << (!rts.measureByTime ? " digits" : "\% of each timeslice with calculations") << endl;
    cout << "Ran for:                               " << rts.runtime/1000/1000                  << " second(s)"           << endl;
    cout << "Total pi calculations (timeslice(s)):  " << rts.piTimes                            << " calculation(s)"      << endl;
    cout << "Total pi digits calculated:            " << rts.piDigits                           << " pi digits"           << endl;
    cout << "Avg. pi digits per slice:              " << rts.piDigits/rts.piTimes               << " pi digits/timeslice" << endl;
    cout << "Avg. pi digits per ms:                 " << rts.piDigits/rts.piDuration            << " pi digits/ms"        << endl << endl;

}