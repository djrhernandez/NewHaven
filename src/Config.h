// Created by Junior on 6/14/2016.
#ifndef CONFIG_H
#define CONFIG_H
#include "windowcompat.h"


//Contains data obtained from config file. Singleton.
class Config
{
public:
	~Config();
	static Config *init();
	static Config *instance();
	static void destroy();

	bool readFile();
	bool saveFile();

	void setScreenSize(int m);
	int screenSize() 			{ return screen_size; }
	int screenWidth() 			{ return screen_width; }
	int screenHeight()			{ return screen_height; }
	float screenA() 			{ return screen_A; }
	float screenFOV() 			{ return screen_FOV; }
	float screenNear() 			{ return screen_near; }
	float screenFar() 			{ return screen_far; }
	float screenBoundX() 		{ return screen_bound[0]; }
    float screenBoundY() 		{ return screen_bound[1]; }
	float zTrans() 				{ return z_trans; }

	bool	fullScreen()		{	return	full_screen;	}
	bool	blend()				{	return	blend_enabled;}
	bool	texBorder()			{	return	tex_border;	}
	bool	audioEnabled()		{	return	audio_enabled;}
	bool	swapStereo()		{	return  swap_stereo;	}
	bool	autoSpeed()			{	return  auto_speed;	}
	bool	showFPS()			{	return  show_fps;		}
	bool	trueColor()			{	return  true_color;	}
	bool	usePlayList()		{	return  use_playList;	}
	bool	useCDROM()			{	return  use_cdrom;		}
	void	setFullScreen(bool s)		{	full_screen = s;	}
	void	setBlend(bool s)			{	blend_enabled = s;}
	void	setTexBorder(bool s)		{	tex_border = s;	}
	void	setAudio(bool s)			{	audio_enabled = s;}
	void	setSwapStereo(bool s)		{	swap_stereo = s;	}
	void	setAutoSpeed(bool s)		{	auto_speed = s;	}
	void	setShowFPS(bool s)			{	show_fps = s;		}
	void	setTrueColor(bool s)		{	true_color = s;	}
	void	setUsePlayList(bool s)		{	use_playList = s;	}
	void	setUseCDROM(bool s)			{	use_cdrom = s;	}

	int		CDROMDevice()			{   return cdrom_device;	}
	int		CDROMCount()			{   return cdrom_count;	}
	void	setCDROMDevice(int i)	{	cdrom_device = i;		}
	void	setCDROMCount(int c)	{	cdrom_count  = c;		}

	int gfxLevel()		{	return gfx_level;	}
	int maxLevel()		{	return max_level;	}

	void setGfxLevel(int a)
	{
		gfx_level = a;
		if (gfx_level < 0)
			gfx_level = 0;
		if (gfx_level > 2)
			gfx_level = 2;
	}
	void setMaxLevel(int a)
	{
		maxLevel = a;
		if (maxLevel > 10)
			maxLevel = 10;
	}

	float	mouseSpeed()		{	return mouse_speed;		}
	float	gameSkillBase()		{	return game_skill_base;	}
	float	viewGamma()			{	return view_gamma;		}
	float	volSound()			{	return vol_sound;		}
	float	volMusic()			{	return vol_music;		}

	void setMouseSpeed(float f)
	{
		mouse_speed = f;
		if (mouse_speed < 0.01)
			mouse_speed = 0.01;
		if (mouse_speed > 0.1)
			mouse_speed = 0.1;
	}
	void setGameSkillBase(float f)
	{	game_skill_base = f;
		if (game_skill_base > 0.9)
			game_skill_base = 0.9;
		if (game_skill_base < 0.2)
			game_skill_base = 0.2;
	}
	void setViewGamma(float f) { view_gamma = f; }
	void setVolSound(float f)
	{	vol_sound = f;
		if (vol_sound < 0.0)
			vol_sound = 0.0;
		if (vol_sound > 1.0)
			vol_sound = 1.0;
	}
	void setVolMusic(float f)
	{	vol_music = f;
		if (m_volMusic < 0.0)
			vol_music = 0.0;
		if (vol_music > 1.0)
			vol_music = 1.0;
	}

	int		intSkill()	{ return (int)((game_skill_base + 0.05) * 10.0); }

	enum AudioType { AudioOpenAL, AudioSDL_Mixer, NumAudioTypes };
	AudioType	audioType()	{ return audio_type; };

private:
	int 	 screen_size;
	int 	 screen_width;
	int 	 screen_height;
	float	 screen_A;
	float	 screen_FOV;
	float	 screen_near;
	float	 screen_far;
	float	 screen_bound[2];
	float	 z_Trans;

	bool	 full_screen;
	bool	 blend_enabled;
	bool	 tex_border;
	bool	 audio_enabled;
	bool	 swap_stereo;
	bool	 auto_speed;
	bool	 show_fps;
	bool	 true_color;
	bool	 use_playList;
	bool	 use_cdrom;

	int		 cdrom_device;
	int		 cdrom_count;
	int 	 gfx_level;
	int 	 max_level;

	float	 mouse_speed;
	float	 game_skill_base;
	float	 view_gamma;
	float	 vol_sound;
	float	 vol_music;

	AudioType	audio_type;

private:
	static Config	*_instance;
	Config();
};

#endif //CONFIG_H
