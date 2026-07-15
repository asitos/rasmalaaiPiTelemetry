# rasmalaaiPiTelemetry

a small c++ script to keep an eye on my raspberry pi. it reads directly from `/proc` and `/sys` to show basic stats in the terminal. nothing fancy, just a quick way to check on things without installing bloated monitoring tools.

## project structure
* `telemetry-01/`: initial version using standard parsing.
* `telemetry-02/`: optimized version incorporating refined network parsing and VFS disk space monitoring.

it tracks:
- cpu temp (turns yellow if it gets over 70°c)
- ram usage
- system load & uptime
- network i/o (rx/tx on wlan0 or eth0)
- root storage space

## how to run

no external dependencies needed. just compile and run it.

```bash
# example for version 02
# cd into telemetry-02
cd telemetry-02

#compile with -O2 optimization (can use -Os too for space optimisation)
g++ -O2 telemetry-02.cpp -o updatedTelemetry
./updatedTelemetry
```

it updates every second, press ctrl+c to exit

## learning note
i learnt about optimisaton flags for gcc while compiling this for my rpi, and realised how there's different flags for each use case, O2, O3, Os, and march=native.

since its a small sized system, i realised the -Os flag would be better to use, compared to the recommended -O2 flag for optimisation for space,takes insignificant extra time to compile, but requires less resources upon execution of the compiled program.
