#ifndef ADM_TRANSLATE_H
#define ADM_TRANSLATE_H

typedef const char *TranslateFunction(const char *__domainname, const char *__msgid);
void ADM_translateInit(TranslateFunction *translateFunction);
const char *ADM_translate(const char *__domainname, const char *__msgid);

#endif