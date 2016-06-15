// Created by Junior on 6/14/2016.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Config.h"
#include "extern.h"
#include "HiScore.h"
#include "NCString.h"
#include "Audio.h"
#include "Ground.h"
#include "MenuGL.h"
#include "MainGL.h"
#include "PowerUps.h"
#include "HeroAmmo.h"
#include "EnemyAmmo.h"
#include "EnemyFleet.h"
#include "Explosions.h"
#include "HeroAircraft.h"
#include "ScreenItemAdd.h"
#include "StatusDisplay.h"
#include "GroundMetal.h"

Config	*Config::m_instance = 0;

//Initializing variables, then reading the config file
Config::Config()
{
	//Setting Up Defaults
	screen_width 	= 800;
	screen_height 	= 600;
	screen_A 		= 1.3333333f;
	screen_FOV 		= 30.0f;
	screen_near		= 10.0;
	screen_far 		= 70.0;
	full_screen		= false;
	screen_size		= 2;
	screen_bound[0] = 11.0;
	screen_bound[1] =  9.0;
	z_Trans 		= -56.5;

	blend_enabled 	= true;
	tex_border		= true;
	audio_enabled	= true;
	swap_stereo		= false;
	show_fps		= true;
	auto_speed		= false;
	true_color		= false;
	use_playList	= false;
	use_cdrom		= true;

	cdrom_device	= 0;
	cdrom_count		= 1;
	max_level		= 1;
	gfx_level		= 2;

	mouse_speed		= 0.03;
	game_skill_base	= 0.5;
	view_gamma		= 1.1;
	vol_sound		= 0.9;
	vol_music		= 0.5;
	audio_type		= AudioOpenAL;

	readFile();
}

//Save file before we exit the program...
Config::~Config()
{
	saveFile();
}

//Creates single Config object.
//Will return Config::_instance
Config *Config::init()
{
	if (!Config::_instance) {
		Config::_instance = new Config;
	} else {
		fprintf(stderr, "WARNING: Config::init() has already been called.\n");
	}

	return Config::_instance;
}

//Will return Config::_instance
Config *Config::instance()
{
	if (!Config::_instance) {
		return Config::init();
	}
	else
		return Config::_instance;
}

//Deletes singleton instance and sets Config::_instance to 0.
void Config::destroy()
{
	delete Config::_instance;
	Config::_instance = 0;
}


//Just A Quickie (ewut?)
bool Config::readFile()
{
	int		i;
	int		numLines;
	bool	returnValue = true;
	char	configFilename[256];
	FILE	*file;
	int		tmp;
	const char *homeDir = getenv("HOME");
	char	configStrings[32][64];

	if (!homeDir)
		homeDir = "./";

	sprintf(configFilename, "%s/%s", homeDir, CONFIG_FILE);
	alterPathForPlatform(configFilename);
	file = fopen(configFilename, "r");
	if (file) {
		i = numLines = 0;
		while(fgets(configStrings[i], 64, file))
			i++;
		numLines = i;
		for(i = 0; i < numLines; i++)
		{
			if (strncmp(configStrings[i], "screen_si", 8) == 0) {
				sscanf(configStrings[i], "screen_size %d\n", &screen_size);
			}
			if (strncmp(configStrings[i], "mouse_spe", 8) == 0) {
				sscanf(configStrings[i], "mouse_speed %f\n", &mouse_speed);
			}
			if (strncmp(configStrings[i], "game_skil", 8) == 0) {
				sscanf(configStrings[i], "game_skill_base %f\n", &game_skill_base);
			}
			if (strncmp(configStrings[i], "gfx_level", 8) == 0) {
				sscanf(configStrings[i], "gfx_level %d\n", &gfx_level);
			}
			if (strncmp(configStrings[i], "vol_sound", 8) == 0) {
				sscanf(configStrings[i], "vol_sound %f\n", &vol_sound);
			}
			if (strncmp(configStrings[i], "vol_music", 8) == 0) {
				sscanf(configStrings[i], "vol_music %f\n", &vol_music);
			}
			if (strncmp(configStrings[i], "full_scr", 8) == 0) {
				sscanf(configStrings[i], "full_screen %d\n",  &tmp);
				full_screen = (bool)tmp;
			}
			if (strncmp(configStrings[i], "true_col", 8) == 0) {
				sscanf(configStrings[i], "true_color %d\n", &tmp);
				true_color  = (bool)tmp;
			}
			if (strncmp(configStrings[i], "swap_ste", 8) == 0) {
				sscanf(configStrings[i], "swap_stereo %d\n", &tmp);
				swap_stereo = (bool)tmp;
			}
			if (strncmp(configStrings[i], "auto_spe", 8) == 0) {
				sscanf(configStrings[i], "auto_speed %d\n", &tmp);
				auto_speed  = (bool)tmp;
			}
			if (strncmp(configStrings[i], "show_fps", 8) == 0) {
				sscanf(configStrings[i], "show_fps %d\n", &tmp);
				show_fps    = (bool)tmp;
			}
			if (strncmp(configStrings[i], "use_play", 8) == 0) {
				sscanf(configStrings[i], "use_playList %d\n", &tmp);
				use_playList= (bool)tmp;
			}
			if (strncmp(configStrings[i], "use_cdro", 8) == 0) {
				sscanf(configStrings[i], "use_cdrom %d\n", &tmp);
				use_cdrom   = (bool)tmp;
			}
			if (strncmp(configStrings[i], "audio_typ", 8) == 0) {
				sscanf(configStrings[i], "audio_type %d\n", &tmp);
				audio_type = (AudioType)tmp;
			}
			if (strncmp(configStrings[i], "max_level", 8) == 0) {
				sscanf(configStrings[i], "max_level %d\n", &max_level);
			}
			if (strncmp(configStrings[i], "view_gamm", 8) == 0) {
				sscanf(configStrings[i], "view_gamma %f\n", &view_gamma);
			}
			if (strncmp(configStrings[i], "cdrom_cou", 8) == 0) {
				sscanf(configStrings[i], "cdrom_count %d\n", &cdrom_count);
			}
			if (strncmp(configStrings[i], "cdrom_dev", 8) == 0) {
				sscanf(configStrings[i], "cdromDevice %d\n", &cdrom_device);
			}
		}
	}
	else
	{
		fprintf(stderr, "WARNING: could not read config file (%s)\n", configFilename);
		returnValue = false;
	}

	//Update all screen size variables
	setScreenSize(screen_size);
	return returnValue;
}

//Save Config Ops
bool Config::saveFile()
{
	bool returnValue;
	char configFilename[256];
	FILE *file;
	const char *homeDir = getenv("HOME");

	if (!homeDir)
		homeDir = "./";

	sprintf(configFilename, "%s/%s", homeDir, CONFIG_FILE);
	alterPathForPlatform(configFilename);
	file = fopen(configFilename, "w");
	if (file) {
		fprintf(file, "use_playList %d\n",	(int)use_playList);
		fprintf(file, "use_cdrom %d\n",		(int)use_cdrom);
		fprintf(file, "full_screen %d\n", 	(int)full_screen);
		fprintf(file, "true_color %d\n", 	(int)true_color);
		fprintf(file, "swap_stereo %d\n",	(int)swap_stereo);
		fprintf(file, "auto_speed %d\n",	(int)auto_speed);
		fprintf(file, "show_fps %d\n",		(int)show_fps);
		fprintf(file, "screenSize %d\n",	screen_size);
		fprintf(file, "gfxLevel %d\n",		gfx_level);
		fprintf(file, "gameSkillBase %g\n",	game_skill_base);
		fprintf(file, "mouseSpeed %g\n",	mouse_speed);
		fprintf(file, "maxLevel %d\n",		max_level);
		fprintf(file, "volSound %g\n",		vol_sound);
		fprintf(file, "volMusic %g\n",		vol_music);
		fprintf(file, "viewGamma %g\n",		view_gamma);
		fprintf(file, "audioType %d\n",		(int)audio_type);
		fprintf(file, "cdromCount %d\n",	cdrom_count);
		fprintf(file, "cdromDevice %d\n",	cdrom_device);

		fclose(file);
		fprintf(stderr, "Config file has been written: (%s)\n", configFilename);
		returnValue = true;
	} else {
		fprintf(stderr, "Error! Unable to write config file (%s)\n", configFilename);
		returnValue = false;
	}

	return returnValue;
}

//Given the screen size, setting all other variables.
void Config::setScreenSize(int m)
{
	screen_size = m;
	if (screen_size > MAX_SCREEN_SIZE)
		screen_size = MAX_SCREEN_SIZE;
	if (screen_size < MIN_SCREEN_SIZE)
		screen_size = MIN_SCREEN_SIZE;
	switch(screen_size)
	{
		case 0:
			screen_width = 512;
			screen_height = 384;
			break;
		case 1:
			screen_width = 640;
			screen_height = 480;
			break;
		case 2:
			screen_width = 800;
			screen_height = 600;
			break;
		case 3:
			screen_width = 1024;
			screen_height = 768;
			break;
		case 4:
			screen_width = 1280;
			screen_height = 960;
			break;
		default:
			screen_size = 1;
			screen_width = 640;
			screen_height = 480;
			break;
	}
	screen_A = (float)screen_width/(float)screen_height;
}

