#ifndef xvidConfigDialog_h
#define xvidConfigDialog_h

extern "C"
{
#include "ADM_vidEnc_plugin.h"
}

static GtkWidget *create_dialog1 (void);
static GtkWidget *create_dialog3 (void);

static void loadOptions(GtkWidget *dialog, XvidOptions *options);
static void saveOptions(GtkWidget *dialog, XvidOptions *options);

static unsigned char editMatrix(unsigned char *inter, unsigned char *intra, GtkWidget *parent);
static void updateMode(GtkWidget *dialog, int encodeMode, int encodeModeParameter);
static int getCurrentEncodeMode(GtkWidget *dialog);

static int cb_mod(GtkObject *object, gpointer user_data);
static int ch_par_asinput(GtkObject *object, gpointer user_data);
static int entryEntry_changed(GtkObject* object, gpointer user_data);

#endif	// xvidConfigDialog_h
