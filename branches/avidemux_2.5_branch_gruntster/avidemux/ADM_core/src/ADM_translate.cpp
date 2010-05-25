#include <stdlib.h>
#include <stdio.h>

#include "ADM_translate.h"

static TranslateFunction *_translateFunction = NULL;

void ADM_translateInit(TranslateFunction *translateFunction)
{
	_translateFunction = translateFunction;
}

const char *ADM_translate(const char *__domainname, const char *__msgid)
{
	return _translateFunction(__domainname, __msgid);
}