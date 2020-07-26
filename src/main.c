#include <gtk/gtk.h>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#define MUSICFILENAME "music.mp3"

/* Global variables */
/* MiniAudio init */
ma_result result;
ma_decoder decoder;
ma_device_config device_config;
ma_device device;

void data_callback(ma_device* device, void* output, const void* input, ma_uint32 frame_count) {
  ma_decoder *decoder = (ma_decoder*)device->pUserData;
  if(decoder == NULL) {
    printf("Decoder error\n");
    return;
  }

  ma_decoder_read_pcm_frames(decoder, output, frame_count);

  (void)input;
}

void stop_playback() {
  if(ma_device_stop(&device) != MA_SUCCESS) {
    printf("Unable stop playback\n");
  }
}

void start_playback() {
  if(ma_device_start(&device) != MA_SUCCESS) {
    printf("Unable start playback\n");
  }
}

/* Destory window and quit */
void window_destroy(void) {
  ma_device_uninit(&device);
  ma_decoder_uninit(&decoder);
  gtk_main_quit();
}

int main(int argc, char** argv) {

  /* GTK init */
  GError *error = NULL;

  GtkBuilder  *builder;
  GtkWidget   *window;

  gtk_init(&argc, &argv);

  /* miniaudio file init */
  result = ma_decoder_init_file(MUSICFILENAME, NULL, &decoder);
  if(result != MA_SUCCESS) {
    printf("no sunch file or directory\n");
    return -1;
  }

  /* setting up */
  device_config = ma_device_config_init(ma_device_type_playback);
  device_config.playback.format = decoder.outputFormat;
  device_config.playback.channels = decoder.outputChannels;
  device_config.sampleRate = decoder.outputSampleRate;
  device_config.dataCallback = data_callback;
  device_config.pUserData = &decoder;

  /* show info */
  printf("channels: %d\n", device_config.playback.channels);
  printf("sample rate: %d\n", device_config.sampleRate);

  if(ma_device_init(NULL, &device_config, &device) != MA_SUCCESS) {
    printf("Failed to open playback device\n");
    return -1;
  }

  builder = gtk_builder_new();
  if(0 == gtk_builder_add_from_file(builder, "data/ui/music.glade", &error)) {
    printf("Error adding build from file. Error: %s\n", error->message);
  }

  window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
  if(window == NULL) {
    printf("Error\n");
  }

  if(ma_device_start(&device) != MA_SUCCESS) {
    printf("Failed to start playback device\n");
    return -1;
  }

  gtk_builder_connect_signals(builder, NULL);

  g_object_unref(builder);

  gtk_widget_set_size_request(window, 400, 600);

  gtk_widget_show(window);
  gtk_main();

  return 0;
}