/***************************************************************************
    \file ADM_vidCrop
    \brief Crop Filter
    \author Mean 2002, fixounet@free.Fr

          Crop top/left/right/bottom
          Each one ,must be even

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "ADM_default.h"
#include "ADM_coreVideoFilterInternal.h"
#include "DIA_factory.h"
#include "crop.h"
#include "crop_desc.cpp"

/**
    \class CropFilter
*/
class  CropFilter:public ADM_coreVideoFilter
 {

 protected:
                crop           configuration;
                ADMImage       *original;

 public:

                                CropFilter(  ADM_coreVideoFilter *in,CONFcouple *couples);
        virtual                 ~CropFilter();

       virtual const char   *getConfiguration(void);          /// Return  current configuration as a human readable string
       virtual bool         getNextFrame(ADMImage *image);    /// Return the next image
       //virtual FilterInfo  *getInfo(void);                    /// Return picture parameters after this filter
	   virtual bool         getCoupledConf(CONFcouple **couples) ;   /// Return the current filter configuration
       virtual bool         configure(void) ;                 /// Start graphical user interface
    
 };
 

// Add the hook to make it valid plugin
DECLARE_VIDEO_FILTER(   CropFilter,   // Class
                        1,0,0,              // Version
                        ADM_UI_TYPE_BUILD,         // UI
                        VF_TRANSFORM,            // Category
                        "crop",            // internal name (must be uniq!)
                        "crop",            // Display name
                        "crop filter" // Description
                    );


//_______________________________________________________________
/**
    \fn CropFilter
*/
CropFilter::CropFilter(ADM_coreVideoFilter *in,CONFcouple *couples) :ADM_coreVideoFilter(in,couples)
{

        original=new ADMImage(info.width,info.height);
        if(!couples || !ADM_paramLoad(couples,crop_param,&configuration))
		{
            // Default value
            configuration.top=0;
            configuration.bottom=0;
            configuration.left=0;
            configuration.right=0;
        }
        if(  in->getInfo()->width<(configuration.right+configuration.left))
                {
                    ADM_warning("Warning Cropping too much width ! Width reseted !\n");
                        configuration.right=configuration.left=0;
                }
        if(  in->getInfo()->height<(configuration.bottom+configuration.top))
                {
                    ADM_warning("Warning Cropping too much height ! Height reseted !\n");
                    configuration.bottom=configuration.top=0;
                }

        info.width= in->getInfo()->width- configuration.right- configuration.left;		
        info.height=in->getInfo()->height-configuration.bottom-configuration.top;	
}
/**
    \fn ~CropFilter
*/
CropFilter::~CropFilter()
{
    if(original) delete original;
    original=NULL;
}

/**
    \fn getNextFrame

*/
bool         CropFilter::getNextFrame(ADMImage *image)
{
FilterInfo  *prevInfo=previousFilter->getInfo();
			// read uncompressed frame
       		if(!previousFilter->getNextFrame(original)) return false;
       		
       		// Crop Y luma
       		uint32_t y,x,line;
       		uint8_t *src,*src2,*dest;
       		
       		y=prevInfo->height;
       		x=prevInfo->width;
       		line=info.width;
       		src=YPLANE(original)+configuration.top*x+configuration.left;
       		dest=YPLANE(image);
       		
       		for(uint32_t k=info.height;k>0;k--)
       			{
       			 	    memcpy(dest,src,line);
       			 	    src+=x;
       			 	    dest+=line;
       			}
         // Crop U  & V
            src=UPLANE(original)+(x*configuration.top>>2)+(configuration.left>>1);
            src2=VPLANE(original)+(x*configuration.top>>2)+(configuration.left>>1);
            dest=UPLANE(image);
            line>>=1;
            x>>=1;       		       		 	
            uint32_t loop=(info.height)>>1;
            for(uint32_t k=loop;k>0;k--)
                {
                        memcpy(dest,src,line);
                        src+=x;
                        dest+=line;
                }
                dest=VPLANE(image);	
                for(uint32_t k=((info.height)>>1);k>0;k--)
                {
                        memcpy(dest,src2,line);
                        src2+=x;
                        dest+=line;
                }	
            return 1;
}
/**
    \fn getCoupledConf
    \brief Return our current configuration as couple name=value
*/
bool         CropFilter::getCoupledConf(CONFcouple **couples)
{
    return ADM_paramSave(couples, crop_param,&configuration);
}
/**
    \fn getConfiguration
    \brief Return current setting as a string
*/
const char *CropFilter::getConfiguration(void)
{
    static char conf[80];
    conf[0]=0;
    snprintf(conf,80,"Crop : %"LU"x%"LU" => %"LU"x%"LU,
                previousFilter->getInfo()->width,
                previousFilter->getInfo()->height,
                info.width,info.height
                );
    return conf;
}
/**
    \fn Configure
*/
//extern int DIA_getCropParams(const char *name, CROP_PARAMS *param, AVDMGenericVideoStream *in);
bool CropFilter::configure(void)

{
#if 0
		uint8_t r;
		uint32_t w,h;
    	if(r = (DIA_getCropParams("Crop Settings",_param,instream )))
    	{
			w=_param->left+_param->right;
			h=_param->top+_param->bottom;
			ADM_assert(w<instream->getInfo()->width);
			ADM_assert(h<instream->getInfo()->height);
			_info.width=instream->getInfo()->width-w;
			_info.height=instream->getInfo()->height-h;
		}
		return r;
#endif
    return false;
}
// EOF
