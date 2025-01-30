#include <raylib.h>
#include "raymath.h"

typedef struct{
    Camera3D camera;
    float mass;
    Vector3 pos;
    Vector3 acc;
    Vector3 vel;
}Player;

void applyForceToPlayer(Player *player, Vector3 force){
    float mass = player->mass;
    player->acc = Vector3Add(player->acc, Vector3Divide(force, (Vector3) {mass, mass, mass}));
}

void playerMove(Player *player, float dt){
    player->vel = Vector3Add(player->vel, Vector3Multiply(player->acc, (Vector3) {dt, dt, dt}));
    player->pos = Vector3Add(player->pos, Vector3Scale(player->vel, dt));
    player->acc = (Vector3) {0, 0, 0};
} 

void updatePlayerCamera(Player *player){
    UpdateCamera(&player->camera, CAMERA_FIRST_PERSON);
    player->camera.position = player->pos;
}

int main(){

    const int screenWidth = 1200;
    const int screenHeight = 800;
   
    InitWindow(screenWidth, screenHeight, "Teste 3D");

    Camera3D camera = {0};
    camera.position = (Vector3) {10.0f, 10.0f, 10.0f};
    //camera.target = (Vector3) {0.0f, 0.0f, 0.0f};
    camera.up = (Vector3) {0.0f, 1.0f, 0.0f}; 
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    const float g = 9.8f;
    const float playerMass = 65; //Kg

    Player player = (Player) {camera, playerMass, (Vector3) {10, 10, 10}, (Vector3) {0, 0, 0}, (Vector3) {0, 0, 0}};
    
    double deltaTime = 0;

    Vector3 cubePosition = (Vector3) {0.0f, 0.0f, 0.0f};

    DisableCursor();

    SetTargetFPS(60);

    while(!WindowShouldClose()){

        deltaTime = GetFrameTime();

        updatePlayerCamera(&player);

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(player.camera);

        Vector3 playerGForce = (Vector3) {0, -g * playerMass, 0};

        if(player.pos.y > 1.0f){
            applyForceToPlayer(&player, playerGForce);
            playerMove(&player, deltaTime);
        }

        DrawCube(cubePosition, 1.0f, 1.0f, 1.0f, RED);
        DrawCubeWires(cubePosition, 1.0f, 1.0f, 1.0f, BLACK);

        EndMode3D();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
