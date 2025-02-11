#include <raylib.h>
#include <stdio.h>
#include "player.c"
#include "target.c"

double deltaTime;
float drag;

const int screenWidth = 1600;
const int screenHeight = 900;

const Target targets[4];  

int main(){

    InitWindow(screenWidth, screenHeight, "Teste 3D");

    Player player = initPlayer(&deltaTime);

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
    Image floor = LoadImage("C:\\dev\\aim\\resources\\textures\\floor.png");

    ImageFlipVertical(&floor);

    Texture2D floorTexture = LoadTextureFromImage(floor);

    SetTextureWrap(floorTexture, TEXTURE_WRAP_REPEAT);

    floorModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = floorTexture;

    Target target = (Target) {(Vector3) {0, 2.0f, 0}, 1.0f, RED};

    while(!WindowShouldClose()){

        deltaTime = GetFrameTime();

        DrawFPS(20, 20);

        updatePlayer(&player);

        updatePlayerCamera(&player);

        Ray hitscanRay = (Ray) {player.camera.position, player.direction};

        RayCollision collision = GetRayCollisionSphere(hitscanRay, target.pos, target.radius);

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if(collision.hit){
                printf("Target Hit.\n");
            }
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(player.camera);

        DrawModel(floorModel, (Vector3){0, -0.1f, 0}, 1.0f, WHITE);

        draw(target);

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
