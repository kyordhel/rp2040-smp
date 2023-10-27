# RP2040-MC
SMP Exercises in C with FreeRTOS for the RP2040 using either the Raspberry Pi Pico or the UNIT dualMCU board.
Minimum setup possible (so far) using CMake.

## Contents:
- `hello-multicore`:   The Hello-World! using BOTH CORES of the RP2040 with pure FreeRTOS.
- `producer-consumer`: A simple SMP producer-consumer example.

Nothing else so far.

## Build:
1. Clone this repository, then
2. `cd rp2040-smp`
3. `cd build`
4. `cmake .. -DPICO_SDK_FETCH_FROM_GIT=1 -DFREERTOS_FETCH_FROM_GIT=1`
5. `make`
6. `find . -type f -iname "*.uf2"`

Finally upload an `uf2` file to the RP2040.

Mind that with CMake 3.16 and erlier, fetching the Pico SDK from Git may use
3GiB+ of disk space and up to an hour to download.

Sorry about that.
