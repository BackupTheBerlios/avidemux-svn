/***************************************************************************
                          ADM_deviceoss.h  -  description
                             -------------------
    begin                : Sat Sep 28 2002
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
#ifdef OSS_SUPPORT
class ossAudioDevice : public audioDevice
{
protected :
                                int oss_fd;
public:
                                        ossAudioDevice(void) { oss_fd=0;}
                        virtual uint8_t init( uint32_t channel,uint32_t fq ) ;
                        virtual uint8_t play( uint32_t len, uint8_t *data ) ;
                        virtual uint8_t stop( void ) ;
                                uint8_t setVolume(int volume);
}     ;
#endif
#ifdef CONFIG_DARWIN

class coreAudioDevice : public audioDevice
{
protected :
                                uint8_t _inUse;
public:
                                        coreAudioDevice(void) ;
                        virtual uint8_t init( uint32_t channel,uint32_t fq ) ;
                        virtual uint8_t play( uint32_t len, uint8_t *data ) ;
                        virtual uint8_t stop( void ) ;
};

#endif
