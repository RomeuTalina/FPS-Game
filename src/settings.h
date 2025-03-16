#include <stdbool.h>

typedef struct{
    double mouseSensitivity;
    double audioVolume;
    bool fullscreen;
}UserSettings;

extern UserSettings userSettings;

int resetSettings();

int loadSettings(UserSettings *settings, char *userSettings);

