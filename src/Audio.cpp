// Created by Junior on 6/14/2016.
#include "Audio.h"
#include "Config.h"
#include "Global.h"

#if !defined(USE_SDL) && defined(SDL_CD_STUBS)
void	SDL_CDPlayTracks(void*, int, int, int, int) {}
void	SDL_CDPause(void*)	{}
void	SDL_CDResume(void*)	{}
void	SDL_CDStop(void*)	{}
int		SDL_CDStatus(void*)	{ return 0; }
#endif //USE_SDL

Audio::Audio()
{
	Config *config = Config::instance();

	fileNames[HeroAmmo00] = "sounds/HeroAmmo00.wav";
	fileNames[PowerUp] = "sounds/HeroAmmo00.wav";
	fileNames[Explosion] = "sounds/HeroAmmo00.wav";
	fileNames[ExploPop] = "sounds/HeroAmmo00.wav";
	fileNames[ExploBig] = "sounds/HeroAmmo00.wav";
	fileNames[LoseLife] = "sounds/HeroAmmo00.wav";
	fileNames[AddLife] = "sounds/HeroAmmo00.wav";
	fileNames[MusicGame] = "sounds/HeroAmmo00.wav";
	fileNames[MusicMenu] = "sounds/HeroAmmo00.wav";

	cdrom = NULL;
	musicMax = 1;
	musicIndex = 0;

	if (config->audioEnabled())
	{
		initCDROM();
	}
}

Audio::~Audio()
{
	if (cdrom) {
		SDL_CDStop(cdrom);
	}
}

//No-Ops
void Audio::update() {}
void Audio::playSound(SoundType, float, int) {}
void Audio::initSound() {}
void Audio::setMusicMode(SoundType) {}
void Audio::setMusicVolume(float) {}
void Audio::setSoundVolume(float) {}

/* If SDL is used as the windowing toolkit,
 * check availability of CDROM for music tracks.
 *
 * Otherwise, this does nothing if GLUT is being used.
 *
*/
#ifndef USE_SDL
void Audio::initCDROM()
{
	cdrom = 0;
}
#else //USE_SDL
static const char *trackType(int t)
{
	static char buf[32];
	switch(t)
	{
		case SDL_AUDIO_TRACK:	return "audio";
		case SDL_DATA_TRACK:	return "data";
		default:
			sprintf(buf, "UNKNOWN(type=%d)", t);
			return buf;
	}
}
void Audio::initCDROM()
{
	Config	*config = Config::instance();
	int tmp;
	if(config->useCDROM())
	{
		tmp = SDL_CDNumDrives();
		fprintf(stderr, "%d CDROM drive(s).\n", tmp);
		config->setCDROMCount(tmp);
		if(tmp > 0)
		{
			cdrom = SDL_CDOpen(config->CDROMDevice());
			if(cdrom)
			{
				tmp = SDL_CDStatus(cdrom);
				if(tmp)
				{
					musicMax = cdrom->numtracks;
					fprintf(stderr, "Tracks: %d\n", cdrom->numtracks);
					int music = 0;
					for(int i = 0; i < cdrom->numtracks; i++)
					{
						fprintf(stderr, "track %2d: %s\n", i, trackType(cdrom->track[i].type));
						// SDL BUG? 'type' should only be SDL_DATA_TRACK(0x04)or SDL_AUDIO_TRACK(0x00), but
						// this is returning SDL_AUDIO_TRACK=0x02 and SDL_DATA_TRACK=0x06 on some copied CDRs!
						// Orignal CDs return correct values, but CDR duplications are off by 2!
						if(cdrom->track[i].type < SDL_DATA_TRACK)
							music++;
					}
					if(!music)
					{
						fprintf(stderr, "\n Data track(s) only. CD audio not available.\n\n");
						SDL_CDStop(cdrom);
						cdrom = 0;
					}
					else
					{
						fprintf(stderr, "\n use_cdrom enabled. Set to '0' in ~/.chromium to disable.\n");
						fprintf(stderr, " Press the \'N\' key to skip to next CD track during a game.\n\n");
					}
				}
				else
				{
					SDL_CDClose(cdrom);
					cdrom = 0;
				}
			}
			else
			{
				fprintf(stderr, "ERROR! Could not access CDROM device %d : %s\n", config->CDROMDevice(), SDL_GetError());
				SDL_ClearError();
				config->setCDROMDevice(0);
			}
		}
	}
	else
		cdrom = 0;
}
#endif // USE_SDL

//Stops CD Track
void Audio::stopMusic()
{
	Config *config = Config::instance();
	if (config->audioEnabled()){
		if (cdrom) {
			int cdromStatus = SDL_CDStatus(cdrom);
			if (status && cdromStatus == CD_PLAYING)
				SDL_CDPause(cdrom);
			else if (!status && cdromStatus == CD_PAUSED)
				SDL_CDResume(cdrom);
			else {
				fprintf(stderr, "CDROM error in "
						"AudioOpenAL::pauseGameMusic(%d)\n", (int)status);
				fprintf(stderr, "CDROM status = %d\n", cdromStatus);
			}
		}
	}
}

//Switch between Menu and Game music modes
void Audio::setMusicMode(SoundType mode)
{
	Global *game = Global::getInstance();
	Config *config = Config::instance();
	if (config->audioEnabled()) {
		int cdromStatus = 0;

		if (cdrom) {
			cdromStatus = SDL_CDStatus(cdrom);
			switch(mode)
			{
				default:
				case MusicGame:
					if (cdromStatus == CD_PAUSED && !game->game_pause)
						SDL_CDResume(cdrom);
					if (cdromStatus == CD_STOPPED) {
						SDL_CDPlayTracks(cdrom, musicIndex, 0, 1, 0);
						if (game->game_pause)
							SDL_CDPause(cdrom);
					}
					break;

				case MusicMenu:
					if (cdromStatus == CD_PLAYING)
						SDL_CDPause(cdrom);
					break;
			}
		}
	}
}

//Play CD Track at index
void Audio::setMusicIndex(int index) {
#ifdef USE_SDL
	Config	*config = Config::instance();
	if(config->audioEnabled())
	{
		bool wasPlaying = false;
		if (musicMax)
			musicIndex = index%musicMax;
		fprintf(stderr, "Audio::setMusicIndex(%d)\n", musicIndex);

		if (cdrom) {
			int cdromStatus = SDL_CDStatus(cdrom);
			if (cdromStatus == CD_PLAYING)
				wasPlaying = true;

			//if it's a data track, increment the index and try again.
			if (cdrom->track[musicIndex].type < SDL_DATA_TRACK) {
				SDL_CDStop(cdrom);
				SDL_CDPlayTracks(cdrom, musicIndex, 0, 1, 0);
				if (!wasPlaying)
					SDL_CDPause(cdrom);
			} else {
				fprintf(stderr, "Track %d is data - trying next track...\n",
				musicIndex);
				Audio::setMusicIndex(musicIndex+1);
			}
		}
	}
#endif
}

//Skip to the next CD Track
void Audio::nextMusicIndex()
{
	int i = musicIndex + 1;
	if (i >= musicIndex)
		i = 0;
	setMusicIndex(i);
}

SoundInfo::SoundInfo()
{
	type = Audio::Explosion;
	pos[0] = 0.0;
	pos[1] = 0.0;
	pos[2] = 0.0;
	age = 1;
	prev = next = 0;
}

SoundInfo::SoundInfo(Audio::SoundType t, float p[3], int a)
{
	type = t;
	pos[0] = 0.0;
	pos[1] = 0.0;
	pos[2] = 0.0;
	age = 1;
	prev = next = 0;
}