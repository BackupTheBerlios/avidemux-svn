/***************************************************************************
  \file audioencoder.cpp

    copyright            : (C) 2002-6 by mean/gruntster/Mihail 
    email                : fixounet@free.fr
 ***************************************************************************/


#ifndef AUDIO_ENCODER_H
#define AUDIO_ENCODER_H
/*!
  This structure defines an audio encoder
  \param encoder Encoder attached to this descriptor
   \param name The name of the codec
  \param bitrate The bitrate in kb/s
  \param configure Function to call to configure the codec
  \param maxChannels The maximum # of channels this codec supports
  \param param : An opaque structure that contains the codec specific configuration datas
*/
#include "ADM_coreAudio.h"
#include "ADM_audioCodecEnum.h"
#include "ADM_audioFilter.h" 

#define AUDIOENC_COPY 0


typedef int AUDIOENCODER;

/**
    \class AUDMEncoder
    \brief audio encoder base class. Combined with the audioaccess class it makes the exact opposite
            of the bridge class, i.e. convert audioFilter to ADM_access then ADM_stream.

*/
#define ADM_AUDIO_ENCODER_BUFFER_SIZE (6*32*1024)
class ADM_AudioEncoder 
{
  protected:

    bool            eof_met;    // True if cannot encode anymore
    //
    uint8_t         *_extraData;
    uint32_t        _extraSize;
    AUDMAudioFilter *_incoming;

    
    float           tmpbuffer[ADM_AUDIO_ENCODER_BUFFER_SIZE*2];  // incoming samples are stored here before encofing
    uint32_t        tmphead,tmptail;

    bool            refillBuffer(int minimum); // Mininum is in float
    
    bool            reorderChannels(float *data, uint32_t nb,CHANNEL_TYPE *input,CHANNEL_TYPE *output);

    
    // The encoder can remap the audio channel (or not). If so, let's store the the configuration here
    CHANNEL_TYPE    outputChannelMapping[MAX_CHANNELS];
    WAVHeader       wavheader;  /// To be filled by the encoder, especially byterate and codec Id.
  public:
    //

    virtual uint8_t extraData(uint32_t *l,uint8_t **d) {*l=_extraSize;*d=_extraData;return 1;}
    WAVHeader       *getInfo(void) {return &wavheader;}

                    ADM_AudioEncoder(AUDMAudioFilter *in);	
                    virtual ~ADM_AudioEncoder();


    virtual bool    initialize(void)=0; /// Returns true if init ok, false if encoding is impossible
    virtual bool    encode(uint8_t *dest, uint32_t *len, uint32_t *samples)=0; /// returns false if eof met
};
#endif
