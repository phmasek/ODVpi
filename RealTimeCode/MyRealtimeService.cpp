#include <stdint.h>
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
        if ((after.toMicroseconds()-before.toMicroseconds())*MICROSECOND >= TIMESLICE*MILLISECOND*((float)OCCUPY/100)) {
            break;
        }

    }

    // Measure time used for calculating
    // pi, and how many timeslices were
    // run.
    piDuration += (after.toMicroseconds()-before.toMicroseconds())/MICROSECOND;
    piTimes++;

}


int32_t main(int32_t argc, char **argv) {
    
    // Initiate the RT object with the
    // time of one timeslice [TIMESLICE].
    MyRealtimeService rts((MyRealtimeService::PERIOD) MyRealtimeService::TIMESLICE);

    // Print out info before starting
    // execution of timeslices.
    cout << endl;
    cout << "Running at:                            "  << MyRealtimeService::MICROSECOND/MyRealtimeService::TIMESLICE << "hz"         << endl;
    cout << "Occupation \% per slice:                " << MyRealtimeService::OCCUPY                                   << "%"          << endl;
    cout << "Duration:                              "  << MyRealtimeService::RUNTIME/1000/1000                        << " second(s)" << endl << endl;

    int timer = 1;

    // Reset benchmark variables
    // within the RT object.
    rts.piDigits    = 0;
    rts.piTimes     = 0;
    rts.piDuration  = 0;

    // Start the RT object and sleep
    // for specified [RUNTIME] time 
    // until stopping.
    rts.start();
    core::base::Thread::usleepFor(MyRealtimeService::RUNTIME);
    rts.stop();


    // Print out results from run
    cout << "Ran for:                               " << MyRealtimeService::RUNTIME/1000/1000   << " second(s)"           << endl;
    cout << "Total pi calculations (timeslice(s)):  " << rts.piTimes                            << " calculation(s)"      << endl;
    cout << "Total pi digits calculated:            " << rts.piDigits                           << " pi digits"           << endl;
    cout << "Avg. pi digits per slice:              " << rts.piDigits/rts.piTimes               << " pi digits/timeslice" << endl;
    cout << "Avg. pi digits per ms:                 " << rts.piDigits/rts.piDuration            << " pi digits/ms"        << endl << endl;

}