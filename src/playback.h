#include <stdio.h>
#include <stdbool.h>
#include <gtk/gtk.h>

bool init_playback(char *filename);

void start_file(void);
void stop_file(void);
void ma_uninit(void);