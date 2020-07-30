#include "main_window.h"
#include "playback.h"
#include <taglib/tag_c.h>

/* Music path */
char *filename = NULL;
char *title = NULL;

GtkWidget *window = NULL;
GtkLabel *label = NULL;
GtkLabel *artist_label = NULL;
GtkLabel *album_label = NULL;

void read_tags(char* filename) {
  TagLib_File *file = NULL;
  TagLib_Tag *tag = NULL;
  const TagLib_AudioProperties *properties = NULL;

  file = taglib_file_new(filename);
  if(file == NULL) {
    printf("Unable to read file\n");
  }

  tag = taglib_file_tag(file);
  properties = taglib_file_audioproperties(file);

  gtk_label_set_text(label, taglib_tag_title(tag));
  gtk_label_set_text(artist_label, taglib_tag_artist(tag));
  gtk_label_set_text(album_label, taglib_tag_album(tag));
}

void setup_main_window(void) {
  GError *error = NULL;
  GtkBuilder *builder = NULL;
  
  builder = gtk_builder_new();
  if(gtk_builder_add_from_file(builder, "data/ui/music.glade", &error) == 0) {
    printf("Error adding build from file. Error: %s\n", error->message);
    exit(1);
  }

  window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
  if(window == NULL) {
    printf("Could not find main_window object\n");
  }

  label = GTK_LABEL(gtk_builder_get_object(builder, "song_name"));
  if(label == NULL) {
    printf("Cound not find song_name object\n");
  }

  artist_label = GTK_LABEL(gtk_builder_get_object(builder, "artist"));
  if(artist_label == NULL) {
    printf("Cound not find artist_label object\n");
  }

  album_label = GTK_LABEL(gtk_builder_get_object(builder, "album"));
  if(album_label == NULL) {
    printf("Cound not find album_label object\n");
  }

  gtk_builder_connect_signals(builder, NULL);

  g_object_unref(builder);

  gtk_widget_set_size_request(window, 400, 600);

  gtk_widget_show(window);
  /* Runs the main loop until gtk_main_quit() is called. */
  gtk_main();
}

void open_file(void) {
  GtkWidget *dialog;
     
  dialog = gtk_file_chooser_dialog_new ("Open File", window, GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);
     
  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
    
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

    printf("Path: %s\n", filename);
    
    read_tags(filename);

    if(init_playback(filename) != true) {
      printf("Error in playback file\n");
    }
  }
     
  gtk_widget_destroy (dialog);
}

/* Play button pressed */
void start_playback(void) {
  if(filename != NULL) {
    start_file();
  } else {
    printf("Path: not found\n");
  }
}

/* Stop button pressed */
void stop_playback(void) {
  if(filename != NULL) {
    stop_file();
  } else {
    printf("Path: not found\n");
  }
}

/* Destory window and quit */
void window_destroy(void) {
  ma_uninit();
  g_free(filename);
  gtk_main_quit();
}