#include "Audio.h"

Audio::Audio() {
	this->channel				= 0;
	this->num_channels			= 1;
	this->sample_format			= MIX_DEFAULT_FORMAT;
	this->sample_bitrate		= MIX_DEFAULT_FREQUENCY;
	this->sample_buffer_size	= 2048;
	this->chunk					= NULL;
}

Audio::Audio(int num_channels, int bitrate, unsigned int byteformat) {
	this->channel				= 0;
	this->num_channels			= num_channels;
	this->sample_format			= byteformat == 16 ? AUDIO_S16SYS : AUDIO_S8;
	this->sample_bitrate		= bitrate;
	this->sample_buffer_size	= byteformat * 256;
	this->chunk					= NULL;
}

Audio::~Audio() {
	Mix_FreeChunk(this->chunk);
	Mix_CloseAudio();
}

bool Audio::readFile(std::string filename) {
	int ret = Mix_OpenAudio(this->sample_bitrate, this->sample_format, this->num_channels, this->sample_buffer_size);

	ERR(ret != 0, false, Mix_GetError());

	this->chunk = Mix_LoadWAV(filename.c_str()); 

	ERR(!this->chunk, false, Mix_GetError());

	return true;
}

bool Audio::play() {
	this->channel = Mix_PlayChannel(-1, this->chunk, 0);

	ERR(this->channel < 0, false, Mix_GetError());

	return true;
}

bool Audio::playLoop() {
	this->channel = Mix_PlayChannel(-1, this->chunk, -1);

	ERR(this->channel < 0, false, Mix_GetError());

	return true;
}

void Audio::pause() {
	Mix_Pause(this->channel);
}

void Audio::resume() {
	Mix_Resume(this->channel);
}

int Audio::setVolume(unsigned int level) {
	return Mix_Volume(this->channel, 128 * (level > 100 ? 100 : level) / 100);
}
