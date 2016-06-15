/* Created by Junior on 6/13/2016.
 *
 *  This file uses the base class Audio, which all audio calls are made.
 *  If USE_SDL is defined, this class will do CD Music Playback, but does
 *  not do any sound effects.
 *
*/
#ifndef AUDIO_H
#define AUDIO_H
#define NUM_EXPLOSIONS 2
#define NUM_EXPLOSIONS_POP 6
#define MAX_MUSIC 32
class SoundInfo;
class Global;


#ifdef USE_SDL
//#include <SDL.h>  //For CDROM
#else
#define SDL_CD void
#endif

#if !defined(USE_SDL) && !defined(SDL_CD_STUBS)
    #define SDL_CD_STUBS
    enum {CD_ERROR = -1, CD_TRAYEMPTY, CD_STOPPED, CD_PLAYING, CD_PAUSED};
    void	SDL_CDPlayTracks(void*, int, int, int, int);
    void	SDL_CDPause(void*);
    void	SDL_CDResume(void*);
    void	SDL_CDStop(void*);
    int		SDL_CDStatus(void*);
#endif //USE_SDL


class Audio
{
protected:
    virtual void initSound();
    virtual void initCDROM();

    char *fileNames[NumSoundTypes];     //Base files for sound effects
    SDL_CD *cdrom;                      //Pointer to CDROM struct (is void)
    char musicFile[MAX_MUSIC][256];     //Array of files for playlist
    int musicMax;                       //Max # of user-defined songs (CD or Playlist)
    int musicIndex;                     //Current Track (0 < musicIndex < musicMax)

public:
    enum SoundType {
        HeroAmmo00,         //Unused
        PowerUp,
        Explosion,          //Standard Explosion
        ExploPop,           //'Light' Explosion
        ExploBig,           //Deep, Long Explosion
        AddLife,            //New Ship Earned
        LoseLife,           //Ship lost
        MusicGame,          //Game-play Music
        MusicMenu,
        NumSoundTypes       //Total Number of Sounds Available
    };

    Audio();
    virtual ~Audio();

    virtual void update();
    virtual void playSound(SoundType type, float *pos, int age = 0);
    virtual void stopMusic();
    virtual void pauseGameMusic(bool);
    virtual void setMusicMode(SoundType);
    virtual void setMusicVolume(float);
    virtual void setSoundVolume(float);
    virtual void setMusicIndex(int);
    virtual void nextMusicIndex();
};

//Node class used for audio queue
class SoundInfo
{
public:
    SoundInfo();
    SoundInfo(Audio::SoundType t, float p[3], int a);

    Audio::SoundType type;
    float pos[3];
    int age;

    SoundInfo *next;
    SoundInfo *prev;
};

#endif //AUDIO_H
