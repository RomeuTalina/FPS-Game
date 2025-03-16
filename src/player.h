#ifndef PLAYER

#include <raylib.h>

typedef struct{
    Camera3D camera;
    float mass;
    float speed;
    double mouseSensitivity;
    Vector2 rotation;
    Vector3 pos;
    Vector3 vel;
    Vector3 direction;
    bool isGrounded;
}Player;

Player initPlayer(double *dt, double sens);

void updatePlayer(Player *player);

void updatePlayerCamera(Player *player);

#endif
