# RealTimeCode
This component is run standalone.  Code based on [realtimeservice tutorial](https://github.com/se-research/OpenDaVINCI/tree/master/tutorials/realtimeservice).

## Compiling
```bash
g++ -I /usr/include/opendavinci -c MyRealtimeService.cpp -o MyRealtimeService.o
g++ -o realtimeservice MyRealtimeService.o -lopendavinci -lpthread -lrt
```

## Executing

The code runs without flags with the default values set and execution mode `occupy`.

Flags:
* `-f` or `--freq` - Sets the hz at which the code executes. `(int)` :: Default: 10
* `-d` or `--duration` – Sets the amount of seconds the code will run. `(int)` :: Default: 5
* `-o` or `--occupy` – Used to set the percentage of timeslice to occupy with pi calculations. `(int)` :: Default: 80
* `-p` or `--pi` – Used to set the limited amount of pi decimals to calculate per timeslice. `(int)`
* `-v` or `--verbose` – Extended verbose. `(none)`

### Examples

Runs occupy scenario with 80% of timeslice occupied with pi calculations and verbose enabled:
```bash
./realtimeservice --freq 10 --verbose --occupy 80
```

Pi limited scenario with 1000 decimals per timeslice and verbose disabled:
```bash
./realtimeservice --freq 10 --pi 1000
```
