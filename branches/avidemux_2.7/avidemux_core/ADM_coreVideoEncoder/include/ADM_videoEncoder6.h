#ifndef ADM_videoEncoder6_h
#define ADM_videoEncoder6_h

#include "IVideoEncoderPlugin.h"

/*!
  This structure defines a video encoder
  \param encoder Encoder attached to this descriptor
  \param name The name of the codec
  \param configure Function to call to configure the codec
  \param param : An opaque structure that contains the codec specific configuration datas
*/
typedef struct
{
    const char   *encoderName;        // Internal name (tag)
    const char   *menuName;         // Displayed name (in menu)
    const char   *description;      // Short description

    uint32_t     apiVersion;            // const
    ADM_coreVideoEncoder *(*create)(ADM_coreVideoFilter *head,bool globalHeader);
    void         (*destroy)(ADM_coreVideoEncoder *codec);
    bool         (*configure)(void);                                // Call UI to set it up
    bool         (*getConfigurationData)(CONFcouple **c); // Get the encoder private conf
    bool         (*setConfigurationData)(CONFcouple *c,bool full);   // Set the encoder private conf
	void         (*resetConfigurationData)();

    ADM_UI_TYPE  UIType;                // Type of UI
    uint32_t     major,minor,patch;     // Version of the plugin

    void         *opaque;               // Hide stuff in here
} ADM_videoEncoderDesc;

/**
    \class ADM_videoEncoder6
    \brief Plugin Wrapper Class

*/
class ADM_videoEncoder6 : public IVideoEncoderPlugin
{
private:
	ADM_LibWrapper *_pluginWrapper;
	PluginVersion *_pluginVersion;
	ADM_videoEncoderDesc *_encoderDesc;

	ADM_videoEncoderDesc *(*_getInfo)();

	ADM_videoEncoder6(ADM_LibWrapper *pluginWrapper);

public:
	ADM_videoEncoder6(ADM_videoEncoderDesc *encoderDesc);
	~ADM_videoEncoder6();

	static ADM_videoEncoder6* loadPlugin(const char *file);

	const char *id();
	const char *name();
	int apiVersion();
	PluginVersion *version();
	const char *underlyingLibraryName();
	const char *underlyingLibraryVersion();

	bool configure(void);
	ADM_coreVideoEncoder* createEncoder(ADM_coreVideoFilter *head, bool globalHeader);
	const char* description(void);
    void destroyEncoder(ADM_coreVideoEncoder *codec);
    bool getConfiguration(CONFcouple **c);
    bool setConfiguration(CONFcouple *c, bool full);
	void resetConfiguration(void);

	ADM_UI_TYPE UIType(void);
};

#endif