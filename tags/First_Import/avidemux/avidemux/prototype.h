
// RGB
void    GUI_RGBDisplay(uint8_t *dis,uint32_t w,uint32_t h,void *widg);


// Render

uint8_t	 GUI_Refresh( void );

uint8_t GUI_ConvertRGB(uint8_t *in,uint8_t *out,uint32_t w,uint32_t h);
uint8_t	GUI_XvRedraw(void);

// GUI


void GUI_loadMP3(char *name);

void GUI_GoToKFrame(uint32_t frame); // same as below execpt																			// closest previous frame
void GUI_GoToFrame(uint32_t frame);

void 			changeAudioStream(AVDMGenericAudioStream *newaudio,AudioSource so);
void 			GUI_UpdateAudioToggle(AudioSource nwsource);

// not in callback.h to avoid importing COMPRESSION MODE in interface.cpp

 void updateVideoFilters(void);
 //
 //
 //
void editorReignitPreview( void );
void editorKillPreview( void );
void editorUpdatePreview(uint32_t framenum)    ;


// EOF

