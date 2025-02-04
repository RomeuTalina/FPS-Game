#include <raylib.h>
#include "player.h"

double deltaTime;
float drag;

int main(){


    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Teste 3D");

    //const Model model = LoadModel("C:/dev/Aim/models/bugatti.obj");

    Player player = initPlayer();

    DisableCursor();

    SetTargetFPS(60);

    while(!WindowShouldClose()){

        deltaTime = GetFrameTime();

        updatePlayer(&player);

        updatePlayerCamera(&player);

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(player.camera);
        DrawCube((Vector3) {0, 0.5f, 0}, 1.0f, 1.0f, 1.0f, RED);
        DrawCubeWires((Vector3) {0, 0.5f, 0}, 1.0f, 1.0f, 1.0f, BLACK);
        DrawCube((Vector3) {0, 1.5f, 0}, 1.0f, 1.0f, 1.0f, RED);
        DrawCubeWires((Vector3) {0, 1.5f, 0}, 1.0f, 1.0f, 1.0f, BLACK);
        //DrawModel(model, (Vector3) {10, 1, 0}, 1, WHITE);

        DrawCylinder((Vector3) {0, -1, 0}, 100, 100, 0.5f, 20, GRAY);

        EndMode3D();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
