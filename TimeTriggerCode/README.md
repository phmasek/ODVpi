# TimeTriggerCode
This component is run as slave for supercomponent. Code based on [time trigger tutorial](https://github.com/se-research/OpenDaVINCI/tree/master/tutorials/timetrigger).

## Compiling
```bash
g++ -I /usr/include/opendavinci -c MyTimeTrigger.cpp -o MyTimeTrigger.o
g++ -o timetrigger MyTimeTrigger.o -lopendavinci -lpthread -lrt
```

## Executing
**Make sure** that supercomponent is running before executing this component.

The code runs with extended flags set to default values and execution mode `occupy`.

Inherited Flags (OpenDaVinci):
* `--freq=` (required)
* `--cid=` (required)
* `--realtime=` – Sets scheduler priority (49 for RT).
* `--verbose=` – Verbose provided by OpenDaVinci

Extended Flags:
* `-oc` or `--occupy` – Used to set the percentage of timeslice to occupy with pi calculations. `(int)` :: Default: 80
* `-p` or `--pi` – Used to set the limited amount of pi decimals to calculate per timeslice. `(int)`
* `-v` or `--verbose` – Extended verbose. `(int)` `1` or `2`
 * Verbose mode 1: Default. (Showing pi calculations within timeslice for *occupy* mode)
 * Verbose mode 2: Time. (Showing duration of timeslice for *both* modes)

### Examples

Occupy scenario with 80% of timeslice occupied with pi calculations and verbose enabled:
```bash
./timetrigger --cid=111 --freq=10 --realtime=49 --verbose --occupy 80
```

Pi limited scenario with 1000 decimals per timeslice and verbose disabled:
```bash
./timetrigger --cid=111 --freq=10 --realtime=49 --pi 1000
```
