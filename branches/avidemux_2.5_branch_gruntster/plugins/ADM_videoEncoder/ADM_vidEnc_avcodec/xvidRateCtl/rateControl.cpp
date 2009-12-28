//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <string.h>
#include <stdlib.h>
#include "rateControl.h"

ADM_ratecontrol::ADM_ratecontrol(uint32_t fps1000, char *logname)
{
	_fps1000 = fps1000;
	_logname = strdup(logname);
	_state = RS_IDLE;
	_nbFrames++;
}

ADM_ratecontrol::~ADM_ratecontrol()
{
	free(_logname);
}
