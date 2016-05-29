#ifndef JB_AUDIO_H
#define JB_AUDIO_H

#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
#include "err_handler.h"

class Audio {
	private:
		int		channel;
		int		num_channels;
		int		sample_bitrate;
		int		sample_buffer_size;
		Uint16	sample_format;
		Mix_Chunk* chunk;

	public:
		enum Type {SFX, MUSIC};

		Audio();
		Audio(int channels, int bitrate, unsigned int byteformat);
		virtual ~Audio();
	
		bool readFile(std::string filename);

		bool play();
		bool playLoop();
		void pause();
		void resume();

		int setVolume(unsigned int level);

		static int init() {
			int ret = SDL_Init(SDL_INIT_AUDIO);
			Mix_AllocateChannels(16);
			ERR(ret != 0, ret, SDL_GetError());
			
			return ret;
		}

		static void quit() {
			SDL_Quit();
		}
};

#endif // !JB_AUDIO_H
