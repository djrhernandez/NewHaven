/* Created by Junior on 6/14/2016.
 *
 * 6/14: Might need to update OpenAL functionality due to deprecation
 * of older function calls.
 *
 *
 *
*/
#ifdef AUDIO_OPENAL
#include "AudioOpenAL.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include "Config.h"
#include "extern.h"
#include "define.h"
#include "Global.h"

#ifdef macintosh
	#include <MacHeaders.h>
	void	*alcCreateContext(int) { alInit(NULL); return (void*)1; }
	void	alcDestroyContext(void*) { alExit(); }
#endif //macintosh

#ifndef _WIN32
	#include <unistd.h>
	#include <errno.h>
	#include <fcntl.h>
	#include <sys/stat.h>
	#include <sys/types.h>
#endif //_WIN32

#ifdef OLD_OPENAL

#ifdef USE_SDL
	//Try to use OpenAL alc[GS]etAudioChannel extensions in linux...
	#ifdef __linux__
		#define CD_VOLUME 1
		//#include <AL/alext.h>
		#include <AL/alkludgetypes.h>
	#endif //__linux__
#endif //USE_SDL
#else
#ifdef USE_SDL
	//Try to use OpenAL alc[GS]etAudioChannel extensions in linux...
	#ifdef __linux__
		#define CD_VOLUME 1
		//#include <AL/alext.h>
		#include <AL/alexttypes.h>
	#endif //__linux__
#endif //USE_SDL

#endif

#ifdef __linux__
#define USE_PLAYLIST 1
#endif

AudioOpenAL::AudioOpenAL() : Audio()
{
	Config	*config = Config::instance();
	int i;
	context_id	= 0;
	initialized = false;
	explosionIndex = 0;
	exploPopIndex = 0;
	origCDvolume = 0.7;

	//OpenAL extensions
	alAttenuationScale	= 0;
	alcSetAudioChannel	= 0;
	alcGetAudioChannel	= 0;
	alutLoadMP3			= 0;
	alutLoadVorbis		= 0;

	//Creating sound queue
	soundQueue = new SoundInfo;
	//Initializing all of our sound buffers, sources, etc.
	for(i = 0; i < NumSoundTypes; i++)
	{
		buffer[i]	= 0;
		source[i]	= 0;
		gain[i]		= 0;
		sourcePos[i][0] = 0.0;
		sourcePos[i][1] = 0.0;
		sourcePos[i][2] = 0.0;
		numReqThisFrame[i] = 0;
	}

	for(i = 0; i < NUM_EXPLO; i++)
	{
		sourceExplosion[i]	= 0;
	}

	for(i = 0; i < NUM_EXPLO_POP; i++)
	{
		sourceExploPop[i]	= 0;
	}

	//If we can't create a valid context,
	//config->audioEnabled() will be set to false.
	if(config->audioEnabled())
		config->setAudio(createContext());

	if(config->audioEnabled())
	{
		initSound();

		if(alAttenuationScale)
		{
			audioScale[0] = 3.0;
			audioScale[1] = 1.0;
			audioScale[2] = 1.0;
		}
		else
		{
			fprintf(stderr, "alAttenuationScale == 0. Kludge it.\n");
			audioScale[0] = 0.5;
			audioScale[1] = 0.3;
			audioScale[2] = 0.3;
		}

		if(config->swapStereo())
			audioScale[0] = -audioScale[0];

		if(config->usePlayList() && !cdrom)
			loadMusicList();
	}
	//Debugging Purposes
	//fprintf(stderr, "AudioOpenAL::Audio done\n");
}

AudioOpenAL::~AudioOpenAL()
{
	if(initialized)
	{
		fprintf(stderr, "stopping OpenAL...");

	#ifdef CD_VOLUME
		if(cdrom && alcSetAudioChannel)
		{
			alcSetAudioChannel(ALC_CHAN_CD_LOKI, origCDvolume);
		}
	#endif //CD_VOLUME

		if(cdrom)
			SDL_CDStop(cdrom);

		checkError("AudioOpenAL::~Audio()");

	#ifdef _WIN32
		if(context_id)
			alcDestroyContext((ALCcontext*)context_id);
	#else
		if(context_id)
			alcDestroyContext(context_id);
	#endif

		fprintf(stderr, "done.\n");
	}
}

#ifdef _WIN32
static void warning(const char *msg, ...)
{
	va_list ap;
	va_start( ap, msg );
	vfprintf( stderr, msg, ap );
	va_end( ap );
	fprintf( stderr, "\n");
}
#else
static void warning(const char *, ...)
{
}
#endif

//Now we create the OpenAL context.
bool AudioOpenAL::createContext()
{
#ifdef _WIN32
	ALCdevice *dev;
	dev=alcOpenDevice((unsigned char*)"DirectSound",NULL);
	context_id=(void*)alcCreateContext(dev,NULL);
	alcMakeContextCurrent((ALCcontext*)context_id);
#else
	#ifdef OLD_OPENAL
	context_id = alcCreateContext(NULL);
#else
	ALCdevice *dev;
	dev = alcOpenDevice( NULL );
	context_id = alcCreateContext(dev, NULL);
	alcMakeContextCurrent(context_id);
#endif
#endif

	if(!context_id)
	{
		fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		fprintf(stderr, "!! ATTENTION !! - one or more errors were encountered during audio check.\n");
		fprintf(stderr, "!!                Audio will be disabled.\n");
		fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	}
	return (bool)context_id;
}

//Initialize OpenAL.
//The context should already have been created in createContext()
void AudioOpenAL::initSound()
{
	Config	*config = Config::instance();
	int i;
	ALfloat pos[] = { 0.0, -5.0,  25.0 };
	//	fprintf(stderr, "AudioOpenAL::initSound() begin...\n");

	fprintf(stderr, "-OpenAL-----------------------------------------------------\n");
	fprintf(stderr, "Vendor     : %s\n", alGetString( AL_VENDOR ) );
	fprintf(stderr, "Renderer   : %s\n", alGetString( AL_RENDERER ) );
	fprintf(stderr, "Version    : %s\n", alGetString( AL_VERSION ) );
	//fprintf(stderr, "Extensions : %s\n", alGetString( AL_EXTENSIONS ) );
	printExtensions(stderr,  (const char*)alGetString( AL_EXTENSIONS ));
	fprintf(stderr, "------------------------------------------------------------\n");
	checkForExtensions();

	alListenerfv(AL_POSITION, pos);
	alGenBuffers(NumSoundTypes, buffer);
	alGenSources(NumSoundTypes, source);

	loadSounds();
	for(i = 0; i < NumSoundTypes; i++)
	{
		switch(i)
		{
			case PowerUp:	gain[PowerUp]	= 0.7;	break;
			case ExploPop:	gain[ExploPop]	= 0.8;	break;
			case MusicGame:	gain[MusicGame]	= 0.7;	break;
			case MusicMenu:	gain[MusicMenu]	= 0.7;	break;
			default:
				gain[i] = 1.0;
				break;
		}
	}

	for(i = 0; i < NumSoundTypes; i++)
	{
		warning("Audio.cpp - init %s", fileNames[i]);
		alSourcefv( source[i], AL_POSITION, pos );
		alSourcei ( source[i], AL_LOOPING, AL_FALSE);
		alSourcei ( source[i], AL_BUFFER, buffer[i] );
		alSourcef ( source[i], AL_GAIN, gain[i]);
		checkError();
	}

	//Generating sources for Explosions
	//We want to overlap each Explosion
	alGenSources(NUM_EXPLO-1, sourceExplosion);
	sourceExplosion[NUM_EXPLO-1] = source[Explosion];
	for(i = 0; i < NUM_EXPLO-1; i++)
	{
		warning("Audio.cpp - init explosion %d", i);
		alSourcefv( sourceExplosion[i], AL_POSITION, pos );
		alSourcei ( sourceExplosion[i], AL_LOOPING, AL_FALSE);
		alSourcei ( sourceExplosion[i], AL_BUFFER, buffer[Explosion] );
		alSourcef ( sourceExplosion[i], AL_GAIN, gain[Explosion]*(0.7+0.3*FRAND) );
		checkError();
	}

	//Generating sources for small Explosions
	//We want to overlap each ExploPop
	alGenSources(NUM_EXPLO_POP-1, sourceExploPop);
	sourceExploPop[NUM_EXPLO_POP-1] = source[ExploPop];
	for(i = 0; i < NUM_EXPLO_POP-1; i++)
	{
		warning("Audio.cpp - init exploPop %d", i);
		alSourcefv( sourceExploPop[i], AL_POSITION, pos );
		alSourcei ( sourceExploPop[i], AL_LOOPING, AL_FALSE);
		alSourcei ( sourceExploPop[i], AL_BUFFER, buffer[ExploPop] );
		alSourcef ( sourceExploPop[i], AL_GAIN, 0.0*gain[ExploPop]*(0.1+0.9*FRAND) );
		checkError();
	}

	setSoundVolume(config->volSound());
	setMusicVolume(config->volMusic());

	initialized = true;
	warning("Audio.cpp - initSound complete.", i);
}

void AudioOpenAL::checkForExtensions()
{
// Note: Win32 doesn't have any useful extensions yet,
// and the alGetProcAddress prototype is different (sigh).
#ifndef _WIN32
	//Check AttenuationScale extension
	alAttenuationScale = (void (*)(ALfloat param))
	alGetProcAddress((ALubyte *)"alAttenuationScale_LOKI");

	if(alAttenuationScale == NULL)
		fprintf(stderr, "ERROR! Could not load alAttenuationScale\n");
	else
		alAttenuationScale(6.0);

	//Check Audio Channel extensions
	alcGetAudioChannel = (float (*)(ALuint channel))
	alGetProcAddress((const ALubyte *)"alcGetAudioChannel_LOKI");

	alcSetAudioChannel = (void (*)(ALuint channel, ALfloat volume))
	alGetProcAddress((const ALubyte *)"alcSetAudioChannel_LOKI");

#ifdef CD_VOLUME
	if(alcGetAudioChannel)
		origCDvolume = alcGetAudioChannel(ALC_CHAN_CD_LOKI);
#endif //CD_VOLUME

	//Check MP3 extension
	alutLoadMP3 = (ALboolean (*)(ALuint, ALvoid *, ALint))
		alGetProcAddress((const ALubyte *)"alutLoadMP3_LOKI");

	//Check Ogg/Vorbis extensions
	alutLoadVorbis = (ALboolean (*)(ALuint, ALvoid *, ALint))
		alGetProcAddress((const ALubyte *)"alutLoadVorbis_LOKI");

#endif //_WIN32
}

//Stops OpenAL music. Also calls Audio::stopMusic()
void AudioOpenAL::stopMusic()
{
	Config	*config = Config::instance();
	if(config->audioEnabled())
	{
		Audio::stopMusic();
		alSourceStop(source[MusicMenu]);
		alSourceStop(source[MusicGame]);
	}
}

//Pauses OpenAL music. Also calls Audio::pauseMusic()
void AudioOpenAL::pauseGameMusic(bool status)
{
	Config	*config = Config::instance();
	if(config->audioEnabled()) {
		if (cdrom) {
			Audio::pauseGameMusic(status);
		} else {
			if (status)
				alSourcePause(source[MusicGame]);
			else
				alSourcePlay(source[MusicGame]);
		}
	}
}

void AudioOpenAL::setMusicMode(SoundType mode)
{
	//fprintf(stderr, "AudioOpenAL::setMusicMode(SoundType mode)\n");
	Global	*game = Global::getInstance();
	Config	*config = Config::instance();
	if (config->audioEnabled()) {
		Audio::setMusicMode(mode);
		switch(mode)
		{
			default:
			case MusicGame:
				alSourceStop(source[MusicMenu]);
				if (!cdrom) {
					alSourcei(source[MusicGame], AL_LOOPING, AL_TRUE);
					if (!game->game_pause)
						alSourcePlay(source[MusicGame]);
				}
				break;
			case MusicMenu:
				if (!cdrom) {
					alSourcePause(source[MusicGame]);
				}
				alSourcei(source[MusicMenu], AL_LOOPING, AL_TRUE);
				alSourcePlay(source[MusicMenu]);
				break;
		}
	}
}

//Adjusts the gain of OpenAL sounds
void AudioOpenAL::setSoundVolume(float vol)
{
	//fprintf(stderr, "AudioOpenAL::setSoundVolume(%f)\n", vol);
	Config	*config = Config::instance();
	if (config->audioEnabled())	{
		int i;
		if (vol > 1.0)
			vol = 1.0;

		if (vol < 0.0)
			vol = 0.0;
		soundVolume = vol;

		for(i = 0; i < MusicGame; i++)
		{
			alSourcef(source[i], AL_GAIN, gain[i]*vol);
		}
		for(i = 0; i < NUM_EXPLO; i++)
		{
			alSourcef(sourceExplosion[i], AL_GAIN,
			(0.5+(FRAND*0.5))*gain[Explosion]*soundVolume);
		}
		for(i = 0; i < NUM_EXPLO_POP; i++)
		{
			alSourcef(sourceExploPop[i], AL_GAIN,
			(0.2+(FRAND*0.8))*gain[Explosion]*soundVolume);
		}
	}
}

//Adjusts the gain of OpenAL music.
//Also adjusts CD channel of hardware mixer
//if appropriate OpenAL extensions are available.
void AudioOpenAL::setMusicVolume(float vol)
{
	Config	*config = Config::instance();
	if(config->audioEnabled()) {
		if(vol > 1.0)
			vol = 1.0;
		if(vol < 0.0)
			vol = 0.0;
		musicVolume = vol;

		alSourcef(source[MusicGame], AL_GAIN, gain[MusicGame]*musicVolume);
		alSourcef(source[MusicMenu], AL_GAIN, gain[MusicGame]*musicVolume);

#ifdef CD_VOLUME
		if (cdrom && alcSetAudioChannel) {
			alcSetAudioChannel(ALC_CHAN_CD_LOKI, musicVolume);
		}
#endif
		//fprintf(stderr, "Music volume = %f\n", vol);
	}
}


//Loads the WAV sound effects
void AudioOpenAL::loadSounds()
{
	int i;
	ALsizei size, freq, bits;
	ALenum format;
	ALvoid *data;

	for(i = 0; i < NumSoundTypes; i++)
	{
		if (!fileNames[i]) {
			fprintf(stderr, "\nERROR! Sound enumerate index %d does not have valid filename!\n", i);
		}
		else
		{
		#ifndef _WIN32
			alutLoadWAV(dataLoc(fileNames[i]), &data, &format, &size, &bits, &freq);
			alBufferData (buffer[i], format, data, size, freq);
			free(data);
		#else //_WIN32
			char nameBuffer[256];
			sprintf(nameBuffer, "%s", dataLoc(fileNames[i]));
			fprintf(stderr, "alutLoadWAVFile(\"%s\",...);\n", nameBuffer);
			alutLoadWAVFile(nameBuffer,&format,&data,&size,&freq);
			alBufferData(buffer[i],format,data,size,freq);
			alutUnloadWAV(format,data,size,freq);
		#endif//_WIN32
		}
	}
}

void AudioOpenAL::checkError(const char* tag)
{
	ALenum error = alGetError();
	if(error != AL_NO_ERROR) {
		fprintf(stderr, "ERROR!! <%s> alGetError() = %s\n",
				tag, alGetString(error));
	}
	//error = alcGetError();
	//if(error != ALC_NO_ERROR)
	//{
	//fprintf(stderr, "ERROR!! <%s> alcGetError() = %s\n", tag, alcGetString(error) );
	//}
}


//This should be called every frame to ensure that delayed sounds are played
// and that the numReqThisFrame counter is reset to 0
void AudioOpenAL::update()
{
	SoundInfo *sound;
	SoundInfo *nextSound;
	SoundInfo *prevSound;

	//Play any delayed sounds
	sound = soundQueue->next;
	while(sound) {
		sound->age++;
		if (sound->age >= 0) {
			prevSound = sound->prev;
			nextSound = sound->next;
			prevSound->next = nextSound;
			if (nextSound)
				nextSound->prev = prevSound;
			playSound(sound->type, sound->pos);
			delete sound;
			sound = prevSound;
		}
		sound = sound->next;
	}

	//Reset the number of requests to 0
	for(int i = 0; i < NumSoundTypes; i++)
	{
		numReqThisFrame[i] = 0;
	}
}


//Plays a sound of a given type.
//Parameters:
//	type = Type of sound to play
//	pos  = Position of the sound coordinates
//	age  = If age is negative, playback of the sound will be delayed by
//		   by (age) frames.
void AudioOpenAL::playSound(SoundType type, float *pos, int age)
{
	Config	*config = Config::instance();
	if (config->audioEnabled()) {
		float p[3];
		p[0] =  pos[0] * audioScale[0];
		p[1] =  pos[1] * audioScale[1];
		p[2] =  pos[2] * audioScale[2];

		if (age >= 0) {
			numReqThisFrame[type]++;
			//Only play one sound of any given type per frame...
			if (numReqThisFrame[type] < 2) {
				switch(type)
				{
					case Explosion:
						playSoundExplosion(p);
						break;
					case ExploPop:
						playSoundExploPop(p);
						break;
					default:
						alSourceStop(source[type]);
						alSourcefv(source[type], AL_POSITION, p);
						alSourcePlay(source[type]);
						//fprintf(stderr, "Playing %d\n", type);
						break;
				}
			}
		} else {
			SoundInfo *first = soundQueue->next;
			SoundInfo *newSound;
			newSound = new SoundInfo(type, pos, age);
			newSound->prev = soundQueue;
			newSound->next = soundQueue->next;
			soundQueue->next = newSound;
			if (first)
				first->prev = newSound;
		}
	}
}

void AudioOpenAL::playSoundExplosion(float p[3])
{
	explosionIndex++;
	explosionIndex = explosionIndex % NUM_EXPLO;

	alSourceStop(sourceExplosion[explosionIndex]);
	alSourcefv(sourceExplosion[explosionIndex], AL_POSITION, p);
	alSourcePlay(sourceExplosion[explosionIndex]);
}

void AudioOpenAL::playSoundExploPop(float p[3])
{
	p[0] *= 1.5;
	exploPopIndex++;
	exploPopIndex = exploPopIndex % NUM_EXPLO_POP;

	alSourceStop(sourceExploPop[exploPopIndex]);
	alSourcefv(sourceExploPop[exploPopIndex], AL_POSITION, p);
	alSourcePlay(sourceExploPop[exploPopIndex]);
}

void AudioOpenAL::loadMusicList()
{
	Config	*config = Config::instance();
	int		i;
	bool	ok = false;
	int		lineCount = 0;
	char	buffer[1024];
	char	configFilename[256];
	FILE	*file;

	const char *homeDir = getenv("HOME");
	if (!homeDir)
		homeDir = "./";

	musicMax = 0;
	musicIndex = 0;
	sprintf(configFilename, "%s/%s", homeDir, CONFIG_MUSIC_FILE);
	alterPathForPlatform(configFilename);
	file = fopen(configFilename, "r");
	if (file) {
		while(fgets(buffer, 1024, file)) {
			int tmp = strlen(buffer);
			buffer[tmp-1] = '\0'; //Get rid of newline character

			ok = false;
			lineCount++;
			if (strlen(buffer) > 255) {
				fprintf(stderr, "ERROR: \"%s\", line %d\n",
						configFilename, lineCount);
				fprintf(stderr, "Filename is too long!\n");
			}
			else if(strlen(buffer) > 4) {
				switch(extensionFormat(buffer))
				{
					case WAV:
						strcpy(musicFile[musicMax], buffer);
						musicMax++;
						break;
					case MP3:
						if (alutLoadMP3) {
							strcpy(musicFile[musicMax], buffer);
							musicMax++;
						} else {
							fprintf(stderr, "WARNING: \"%s\", line %d\n",
									configFilename, lineCount);
							fprintf(stderr, "MP3 support not available.\n");
						}
						break;
					case OGG:
						if (alutLoadVorbis) {
							strcpy(musicFile[musicMax], buffer);
							musicMax++;
						} else {
							fprintf(stderr, "WARNING: \"%s\", line %d\n",
							 		configFilename, lineCount);
							fprintf(stderr, "Ogg/Vorbis support not available.\n");
						}
						break;
					default:
						fprintf(stderr, "WARNING: \"%s\", line %d\n",
						 		configFilename, lineCount);
						fprintf(stderr, "File type not recognized: \"%s\"\n",
						 		buffer);
						break;
				}
			}
			if (musicMax+1 >= MAX_MUSIC) //Limited music array size.
				break;
		}
	}
	else
		config->setUsePlayList(false);
	if (musicMax < 1)
		config->setUsePlayList(false);

	fprintf(stderr, "Music Playlist:\n");
	for(i = 0; i < musicMax; i++)
	{
		fprintf(stderr, " %2d : %s\n", i+1, musicFile[i]);
	}
	fprintf(stderr, "Loaded %s\n", configFilename);
}


//Set music track to index.
//If CDROM is enabled, calls Audio::setMusicIndex().
//If playList is enabled, set to Nth track.
void AudioOpenAL::setMusicIndex(int index)
{
	Config	*config = Config::instance();
	bool	wasPlaying = false;

	if (musicMax)
		musicIndex = index%musicMax;

	if (initialized && cdrom) {
		Audio::	setMusicIndex(index);
	}
	else if (initialized && config->usePlayList()) {
		bool loadSuccess = true;
		if (config->audioEnabled()) {
			checkError("AudioOpenAL::setMusicIndex -- begin");

			//If the music is currently playing,
			//We want to re-start playing after loading new song
			ALint state = AL_INITIAL;
			#ifdef OLD_OPENAL
			alGetSourcei(source[MusicGame], AL_SOURCE_STATE, &state);
			#else
			alGetSourceiv(source[MusicGame], AL_SOURCE_STATE, &state);
			#endif

			if(state == AL_PLAYING)
				wasPlaying = true;

			alSourceStop(source[MusicGame]);
			checkError("AudioOpenAL::setMusicIndex: Before setting source buffer to 0");
			alSourcei(source[MusicGame], AL_BUFFER, 0);
			//alSourcei(source[MusicGame], AL_BUFFER, AL_NONE);
			checkError("AudioOpenAL::setMusicIndex: After setting source buffer to 0");
			alDeleteBuffers(1 , &buffer[MusicGame]);
			alGenBuffers(1, &buffer[MusicGame]);

			switch(extensionFormat(musicFile[musicIndex]))
			{
				case WAV:
					loadSuccess = loadWAV(musicFile[musicIndex]);
					break;
				case MP3:
					loadSuccess = loadMP3(musicFile[musicIndex]);
					break;
				case OGG:
					loadSuccess = loadVorbis(musicFile[musicIndex]);
					break;
				default:
					loadSuccess = false;
					break;
			}
			if (loadSuccess) {
				ALfloat pos[] = { 0.0, -5.0,  25.0 };
				alSourcei(source[MusicGame], AL_BUFFER, buffer[MusicGame]);
				alSourcefv(source[MusicGame], AL_POSITION, pos);
				alSourcef(source[MusicGame], AL_GAIN, gain[MusicGame]*musicVolume);
				alSourcei(source[MusicGame], AL_LOOPING, AL_TRUE);
				if (wasPlaying)
					alSourcePlay(source[MusicGame]);
			} else {
				//Fall back to the default loop
				loadSuccess = loadWAV(dataLoc(fileNames[MusicGame]));
			}
		}
		checkError("AudioOpenAL::setMusicIndex: End");
	}
	else if (initialized) {
		ALint state = AL_INITIAL;
		#ifdef OLD_OPENAL
		alGetSourcei(source[MusicGame], AL_SOURCE_STATE, &state);
		#else
		alGetSourceiv(source[MusicGame], AL_SOURCE_STATE, &state);
		#endif

		if (state == AL_PLAYING)
			wasPlaying = true;
		alSourceStop(source[MusicGame]);
		if (wasPlaying)
			alSourcePlay(source[MusicGame]);
	}
}

#ifndef __linux__
//Kludge so things compile on Win32 and Mac
int strcasecmp(const char *a, const char *b)
{
	return strcmp(a, b);
}
#endif

AudioOpenAL::AudioFormat AudioOpenAL::extensionFormat(char* filename)
{
	AudioFormat returnValue = Unknown;
	char *walker;
	walker = strrchr(filename, '.');
	if (walker) {
		if (strcasecmp(walker, ".wav") == 0)
			returnValue = WAV;
		else if (strcasecmp(walker, ".mp3") == 0)
			returnValue = MP3;
		else if (strcasecmp(walker, ".ogg") == 0)
			returnValue = OGG;
	}
	else
		returnValue = Unknown;

	return returnValue;
}

bool AudioOpenAL::loadWAV(const char *filename)
{
#ifndef USE_PLAYLIST
	fprintf(stderr, "WARNING: %s\n", filename);
	fprintf(stderr, "PlayList support not compiled into New Haven (AudioOpenAL.cpp)\n");
	return false;
	#else //USE_PLAYLIST

	bool returnValue;
	ALsizei size, freq, bits;
	ALenum format;
	ALvoid *data;
	returnValue = alutLoadWAV(filename, &data, &format, &size, &bits, &freq);
	if (returnValue) {
		alBufferData(buffer[MusicGame], format, data, size, freq);
		free(data);
	}
	return returnValue;
#endif//USE_PLAYLIST
}

bool AudioOpenAL::loadMP3(const char *filename)
{
#ifndef USE_PLAYLIST
	fprintf(stderr, "WARNING: %s\n", filename);
	fprintf(stderr, "PlayList support not compiled into New Haven (AudioOpenAL.cpp)\n");
	return false;
#else//USE_PLAYLIST

	FILE	*file;
	struct	stat sbuf;
	int		size;
	void	*data;

	if(stat(filename, &sbuf) == -1)
	{
		perror(filename);
		return false;
	}
	size = sbuf.st_size;
	data = malloc(size);
	if(!data)
	{
		fprintf(stderr, "ERROR: Could not allocate memory in AudioOpenAL::loadMP3\n");
		return false;
	}
	file = fopen(filename, "rb");
	if(!file)
	{
		fprintf(stderr, "ERROR: Could not open \"%s\" in AudioOpenAL::loadMP3\n", filename);
		free(data);
		return false;
	}
	fread(data, 1, size, file);
	fclose(file);
	if( !(alutLoadMP3(buffer[MusicGame], data, size)) )
	{
		fprintf(stderr, "ERROR: alutLoadMP3() failed in AudioOpenAL::loadMP3\n");
		free(data);
		return false;
	}
	free(data);
	return true;
#endif//USE_PLAYLIST
}

bool AudioOpenAL::loadVorbis(const char *filename)
{
#ifndef USE_PLAYLIST
	fprintf(stderr, "WARNING: %s\n", filename);
	fprintf(stderr, "PlayList support not compiled into New Haven (AudioOpenAL.cpp)\n");
	return false;
	#else//USE_PLAYLIST

	if (initialized) {
		FILE	*file;
		struct	stat sbuf;
		int		size;
		void	*data;

		if (stat(filename, &sbuf) == -1) {
			perror(filename);
			return false;
		}
		size = sbuf.st_size;
		data = malloc(size);
		if (!data) {
			fprintf(stderr, "ERROR: Could not allocate memory in AudioOpenAL::loadVorbis\n");
			return false;
		}

		file = fopen(filename, "rb");
		if (!file) {
			fprintf(stderr, "ERROR: Could not open \"%s\" in AudioOpenAL::loadVorbis\n", filename);
			free(data);
			return false;
		}

		fread(data, 1, size, file);
		fclose(file);
		if (!(alutLoadVorbis(buffer[MusicGame], data, size))) {
			fprintf(stderr, "ERROR: alutLoadVorbis() failed in AudioOpenAL::loadVorbis\n");
			free(data);
			return false;
		}
		free(data);
		return true;
	}
	else
		return false;
#endif//USE_PLAYLIST
}
