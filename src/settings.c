#include <stdbool.h>
#include <stdio.h>
#include "cJSON.h"

typedef struct{
    double mouseSensitivity;
    double audioVolume;
    bool fullscreen;
}UserSettings;

int resetSettings(){
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "sensitivity", 1.0f);
    cJSON_AddNumberToObject(json, "volume", 1.0f);
    cJSON_AddBoolToObject(json, "fullscreen", true);

    char *json_str = cJSON_Print(json);

    FILE *settingsPtr = fopen("UserSettings.json", "w");
    if(settingsPtr == NULL){
        printf("Error creating the settings file.");
        return 1;
    }

    fputs(json_str, settingsPtr);

    cJSON_free(json_str);
    cJSON_Delete(json);

    return 0;
}

int loadSettings(UserSettings *settings, char *userSettings){

    FILE *settingsPtr = fopen(userSettings, "r");
    if(settingsPtr == NULL){
        resetSettings();
        loadSettings(settings, userSettings);
    }

    char buffer[1024];
    int len = fread(buffer, 1, sizeof(buffer), settingsPtr);
    fclose(settingsPtr);

    cJSON *json = cJSON_Parse(buffer);
    if(json == NULL){
        const char *error_ptr = cJSON_GetErrorPtr();
        if(error_ptr != NULL){
            printf("Error: %s\n", error_ptr);
        }
        cJSON_Delete(json);
        return 1;
    }

    cJSON *sens = cJSON_GetObjectItemCaseSensitive(json, "sensitivity");
    if(cJSON_IsNumber(sens) && (sens->valuedouble > 0)){
        settings->mouseSensitivity = sens->valuedouble;
    }
    else{
        settings->mouseSensitivity = 1.0f;
    }

    cJSON *fullscreen = cJSON_GetObjectItemCaseSensitive(json, "fullscreen");
    if(cJSON_IsBool(fullscreen)){
        if(fullscreen->valueint == true){
            settings->fullscreen = true;
        }
        else{
            settings->fullscreen = false;
        }
    }
    else{
        settings->fullscreen = false;
    }

    cJSON *volume = cJSON_GetObjectItemCaseSensitive(json, "volume");
    if(cJSON_IsNumber(volume) && (volume->valuedouble > 0)){
        settings->audioVolume = volume->valuedouble;
    }
    else{
        settings->audioVolume = 0;
    }
     
    return 0;
}
