/***************************************************************************
                          ADM_genvideo.hxx  -  description
                             -------------------
    begin                : Tue Mar 19 2002
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
/**
	Not for filter writters :

		The constructor accept 2 parameters

				_in : previous filter in the chain, the source of incoming image
				couple : A configuration seen as couples of string
						"size" "13" for example. Passing null means either
						it has no parameters or set the default value for them.

				_uncompressed : optionnal internal buffer

				configure return 1 if the conf has changed, 0 else.



*/
#ifndef __ADM_VIDEO__
#define __ADM_VIDEO__

#define INT int32_t
#include "ADM_video/ADM_confCouple.h"
#include "ADM_library/ADM_image.h"


void GUI_PreviewInit(uint32_t w , uint32_t h, uint32_t modal);
uint8_t 	GUI_PreviewUpdate(uint8_t *data);
void 	GUI_PreviewEnd( void);
uint8_t  	GUI_PreviewRun(uint8_t *data);
uint8_t GUI_StillAlive( void );
void GUI_PreviewShow(uint32_t w, uint32_t h, uint8_t *data);
#define Pixel uint8_t
typedef struct
 {
 		uint32_t width;
 		uint32_t height;
 		uint32_t nb_frames;
 		uint32_t encoding;
 		uint32_t codec;
 		uint32_t fps1000;
		uint32_t orgFrame;
 }ADV_Info;


typedef struct
{
    uint32_t					w,h;
    uint32_t				algo;
}RESIZE_PARAMS;

typedef struct
{
 	uint32_t left,right;
 	uint32_t top,bottom;
}CROP_PARAMS;

typedef struct
{
 	uint32_t newfps, precision;
}CHFPS_PARAMS;

typedef struct {
    int pixel;
    double weight;
} CONTRIB;


typedef struct {
    uint32_t  n;			/* number of contributors */
    CONTRIB *p;			/* pointer to list of contributions */
} CLIST;

typedef struct
{
	uint32_t 	width;		/* horizontal size of the image in Pixels */
	uint32_t	height;		/* vertical size of the image in Pixels */
	Pixel *	data;		/* pointer to first scanline of image */

} Image;



 class AVDMGenericVideoStream
 {
   protected:
      	ADV_Info 					_info;
   	  ADMImage   					*_uncompressed;
	  AVDMGenericVideoStream 	*_in;
/* not really used */
          uint8_t		getPixel(int32_t x,int32_t y,uint8_t *data);
          uint8_t		getPixelU(int32_t x,int32_t y,uint8_t *data);
          uint8_t		setPixelU(uint8_t val,int32_t x,int32_t y,uint8_t *data);
          uint8_t 	unPackChroma(uint8_t *ssrc,uint8_t *ddst);
/* /not really used */
   public:
   	// return 1 -> conf changed need rebuild, 0 means conf not changed
        virtual 	uint8_t 	configure( AVDMGenericVideoStream *instream)=0;
          			AVDMGenericVideoStream( void)  {_uncompressed=NULL;_in=NULL;};
  	virtual 		~AVDMGenericVideoStream( ) {};
        virtual char 	*printConf(void) { static char *str=(char *)"."; return str;};

        virtual uint8_t 	getFrameNumberNoAlloc(uint32_t frame, uint32_t *len,
          							ADMImage *data,uint32_t *flags)=0;
          	ADV_Info 	*getInfo( void ) { return &_info;};
         virtual uint8_t	getCoupledConf( CONFcouple **couples)
	  				{*couples=NULL;return 0;};
 };

 // Pseudo class used to register filters automagically
 // Does not work atm
class AVDMVideo_FilterDec
{
private:
	int dum;
public:
  	AVDMVideo_FilterDec(char *name, AVDMGenericVideoStream *(*create) (AVDMGenericVideoStream *in, void *));
};



class  AVDMVideoStreamNull :public AVDMGenericVideoStream
 {

 protected:
				ADM_Composer 			*_in;
				uint32_t			_start;

				
				

 public:

  				AVDMVideoStreamNull(  ADM_Composer *in,uint32_t framestart, uint32_t nb);
  				virtual ~AVDMVideoStreamNull();


          virtual 	uint8_t getFrameNumberNoAlloc(uint32_t frame, uint32_t *len,
								ADMImage *data,uint32_t *flags);
           	 	uint8_t configure( AVDMGenericVideoStream *instream);


 }     ;

class  AVDMVideoStreamRaw :public AVDMGenericVideoStream
 {

 protected:
				ADM_Composer *_in;
				uint32_t			_start;

 public:

  				AVDMVideoStreamRaw(  ADM_Composer *in,uint32_t framestart, uint32_t nb);
  				virtual ~AVDMVideoStreamRaw();


          virtual 	uint8_t getFrameNumberNoAlloc(uint32_t frame, uint32_t *len,
								ADMImage *data,uint32_t *flags);
           	 	uint8_t configure( AVDMGenericVideoStream *instream);


 }     ;


/**
	These macros are used to build the create_ function automatically
	That way external sw has no need to know the underlying class
*/
#define NEW(x) (x *)ADM_alloc(sizeof(x))
#define DELETE(x) {if(x){ADM_dealloc(x);x=NULL;}}
#define CREATOR(x,clss)  return new clss(in,x);
#define BUILD_CREATE(name,clss) AVDMGenericVideoStream *name(AVDMGenericVideoStream *in, CONFcouple *conf) \
{		CREATOR(conf,clss); }

#define SCRIPT_CREATE(name,clss,tmplate) \
AVDMGenericVideoStream *name (AVDMGenericVideoStream *in, int n,Arg *args); \
AVDMGenericVideoStream *name (AVDMGenericVideoStream *in, int n,Arg *args) \
{ 							\
CONFcouple		*couple; 			\
AVDMGenericVideoStream *filter; 			\
	couple=filterBuildCouple(&tmplate,n,args);	\
	if(!couple) { printf("Filter built failed\n");return NULL;}\
	filter= new clss(in,couple);			\
	delete couple;					\
	return filter;					\
}



#define GET(x) assert(couples->getCouple((char *)#x,&(_param->x)))
#define CSET(x)  (*couples)->setCouple((char *)#x,(_param->x))

#define MPLAYER_RESIZE_PREFFERED
#endif

