#ifndef AUDM_AUDIO_MIXER_H
#define AUDM_AUDIO_MIXER_H
class AUDMAudioFilterMixer : public AUDMAudioFilter
{
    protected:
        CHANNEL_CONF    _output;
	inline bool compareChType(WAVHeader *wh1, WAVHeader *wh2);
    public:

      ~AUDMAudioFilterMixer();
      AUDMAudioFilterMixer(AUDMAudioFilter *instream,CHANNEL_CONF out);
      uint32_t   fill(uint32_t max,float *output,AUD_Status *status);
};
#endif
