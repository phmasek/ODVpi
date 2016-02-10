/**
 * OpenDaVINCI - Tutorial.
 * Copyright (C) 2015 Christian Berger
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <core/base/module/TimeTriggeredConferenceClientModule.h>


class TimeTrigger : public core::base::module::TimeTriggeredConferenceClientModule {
    public:
        enum {
            NANOSECOND  = 1,
            MICROSECOND = 1000 * NANOSECOND,    // 1000 nanoseconds are one microsecond.
            MILLISECOND = 1000 * MICROSECOND,   // 1000 microseconds are one millisecond.
            SECOND      = 1000 * MILLISECOND,   // 1000 milliseconds are one second.
            
            // first digit is runtime in seconds for exection
            RUNTIME     = 5 * MICROSECOND * MICROSECOND, 
            TIMESLICE   = 10,                   // ms/timeslice
            OCCUPY      = 80                    // percent of timeslice to execute
        };

    // Benchmark Variables
    public:
        int piDigits;
        int piTimes;
        int piDuration;
        core::data::TimeStamp timer;


    // Functions used
    private:
        /**
         * "Forbidden" copy constructor. Goal: The compiler should warn
         * already at compile time for unwanted bugs caused by any misuse
         * of the copy constructor.
         *
         * @param obj Reference to an object of this class.
         */
        TimeTrigger(const TimeTrigger &/*obj*/);

        /**
         * "Forbidden" assignment operator. Goal: The compiler should warn
         * already at compile time for unwanted bugs caused by any misuse
         * of the assignment operator.
         *
         * @param obj Reference to an object of this class.
         * @return Reference to this instance.
         */
        TimeTrigger& operator=(const TimeTrigger &/*obj*/);

    public:
        /**
         * Constructor.
         *
         * @param argc Number of command line arguments.
         * @param argv Command line arguments.
         */
        TimeTrigger(const int32_t &argc, char **argv);

        virtual ~TimeTrigger();

        coredata::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

    private:
        virtual void setUp();

        virtual void tearDown();
};


