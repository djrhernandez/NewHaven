// Created by Junior on 6/13/2016.
#ifndef AUDIOSDL_H
#define AUDIOSDL_H
#ifdef AUDIO_SDLMIXER
#include <SDL_mixer.h>
#include "Audio.h"

//Use SDL_mixer for simple audio effects (don't need position or panning)
class AudioSDLMixer : public Audio
{
public:
	AudioSDLMixer();
	~AudioSDLMixer();

	virtual void	playSound(SoundType type, float *pos, int age = 0);
	virtual void	stopMusic();
	virtual void	pauseGameMusic(bool);
	virtual void	setMusicMode(SoundType);
	virtual void	setMusicVolume(float);
	virtual void	setSoundVolume(float);

protected:
	virtual void	initSound();

private:
	Mix_Chunk *sounds[NumSoundTypes];
};

#endif  //AUDIO_SDLMIXER

#endif //AUDIOSDL_H