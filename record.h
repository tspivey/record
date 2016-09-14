#pragma once
bool reset_audio();
void set_audio(char *sourceId, char *deviceId, char *deviceDesc, int channel, int mixers);
void output_stopped(void *data, calldata_t *calldata);
