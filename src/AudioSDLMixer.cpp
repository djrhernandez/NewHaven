// Created by Junior on 6/14/2016.
#ifdef AUDIO_SDLMIXER
#include "AudioSDLMixer.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "extern.h"
#include "define.h"
#include "Config.h"
#include "Global.h"

#ifndef _WIN32
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#endif // _WIN32


AudioSDLMixer::AudioSDLMixer() : Audio()
{
	Config	*config = Config::instance();
	//If initSound fails, config->getAudio() will be set to false
	if(config->getAudio())
		initSound();
}

AudioSDLMixer::~AudioSDLMixer()
{
	Config	*config = Config::instance();
	if(config->getAudio())
	{
    	for (int i = 0; i < NumSoundTypes; i++)
        	Mix_FreeChunk(sounds[i]);
    	Mix_CloseAudio ();
	}
}


//Opens audio device and load sounds
void	AudioSDLMixer::initSound()
{
	Config	*config = Config::instance();
    if (Mix_OpenAudio(22050, AUDIO_S16, 2, 512) < 0) {
		fprintf(stderr, "ERROR initializing audio: AudioSDLMixer::initSound()\n");
        config->setAudio(false);
	} else {
		for (int i = 0; i < NumSoundTypes; i++)
    		sounds[i] = Mix_LoadWAV(dataLoc(fileNames[i]));
		Mix_ReserveChannels(1); //Channel 0 is for music
		atexit (Mix_CloseAudio);
	}
}


//Plays the desired sound
void	AudioSDLMixer::playSound(SoundType type, float pos[3], int)
{
	Config	*config = Config::instance();
	if (config->getAudio())
	{
		Mix_PlayChannel(-1, sounds[type], 0);
    }
}

//Pauses the music channel.
//If CDROM enabled, call Audio::pauseGameMusic();
void AudioSDLMixer::pauseGameMusic(bool status)
{
	Config	*config = Config::instance();
	if (config->getAudio())	{
		if (cdrom) {
			Audio::pauseGameMusic(status);
		} else {
	    	if (status)
				Mix_PauseMusic();
			else
				Mix_ResumeMusic();
		}
	}
}


//Stops the music channel.
//If CDROM enabled, call Audio::stopMusic();
void AudioSDLMixer::stopMusic()
{
	Config	*config = Config::instance();
    if (config->getAudio())	{
		Audio::stopMusic();
		Mix_HaltChannel(0);
	}
}

void AudioSDLMixer::setMusicMode(SoundType mode)
{
	Config	*config = Config::instance();
    if (config->getAudio())	{
		Audio::setMusicMode(mode);
		switch(mode)
		{
			default:
			case MusicGame:
				if (cdrom)
					Mix_HaltChannel(0);
				else
					Mix_PlayChannel(0, sounds[mode], -1);
				break;
			case MusicMenu:
				Mix_PlayChannel(0, sounds[mode], -1);
				break;
		}
	}
}

//Sets the volume for music channel
void AudioSDLMixer::setMusicVolume(float value)
{
	Config	*config = Config::instance();
    if (config->getAudio())	{
		Mix_Volume(0, (int)(MIX_MAX_VOLUME*value));
	}
}

//Sets the volume for sounds
void	AudioSDLMixer::setSoundVolume(float value)
{
	Config	*config = Config::instance();
    if (config->getAudio()) {
		for (int i = 1; i < MIX_CHANNELS; i++)
			Mix_Volume(i,(int)(MIX_MAX_VOLUME*value));
	}
}

#endif // AUDIO_SDLMIXER