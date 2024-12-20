# cache_simulator

## Overview

This project is a simple cache simulator written in C. It includes a main program and a cache implementation, allowing users to simulate cache memory operations, track cache hits and misses, and print relevant statistics.

## Files

- main.c: The main program that initializes memory, performs cache operations, and logs results.
- cache.c: The implementation of the cache, including functions for initializing the cache and retrieving values.

## Compilation

To compile the project, use the following command:
gcc -o cache_simulator main.c cache.c

## Usage

After compiling, you can run the simulator as follows:
./cache_simulator
The program expects three inputs:
1. The size of the fast memory (cache).
2. The size of the main memory.
3. The number of memory references to simulate.

For each memory reference, the program will output whether it was a cache hit or miss and the value retrieved from the cache.

### Example

Input:
1024      # Fast memory size (bytes)
4096      # Main memory size (bytes)
5         # Number of memory references
256       # Memory address reference
512       # Memory address reference
...
stats     # Print cache statistics

Output:
Loaded value [0x0000000000000000] @ address 0x00000100
Loaded value [0x0000000000000000] @ address 0x00000200
...
Cache hits: 3, misses: 2 -- hit rate 60%

## Code Structure

### main.c

- main(): Initializes memory and reads inputs. It performs cache operations for each reference and logs the results.
- log_result(unsigned long address): Logs whether a cache operation was a hit or miss.
- memget(unsigned int address, void *buffer, unsigned int size): Fetches data from the main memory and increments the miss counter.

### cache.c

- init(): Initializes the cache structure.
- cache_get(unsigned long address, unsigned long *value): Retrieves a value from the cache. If the value is not present, it fetches it from the main memory and updates the cache.

## Debugging

To enable debugging messages, compile with the DEBUG flag:
gcc -DDEBUG -o cache_simulator main.c cache.c

## License

This project is licensed under the MIT License.
