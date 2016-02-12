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


#include <iostream>
#include <iomanip>

#include "MyTimeTrigger.hpp"

using namespace std;

// We add some of OpenDaVINCI's namespaces for the sake of readability.
using namespace core::base;

TimeTrigger::TimeTrigger(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "TimeTrigger")
    {}

TimeTrigger::~TimeTrigger() {}

void TimeTrigger::setUp() {

    // Reset benchmark variables
    // within the RT object.
    piDigits    = 0.00;
    piTimes     = 0;
    piDuration  = 0.00;


    // Print out info before starting
    // execution of timeslices.
    cout << endl;
    cout << "Running at:                            "  << getFrequency()    << "hz" << endl;
    cout << "Occupation \% per slice:                " << occupy << "%" << endl << endl;

    timer = core::data::TimeStamp();

}

void TimeTrigger::tearDown() {
    // Print out results from run
    if (verbose==MODE3) {
        cout << getFrequency() << "hz;" << occupy << "%" << endl;
        cout << piDigits << "digits/slice;" << piDuration << "us/slice" << endl;
    } else {
        const char* measured = (measureByTime ? "Occupied " : "Limited pi decimals per slice to ");
        cout << endl << endl;;
        cout << "Measured by:    " << measured << (!measureByTime ? piLimit : occupy) << (!measureByTime ? " digits" : "\% of each timeslice with calculations") << endl;
        cout << "Ran for:                               " << core::data::TimeStamp().getSeconds()-timer.getSeconds()  << " second(s)"           << endl;
        cout << "Total pi timeslice(s):                 " << piTimes                            << " calculation(s)"      << endl;
        cout << fixed << setprecision(4) << "Avg. pi digits per slice:              " << piDigits                           << " pi digits/timeslice" << endl;
        cout << fixed << setprecision(4) << "Avg. us spent calculating per slice:   " << piDuration                         << " us/timeslice"        << endl << endl;
    }
}

coredata::dmcp::ModuleExitCodeMessage::ModuleExitCode TimeTrigger::body() {

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == coredata::dmcp::ModuleStateMessage::RUNNING) {
        
        // Pi algorithm variable are
        // reset after each timeslice.
        long double tempPi;
        long double pi     = 4.0;
        int         i      = 1;
        int         j      = 3;

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
                ((after.toMicroseconds()-before.toMicroseconds()) >= (1000000/getFrequency())*((float)occupy/100))) {
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
    return coredata::dmcp::ModuleExitCodeMessage::OKAY;
}

int32_t main(int32_t argc, char **argv) {
    TimeTrigger tte(argc, argv);

    // Setup the default value
    // for the flag variables.
    tte.occupy          = 80;
    tte.verbose         = TimeTrigger::QUIET;
    tte.measureByTime   = true;
    tte.piLimit         = 1000;

    for (int args=0;args<argc;args++){
        if (string(argv[args])=="-p" || string(argv[args])=="--pi") {
            tte.measureByTime = false;
            istringstream buffer(string(argv[args+1]));
            buffer >> tte.piLimit;
        } else if (string(argv[args])=="-v" || string(argv[args])=="--verbose") {
            int tmpV;
            istringstream buffer(string(argv[args+1]));
            buffer >> tmpV;
            switch(tmpV) {
                case 1 : tte.verbose = TimeTrigger::MODE1; break;
                case 2 : tte.verbose = TimeTrigger::MODE2; break;
                case 3 : tte.verbose = TimeTrigger::MODE3; break;
                default:  tte.verbose = TimeTrigger::MODE1; break;
            }
        } else if (string(argv[args])=="-o" || string(argv[args])=="--occupy") {
            istringstream buffer(string(argv[args+1]));
            buffer >> tte.occupy;
        }
    }



    return tte.runModule();
}