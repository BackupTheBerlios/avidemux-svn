#include "avi_vars.h"
#include "DIA_factory.h"
#include "ADM_outputs/oplug_mpegFF/ps_muxer.h"
#include "ADM_outputs/ADM_lavformat.h"

extern ps_muxer psMuxerConfig;

bool muxerMpegPsConfigure(void)
{
	diaMenuEntry format[] = {{PS_MUXER_VCD, "Video CD"}, {PS_MUXER_SVCD, "Super Video CD"}, {PS_MUXER_DVD, "DVD"}};

	diaElemMenu menuFormat(&psMuxerConfig.muxingType, "Muxing Format", 3, format, "");
	diaElemToggle alternate(&psMuxerConfig.acceptNonCompliant, "Allow Non-compliant Stream");

	diaElem *tabs[] = {&menuFormat, &alternate};

	return diaFactoryRun(("MPEG-PS Muxer"), 2, tabs);
}
