/***************************************************************************
                          GUI_binding.cpp  -  description
                             -------------------
    begin                : Fri Jan 17 2003
    copyright            : (C) 2003 by mean
    email                : fixounet@free.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 * GtkObject *sliderAdjustment=NULL;
 *sliderAdjustment=gtk_adjustment_new (52.1, 0, 99.99, 0.01, 0, 0);
 *sliderNavigate = gtk_hscale_new (GTK_ADJUSTMENT (sliderAdjustment));
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#  include <config.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include <assert.h>

#include "interface.h"
#include "support.h"
#include "math.h"
#include "default.h"
#include "ADM_gui2/GUI_render.h"
#include "gui_action.hxx"

#include "ADM_toolkit/ADM_debugID.h"
#define MODULE_NAME MODULE_UI
#include "ADM_toolkit/ADM_debug.h"
#include "ADM_toolkit/toolkit_gtk.h"


void frame2time(uint32_t frame, uint32_t fps, uint16_t * hh, uint16_t * mm,
	   uint16_t * ss, uint16_t * ms);

static GtkWidget *guiRootWindow=NULL;

static GtkWidget *guiDrawingArea=NULL;
static GtkWidget *guiSlider=NULL;

static GtkWidget *guiMarkA=NULL;
static GtkWidget *guiMarkB=NULL;
static GtkWidget *guiCurFrame=NULL;
static GtkWidget *guiTotalFrame=NULL;
static GtkWidget *guiCurTime=NULL;
static GtkWidget *guiTotalTime=NULL;

static GtkWidget *guiPreviewToggle=NULL;
static GtkWidget *guiOutputToggle=NULL;
static GtkWidget *guiAudioToggle=NULL;
static GtkWidget *guiVideoToggle=NULL;
static GdkCursor *guiCursorBusy=NULL;
static GdkCursor *guiCursorNormal=NULL;
static gint	  guiCursorEvtMask=0;

// heek !
static  GtkAdjustment *sliderAdjustment;


extern GtkWidget	*create_mainWindow (void);
extern void guiCallback(GtkMenuItem * menuitem, gpointer user_data);
extern void HandleAction(Action act);
extern void UI_on_key_press(GtkWidget *widget, GdkEventKey* event, gpointer user_data);

static uint8_t  bindGUI( void );
static gboolean destroyCallback(GtkWidget * widget,	  GdkEvent * event, gpointer user_data);
static gboolean  on_drawingarea1_expose_event(GtkWidget * widget,  GdkEventExpose * event, gpointer user_data);
// Currentframe taking/loosing focus
static int  UI_grabFocus( void);
static int  UI_looseFocus( void);
static void GUI_initCursor( void );
 void UI_BusyCursor( void );
 void UI_NormalCursor( void );
// Global
GtkAccelGroup *accel_group;
//
void guiCallback(GtkMenuItem * menuitem, gpointer user_data);

typedef struct buttonCallBack_S
{
	const char *name;
	const char *signal;
	Action action;

}buttonCallBack_S;




buttonCallBack_S buttonCallback[]=
{
	{"buttonPlay"			,"clicked"		,ACT_PlayAvi},
	{"buttonStop"			,"clicked"		,ACT_StopAvi},
	{"buttonPrevFrame"		,"clicked"		,ACT_PreviousFrame},
	{"buttonNextFrame"		,"clicked"		,ACT_NextFrame},
	{"buttonPrevKFrame"		,"clicked"		,ACT_PreviousKFrame},
	{"buttonNextKFrame"		,"clicked"		,ACT_NextKFrame},
	{"buttonMarkA"			,"clicked"		,ACT_MarkA},
	{"buttonMarkB"			,"clicked"		,ACT_MarkB},
	{"buttonBegin"			,"clicked"		,ACT_Begin},
	{"buttonEnd"			,"clicked"		,ACT_End},
	{"buttonOpen"			,"clicked"		,ACT_OpenAvi},
	{"buttonInfo"			,"clicked"		,ACT_AviInfo},
	{"buttonSaveAvi"		,"clicked"		,ACT_SaveAvi},
	{"buttonFilters"		,"clicked"		,ACT_VideoParameter},
	{"buttonAudioFilter"		,"clicked"		,ACT_AudioFilters},
	{"buttonVCodec"			,"clicked"		,ACT_VideoCodec},
	{"buttonACodec"			,"clicked"		,ACT_AudioCodec},
	{"buttonRecent"			,"clicked"		,ACT_RecentFiles},
	{"buttonPrevBlack"		,"clicked"		,ACT_PrevBlackFrame},
	{"buttonNextBlack"		,"clicked"		,ACT_NextBlackFrame},

	{"togglebuttonPreview"		,"toggled"		,ACT_PreviewToggle},
	{"toggleOutput"			,"toggled"		,ACT_OuputToggle},
	
	
		
	{"togglebuttonVideo"		,"toggled"		,ACT_VideoModeToggle},
	{"boxCurFrame"			,"editing_done"		,ACT_JumpToFrame},
	{"boxCurFrame"			,"activate"		,ACT_JumpToFrame},
	{"boxCurTime"			,"editing_done"		,ACT_TimeChanged},
	{"togglebuttonAudio"		,"toggled"		,ACT_AudioModeToggle}

};

///
///	Create main window and bind to it
///
uint8_t initGUI( void )
{
uint8_t ret=0;
		// create top window
		guiRootWindow=create_mainWindow();
		if(!guiRootWindow) return 0;
		
					
		// and seek global sub entity
		ret= bindGUI();		
		if(ret) gtk_widget_show(guiRootWindow);
		// Set it as always low level
		//gtk_window_set_keep_below(GTK_WINDOW(guiRootWindow), 1);
		renderInit();
		GUI_initCursor(  );
		
	return ret;
}
/** 
	Set the parameter widget as transient 
	for the main window
*/
void		gtk_transient(GtkWidget *widget)
{
	if(!guiRootWindow) return;
	gtk_window_set_modal(GTK_WINDOW(widget), 1);
	gtk_window_set_transient_for (GTK_WINDOW(widget),GTK_WINDOW(guiRootWindow));
	// set it modal also
	
	//gtk_window_set_transient_for (GTK_WINDOW(guiRootWindow),GTK_WINDOW(widget));


}
///_____________________________________________________
/// retrieve gtkwidget through their name
/// the aim is to avoid having to modify the glade generated file
///_____________________________________________________
uint8_t  bindGUI( void )
{

#define ADM_LOOKUP(a,b) a= lookup_widget (guiRootWindow,#b);if(!a) return 0;


	ADM_LOOKUP(guiDrawingArea,guiDrawing);
	ADM_LOOKUP(guiSlider,sliderNavigate);

	sliderAdjustment=gtk_range_get_adjustment (GTK_RANGE(guiSlider));

	ADM_LOOKUP(guiMarkA,labelMarkA);
	ADM_LOOKUP(guiMarkB,labelMarkB);
	ADM_LOOKUP(guiCurFrame,boxCurFrame);
	ADM_LOOKUP(guiTotalFrame,labelTotalFrame);
	
	ADM_LOOKUP(guiCurTime,boxCurTime);
	ADM_LOOKUP(guiTotalTime,labelTotalTime);
	
	ADM_LOOKUP(guiPreviewToggle,togglebuttonPreview);
	ADM_LOOKUP(guiOutputToggle,toggleOutput);
	ADM_LOOKUP(guiAudioToggle,togglebuttonAudio);
	ADM_LOOKUP(guiVideoToggle,togglebuttonVideo);
#undef ADM_LOOKUP
  // bind menu
 #define CALLBACK(x,y) gtk_signal_connect(GTK_OBJECT(lookup_widget(guiRootWindow,#x)), "activate", \
                      GTK_SIGNAL_FUNC(guiCallback),                   (void *) y)

 	#include "GUI_menumap.h"
  #undef CALLBACK
  /// /bind menu
	
// destroy
	 gtk_object_set_data_full(GTK_OBJECT(guiRootWindow),
			     "guiRootWindow",
			     guiRootWindow,
			     (GtkDestroyNotify) destroyCallback);
	
	
//	now add callbacks
#define ADD_SIGNAL(a,b,c)  gtk_signal_connect(GTK_OBJECT(a),b, \
		       GTK_SIGNAL_FUNC(guiCallback), (void *) c);
				       
      	ADD_SIGNAL(guiSlider,"value_changed",ACT_Scale);
	
	// We need to know when the current frame box has the focus to cancel
	// shortcuts
	
	gtk_signal_connect(GTK_OBJECT(lookup_widget(guiRootWindow,"boxCurFrame")), "focus_in_event", 
                      GTK_SIGNAL_FUNC(UI_grabFocus),                   (void *) NULL);	   
	gtk_signal_connect(GTK_OBJECT(lookup_widget(guiRootWindow,"boxCurFrame")), "focus_out_event", 	
                      GTK_SIGNAL_FUNC(UI_looseFocus),                   (void *) NULL);	 




		       
// Callbacks for buttons
		uint32_t nb=sizeof(buttonCallback)/sizeof(buttonCallBack_S); 
		GtkWidget *bt;
		
		
		for(uint32_t i=0;i<nb;i++)
		{
			bt= lookup_widget (guiRootWindow,buttonCallback[i].name);
			if(!bt)
			{
				printf("Binding failed for %s\n",buttonCallback[i].name);
				assert(0);
			}
			ADD_SIGNAL(bt,buttonCallback[i].signal,buttonCallback[i].action);
			GTK_WIDGET_UNSET_FLAGS (bt, GTK_CAN_FOCUS);			
		}

	GTK_WIDGET_SET_FLAGS (lookup_widget(guiRootWindow,"boxCurFrame"), GTK_CAN_FOCUS);
		       
// set some tuning
    gtk_widget_set_usize(guiDrawingArea, 512, 288);
    
// hscale    
    GTK_WIDGET_UNSET_FLAGS (guiSlider, GTK_CAN_FOCUS);	
    gtk_widget_show(guiSlider);
    // And, the size now scales to the width of the window.
    gtk_widget_set_usize(guiSlider, 0, 0);
    // Plus, two-decimal precision.
    gtk_scale_set_digits(GTK_SCALE(guiSlider), 2);
    // And continuous updates!
    gtk_range_set_update_policy (GTK_RANGE (guiSlider), GTK_UPDATE_CONTINUOUS);	    
    
    // keyboard events
    /*
     gtk_signal_connect_after(GTK_OBJECT(guiRootWindow), "key_press_event",
		       GTK_SIGNAL_FUNC(UI_on_key_press),
		       NULL);
   */
 	gtk_signal_connect(GTK_OBJECT(guiDrawingArea), "expose_event",
		       GTK_SIGNAL_FUNC(on_drawingarea1_expose_event),
		       NULL);
    return 1;

}
gboolean  on_drawingarea1_expose_event(GtkWidget * widget,  GdkEventExpose * event, gpointer user_data)
{
UNUSED_ARG(widget);
UNUSED_ARG(event);
UNUSED_ARG(user_data);
	
		//renderRefresh();
		renderExpose();
		
    	return true;
}

///
///	Return the widget containing the image
///
GtkWidget *getDrawWidget( void )
{
	return guiDrawingArea;

}
///
///	Set slider position
///
static int _upd_in_progres=0;
void UI_setScale( double val )
{
if(_upd_in_progres) return;
 _upd_in_progres++;
   gtk_adjustment_set_value( GTK_ADJUSTMENT(sliderAdjustment),(  gdouble  ) val );
   gtk_signal_emit_by_name (GTK_OBJECT (sliderAdjustment), "changed");
 _upd_in_progres--;
   
}
void UI_setTitle(char *name)
{
	char title[1024];

		strcpy(title,"Avidemux 2 ");
		strncat(title,name,200);

 	gtk_window_set_title (GTK_WINDOW (guiRootWindow), title);

}
void UI_setFrameType( uint32_t frametype)
{
GtkLabel *wid=(GtkLabel *)lookup_widget(guiRootWindow,"labelFrameType");

	if(frametype==0xff)
  			gtk_label_set_text( wid,"Frame: ?");
	else
		{
			
			if(frametype & 0x10)
					gtk_label_set_text( wid,"Frame: I");
			else
				if(frametype & 0x4000)
						gtk_label_set_text( wid,"Frame: B");
					else
						gtk_label_set_text( wid,"Frame: P");


		}


}
///
///	Get slider position
///
double  UI_readScale( void )
{
 
	return (double)GTK_ADJUSTMENT(sliderAdjustment)->value;
 
}
void UI_updateFrameCount(uint32_t curFrame)
{
    //char text[80];   
    // frames 
    //sprintf(text, "%lu ", curFrame);
//    gtk_label_set_text((GtkLabel *) guiCurFrame, text);
	gtk_write_entry(guiCurFrame,curFrame);
    
}
void UI_setFrameCount(uint32_t curFrame,uint32_t total)
{
    char text[80]; 
    if(total) total--; // We display from 0 to X  
    // frames 
   // sprintf(text, "%lu ", curFrame);
   // gtk_label_set_text((GtkLabel *) guiCurFrame, text);
    gtk_write_entry(guiCurFrame,curFrame);
      sprintf(text, "/ %lu ", total);
    gtk_label_set_text((GtkLabel *) guiTotalFrame, text);
    
}
void UI_updateTimeCount(uint32_t curFrame,uint32_t fps)
{
  char text[80];   
 uint16_t mm,hh,ss,ms;
 
 	frame2time(curFrame,fps, &hh, &mm, &ss, &ms);
  	sprintf(text, "%02d:%02d:%02d.%03d", hh, mm, ss, ms);
//     gtk_label_set_text((GtkLabel *) guiCurTime, text);
	gtk_write_entry_string(guiCurTime,text);

}
void UI_setTimeCount(uint32_t curFrame,uint32_t total, uint32_t fps)
{
  char text[80];   
 uint16_t mm,hh,ss,ms;
 
 	frame2time(curFrame,fps, &hh, &mm, &ss, &ms);
  	sprintf(text, "%02d:%02d:%02d.%03d", hh, mm, ss, ms);
     	//gtk_label_set_text((GtkLabel *) guiCurTime, text);
     	gtk_write_entry_string(guiCurTime,text);
     
 	frame2time(total,fps, &hh, &mm, &ss, &ms);
  	sprintf(text, "%02d:%02d:%02d.%03d", hh, mm, ss, ms);
     	gtk_label_set_text((GtkLabel *) guiTotalTime, text);     
     

}
///
/// Called upon destroy event
/// just cleanly exit the application
///

gboolean destroyCallback(GtkWidget * widget,
				  GdkEvent * event, gpointer user_data)
{
    UNUSED_ARG(widget);
    UNUSED_ARG(event);
    UNUSED_ARG(user_data);
    HandleAction(ACT_Exit);
    return 1;
}
int UI_grabFocus( void)
{
//	printf("Grabbing focus\n");
#define RM(x,y)   gtk_widget_remove_accelerator (lookup_widget(guiRootWindow,#x), accel_group, \
                              y, (GdkModifierType) 0  );
	RM(buttonNextFrame,GDK_KP_6);
	RM(buttonPrevFrame,GDK_KP_4);
	RM(buttonNextKFrame,GDK_KP_8);
	RM(buttonPrevKFrame,GDK_KP_2);
	RM(delete1,GDK_Delete);
#define RMCTRL(x,y)   gtk_widget_remove_accelerator (lookup_widget(guiRootWindow,#x), accel_group, \
                              y, (GdkModifierType) GDK_CONTROL_MASK  );	
	RMCTRL(paste1,GDK_V);
	RMCTRL(copy1,GDK_C);
	RMCTRL(cut1,GDK_X);
	return FALSE;
	
}
int UI_looseFocus( void)
{
#define ADD(x,y)  gtk_widget_add_accelerator (lookup_widget(guiRootWindow,#x), "clicked", accel_group, \
                              y, (GdkModifierType) 0, \
                              GTK_ACCEL_VISIBLE);
//	printf("Dropping focus\n");			      
	ADD(buttonNextFrame,GDK_KP_6);
	ADD(buttonPrevFrame,GDK_KP_4);
	ADD(buttonNextKFrame,GDK_KP_8);
	ADD(buttonPrevKFrame,GDK_KP_2);
	
	gtk_widget_add_accelerator (lookup_widget(guiRootWindow,"delete1"), "activate", accel_group, \
                              GDK_Delete, (GdkModifierType) 0, \
                              GTK_ACCEL_VISIBLE);	
	
#define ADDCTRL(x,y) gtk_widget_add_accelerator (lookup_widget(guiRootWindow,#x), "activate", accel_group, \
                              y, (GdkModifierType) GDK_CONTROL_MASK, \
                              GTK_ACCEL_VISIBLE);
	
	ADDCTRL(paste1,GDK_V);
	ADDCTRL(copy1,GDK_C);
	ADDCTRL(cut1,GDK_X);
	
	return FALSE;
}
void UI_setMarkers(uint32_t a, uint32_t b )
{
char string[500];
		sprintf(string,"A: %06lu",a);
        	gtk_label_set_text(GTK_LABEL(guiMarkA),string);
		sprintf(string,"B: %06lu",b);
        	gtk_label_set_text(GTK_LABEL(guiMarkB),string);
}
///
///	Return status of preview button toggle
///	1-> Activated
/// 	0-> deActivated

uint8_t UI_getPreviewToggleStatus( void )
{                         //gtk_toggle_button_get_active

	if(TRUE==gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(guiPreviewToggle))  )
 		return 1;
   	else
    	return 0;
}
///
///	Update the preview button toggle
///
void UI_setPreviewToggleStatus( uint8_t status )
{
gint b;
	 if(status) b=TRUE;
  	else			b=FALSE;
     gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(guiPreviewToggle),b);


}
/**
	Set/unset the toggle button audio process
*/
void UI_setAProcessToggleStatus( uint8_t status )
{
gint b;
	 if(status) b=TRUE;
  	else			b=FALSE;

	if(b!=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(guiAudioToggle)))
     		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(guiAudioToggle),b);
	aprintf("++ audio toggle : %d(%d)\n",b,status);

}
/*
	Set/unset the toggle button video process
*/
void UI_setVProcessToggleStatus( uint8_t status )
{
gint b;
	 if(status) 		b=TRUE;
  	else			b=FALSE;
	if(b!=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(guiVideoToggle)))
	{
     		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(guiVideoToggle),b);
		aprintf("Changing it to %d\n",b);
	}
	aprintf("++ video toggle : %d\n",b);

}

///
///	This is a relay function to do UI events -> gtk_gui.cpp
///
void guiCallback(GtkMenuItem * menuitem, gpointer user_data)
{
    UNUSED_ARG(menuitem);
    Action act;
    uint32_t aint;

    aint = (uint32_t) user_data;
    act = (Action) aint;
    if(act==ACT_Scale)
    {
    	if( _upd_in_progres) return;
	_upd_in_progres++;
   	 HandleAction(act);
	_upd_in_progres--;
   	return;
    }
     HandleAction(act);
}
void UI_JumpDone(void )
{


}
uint32_t UI_readCurFrame( void )
{
int i;
	i=gtk_read_entry(guiCurFrame);
	if(i<0) i=0;
	return  (uint32_t)i;
	 
}
/**
		in=0 -> arts1
		in=1 -> alsa
		in=2->oss

*/

void UI_iconify( void )
{
	gtk_window_iconify(GTK_WINDOW(guiRootWindow));
}
void UI_deiconify( void )
{
	gtk_window_deiconify(GTK_WINDOW(guiRootWindow));
}
void GUI_initCursor( void )
{
	
	guiCursorBusy=gdk_cursor_new(GDK_WATCH);
	guiCursorNormal=gdk_cursor_new(GDK_X_CURSOR);
}
// Change cursor and drop all events
void UI_BusyCursor( void )
{
	 gdk_window_set_cursor((guiRootWindow->window),
                                          guiCursorBusy);
/*					  
	guiCursorEvtMask=gtk_widget_get_events(guiRootWindow);
	gtk_widget_set_events(guiRootWindow,0);
*/	

	
}
void UI_NormalCursor( void )
{
//	gtk_widget_set_events(guiRootWindow,guiCursorEvtMask);
	gdk_window_set_cursor((guiRootWindow->window),
                                          NULL); //guiCursorNormal);

	
}
