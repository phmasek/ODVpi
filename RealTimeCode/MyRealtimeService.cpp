#include <stdint.h>
#include <sstream>
#include <iostream>
#include <core/base/Thread.h>
#include "core/data/TimeStamp.h"

#include "MyRealtimeService.hpp"


using namespace std;

MyRealtimeService::MyRealtimeService(const uint32_t &periodInMilliseconds) :
    core::base::RealtimeService((PERIOD) (periodInMilliseconds*MICROSECOND)) {}


void MyRealtimeService::nextTimeSlice() {

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

        // Calculate the amount of
        // pi digits in one timeslice.
        piDigits++;

        
        // How long the [while] has ran.
        // Break if run more than specified
        // occupation % of timeslice.
        
        after = core::data::TimeStamp();
        if (measureByTime &&
            ((after.toMicroseconds()-before.toMicroseconds()) >= timeslice*MICROSECOND*((float)occupy/100))) {
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
    piDuration += (after.toMicroseconds()-before.toMicroseconds())/MICROSECOND;
    piTimes++;

}


int32_t main(int32_t argc, char **argv) {
    

    int freq = MyRealtimeService::TIMESLICE;
    for (int args=0;args<argc;args++){
        if (string(argv[args])=="-f" || string(argv[args])=="--freq") {
            istringstream buffer(string(argv[args+1]));
            buffer >> freq;
            freq = 1000/freq;
        }
    }

    // Initiate the RT object with the
    // time of one timeslice [TIMESLICE].
    MyRealtimeService rts((MyRealtimeService::PERIOD) freq);

    int timer = 1;

    // Reset benchmark variables
    // within the RT object.
    rts.piDigits        = 0;
    rts.piTimes         = 0;
    rts.piDuration      = 0;
    rts.timeslice       = freq;
    rts.measureByTime   = true;
    rts.verbose         = false;
    rts.occupy          = MyRealtimeService::OCCUPY;
    rts.runtime         = MyRealtimeService::RUNTIME;

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
    cout << "Occupation \% per slice:                " << rts.occupy                                   << "%"          << endl;
    cout << "Duration:                              "  << rts.runtime/1000/1000                        << " second(s)" << endl << endl;


    // Start the RT object and sleep
    // for specified [RUNTIME] time 
    // until stopping.
    rts.start();
    core::base::Thread::usleepFor(rts.runtime);
    rts.stop();


    // Print out results from run
    cout << endl;
    cout << "Measured by:                           " << (rts.measureByTime ? "Time" : "Pi calculations with ") << (rts.measureByTime ?  : rts.piLimit) << (rts.measureByTime ? "" : " digits per slice") << endl;
    cout << "Ran for:                               " << rts.runtime/1000/1000                  << " second(s)"           << endl;
    cout << "Total pi calculations (timeslice(s)):  " << rts.piTimes                            << " calculation(s)"      << endl;
    cout << "Total pi digits calculated:            " << rts.piDigits                           << " pi digits"           << endl;
    cout << "Avg. pi digits per slice:              " << rts.piDigits/rts.piTimes               << " pi digits/timeslice" << endl;
    cout << "Avg. pi digits per ms:                 " << rts.piDigits/rts.piDuration            << " pi digits/ms"        << endl << endl;

}