/***************************************************************************
                          ADM_codecNull.h  -  description
                             -------------------
    begin                : Fri Apr 19 2002
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
class decoderNull:public decoders
{
protected:

public:
  decoderNull (uint32_t w, uint32_t h):decoders (w, h)
  {
  }
  virtual ~ decoderNull ()
  {
  };
  virtual uint8_t uncompress (uint8_t * in, ADMImage * out, uint32_t len)
  {
    memcpy (out->data, in, len);
    return 1;
  }
  virtual uint8_t uncompress (uint8_t * in, ADMImage * out, uint32_t len,
			      uint32_t * f)
  {
    memcpy (out->data, in, len);
    return 1;
  }
};
