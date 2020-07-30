#include "playback.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

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

bool init_playback(char *filename) {

  result = ma_decoder_init_file(filename, NULL, &decoder);
  if(result != MA_SUCCESS) {
    printf("no sunch file or directory\n");
    return false;
  } else {
    printf("File loaded\n");
  }

  /* setting up */
  device_config = ma_device_config_init(ma_device_type_playback);
  device_config.playback.format = decoder.outputFormat;
  device_config.playback.channels = decoder.outputChannels;
  device_config.sampleRate = decoder.outputSampleRate;
  device_config.dataCallback = data_callback;
  device_config.pUserData = &decoder;

  printf("PLAYBACK INFO:\n");
  printf("\tchannels: %d\n", device_config.playback.channels);
  printf("\tsample rate: %d\n", device_config.sampleRate);

  if(ma_device_init(NULL, &device_config, &device) != MA_SUCCESS) {
    printf("Failed to open playback device\n");
    return false;
  } else {
    printf("Device initialized\n");
  }
}

void start_file(void) {
  if(ma_device_start(&device) != MA_SUCCESS) {
    printf("Failed to start playback device\n");
    return;
  } else {
    printf("Playback started\n");
  }
}

void stop_file(void) {
  if(ma_device_stop(&device) != MA_SUCCESS) {
    printf("Failed to stop playback device\n");
    return;
  } else {
    printf("Playback stoped\n");
  }
}

void ma_uninit(void) {
  ma_device_uninit(&device);
  ma_decoder_uninit(&decoder);
}