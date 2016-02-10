# TimeTriggerCode
This component is run as slave for supercomponent. Make sure that supercomponent is running before executing this.

## Compiling
```bash
g++ -I /usr/include/opendavinci -c MyTimeTrigger.cpp -o MyTimeTrigger.o
g++ -o timetrigger MyTimeTrigger.o -lopendavinci -lpthread -lrt
```

## Executing

Inherited Flags (OpenDaVinci):
* `--freq=` (required)
* `--cid=` (required)
* `--realtime=` – Sets scheduler priority (49 for RT).

Extended Flags:
* `-o` or `--occupy` – Used to set the percentage of timeslice to occupy with pi calculations. `(int)`
* `-p` or `--pi` – Used to set the limited amount of pi decimals to calculate per timeslice. `(int)`
* `-v` or `--verbose` – Extended verbose. `(none)`

### Examples

Occupy scenario with 80% of timeslice occupied with pi calculations:
```bash
./timetrigger --cid=111 --freq=10 --realtime=49 --verbose --occupy 80
```

Pi limited scenario with 1000 decimals per timeslice:
```bash
./timetrigger --cid=111 --freq=10 --realtime=49 --verbose --pi 1000
```
