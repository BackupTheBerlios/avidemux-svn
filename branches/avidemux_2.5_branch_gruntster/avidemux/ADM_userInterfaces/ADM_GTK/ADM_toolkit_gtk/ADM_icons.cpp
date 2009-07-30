/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */





#include <string.h>
#include <stdio.h>

#include <gtk/gtk.h>

#include "ADM_gladeSupport.h"
#include "ADM_toolkitGtk.h"
#include "ADM_default.h"

#include "ADM_assert.h"
#include "xpm/about.xpm"

#include "xpm/audio_volume_medium.inc"
#include "xpm/avidemux_icon.inc"
#include "xpm/avidemux_icon_small.inc"
#include "xpm/film11.inc"
#include "xpm/film13.inc"
#include "xpm/film15.inc"
#include "xpm/film17.inc"
#include "xpm/film19.inc"
#include "xpm/film1.inc"
#include "xpm/film21.inc"
#include "xpm/film23.inc"
#include "xpm/film3.inc"
#include "xpm/film5.inc"
#include "xpm/film7.inc"
#include "xpm/film9.inc"
#include "xpm/first_frame.inc"
#include "xpm/gnome_calculator.inc"
#include "xpm/gnome_calculator_small.inc"
#include "xpm/last_frame.inc"
#include "xpm/markA.inc"
#include "xpm/markB.inc"
#include "xpm/next_black_frame.inc"
#include "xpm/next_frame.inc"
#include "xpm/next_key_frame.inc"
#include "xpm/output.inc"
#include "xpm/play.inc"
#include "xpm/preview_button.inc"
#include "xpm/preview.inc"
#include "xpm/previous_black_frame.inc"
#include "xpm/previous_frame.inc"
#include "xpm/previous_key_frame.inc"
#include "xpm/stock_allow_effects.inc"
#include "xpm/stock_filters_aging.inc"
#include "xpm/stop.inc"
#include "xpm/systray2.inc"
#include "xpm/systray.inc"
typedef enum 
{
        A_ICON_XPM,
        A_ICON_PNG
}ADM_Icon;

typedef struct name2xpm
{
        const ADM_Icon icon;
	const char *name;
	const void *data;
        
}name2xpm;

//static const char *xpm_Kbackward[] = {

name2xpm iconTranslation[]=
{
	{A_ICON_XPM,"about.xpm",	(void *)	xpm_about},

	{A_ICON_PNG,"Kbackward.xpm",	(void *)	previous_key_frame},	
	{A_ICON_PNG,"Kforward.xpm",	(void *)	next_key_frame},
	{A_ICON_PNG,"backward.xpm",	(void *)	previous_frame},	
	{A_ICON_PNG,"forward.xpm",	(void *)	next_frame},
	{A_ICON_PNG,"begin.xpm",	(void *)	first_frame},
	{A_ICON_PNG,"end.xpm",		(void *)	last_frame},
	{A_ICON_PNG,"play.xpm",		(void *)	play},
	{A_ICON_PNG,"stop.xpm",		(void *)	stop},
	{A_ICON_PNG,"markA.xpm",	(void *)	markA},
	{A_ICON_PNG,"markB.xpm",	(void *)	markB},
	{A_ICON_PNG,"xpm_prevblack.xpm",(void *)        previous_black_frame},
	{A_ICON_PNG,"xpm_nextblack.xpm",(void *)        next_black_frame},
        {A_ICON_PNG,"xpm_prevblack.xpm",(void *)        previous_black_frame},
        {A_ICON_PNG,"gnome-calculator.png",(void *)     gnome_calculator},
        {A_ICON_PNG,"gnome-calculator_small.xpm",(void *) gnome_calculator_small},
        {A_ICON_PNG,"preview.png",(void *) preview},
        {A_ICON_PNG,"output.png",(void *) output},
#if 0
        {A_ICON_PNG,"1.png",(void *) x1},
        {A_ICON_PNG,"2.png",(void *) x2},
        {A_ICON_PNG,"3.png",(void *) x3},
        {A_ICON_PNG,"4.png",(void *) x4},
        {A_ICON_PNG,"5.png",(void *) x5},
        {A_ICON_PNG,"6.png",(void *) x6},
        {A_ICON_PNG,"7.png",(void *) x7},
#endif
        {A_ICON_PNG,"avidemux_icon.xpm",        (void *) avidemux_icon},
        {A_ICON_PNG,"avidemux_icon_small.png",  (void *) avidemux_icon_small},
        {A_ICON_PNG,"systray.png",              (void *) systray},
        {A_ICON_PNG,"audio-volume-medium.png",  (void *) audio_volume_medium},
        {A_ICON_PNG,"preview-button.png",       (void *) preview_button},
	// Jakub nice animation
#define MKFILM(x) 	{A_ICON_PNG,"film"#x".xpm",                 (void *)film##x},
	MKFILM(1)
	MKFILM(3)
	MKFILM(5)
	MKFILM(7)
	MKFILM(9)
	MKFILM(11)
	MKFILM(13)
	MKFILM(15)
	MKFILM(17)
	MKFILM(19)
	MKFILM(21)
	MKFILM(23)
	// DUMMY
	{A_ICON_PNG,"systray.xpm",                 (void *)systray}
};
GdkPixbuf	*create_pixbuf                  (const gchar     *filename)
{
 	int nbIcon=sizeof(iconTranslation)/sizeof(name2xpm);
	int found=-1;
	
	for(int i=nbIcon-1;i>=0;i--)
	{
		if(!strcmp(iconTranslation[i].name,filename)) found=i;
	}
	if(found==-1)
	{
		printf("\n Mmmm problem with pixmap button. Did you change the code ?(%s)\n",filename);
		//ADM_assert(0);
                return NULL;
	
	}

   GdkPixbuf *pix=NULL;
   guint8 *gpix;
   switch(iconTranslation[found].icon)
        {
        case A_ICON_XPM:        
                pix= gdk_pixbuf_new_from_xpm_data((const char **)iconTranslation[found].data);
                break;
        case A_ICON_PNG:
                gpix=( guint8 *)iconTranslation[found].data;
                pix=gdk_pixbuf_new_from_inline (-1, gpix, FALSE, NULL);
                break;
        default:
                ADM_assert(0);
        
        }
  
  return pix;
}

GtkWidget	*create_pixmap                          (GtkWidget       *widget,
                         					               const gchar     *filename)
{
GdkPixbuf *pix=NULL;
GtkWidget *pixmap=NULL;
 
  UNUSED_ARG( widget );
   
  pix= create_pixbuf    (filename);
  pixmap= gtk_image_new_from_pixbuf(pix);
  return pixmap;
}

static GList *pixmaps_directories = NULL;

/* Use this function to set the directory containing installed pixmaps. */
void
add_pixmap_directory                   (const gchar     *directory)
{
  pixmaps_directories = g_list_prepend (pixmaps_directories,
                                        g_strdup (directory));
}

/* This is an internally used function to find pixmap files. */
static gchar*
find_pixmap_file                       (const gchar     *filename)
{
  GList *elem;

  /* We step through each of the pixmaps directory to find it. */
  elem = pixmaps_directories;
  while (elem)
    {
      gchar *pathname = g_strdup_printf ("%s%s%s", (gchar*)elem->data,
                                         G_DIR_SEPARATOR_S, filename);
      if (g_file_test (pathname, G_FILE_TEST_EXISTS))
        return pathname;
      g_free (pathname);
      elem = elem->next;
    }
  return NULL;
}

/* This is an internally used function to create pixmaps. */
GtkWidget*
create_pixmap_old                          (GtkWidget       *widget,
                                        const gchar     *filename)
{
  gchar *pathname = NULL;
  GtkWidget *pixmap;

  UNUSED_ARG( widget );

  if (!filename || !filename[0])
      return gtk_image_new ();

  pathname = find_pixmap_file (filename);

  if (!pathname)
    {
      g_warning (QT_TR_NOOP("Couldn't find pixmap file: %s"), filename);
      return gtk_image_new ();
    }

  pixmap = gtk_image_new_from_file (pathname);
  g_free (pathname);
  return pixmap;
}

/* This is an internally used function to create pixmaps. */
GdkPixbuf*
create_pixbuf_old                          (const gchar     *filename)
{
  gchar *pathname = NULL;
  GdkPixbuf *pixbuf;
  GError *error = NULL;

  if (!filename || !filename[0])
      return NULL;

  pathname = find_pixmap_file (filename);

  if (!pathname)
    {
      g_warning (QT_TR_NOOP("Couldn't find pixmap file: %s"), filename);
      return NULL;
    }

  pixbuf = gdk_pixbuf_new_from_file (pathname, &error);
  if (!pixbuf)
    {
      fprintf (stderr, "Failed to load pixbuf file: %s: %s\n",
               pathname, error->message);
      g_error_free (error);
    }
  g_free (pathname);
  return pixbuf;
}


