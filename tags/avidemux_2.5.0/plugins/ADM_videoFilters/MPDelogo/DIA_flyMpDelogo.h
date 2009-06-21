#ifndef FLY_MPDELOGO_H
#define FLY_MPDELOGO_H
class flyMpDelogo : public FLY_DIALOG_TYPE
{
public:
	uint32_t x, y, width, height;

	uint8_t    process(void);
	uint8_t    download(void);
	uint8_t    upload(void);
	flyMpDelogo(uint32_t width, uint32_t height, AVDMGenericVideoStream *in, void *canvas, void *slider);
};
#endif
