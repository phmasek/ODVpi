# RealTimeCode
This component is run standalone.

## Compiling
```bash
g++ -I /usr/include/opendavinci -c MyRealtimeService.cpp -o MyRealtimeService.o
g++ -o realtimeservice MyRealtimeService.o -lopendavinci -lpthread -lrt
```

## Executing

The code runs without flags with the default values set and measure mode `occupy`.

Flags:
* `-f` or `--freq` - Sets the hz at which the code executes. `(int)` :: Default: 10
* `-d` or `--duration` – Sets the amount of seconds the code will run. `(int)` :: Default: 5
* `-o` or `--occupy` – Used to set the percentage of timeslice to occupy with pi calculations. `(int)` :: Default: 80
* `-p` or `--pi` – Used to set the limited amount of pi decimals to calculate per timeslice. `(int)`
* `-v` or `--verbose` – Extended verbose. `(none)`

### Examples

Occupy scenario with 80% of timeslice occupied with pi calculations:
```bash
./timetrigger --freq 10 --verbose --occupy 80
```

Pi limited scenario with 1000 decimals per timeslice:
```bash
./timetrigger --freq 10 --verbose --pi 1000
```
