#ifndef PLAYER

#include <raylib.h>
#include "raymath.h"

typedef struct{
    Camera3D camera;
    float mass;
    float speed;
    Vector2 rotation;
    Vector3 pos;
    Vector3 vel;
    Vector3 direction;
    bool isGrounded;
}Player;

Player initPlayer(){
    Player player;
    Camera3D camera;
    float mass = 65;
    float speed = 10.0f;
    Vector2 rotation = (Vector2) {0, 0};
    Vector3 pos = (Vector3) {-10.0f, 0.0f, 0};
    Vector3 vel = (Vector3) {0, 0, 0};
    Vector3 direction = (Vector3) {0, 0, 0};
    camera.target = (Vector3) {rotation.x, pos.y + 1.7f, rotation.y};
    camera.fovy = 90.0f;
    camera.position = (Vector3) {pos.x, pos.y + 1.7f, pos.z};
    camera.up = (Vector3) {0, 1, 0}; 
    camera.projection = CAMERA_PERSPECTIVE;
    player = (Player) {camera, mass, speed, rotation, pos, vel};
    return player;
}

void updatePlayer(Player *player){

    if(player->pos.y <= 0) player->isGrounded = true;

    Vector3 *vel = &player->vel;
    Vector3 *pos = &player->pos;
    Vector2 dir = (Vector2) {player->direction.x, player->direction.z};
    float temp = 0;
    bool anyDirectionPressed = false;
    
    Vector3 targetVel = Vector3Zero();
    Vector2 horizontalVel = Vector2Zero();

    if(IsKeyDown(KEY_W)){
        anyDirectionPressed = true;
        horizontalVel = Vector2Add(horizontalVel, Vector2Scale(dir, player->speed));
    } 
    if(IsKeyDown(KEY_D)) {
        anyDirectionPressed = true;
        Vector2 dir1 = dir;
        temp = dir1.x;
        dir1.x = -dir1.y;
        dir1.y = temp;
        horizontalVel = Vector2Add(horizontalVel, Vector2Scale(dir1, player->speed));
    }
    if(IsKeyDown(KEY_A)){
        anyDirectionPressed = true;
        Vector2 dir1 = dir;
        temp = dir1.x;
        dir1.x = dir1.y;
        dir1.y = -temp;
        horizontalVel = Vector2Add(horizontalVel, Vector2Scale(dir1, player->speed));
    }
    if(IsKeyDown(KEY_S)){
        anyDirectionPressed = true;
        Vector2 dir1 = dir;
        dir1 = Vector2Scale(dir, -1);
        horizontalVel = Vector2Add(horizontalVel, Vector2Scale(dir1, player->speed));
    }

    float length = sqrtf(horizontalVel.x * horizontalVel.x + horizontalVel.y * horizontalVel.y);
    if(length > player->speed){
        horizontalVel = Vector2Scale(Vector2Normalize(horizontalVel), player->speed);
    }
    
    targetVel = (Vector3) {horizontalVel.x, 0, horizontalVel.y};

    if(player->isGrounded && IsKeyPressed(KEY_SPACE)){
        targetVel = Vector3Add(targetVel, (Vector3) {0, 2, 0});
    }
    
    anyDirectionPressed ? (*vel = Vector3Lerp(*vel, targetVel, 5.0f * deltaTime)) : 
                          (*vel = Vector3Lerp(*vel, targetVel, 6.0f * deltaTime));
    *pos = Vector3Multiply(Vector3Add(*pos, Vector3Scale(*vel, deltaTime)), (Vector3) {1, 0, 1});
}

void updatePlayerCamera(Player *player){
    
    Vector2 mouseDelta = GetMouseDelta();

    const float mouseSensitivity = 0.05f;

    player->rotation.x += mouseDelta.x * mouseSensitivity;
    player->rotation.y -= mouseDelta.y * mouseSensitivity;

    float yaw = player->rotation.x;
    float pitch = player->rotation.y;
    
    if(pitch >  89.0f) pitch =  89.0f; 
    if(pitch < -89.0f) pitch = -89.0f;

    Vector3 *dir = &player->direction;

    *dir = Vector3Normalize((Vector3) {
        cosf(DEG2RAD * yaw) * cosf(DEG2RAD * pitch),
        sinf(DEG2RAD * pitch),
        sinf(DEG2RAD * yaw) * cosf(DEG2RAD * pitch),
    });

    Vector3 pos = player->pos;

    player->camera.position = (Vector3) {pos.x, pos.y + 1.7f, pos.z};

    player->camera.target = Vector3Add(player->camera.position, (Vector3) {
        dir->x,
        dir->y,
        dir->z
    });
}

#endif
