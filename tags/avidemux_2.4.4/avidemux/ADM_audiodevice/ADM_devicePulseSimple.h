/***************************************************************************
                          ADM_deviceEsd.h  -  description
                             -------------------
                             Audio device for ESD sound daemon
    copyright            : (C) 2005 by mean
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
#ifdef USE_PULSE_SIMPLE
class pulseSimpleAudioDevice : public audioDevice
	 {
		 protected :
                    void  *instance;
		  public:
		  			pulseSimpleAudioDevice(void);
		     		virtual uint8_t init(uint8_t channels, uint32_t fq);
	    			virtual uint8_t play(uint32_t len, float *data);
		      		virtual uint8_t stop(void);
				uint8_t setVolume(int volume);
		 }     ;
#endif
