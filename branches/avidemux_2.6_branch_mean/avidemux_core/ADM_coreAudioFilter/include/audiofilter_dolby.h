//
// C++ Interface: audiofilter_dolby
//
// Description: 
//
//
// Author: Mihail Zenkov <kreator@tut.by>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef AUDM_DOLBY_H
#define AUDM_DOLBY_H


#define NZEROS 500
#define GAIN 1.571116176e+00

/**
 * \class  ADMDolbyContext
 */
class ADMDolbyContext
{
public:    
        ADMDolbyContext();
       
        float *xv_left[4];
        float *xv_right[4];
        float lefty[4][NZEROS*2+2];
        float righty[4][NZEROS*2+2];
        static bool  skip;
        int    posLeft;
        int    posRight;
        
        float DolbyShiftLeft(float isamp);
        float DolbyShiftRight(float isamp);
        void  DolbyInit();
        
static   bool setValue(  float **target,int offset, float value);
static   bool setOneValue(float *target,int offset, float value);
        
static   void  DolbySkip(bool on);
        
static  float DolbyShift_simple(int pos, float *oldie, float *coef);        
static  float DolbyShift_convolution(int pos, float *oldie, float *coef);
static  float DolbyShift_convolutionAlign1(int pos, float *oldie, float *coef);
static  float DolbyShift_convolutionAlign2(float *oldie, float *coef);
};

#endif
