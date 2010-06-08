/***************************************************************************
    copyright            : (C) 2001 by mean
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

#include "Q_props.h"
#include "avi_vars.h"
#include "avidemutils.h"
#include "ADM_video/ADM_vidMisc.h"
#include "ADM_toolkitQt.h"

extern const char *getStrFromAudioCodec( uint32_t codec);

propWindow::propWindow(QWidget *parent) : QDialog(parent)
 {
	 QString yesno[2] = {propWindow::tr("No"), propWindow::tr("Yes")};

     ui.setupUi(this);
     uint8_t gmc, qpel,vop;
 uint32_t info=0;
 uint32_t war,har;
 uint16_t hh, mm, ss, ms;
 const char *s;
  
     if (!avifileinfo)
        return;
  
        // Fetch info
        info=video_body->getSpecificMpeg4Info();
        vop=!!(info & ADM_VOP_ON);
        qpel=!!(info & ADM_QPEL_ON);
        gmc=!!(info & ADM_GMC_ON);

		ui.labeImageSize->setText(tr("%1 x %2").arg(avifileinfo->width).arg(avifileinfo->height));
		ui.labelFrameRate->setText(tr("%1 fps").arg(avifileinfo->fps1000 / 1000.F, 2, 'f', 3));
        ui.labelNbOfFrames->setText(tr("%1 frames").arg(avifileinfo->nb_frames));
        ui.label4CC->setText(fourCC::tostring(avifileinfo->fcc));

		if (avifileinfo->nb_frames)
		{
			frame2time(avifileinfo->nb_frames, avifileinfo->fps1000,
				&hh, &mm, &ss, &ms);

			ui.labelVideoDuration->setText(tr("%1:%2:%3.%4").arg(hh, 2, 10, QLatin1Char('0')).arg(mm, 2, 10, QLatin1Char('0')).arg(ss, 2, 10, QLatin1Char('0')).arg(ms, 3, 10, QLatin1Char('0')));
		}

        war=video_body->getPARWidth();
        har=video_body->getPARHeight();
        getAspectRatioFromAR(war,har, &s);
		ui.LabelAspectRatio->setText(tr("%1 (%2:%3)").arg(s).arg(war).arg(har));

#define SET_YES(a,b) ui.a->setText(yesno[b])

        SET_YES(LabelPackedBitstream,vop);
        SET_YES(LabelQuarterPixel,qpel);
        SET_YES(LabelGMC,gmc);
        
         WAVHeader *wavinfo=NULL;
        if (currentaudiostream) wavinfo=currentaudiostream->getInfo();

		if (wavinfo)
		{              
			switch (wavinfo->channels)
			{
				case 1:
					ui.labelChannels->setText(tr("Mono"));
					break;
				case 2:
					ui.labelChannels->setText(tr("Stereo"));
					break;
				default:
					ui.labelChannels->setText(QString("%1").arg(wavinfo->channels));
					break;
				}

                ui.labelFrequency->setText(tr("%1 Hz").arg(wavinfo->frequency));
                ui.labelBitrate->setText(tr("%1 Bps / %2 kbps").arg(wavinfo->byterate).arg(wavinfo->byterate * 8 / 1000));
                ui.labelACodec->setText(getStrFromAudioCodec(wavinfo->encoding));

                // Duration in seconds too
                if(currentaudiostream && wavinfo->byterate>1)
                {
                        uint32_t l=currentaudiostream->getLength();
                        double du;
                        du=l;
                        du*=1000;
                        du/=wavinfo->byterate;
                        ms2time((uint32_t)floor(du), &hh, &mm, &ss, &ms);

						ui.labelAudioDuration->setText(tr("%1:%2:%3.%4").arg(hh, 2, 10, QLatin1Char('0')).arg(mm, 2, 10, QLatin1Char('0')).arg(ss, 2, 10, QLatin1Char('0')).arg(ms, 3, 10, QLatin1Char('0')));
						ui.labelFileSize->setText(tr("%1 MB").arg(l / 1048576.F, 0, 'f', 2));
                }

                SET_YES(labelVBR,currentaudiostream->isVBR());
        } else
          {
			  ui.groupBoxAudio->setEnabled(false);
          }
 }
/**
    \fn DIA_properties
    \brief Display dialog with file information (size, codec, duration etc....)
*/
void DIA_properties( void )
{

      if (playing)
        return;
      if (!avifileinfo)
        return;
     // Fetch info
     propWindow propwindow(qtLastRegisteredDialog());
	 qtRegisterDialog(&propwindow);
     propwindow.exec();
	 qtUnregisterDialog(&propwindow);
}  
//********************************************
//EOF
