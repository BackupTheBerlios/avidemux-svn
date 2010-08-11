/***************************************************************************
                          Resample fps
                             -------------------
    begin                : Wed Nov 6 2002
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

#include <math.h>
#include "ADM_default.h"
#include "ADM_coreVideoFilterInternal.h"
#include "DIA_coreToolkit.h"
#include "DIA_factory.h"

#include "confResampleFps.h"
#include "confResampleFps_desc.cpp"

#if 1
    #define aprintf(...) {}
#else
    #define aprintf ADM_info
#endif
/**
    \class resampleFps

*/
class  resampleFps:public ADM_coreVideoFilter
{
protected:
        confResampleFps     configuration;
        bool                updateIncrement(void);
        uint64_t            baseTime;
        ADMImage            *frames[2];
        bool                refill(void);
        bool                prefill;
public:
                            resampleFps(ADM_coreVideoFilter *previous,CONFcouple *conf);
                            ~resampleFps();
        bool                goToTime(uint64_t usSeek);
        virtual const char   *getConfiguration(void);                   /// Return  current configuration as a human readable string
        virtual bool         getNextFrame(uint32_t *fn,ADMImage *image);    /// Return the next image
        virtual bool         getCoupledConf(CONFcouple **couples) ;   /// Return the current filter configuration
        virtual bool         configure(void) ;           /// Start graphical user interface
};
//***********************************
// Add the hook to make it valid plugin
DECLARE_VIDEO_FILTER(   resampleFps,   // Class
                        1,0,0,              // Version
                        ADM_UI_ALL,         // UI
                        VF_TRANSFORM,            // Category
                        "resampleFps",            // internal name (must be uniq!)
                        "Resample FPS",            // Display name
                        "Change and enforce FPS. Keep duration and sync." // Description
                    );
/**
    \fn configure
*/
bool resampleFps::configure(void)
{
  float f=configuration.newFps; 
  f/=1000;
  
  
    diaElemFloat fps(&f,QT_TR_NOOP("_New frame rate:"),1,200.);
    
    diaElem *elems[1]={&fps};
  
    if( diaFactoryRun(QT_TR_NOOP("Resample fps"),1,elems))
    {
        f*=1000;
      configuration.newFps=(uint32_t)floor(f+0.4);
      prefill=0;
      updateIncrement();
      return 1;
    }
    return 0;
}
/**
    \fn updateIncrement
    \brief FPS->TimeIncrement
*/
bool resampleFps::updateIncrement(void)
{
    info.frameIncrement=ADM_UsecFromFps1000(configuration.newFps);
  
    return true;
}
/**
    \fn getConfiguration
*/
const char *resampleFps::getConfiguration( void )
{
static char buf[100];
 snprintf(buf,99," Resample to %2.2f fps",(double)configuration.newFps/1000.);
 return buf;  
}
/**
    \fn ctor
*/
resampleFps::resampleFps(  ADM_coreVideoFilter *previous,CONFcouple *setup) : ADM_coreVideoFilter(previous,setup)
{
    baseTime=0;
    prefill=false;
    frames[0]=frames[1]=NULL;
    if(!setup || !ADM_paramLoad(setup,confResampleFps_param,&configuration))
    {
        // Default value
        configuration.newFps=ADM_Fps1000FromUs(previous->getInfo()->frameIncrement);
    }
    if(!frames[0]) frames[0]=new ADMImageDefault(info.width,info.height);
    if(!frames[1]) frames[1]=new ADMImageDefault(info.width,info.height);
    updateIncrement();
}
/**
    \fn dtor

*/
resampleFps::~resampleFps()
{
    if(frames[0]) delete frames[0];
    if(frames[1]) delete frames[1];
    frames[0]=frames[1]=NULL;
}
/**
     \fn refill
*/
bool resampleFps::refill(void)
{
    ADMImage *nw=frames[0];
    uint32_t img=0;
    frames[0]=frames[1];
    frames[1]=nw;
    return previousFilter->getNextFrame(&img,nw);
}
/**
    \fn goToTime
    \brief called when seeking. Need to cleanup our stuff.
*/
bool         resampleFps::goToTime(uint64_t usSeek)
{
    if(false==ADM_coreVideoFilter::goToTime(usSeek)) return false;
    prefill=false;
    return true;
}

/**
    \fn getCoupledConf
*/ 
bool         resampleFps::getCoupledConf(CONFcouple **couples)
{
    return ADM_paramSave(couples, confResampleFps_param,&configuration);
}
/**
    \fn getNextFrame
*/
 bool         resampleFps::getNextFrame(uint32_t *fn,ADMImage *image)
{

    if(!prefill)
    {
          if(false==refill()) return false;
          if(false==refill()) return false;
          prefill=1;
    }

    uint64_t thisTime=baseTime+(nextFrame*info.frameIncrement);

again:
    
    uint64_t frame1Dts=frames[0]->Pts;
    uint64_t frame2Dts=frames[1]->Pts;
    aprintf("Frame : %d, timeIncrement %d ms, Wanted : %"LLU", available %"LLU" and %"LLU"\n",
                    nextFrame,info.frameIncrement/1000,thisTime,frame1Dts,frame2Dts);
    if(thisTime>frame1Dts && thisTime>frame2Dts)
    {
        if(false==refill()) return false;
        goto again;
    }
    if(thisTime<frame1Dts && thisTime<frame2Dts)
    {
        image->duplicate(frames[0]);
        image->Pts=thisTime;
        *fn=nextFrame++;
        return true;
    }
    // In between, take closer
    double diff1=(double)thisTime-double(frame1Dts);
    double diff2=(double)thisTime-double(frame2Dts);
    if(diff1<0) diff1=-diff1;
    if(diff2<0) diff2=-diff2;
    int index=1;
    if(diff1<diff2) index=0;

    image->duplicate(frames[index]);
    image->Pts=thisTime;
    *fn=nextFrame++;
    return true;
}
#if 0
  ADMImage *mysrc1=NULL;
  ADMImage *mysrc2=NULL;

  if(frame>=_info.nb_frames) return 0;
  // read uncompressed frame
  
  // What frame are we seeking ?
  double f;
  uint32_t page=_info.width*_info.height;
  
  f=frame;
  f*=_in->getInfo()->fps1000;
  f/=_param->newfps;
  
  if(!_param->use_linear)
  {
      uint32_t nw;
      
      nw=(uint32_t)floor(f+0.4);
      if(nw>_in->getInfo()->nb_frames-1)
        nw=_in->getInfo()->nb_frames-1;
    
      mysrc1=vidCache->getImage(nw);
      if(!mysrc1) return 0;
      
      memcpy(YPLANE(data),YPLANE(mysrc1),page);
      memcpy(UPLANE(data),UPLANE(mysrc1),page>>2);
      memcpy(VPLANE(data),VPLANE(mysrc1),page>>2);
    
      vidCache->unlockAll();
      
      return 1;
  }
  /* With linear blending */
  uint32_t nw;
  uint8_t lowweight;
  uint8_t highweight;
  
  double diff;
  
  nw=(uint32_t)floor(f);
  diff=f-floor(f);
  highweight = (uint8_t)floor(diff*256);
  lowweight = 256 - highweight;

  if(nw>=_in->getInfo()->nb_frames-1)
    {
      printf("[ResampleFps] In %u Out %u\n",frame,nw);
      nw=_in->getInfo()->nb_frames-1;
      highweight=0;
    }
  //printf("New:%lu old:%lu\n",frame,nw);

  if(highweight == 0)
    {
      mysrc1=vidCache->getImage(nw);  
      if(!mysrc1) return 0;
      
      memcpy(YPLANE(data),YPLANE(mysrc1),page);
      memcpy(UPLANE(data),UPLANE(mysrc1),page>>2);
      memcpy(VPLANE(data),VPLANE(mysrc1),page>>2);
      
      vidCache->unlockAll();
    }
  else
    {
      mysrc1=vidCache->getImage(nw);
      mysrc2=vidCache->getImage(nw+1);
      if(!mysrc1 || !mysrc2) return 0;
      
      uint8_t *out, *in1, *in2;
      uint32_t count;
      uint32_t idx;
      
      out = YPLANE(data);
      in1 = YPLANE(mysrc1);
      in2 = YPLANE(mysrc2);
        
      count = page;

#ifdef ADM_CPU_X86
        if(CpuCaps::hasMMX())
                blendMMX(in1,in2,out,lowweight,highweight,(count*3)>>1);
        else
#endif
      {
      for(idx = 0; idx < count; ++idx)
	out[idx] = ((in1[idx]*lowweight) + (in2[idx]*highweight))>>8;

      out = UPLANE(data);
      in1 = UPLANE(mysrc1);
      in2 = UPLANE(mysrc2);
      count = page>>2;

      for(idx = 0; idx < count; ++idx)
        out[idx] = ((in1[idx]*lowweight) + (in2[idx]*highweight))>>8;      


      out = VPLANE(data);
      in1 = VPLANE(mysrc1);
      in2 = VPLANE(mysrc2);
      count = page>>2;

      for(idx = 0; idx < count; ++idx)
	out[idx] = ((in1[idx]*lowweight) + (in2[idx]*highweight))>>8;
      }

      vidCache->unlockAll();
    }
  return 1;
}
#endif 


//EOF
