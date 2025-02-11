#include <raylib.h>

#ifndef TARGET

typedef struct{
    Vector3 pos;
    float radius;
    Color color;
}Target;

void draw(Target target){
    DrawSphere(target.pos, target.radius, target.color);
}

#endif /* ifndef TARGET */
