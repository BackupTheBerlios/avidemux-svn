/***************************************************************************
                          main.cpp  -  description
                             -------------------
	Initialize the env.

    begin                : Sat Feb 2 2002
    copyright            : (C) 2002 by mean
    email                : fixounet@free.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "ADM_cpp.h"
#include "config.h"
#include "ADM_default.h"
#include "ADM_threads.h"
#include "DIA_uiTypes.h"
#include "ADM_preview.h"
#include "ADM_win32.h"
#include "ADM_crashdump.h"
#include "ADM_memsupport.h"
#include "ADM_script2/include/ADM_script.h"
#include "ADM_ffmp43.h"
#include "ADM_coreVideoFilterFunc.h"
#include "ADM_coreDemuxer.h"
#include "ADM_muxerProto.h"

#define __DECLARE__
#include "avi_vars.h"

#include "prefs.h"
#include "audio_out.h"
#include "ADM_assert.h"

#ifdef USE_SDL
extern "C" {
	#ifdef __HAIKU__
	#include "SDL/SDL.h"
	#else
	#include "SDL.h"
	#endif
}

#include "ADM_render/GUI_sdlRender.h"
#endif

static bool setPrefsDefault(void);
extern bool  vdpauProbe(void);

extern void registerVideoFilters( void );
extern void filterCleanUp( void );
extern void register_Encoders( void )  ;

extern uint8_t initGUI(const vector<IScriptEngine*>& engines);
extern void destroyGUI(void);
extern void initFileSelector(void);
extern void getUIDescription(char*);
extern uint8_t ADM_ad_loadPlugins(const char *path);
extern uint8_t ADM_vf_loadPlugins(const char *path);
extern uint8_t ADM_vd6_loadPlugins(const char *path);
extern uint8_t ADM_av_loadPlugins(const char *path);
extern uint8_t ADM_ae_loadPlugins(const char *path);
extern uint8_t ADM_ve6_loadPlugins(const char *path);

extern bool ADM_ad_cleanup(void);
extern bool ADM_ae_cleanup(void);
extern bool ADM_vf_cleanup(void);
extern void ADM_ve6_cleanup(void);

extern bool vdpauProbe(void);
extern bool vdpauCleanup(void);

extern void loadPlugins(void);
extern void InitFactory(void);
extern void InitCoreToolkit(void);
extern uint8_t  quotaInit(void);

extern int UI_Init(int nargc,char **nargv);
extern int UI_RunApp(void);
extern bool UI_End(void);
extern bool ADM_jobInit(void);
extern bool ADM_jobShutDown(void);
extern void cleanUp (void);

#if !defined(NDEBUG) && defined(FIND_LEAKS)
extern const char* new_progname;
#endif

void onexit(void);
int startAvidemux(int argc, char *argv[]);
bool isPortableMode(int argc, char *argv[]);

int main(int _argc, char *_argv[])
{
	ADM_initBaseDir(isPortableMode(_argc, _argv));

#if defined(_WIN32) && (ADM_UI_TYPE_BUILD == ADM_UI_GTK || ADM_UI_TYPE_BUILD == ADM_UI_QT4)
	// redirect output before registering exception handler so error dumps are captured
	redirectStdoutToFile();
#endif

	installSigHandler();

	char **argv;
	int argc;

#ifdef _WIN32
	getUtf8CommandLine(&argc, &argv);
#else
	argv = _argv;
	argc = _argc;
#endif

#if !defined(NDEBUG) && defined(FIND_LEAKS)
	new_progname = argv[0];
#endif

	int exitVal = startAvidemux(argc, argv);

#ifdef _WIN32
	freeUtf8CommandLine(argc, argv);
#endif

	uninstallSigHandler();

	return exitVal;
}

int startAvidemux(int argc, char *argv[])
{
    printf("*************************\n");
    printf("  Avidemux v" VERSION);

  	if(ADM_SUBVERSION)
	{
		printf(" (r%04u)", ADM_SUBVERSION);
	}

    printf("\n*************************\n");
    printf(" http://www.avidemux.org\n");
    printf(" Code      : Mean, JSC, Grant Pedersen\n");
    printf(" GFX       : Nestor Di, nestordi@augcyl.org\n");
    printf(" Design    : Jakub Misak\n");
    printf(" FreeBSD   : Anish Mistry, amistry@am-productions.biz\n");
    printf(" Audio     : Mihail Zenkov\n");
    printf(" Mac OS X  : Kuisathaverat, Harry van der Wolf\n");
    printf(" Win32     : Grant Pedersen\n\n");

#ifdef __GNUC__
	printf("Compiler: GCC %s\n", __VERSION__);
#endif

	printf("Build Target: ");

#if defined(_WIN32)
	printf("Microsoft Windows");
#elif defined(__APPLE__)
	printf("Apple");
#else
	printf("Linux");
#endif

#if defined(ADM_CPU_X86_32)
	printf(" (x86)");
#elif defined(ADM_CPU_X86_64)
	printf(" (x86-64)");
#endif

	char uiDesc[15];
	getUIDescription(uiDesc);
	printf("\nUser Interface: %s\n", uiDesc);

#ifdef _WIN32
	char version[250];

	if (getWindowsVersion(version))
		printf("Operating System: %s\n", version);
#endif

#if defined(__USE_LARGEFILE) && defined(__USE_LARGEFILE64)
	printf("\nLarge file available: %d offset\n", __USE_FILE_OFFSET64);
#endif

    printf("Time: %s\n", ADM_epochToString(ADM_getSecondsSinceEpoch()));

	for(int i = 0; i < argc; i++)
	{
		printf("%d: %s\n", i, argv[i]);
	}

	// Start counting memory
	ADM_memStatInit();
#ifndef __APPLE__
    ADM_InitMemcpy();
#endif
	printf("\nInitialising prefs\n");
	initPrefs();
	if(false==prefs->load()) // no prefs, set some sane default
    {
        setPrefsDefault();
    }
    CpuCaps::init();

#ifdef USE_SDL
	uint32_t videoDevice = RENDER_LAST;

	prefs->get(VIDEODEVICE, &videoDevice);

	initSdl(videoDevice);
#endif

	atexit(onexit);

#ifdef _WIN32
    win32_netInit();
#endif

	video_body = new ADM_Composer;

	UI_Init(argc, argv);
    AUDMEncoder_initDither();

    // Hook our UI...
    InitFactory();
    InitCoreToolkit();
    initFileSelector();

	// Load .avidemuxrc
    quotaInit();

    ADM_lavFormatInit();
	//***************Plugins *********************
	// Load system wide audio decoder plugin
#ifdef __APPLE__
    const char *startDir="../lib";
#else
    const char *startDir=ADM_RELATIVE_LIB_DIR;
#endif
    char *adPlugins = ADM_getInstallRelativePath(startDir, ADM_PLUGIN_DIR, "audioDecoder");
    char *avPlugins = ADM_getInstallRelativePath(startDir, ADM_PLUGIN_DIR, "audioDevices");
    char *aePlugins = ADM_getInstallRelativePath(startDir, ADM_PLUGIN_DIR, "audioEncoders");
    char *dmPlugins = ADM_getInstallRelativePath(startDir, ADM_PLUGIN_DIR, "demuxers");
    char *mxPlugins = ADM_getInstallRelativePath(startDir, ADM_PLUGIN_DIR, "muxers");
    char *vePlugins = ADM_getInstallRelativePath(startDir, ADM_PLUGIN_DIR, "videoEncoders");
    char *vdPlugins = ADM_getInstallRelativePath(startDir, ADM_PLUGIN_DIR, "videoDecoders");
    char *vfPlugins = ADM_getInstallRelativePath(startDir, ADM_PLUGIN_DIR, "videoFilters");
	char *sePlugins = ADM_getInstallRelativePath(startDir, ADM_PLUGIN_DIR, "scriptEngines");

    ADM_mx_loadPlugins(mxPlugins);
    delete [] mxPlugins;

	ADM_ad_loadPlugins(adPlugins);
	delete [] adPlugins;

    ADM_av_loadPlugins(avPlugins);
    delete [] avPlugins;

    ADM_ae_loadPlugins(aePlugins);
    delete [] aePlugins;

	ADM_dm_loadPlugins(dmPlugins);
    delete [] dmPlugins;

    ADM_ve6_loadPlugins(vePlugins);
    delete [] vePlugins;

    ADM_vf_loadPlugins(vfPlugins);
    delete [] vfPlugins;

    ADM_vd6_loadPlugins(vdPlugins);
    delete [] vdPlugins;


    // load local audio decoder plugins
	adPlugins=ADM_getHomeRelativePath("plugins6","audioDecoder");
	ADM_ad_loadPlugins(adPlugins);
	delete [] adPlugins;

	// load local video filter plugins
	vfPlugins=ADM_getHomeRelativePath("plugins6","videoFilter");
	ADM_vf_loadPlugins(vfPlugins);
	delete [] vfPlugins;


	//***************Plugins *********************

	if(!initGUI(initialiseScriptEngines(sePlugins, video_body)))
	{
		printf("\n Fatal : could not init GUI\n");
		exit(-1);
	}

    delete [] sePlugins;

    ADM_lavInit();
    AVDM_audioInit();

#if defined( USE_VDPAU)
  #if (ADM_UI_TYPE_BUILD!=ADM_UI_CLI)
    printf("Probing for VDPAU...\n");
    if(vdpauProbe()==true) printf("VDPAU available\n");
        else printf("VDPAU not available\n");
  #else
    printf("Cannot use VDPAU in cli mode %d,%d\n",ADM_UI_TYPE_BUILD,ADM_UI_CLI);
  #endif
#endif

    // Init jobs
    ADM_jobInit();

    UI_RunApp();
    cleanUp();

    printf("Normal exit\n");
    return 0;
}
extern uint8_t GUI_close(void);
void onexit( void )
{
	printf("Cleaning up\n");
    if(video_body) video_body->cleanup ();
    delete video_body;
    // wait for thread to finish executing
    destroyScriptEngines();
//    filterCleanUp();
	ADM_lavDestroy();

#ifdef USE_SDL
	quitSdl();
#endif


	AVDM_cleanup();


	destroyGUI();
    destroyPrefs();

    admPreview::destroy();
    UI_End();

    ADM_ad_cleanup();
    ADM_ae_cleanup();
    ADM_mx_cleanup();
    ADM_vf_clearFilters();
    ADM_vf_cleanup();
    ADM_dm_cleanup();
    ADM_ve6_cleanup();

	ADM_jobShutDown();
    printf("--End of cleanup--\n");
    ADMImage_stat();
    ADM_memStat();
    ADM_memStatEnd();

    ADM_info("\nGoodbye...\n\n");
}
/**
    \fn setPrefsDefault
*/
bool setPrefsDefault(void)
{
#ifdef __MINGW32__
        prefs->set(AUDIO_DEVICE_AUDIODEVICE,"Win32");
    #ifdef USE_OPENGL
        prefs->set(VIDEODEVICE,(uint32_t)5); // QTGL
    #endif
#endif
#ifdef __linux__
            prefs->set(AUDIO_DEVICE_AUDIODEVICE,"PulseAudioS");
    #ifdef USE_VDPAU
            prefs->set(VIDEODEVICE,(uint32_t)4); // VDPAU
    #else
            prefs->set(VIDEODEVICE,(uint32_t)1); // XV
    #endif
#endif
    return true;
}
/**
    \fn isPortableMode
    \brief returns true if we are in portable mode
*/
bool isPortableMode(int argc, char *argv[])
{
	bool portableMode = false;
    std::string mySelf=argv[0];
    // if the name ends by "_portable.exe" => portable
    int match=mySelf.find("portable");
    if(match!=-1)
    {
        ADM_info("Portable mode\n");
        return true;
    }

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "--portable") == 0)
		{
			portableMode = true;
			break;
		}
	}

	return portableMode;
}

//EOF
