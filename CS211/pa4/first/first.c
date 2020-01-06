#include "first.h"


int main(int argc, char** argv){

    // Initialize caches. If inputs are invalid, terminate immediately
    Cache prefetcher = initialize_cache(atoi(argv[1]), atoi(argv[2]), argv[3],
        argv[4], atoi(argv[5]));
    Cache non_prefetcher = initialize_cache(atoi(argv[1]), atoi(argv[2]), argv[3],
        argv[4], 0);

    FILE* f = fopen(argv[6], "r");
	if(f == NULL){
		printf("error");
		exit(0);
	}

    // Prefetcher counters
    int P_memory_reads = 0;
    int P_memory_writes = 0;
    int P_cache_hits = 0;
    int P_cache_misses = 0;

    // Non-prefetcher counters
    int NP_memory_reads = 0;
    int NP_memory_writes = 0;
    int NP_cache_hits = 0;
    int NP_cache_misses = 0;

    // Run simulations in parallel
    char c;
    size_t address;
    int i = 0;
    while(fscanf(f, "%c %zx\n", &c, &address) == 2){
    //while(i < 100){
        //fscanf(f, "%c %lx\n", &c, &address);
        if(c == 'R'){
            read(&prefetcher, address, &P_memory_reads, &P_cache_hits, &P_cache_misses);
            read(&non_prefetcher, address, &NP_memory_reads, &NP_cache_hits, &NP_cache_misses);
        }else if(c == 'W'){
            write(&prefetcher, address, &P_memory_reads, &P_memory_writes, &P_cache_hits, &P_cache_misses);
            write(&non_prefetcher, address, &NP_memory_reads, &NP_memory_writes, &NP_cache_hits, &NP_cache_misses);
        // If neither R or W are scanned, we reached "#eof" and we're done
        }else{
            break;
        }
        //printf("%d\n", i);
        i++;
    }
    fclose(f);

    // Output results
    printf("no-prefetch\n");
    printf("Memory reads: %d\n", NP_memory_reads);
    printf("Memory writes: %d\n", NP_memory_writes);
    printf("Cache hits: %d\n", NP_cache_hits);
    printf("Cache misses: %d\n", NP_cache_misses);
    printf("with-prefetch\n");
    printf("Memory reads: %d\n", P_memory_reads);
    printf("Memory writes: %d\n", P_memory_writes);
    printf("Cache hits: %d\n", P_cache_hits);
    printf("Cache misses: %d\n", P_cache_misses);
    return 0;

}
