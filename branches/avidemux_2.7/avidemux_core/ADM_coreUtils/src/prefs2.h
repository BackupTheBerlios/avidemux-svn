// automatically generated by admSerialization.py do not edit
#ifndef ADM_my_prefs_struct_CONF_H
#define ADM_my_prefs_struct_CONF_H
typedef struct {
struct  {
	bool swap_if_A_greater_than_B;
	bool saveprefsonexit;
	bool ignoresavedmarkers;
	bool use_odml;
	bool use_systray;
	bool reuse_2pass_log;
	bool audiobar_uses_master;
	uint32_t threading_lavc;
	uint32_t cpu_caps;
	bool mpeg_no_limit;
	bool alternate_mp3_tag;
	bool vdpau;
}features;
struct  {
	char * lastdir_read;
	char * lastdir_write;
	char * file1;
	char * file2;
	char * file3;
	char * file4;
}lastfiles;
struct  {
	char * lastdir_read;
	char * lastdir_write;
	char * file1;
	char * file2;
	char * file3;
	char * file4;
}lastprojects;
uint32_t message_level;
struct  {
	char * audiodevice;
	char * alsa_device;
}audio_device;
uint32_t videodevice;
struct  {
	uint32_t encoding;
	uint32_t indexing;
	uint32_t playback;
}priority;
struct  {
	uint32_t postproc_type;
	uint32_t postproc_value;
	uint32_t downmixing;
}Default;
uint32_t mpegsplit_autosplit;
}my_prefs_struct;
#endif
