#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <obs.h>
#include "record.h"

int main(int argc, char **argv)
{
	if (argc < 3) {
		fprintf(stderr, "Usage: %s outfile codec", argv[0]);
		exit(1);
	}
	if ((obs_startup("en-us", nullptr, nullptr)) == false) {
		fprintf(stderr, "Error starting obs.\n");
		exit(1);
	}
	obs_load_all_modules();
	if (!reset_audio()) {
		fprintf(stderr, "Failed to reset audio\n");
		exit(1);
	}
	//Create a source for the mic in channel 0, and system audio in 1
	set_audio("wasapi_input_capture", "default", "mic", 0, (1<<0));
	set_audio("wasapi_output_capture", "default", "system", 1, (1<<1));
	//Create one aac encoder per channel
	obs_data_t *settings = obs_data_create();
	obs_data_set_string(settings, "encoder", argv[2]);
	obs_encoder_t *enc0 = obs_audio_encoder_create("ffmpeg_aac", "aac0", settings, 0, nullptr);
	obs_data_release(settings);
	settings = obs_data_create();
	obs_data_set_string(settings, "encoder", argv[2]);
	obs_encoder_t *enc1 = obs_audio_encoder_create("ffmpeg_aac", "aac1", settings, 1, nullptr);
	obs_data_release(settings);
//And the file output
	obs_output_t *output = obs_output_create("ffmpeg_muxer", "outfile", nullptr, nullptr);
	obs_output_set_audio_encoder(output, enc0, 0);
	obs_output_set_audio_encoder(output, enc1, 1);
	audio_t *audio = obs_output_audio(output);
	obs_encoder_set_audio(enc0, audio);
	obs_encoder_set_audio(enc1, audio);

	settings = obs_data_create();
	obs_data_set_string(settings, "format_name", "mkv");
	obs_data_set_string(settings, "path", argv[1]);
	obs_output_update(output, settings);
	obs_data_release(settings);
	if (obs_output_can_begin_data_capture(output, 0) == false ||
	obs_output_can_begin_data_capture(output, 1) == false) {
		fprintf(stderr, "can't begin data capture.\n");
		exit(1);
	}
	if (obs_output_start(output) == false) {
		fprintf(stderr, "Failed to start output\n");
		exit(1);
	}

	while (1) {
		Sleep(5000);
	}
}

bool reset_audio()
{
	struct obs_audio_info ai;
	ai.samples_per_sec = 48000;
	ai.speakers = SPEAKERS_STEREO;
	return obs_reset_audio(&ai);
}

void set_audio(char *sourceId, char *deviceId, char *deviceDesc, int channel, int mixers)
{
	obs_data_t *settings = obs_data_create();
	obs_data_set_string(settings, "device_id", deviceId);
	obs_source_t *source = obs_source_create(sourceId, deviceDesc, settings, nullptr);
	obs_data_release(settings);
	obs_set_output_source(channel, source);
	obs_source_set_audio_mixers(source, mixers);
	obs_source_release(source);
}
