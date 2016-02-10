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
    piDigits    = 0;
    piTimes     = 0;
    piDuration  = 0;


    // Setup the flag variables
    occupy          = getKeyValueConfiguration().getValue<float>("timetrigger.occupy");
    verbose         = getKeyValueConfiguration().getValue<bool>("timetrigger.verbose");
    measureByTime   = getKeyValueConfiguration().getValue<bool>("timetrigger.measure_by_time");
    piLimit         = getKeyValueConfiguration().getValue<uint32_t>("timetrigger.pi_limit");

    cout << occupy << endl;

    // Print out info before starting
    // execution of timeslices.
    cout << endl;
    cout << "Running at:                            "  << getFrequency()    << "hz" << endl;
    cout << "Occupation \% per slice:                " << occupy*100 << "%" << endl << endl;

    timer = core::data::TimeStamp();

}

void TimeTrigger::tearDown() {
    // Print out results from run
    cout << endl;
    cout << "Measured by:                           " << (measureByTime ? "Time" : "Pi calculations with ") << (measureByTime ?  : piLimit) << (measureByTime ? "" : " digits per slice") << endl;
    cout << "Ran for:                               " << core::data::TimeStamp().getSeconds()-timer.getSeconds()  << " second(s)"           << endl;
    cout << "Total pi calculations (timeslice(s)):  " << piTimes                            << " calculation(s)"      << endl;
    cout << "Total pi digits calculated:            " << piDigits                           << " pi digits"           << endl;
    cout << "Avg. pi digits per slice:              " << piDigits/piTimes                   << " pi digits/timeslice" << endl;
    // cout << "Avg. pi digits per ms:                 " << piDigits/piDuration                << " pi digits/ms"        << endl << endl;
    cout << piDuration << endl;
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

            // Calculate the amount of
            // pi digits in one timeslice.
            piDigits++;

            
            // How long the [while] has ran.
            // Break if run more than specified
            // occupation % of timeslice.
            after = core::data::TimeStamp();
            if (measureByTime &&
                ((after.toMicroseconds()-before.toMicroseconds())*MICROSECOND >= (SECOND/getFrequency())*((float)occupy/100))) {
                break;
            }else if (i >= piLimit) {
                break;
            }

        }

        if (verbose && !measureByTime) {
            cout << "Duration of 1 timeslice calculation: " << (after.toMicroseconds()-before.toMicroseconds()) << endl;
        } else if (verbose) {
            cout << "Pi calculations within timeslice: " << i-1 << endl;
        }

        // Measure time used for calculating
        // pi, and how many timeslices were
        // run.
        piDuration += (after.toMicroseconds()-before.toMicroseconds());
        piTimes++;
    }
    return coredata::dmcp::ModuleExitCodeMessage::OKAY;
}

int32_t main(int32_t argc, char **argv) {
    TimeTrigger tte(argc, argv);
    return tte.runModule();
}