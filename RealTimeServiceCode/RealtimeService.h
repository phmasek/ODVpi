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

#include <opendavinci/odcore/base/RealtimeService.h>


// Realtime concurrency is provided by the class core::base::RealtimeService.
class RealtimeService : public odcore::base::RealtimeService {
    public:
        enum {
            NANOSECOND  = 1,
            MICROSECOND = 1000 * NANOSECOND,    // 1000 nanoseconds are one microsecond.
            MILLISECOND = 1000 * MICROSECOND,   // 1000 microseconds are one millisecond.
            SECOND      = 1000 * MILLISECOND,   // 1000 milliseconds are one second.
            
            // first digit is runtime in seconds for exection
            RUNTIME     = 5 * MICROSECOND * MICROSECOND, 
            TIMESLICE   = 100,                  // ms/timeslice
            OCCUPY      = 80                    // percent of timeslice to execute
        };

    // Benchmark Variables
    public:
        int piDigits;
        int piTimes;
        int piDuration;
        int timeslice;


    // Flag Variables
    public:
        int piLimit;
        int occupy;
        int runtime;
        bool measureByTime;
        bool verbose;


    // Functions used
    public:
        RealtimeService(const uint32_t &periodInMicroseconds);

        // Your class needs to implement the method void nextTimeSlice().
        virtual void nextTimeSlice();
};
