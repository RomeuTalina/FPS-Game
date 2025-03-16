#include <raylib.h>
#include <stdbool.h>

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

TargetMap *initMap(int len);


int getIdx(TargetMap *map, Target *target);


void putPair(TargetMap *map, Target target, int idx);


bool isTaken(TargetMap *map, int idx);


void draw(Target target);


void generateTarget(TargetMap *map, float grid[12][3]);


void changePosition(TargetMap *map, Target *target, float grid[12][3]);


#endif /* ifndef TARGET */
