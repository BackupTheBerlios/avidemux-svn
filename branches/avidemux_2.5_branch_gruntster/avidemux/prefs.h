#ifndef _preferences_h_
#define _preferences_h_

#include "ADM_default.h"

#define RC_OK     1
#define RC_FAILED 0

// <prefs_gen>
typedef enum {
	FILTERS_SUBTITLE_FONTNAME,
	FILTERS_SUBTITLE_CHARSET,
	FILTERS_SUBTITLE_FONTSIZE,
	FILTERS_SUBTITLE_YPERCENT,
	FILTERS_SUBTITLE_UPERCENT,
	FILTERS_SUBTITLE_VPERCENT,
	FILTERS_SUBTITLE_SELFADJUSTABLE,
	FILTERS_SUBTITLE_USEBACKGROUNDCOLOR,
	FILTERS_AVSFILTER_WINE_APP,
	FILTERS_AVSFILTER_AVS_SCRIPT,
	FILTERS_AVSFILTER_AVS_LOADER,
	FILTERS_AVSFILTER_PIPE_TIMEOUT,
	FILTERS_AVSFILTER_DEBUG_MSG,
	DEVICE_AUDIODEVICE,
	DEVICE_AUDIO_ALSA_DEVICE,
	DEVICE_VIDEODEVICE,
	DEFAULT_POSTPROC_TYPE,
	DEFAULT_POSTPROC_VALUE,
	LASTFILES_FILE1,
	LASTFILES_FILE2,
	LASTFILES_FILE3,
	LASTFILES_FILE4,
	LASTFILES_FILE5,
	LASTFILES_FILE6,
	LASTPROJECT_FILE1,
	LASTPROJECT_FILE2,
	LASTPROJECT_FILE3,
	LASTPROJECT_FILE4,
	LASTPROJECT_FILE5,
	LASTPROJECT_FILE6,
	LASTDIR_READ,
	LASTDIR_WRITE,
	MESSAGE_LEVEL,
	FEATURE_SWAP_IF_A_GREATER_THAN_B,
	FEATURE_SVCDRES_PREFEREDSOURCERATIO,
	FEATURE_SAVEPREFSONEXIT,
	FEATURE_IGNORESAVEDMARKERS,
	FEATURE_DISABLE_NUV_RESYNC,
	FEATURE_TRYAUTOIDX,
	FEATURE_USE_ODML,
	FEATURE_USE_SYSTRAY,
	FEATURE_REUSE_2PASS_LOG,
	FEATURE_AUDIOBAR_USES_MASTER,
	FEATURE_THREADING_LAVC,
	FEATURE_CPU_CAPS,
	FEATURE_MPEG_NO_LIMIT,
	FEATURE_AUTO_BUILDMAP,
	FEATURE_AUTO_REBUILDINDEX,
	FEATURE_AUTO_UNPACK,
	DOWNMIXING_PROLOGIC,
	FEATURE_ALTERNATE_MP3_TAG,
	FEATURE_GLOBAL_GLYPH_ACTIVE,
	FEATURE_GLOBAL_GLYPH_NAME,
	PRIORITY_ENCODING,
	PRIORITY_INDEXING,
	PRIORITY_PLAYBACK,
	PLUGIN_ORDER
} options;
// </prefs_gen>

class preferences {
	private:
		char *internal_lastfiles[7];
		char *_lastProjects[7];

		int save_xml_to_file();
	public:
		preferences();
		~preferences();
		int load();
		int save();
		int get(options option, unsigned int *val);
		int get(options option,          int *val);
		int get(options option, unsigned long *val);
		int get(options option,          long *val);
		int get(options option, float *val);
		int get(options option, char **val);
                int get(options option, ADM_filename **val);
		int get(options option, uint8_t *val);
		int get(options option, uint16_t *val);
		const char * get_str_min(options option);
		const char * get_str_max(options option);
		// handled by compiler: const uint8_t is full handled by any 
		//    method that can handle const unsigned int
		// int set(options option, const uint8_t val);
		// int set(options option, const uint16_t val);
		int set(options option, const unsigned int val);
		int set(options option, const          int val);
		int set(options option, const unsigned long val);
		int set(options option, const          long val);
		int set(options option, const          float val);
		int set(options option, const          char * val);
                int set(options option, const ADM_filename * val);
		int set_lastfile(const char* file);
		const char **get_lastfiles(void);
		const char **getLastProjects(void);
		int setLastProject(const char* file);
};

extern preferences *prefs;
int initPrefs(  void );
int destroyPrefs(  void );
#endif
