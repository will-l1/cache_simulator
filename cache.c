#include "cache.h"
typedef struct line{
    unsigned int timer;
    unsigned char valid;
    unsigned long tag;
    unsigned char block[64];
}line;

typedef struct set{
    line* lines;
}set;

typedef struct Cache{
    unsigned char init;
    set *sets;
}Cache;

static void init(){
    Cache *cache = c_info.F_memory;
    unsigned int numLines = (c_info.F_size - sizeof(Cache) - sizeof(set)) / sizeof(line);
    cache->sets = (set*)((char*)cache+sizeof(Cache));
    cache->sets->lines = (line*)((char*)cache->sets+sizeof(set));

    for (int i = 0; i < numLines; i++) {
        line * templine = &cache->sets->lines[i];
        templine->timer = i;
        templine->valid = 0;
        templine->tag = 0;
    }
    cache->init=1;
}

extern int cache_get(unsigned long address, unsigned long *value) {
    unsigned char tempValue[8]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    *value = 0;
    Cache *cache =(Cache*)(c_info.F_memory);
    if(cache->init!=1){
        init();
    }//init  //    offset=6
    unsigned int numLines = (c_info.F_size - sizeof(Cache)) / sizeof(line);
    unsigned long tag = (address>>6)&0x3ffffffffffffff;
    unsigned int offset = address & 0x3f;

    //cache get
    for(int i = 0; i < numLines; i++){
        if(offset+8>64){//doing overflow
            unsigned int recorded = 64 - offset;
            unsigned int overlap = 8-recorded;
            cache_get(address-offset+56,value);
            unsigned long record = *(unsigned long*) value;
            record=record>>(overlap*8);
            cache_get(address-offset+64,value);
            unsigned long record1 = *(unsigned long*) value;
            record1=record1<<(recorded*8);
            record = record|record1;
            *value = record;
            return 1;
        }
        else{
            if(cache->sets->lines[i].tag == tag){
                if(cache->sets->lines[i].valid){
                    //found clone
                    for(int j=0;j<8;j++){
                        tempValue[j]=cache->sets->lines[i].block[offset + j];
                    }
                    for(int a=0;a<numLines;a++){
                        if(cache->sets->lines[a].timer<cache->sets->lines[i].timer){
                            cache->sets->lines[a].timer=cache->sets->lines[a].timer+1;
                        }
                    }
                    cache->sets->lines[i].timer=0;
                    unsigned long output=0;
                    for(int a=7;a>=0;a--){
                        output=output<<8|tempValue[a];
                    }
                    *value = output;
                    return 1;
                }
            }
        }
    }
    unsigned char buffer[64];
    memget(address-offset,buffer,64);
    for(int i = 0; i < numLines; i++){
        if(cache->sets->lines[i].timer==numLines-1){
            //replace
            cache->sets->lines[i].tag=tag;
            cache->sets->lines[i].valid=1;
            for(int j = 0; j < 64; j++){
                cache->sets->lines[i].block[j] = buffer[j];
            }
            for(int a=0;a<numLines;a++){
                cache->sets->lines[a].timer=cache->sets->lines[a].timer+1;
            }
            cache->sets->lines[i].timer=0;
            break;
        }
    }
    for(int j=0;j<8;j++){
        tempValue[j]=buffer[j+offset];
    }
    unsigned long output=0;
    for(int i=7;i>=0;i--){
        output=output<<8|tempValue[i];
    }
    *value = output;
    return 0;
}