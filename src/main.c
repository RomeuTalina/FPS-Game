#include <raylib.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "player.c"
#include "target.c"
#include "cJSON.c"
#include "settings.c"

#define NUM_TARGETS 4

double deltaTime;
float drag;

const int screenWidth = 1600;
const int screenHeight = 900;

UserSettings userSettings;  

float targetCoordinates[12][3] = {
    {0, 4, -3}, {0, 4, -1.5}, {0, 4, 1.5}, {0, 4, 3}, 
    {0, 2.5, -3}, {0, 2.5, -1.5}, {0, 2.5, 1.5}, {0, 2.5, 3}, 
    {0, 1, -3}, {0, 1, -1.5}, {0, 1, 1.5}, {0, 1, 3}
};

int main(){

    loadSettings(&userSettings, "UserSettings.json");

    printf("Sensitivity: %lf", userSettings.mouseSensitivity);

    RLAPI const char *workDir = GetWorkingDirectory();
    RLAPI const char *appDir = GetApplicationDirectory();

    TargetMap *targets = initMap(NUM_TARGETS);

    srand(time(NULL));

    for(int i = 0; i < NUM_TARGETS; i++){
        generateTarget(targets, targetCoordinates);
    }

    InitWindow(screenWidth, screenHeight, "Aim Trainer");

    if(userSettings.fullscreen){
        ToggleFullscreen();
    }

    Player player = initPlayer(&deltaTime, userSettings.mouseSensitivity);

    DisableCursor();

    SetTargetFPS(144);

    Mesh floorMesh = GenMeshCube(100, 0.2, 50);
    Model floorModel = LoadModelFromMesh(floorMesh);

    for(int i = 0; i < floorModel.meshCount; i++){
        Mesh *mesh = &floorModel.meshes[i];
        float *texcoords = mesh->texcoords;
        float repeatFactorU = 16.0f;
        float repeatFactorV = 16.0f;
        for(int j = 0; j < mesh->vertexCount; j++){
            texcoords[j * 2] *= repeatFactorU;
            texcoords[j * 2 + 1] *= repeatFactorV;
        }
        UpdateMeshBuffer(*mesh, 1, texcoords, mesh->vertexCount * 2 * sizeof(float), 0);
    }

    //Must load each texture as an image first, then flip them before turning
    //them into textures, otherwise raylib will render them upside down.
    /*Image floor = LoadImage("/home/romeu/dev/aim/resources/textures/floor.png");*/
    Image floor = LoadImage("./resources/textures/floor.png");

    ImageFlipVertical(&floor);

    Texture2D floorTexture = LoadTextureFromImage(floor);

    SetTextureWrap(floorTexture, TEXTURE_WRAP_REPEAT);

    floorModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = floorTexture;

    while(!WindowShouldClose()){

        deltaTime = GetFrameTime();

        DrawFPS(20, 20);

        updatePlayer(&player);

        updatePlayerCamera(&player);

        Ray hitscanRay = (Ray) {player.camera.position, player.direction};

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            for(int i = 0; i < NUM_TARGETS; i++){
                RayCollision collision = GetRayCollisionSphere(hitscanRay, targets->targets[i].pos, targets->targets[i].radius);
                if(collision.hit){
                    changePosition(targets, &targets->targets[i], targetCoordinates); 
                    break;
                }
            }
            
        }

        if(IsKeyPressed(KEY_F11)){
            ToggleFullscreen();
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(player.camera);

        DrawModel(floorModel, (Vector3){0, -0.1f, 0}, 1.0f, WHITE);

        for(int i = 0; i < NUM_TARGETS; i++){
            draw(targets->targets[i]);
        }

        EndMode3D();

        Vector2 crosshairCoords = (Vector2) {screenWidth/2.0f, screenHeight/2.0f};

        DrawRectangle(crosshairCoords.x - 7, crosshairCoords.y - 1, 5, 2, RED);
        DrawRectangle(crosshairCoords.x + 2, crosshairCoords.y - 1, 5, 2, RED);
        DrawRectangle(crosshairCoords.x - 1, crosshairCoords.y - 7, 2, 5, RED);
        DrawRectangle(crosshairCoords.x - 1, crosshairCoords.y + 2, 2, 5, RED);

        EndDrawing();
    }

    UnloadTexture(floorTexture);
    UnloadModel(floorModel);

    CloseWindow();

    return 0;
}
