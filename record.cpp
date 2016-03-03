#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <obs.h>
#include "record.h"

int main()
{
	if ((obs_startup("en-us", nullptr, nullptr)) == false) {
		fprintf(stderr, "Error starting obs.\n");
		exit(1);
	}
	obs_load_all_modules();
	if (!reset_audio()) {
		fprintf(stderr, "Failed to reset audio\n");
		exit(1);
	}
//Create a source for the mic in channel 0
	set_audio("wasapi_input_capture", "default", "mic", 0);
//aac encoder
	obs_encoder_t *enc = obs_audio_encoder_create("ffmpeg_aac", "aac0", nullptr, 0, nullptr);
//And the file output
	obs_output_t *output = obs_output_create("ffmpeg_muxer", "outfile", nullptr, nullptr);
	obs_output_set_audio_encoder(output, enc, 0);
	audio_t *audio = obs_output_audio(output);
	obs_encoder_set_audio(enc, audio);

	obs_data_t *settings = obs_data_create();
	obs_data_set_string(settings, "format_name", "mkv");
	obs_data_set_string(settings, "audio_encoder", "aac");
	obs_data_set_string(settings, "path", "test.mkv");
	obs_output_update(output, settings);
	obs_data_release(settings);
	if (obs_output_can_begin_data_capture(output, 0) == false) {
		fprintf(stderr, "can't begin data capture.\n");
		exit(1);
	}
	if (obs_output_start(output) == false) {
		fprintf(stderr, "Failed to start output\n");
		exit(1);
	}

	Sleep(5000);
}

bool reset_audio()
{
	struct obs_audio_info ai;
	ai.samples_per_sec = 48000;
	ai.speakers = SPEAKERS_STEREO;
	return obs_reset_audio(&ai);
}

void set_audio(char *sourceId, char *deviceId, char *deviceDesc, int channel)
{
	obs_data_t *settings = obs_data_create();
	obs_data_set_string(settings, "device_id", deviceId);
	obs_source_t *source = obs_source_create(sourceId, deviceDesc, settings, nullptr);
	obs_data_release(settings);
	obs_set_output_source(channel, source);
	obs_source_release(source);
}
