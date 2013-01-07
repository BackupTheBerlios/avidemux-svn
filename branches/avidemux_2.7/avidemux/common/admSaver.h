#ifndef admSaver_h
#define admSaver_h

#include "IMuxerPlugin.h"
#include "IVideoEncoderPlugin.h"
#include "ADM_filterChain.h"
#include "ADM_coreMuxerFfmpeg.h"
#include "ADM_coreVideoEncoder.h"

/**
    \class admSaver
    \brief Wrapper for saving

*/
class admSaver
{
protected:
		IMuxerPlugin *_muxerPlugin;
		IVideoEncoderPlugin *_videoEncoderPlugin;
        uint64_t             startAudioTime; // Actual start time (for both audio & video) can differ from markerA
        std::string          fileName;
        std::string          logFileName;
        ADM_muxer            *muxer;
        ADM_videoFilterChain *chain;
        ADM_audioStream      *audio;
        ADM_videoStream      *video;
        uint64_t             markerA,markerB;
        int                  videoEncoderIndex;
        ADM_coreVideoEncoder *handleFirstPass(ADM_coreVideoEncoder *pass1);
        ADM_videoStream      *setupVideo(void);
        bool                  setupAudio();
        ADM_audioStream      *audioAccess[ADM_MAX_AUDIO_STREAM]; // audio tracks to feed to the muxer
        int                   nbAudioTracks;
        
public:
                              admSaver(IVideoEncoderPlugin *videoEncoderPlugin, IMuxerPlugin *muxerPlugin, const char *out);
                              ~admSaver();
        bool                  save(void);

};

#endif