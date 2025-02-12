#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef TARGET

#ifndef NUM_TARGETS
    #define NUM_TARGETS 4
#endif

typedef struct{
    Vector3 pos;
    float radius;
    int slot;
    Color color;
}Target;

typedef struct{
    Target *targets;
    int *idxs;
    int lastIndex;
}TargetMap;

TargetMap *initMap(int len){
    TargetMap *newMap = (TargetMap *) malloc(sizeof(TargetMap));
    newMap->idxs = (int *) malloc(len * sizeof(int));
    newMap->targets = (Target *) malloc(len * sizeof(Target));
    newMap->lastIndex = 0;
    return newMap;
}

int getIdx(TargetMap *map, Target *target){
    for(int i = 0; i < NUM_TARGETS; i++){
        if(map->targets == target){
            return map->idxs[i];
        }
    }
    return -1;
}

void putPair(TargetMap *map, Target target, int idx){
    map->targets[map->lastIndex] = target;
    map->idxs[map->lastIndex] = idx;
    map->lastIndex++;
}

bool isTaken(TargetMap *map, int idx){
    for(int i = 0; i < NUM_TARGETS; i++){
        if(map->idxs[i] == idx){
            return true;
        }
    }

    return false;
}

void draw(Target target){
    DrawSphere(target.pos, target.radius, target.color);
}

void generateTarget(TargetMap *map, float grid[12][3]){
    int idx;

    do{
       idx = rand() % 12;
    }while(isTaken(map, idx));

    Target newTarget = {(Vector3){grid[idx][0], grid[idx][1], grid[idx][2]}, 0.5, 
                        map->lastIndex, BLUE};

    putPair(map, newTarget, idx);
}

void changePosition(TargetMap *map, Target *target, float grid[12][3]){
    int freeIdxs[12];
    int freeCount = 0;

    for(int i = 0; i < 12; i++){
        if(!isTaken(map, i)){
            freeIdxs[freeCount++] = i;
        }
    }

    int idx = freeIdxs[rand() % freeCount];

    map->idxs[target->slot] = idx;

    target->pos.x = grid[idx][0];
    target->pos.y = grid[idx][1];
    target->pos.z = grid[idx][2];
}

#endif /* ifndef TARGET */
