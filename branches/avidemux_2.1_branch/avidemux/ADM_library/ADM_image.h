//
// C++ Interface: %{MODULE}
//
// Description: 
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
//	This is the base time for image exchanged between codec/filters/...
//
//	We (optionnally) can carry extra informations
//		- aspect ratio
//		- frame type
//		- quantizer for each macroblock (16x16 pixels)
//	For the latter 3 infos are used
//		quant which leads to the int8 quant array
//		qstride = stride of array. Usually width+15)/16. 0 MEANS NOT USABLE
//		qsize = size of the array (needed to be able to copy it)
//
#ifndef ADM_IMAGE
#define ADM_IMAGE
#include "ADM_assert.h"
#include "ADM_colorspace/ADM_rgb.h"

typedef enum ADM_ASPECT
{
	ADM_ASPECT_4_3=1,
	ADM_ASPECT_16_9,
	ADM_ASPECT_1_1
};
// Avisynth compatibility layer

//#define vi.num_frames _info.nb_frames
//#define vi.IsYV12()   1
#define GetReadPtr GetWritePtr
#define GetRowSize GetPitch

typedef enum ADM_PLANE
{
        PLANAR_Y=1,
        PLANAR_U=2,
        PLANAR_V=3
        
};
class ADMImage
{
public:
        
	uint8_t		*data;		/// Pointer to actual image data
	uint32_t	_width;		/// Width of image
	uint32_t	_height;	/// Height of image
	uint32_t	_qStride;	/// Stride of Q infos, usually about width/8 <- ***if 0 means no quant usable***
	uint8_t		*quant;		/// Byte representing quantize used for this block
	uint32_t	_Qp;		/// Average quantizer for this image, Default=2
	uint32_t	_qSize;		/// Size of the *quant bitfield
	ADM_ASPECT	_aspect;	/// Aspect ratio
	uint32_t	flags;		/// Flags for this image (AVI_KEY_FRAME/AVI_B_FRAME)

// This 3 fields are only used to convery container (reference to other datas)
// Between codec & editor
	uint8_t         _isRef;         /// If True means the datas are just a link to data we don't own!
        ADM_colorspace  _colorspace;    /// Colorspace we are moving, default is YV12
        uint8_t         _noPicture;     /// No picture to display

// End of section dedicated to codec/editor transfer

        void            commonInit(uint32_t w,uint32_t h); /// sub constructor
        
        uint32_t        GetPitch(ADM_PLANE plane)
                                {
                                        switch(plane)
                                        {
                                                case PLANAR_Y:return _width;break;
                                                case PLANAR_U:
                                                case PLANAR_V:return _width>>1;break;
                                                default: ADM_assert(0);
                                        }
                                };
        uint8_t         *GetWritePtr(ADM_PLANE plane)
                        {       
                                uint32_t plan=_width*_height;
                                switch(plane)
                                        {
                                                case PLANAR_Y:return data;break;
                                                case PLANAR_U:return data+plan;break;
                                                case PLANAR_V:return data+((plan*5)>>2);break;
                                                default: ADM_assert(0);
                                        }
                        };
   
        uint32_t GetHeight(ADM_PLANE plane)
                                {
                                        switch(plane)
                                        {
                                                case PLANAR_Y:return _height;break;
                                                case PLANAR_U:
                                                case PLANAR_V:return _height>>1;break;
                                                default: ADM_assert(0);
                                        }
                                };
        uint8_t duplicateMacro(ADMImage *src,uint32_t swap);       /// copy an image to ourself, including info
public:

        uint8_t         *_planes[3];     /// In case of linked data store y/u/v pointers
        uint32_t        _planeStride[3]; /// Same story

		ADMImage(uint32_t width, uint32_t height);
                ADMImage(uint32_t width, uint32_t height,uint32_t dummy); /// To create linked datas image        

                
		~ADMImage();
	uint8_t duplicate(ADMImage *src);	/// copy an image to ourself, including info
        uint8_t duplicateSwapUV(ADMImage *src); /// copy an image to ourself, including info
	uint8_t duplicateFull(ADMImage *src);	/// copy an image to ourself, including info
	uint8_t copyInfo(ADMImage *src);	/// copy all the flags, not the data themselves
	uint8_t copyQuantInfo(ADMImage *src);	/// copy quant table if any
        uint8_t isRef(void) { return _isRef;};
        uint8_t setLinkInfos(uint8_t *y,        /// To fill in infos for linked image
                        uint8_t *u,uint8_t *v,uint32_t stridey,
                        uint32_t strideu, uint32_t stridev);
        uint8_t merge(ADMImage *src1,ADMImage *src2);
        uint8_t substract(ADMImage *src1,ADMImage *src2);
static uint32_t lumaDiff(ADMImage *src1,ADMImage *src2,uint32_t noise);
};
void drawString(ADMImage *dst, int x, int y, const char *s) ;
#define YPLANE(x) (x->data)
#define UPLANE(x) (x->data+(x->_width*x->_height))
#define VPLANE(x) (x->data+(5*(x->_width*x->_height)>>2))

uint8_t BitBlit(uint8_t *dst, uint32_t pitchDest,uint8_t *src,uint32_t pitchSrc,uint32_t width, uint32_t height);

#endif
