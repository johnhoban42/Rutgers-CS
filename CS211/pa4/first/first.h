#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

struct Metadata{
    size_t tag;
    int valid;
    int insert_time;
    int last_use;
};
typedef struct Metadata Metadata;

struct Cache{
    // Cache parameters
    int size;
    int sets;
    int block_size;
    char* policy;
    char* associativity;
    int assoc_n;
    int prefetch_size;
    // Metadata
    Metadata** data;
    // Replacement policy data
    int inserts;
    int uses;
};
typedef struct Cache Cache;

Cache initialize_cache(int size, int block_size, char* policy,
char* associativity, int prefetch_size);

// In this file, mr = memory reads, mw = memory writes,
// ch = cache hits, cm = cache misses.
void read(Cache* c, size_t address, int* mr, int* ch, int* cm);
void write(Cache* c, size_t address, int *mr, int* mw, int* ch, int* cm);

// Globals for indexing into the cache that should be computed only once
int block_bits;
int set_bits;


int log_2(int x){
    int l = 0;
    while(x > 1){
        if(x % 2 == 1){return -1;} // check for bad values
        x = x >> 1;
        l++;
    }
    return l;
}

Cache initialize_cache(int size, int block_size, char* policy,
char* associativity, int prefetch_size){

    // Check for bad inputs first; if so, return nothing
    if(log_2(size) == -1){
        printf("error");
        exit(0);
    }
    if(log_2(block_size) == -1){
        printf("error");
        exit(0);
    }
    if(!(strcmp(policy, "fifo") == 0) && !(strcmp(policy, "lru") == 0)){
        printf("error");
        exit(0);
    }

    Cache c;
    // Initialize parameters
    c.size = size;
    c.block_size = block_size;
    c.policy = policy;
    c.associativity = associativity;
    if(strcmp(associativity, "direct") == 0){
        c.assoc_n = 1; // Direct mapped caches have associativty 1
    }else if(strcmp(associativity, "assoc") == 0){
        c.assoc_n = size / block_size;
    }else{
        char* str_n = malloc(10*sizeof(char));
        strcpy(str_n, associativity+6);
        c.assoc_n = atoi(str_n);
        free(str_n);
    }
    if(log_2(c.assoc_n) == -1){
        printf("error");
        exit(0);
    }
    c.sets = size / (block_size * c.assoc_n);
    c.prefetch_size = prefetch_size;

    // Initialize metadata
    c.data = malloc(c.sets*sizeof(Metadata*));
    for(int i = 0; i < c.sets; i++){
        c.data[i] = malloc(c.assoc_n*sizeof(Metadata));
        for(int j = 0; j < c.assoc_n; j++){
            c.data[i][j].valid = 0;
            c.data[i][j].insert_time = -1;
            c.data[i][j].last_use = -1;
        }
    }

    c.inserts = -1;
    c.uses = -1;

    // Calculate indexing parameters
    block_bits = log_2(c.block_size);
    set_bits = log_2(c.sets);

    return c;

}


void insert(Cache* c, size_t address, bool prefetch, int *mr){
    // Find indexing information
    //int block_offset = address % c->block_size;
    int set_index = (address >> block_bits) % c->sets;
    size_t tag = address >> (block_bits + set_bits);

    // Look for an empty block in the appropriate set. If found, insert there
    bool inserted = false;
    int b = 0;
    while(!inserted && b < c->assoc_n){
        // Make sure this block isn't already in the cache. If so, break
        if(c->data[set_index][b].tag == tag){
            return;
        }
        if(c->data[set_index][b].valid == 0){
            c->data[set_index][b].tag = tag;
            c->data[set_index][b].valid = 1;
            c->inserts++;
            c->data[set_index][b].insert_time = c->inserts;
            c->uses++;
            c->data[set_index][b].last_use = c->uses;
            inserted = true;
        }
        b++;
    }
    // If not found, replace an existing block based on the cache's
    // replacement policy
    if(!inserted){
        int target = 0;
        for(int i = 1; i < c->assoc_n; i++){
            if(strcmp(c->policy, "fifo") == 0){
                if(c->data[set_index][i].insert_time
                    < c->data[set_index][target].insert_time){
                    target = i;
                }
            }else{ // LRU
                if(c->data[set_index][i].last_use
                    < c->data[set_index][target].last_use){
                    target = i;
                }
            }
        }
        c->data[set_index][target].tag = tag;
        c->inserts++;
        c->data[set_index][target].insert_time = c->inserts;
        c->uses++;
        c->data[set_index][target].last_use = c->uses;
    }

    (*mr)++;

    // Prefetch by inserting the next c->prefetch_size blocks
    if(prefetch){
        for(int i = 1; i < c->prefetch_size+1; i++){
            insert(c, (address)+(i*c->block_size), false, mr);
        }
    }
    return;
}


void read(Cache* c, size_t address, int* mr, int* ch, int* cm){
    // Find indexing information
    //int block_offset = address % c->block_size;
    int set_index = (address >> block_bits) % c->sets;
    size_t tag = address >> (block_bits + set_bits);

    // Access cache at appropriate index and look for a hit in all blocks
    for(int i = 0; i < c->assoc_n; i++){
        if(c->data[set_index][i].tag == tag && c->data[set_index][i].valid == 1){
            (*ch)++;
            //printf("Cache hit %d\n", *ch);

            // Update last use policy
            c->uses++;
            c->data[set_index][i].last_use = c->uses;
            return;
        }
    }

    // If there's no hit, register the miss and insert the new block into
    // the cache
    (*cm)++;
    //printf("Cache miss %d\n", *cm);
    insert(c, address, true, mr);

    return;
}


void write(Cache* c, size_t address, int* mr, int* mw, int* ch, int* cm){
    // Find indexing information
    //int block_offset = address % c->block_size;
    int set_index = (address >> block_bits) % c->sets;
    size_t tag = address >> (block_bits + set_bits);

    // Access cache at appropriate index and look for a hit in all blocks
    for(int i = 0; i < c->assoc_n; i++){
        if(c->data[set_index][i].tag == tag && c->data[set_index][i].valid == 1){
            (*ch)++;
            //printf("Cache hit %d\n", *ch);
            (*mw)++;

            // Update last use policy
            c->uses++;
            c->data[set_index][i].last_use = c->uses;
            return;
       }
    }

    // If there's no hit, register the miss and insert the new block into
    // the cache
    (*cm)++;
    //printf("Cache miss %d\n", *cm);
    insert(c, address, true, mr);
	(*mw)++;
    return;
}
