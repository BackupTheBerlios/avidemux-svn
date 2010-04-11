/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "ADM_toolkitGtk.h"
#include <vector>

#include "DIA_coreToolkit.h"
#include "DIA_fileSel.h"
#include "ADM_videoFilter.h"
#include "ADM_videoFilter_internal.h"
#include "ADM_editor/ADM_edit.hxx"
#include "ADM_video/ADM_videoNull.h"

#include "ADM_video/ADM_vidPartial.h"
#include "avi_vars.h"

typedef enum 
{
  A_BEGIN = 10,
  A_ADD,
  A_CONFIGURE,
  A_UP,
  A_DOWN,
  A_REMOVE,
  A_DONE,
  A_LOAD,
  A_SAVE,
  A_PREVIEW,
  A_PARTIAL,
  A_SCRIPT,
  A_CLOSE,
  A_DOUBLECLICK,
  A_END
}gui_act;
//___________________________________________
extern AVDMGenericVideoStream *filterCreateFromTag (VF_FILTERS tag,
						    CONFcouple * conf,
						    AVDMGenericVideoStream *
						    in);
extern const char  *filterGetNameFromTag(VF_FILTERS tag);
extern uint8_t DIA_filterPreview(const char *captionText, AVDMGenericVideoStream *videoStream, uint32_t frame);
//___________________________________________
extern FILTER videofilters[VF_MAX_FILTER];
extern uint32_t nb_active_filter;
extern std::vector <FilterDescriptor *> allfilters;
extern std::vector <FilterDescriptor *> filterCategories[VF_MAX];


extern ADM_Composer *video_body;
//___________________________________________
static gulong row_inserted_id;
static gulong row_deleted_id;

static void on_treeActive_row_deleted(GtkTreeModel *treemodel, GtkTreePath *arg1, gpointer user_data);
static void on_treeActive_row_inserted(GtkTreeModel *treemodel, GtkTreePath *arg1, GtkTreeIter *arg2, gpointer user_data);
static void on_tree_size_allocate(GtkWidget *widget, GtkAllocation *allocation, GtkCellRenderer *cell);
static void on_action (gui_act action);
static void on_action_double_click (GtkButton * button, gpointer user_data);
static void on_action_double_click_1 (GtkButton * button, gpointer user_data);
static void on_action_change_category (GtkWidget *tree, gpointer user_data);
static void updateFilterList (void);
static VF_FILTERS getFilterFromSelection (void);
static void button_clicked(GtkWidget * wid, gpointer user_data);
//___________________________________________
#define NB_TREE 9
const   gint tree_width = 300;
static  uint32_t max = 0;
static  GtkListStore *storeCategories;
static  GtkWidget *treeCategories;
static  GtkWidget *treeAvailable;
static  GtkWidget *treeActive;
static  GtkListStore *stores[NB_TREE];
static  GtkTreeViewColumn *columns[NB_TREE];
static  GtkCellRenderer *renderers[NB_TREE];
static  int startFilter[NB_TREE];
//___________________________________________
static GtkWidget *createTree (const gchar *ref);
static GtkWidget *createFilterDialog (void);
extern GtkWidget *create_dialog1 (void);
static GtkWidget *dialog = 0;
//___________________________________________


// Open the main filter dialog and call the handlers
// if needed.
int
GUI_handleVFilter (void)
{

    getLastVideoFilter ();	// expand video to full size

    // sanity check
    if (nb_active_filter == 0)
    {
        nb_active_filter = 1;
        videofilters[0].filter =
        new AVDMVideoStreamNull (video_body, frameStart,
				 frameEnd - frameStart);
    }
        dialog = createFilterDialog();
        GdkWMDecoration decorations=(GdkWMDecoration)0;
        gtk_widget_realize(dialog);
        gdk_window_set_decorations(dialog->window, (GdkWMDecoration)(GDK_DECOR_ALL | GDK_DECOR_MINIMIZE));
        /*GdkScreen* screen = gdk_screen_get_default();
        gint width = gdk_screen_get_width(screen);
        if(width>=1024)
            gtk_window_set_default_size(GTK_WINDOW(dialog), 900, 600);*/
        updateFilterList ();
        gtk_register_dialog (dialog);
        //gtk_widget_show (dialog);
        int r;
        while(1)
        {
          r=gtk_dialog_run(GTK_DIALOG(dialog));
          if(r>A_BEGIN && r<A_END)
          {
            on_action((gui_act)r);
          }
          else break;
        };
        gtk_unregister_dialog (dialog);
        gtk_widget_destroy(dialog);
        dialog=NULL;
        
    return 1;
    
}

//
// One of the button of the main dialog was pressed
// Retrieve also the associated filter and handle
// the action
//______________________________________________________
void on_action (gui_act action)
{
    uint32_t action_parameter;
    VF_FILTERS tag = VF_INVALID;

    action_parameter = 0;
    if (nb_active_filter > 1)
        if (getSelectionNumber(nb_active_filter - 1,
                                WID(treeviewActive),
                                stores[0],
                                &action_parameter))
            action_parameter++;

    switch (action)
    {

    case A_ADD:
        tag = getFilterFromSelection();
        if (tag == VF_INVALID) break;
        CONFcouple *coup;
        videofilters[nb_active_filter].filter =
            filterCreateFromTag (tag, NULL, videofilters[nb_active_filter - 1].filter);
        videofilters[nb_active_filter].tag = tag;
        if(!videofilters[nb_active_filter].filter->
                    configure (videofilters[nb_active_filter - 1].filter))
        {
            delete videofilters[nb_active_filter].filter;
            break;
        }
        videofilters[nb_active_filter].filter->getCoupledConf (&coup);
        videofilters[nb_active_filter].conf = coup;
        nb_active_filter++;
        updateFilterList ();
        setSelectionNumber(nb_active_filter-1, WID(treeviewActive), stores[0], nb_active_filter-2);
        break;
    default:
    case A_DOUBLECLICK:
        printf ("Double clicked..");
    case A_CONFIGURE:
        if(!action_parameter) break;
        if(!videofilters[action_parameter].filter->
            configure (videofilters[action_parameter - 1].filter)) break;
        CONFcouple *couple;
        videofilters[action_parameter].filter->getCoupledConf (&couple);
        videofilters[action_parameter].conf = couple;
        getFirstVideoFilter ();
        updateFilterList ();
        setSelectionNumber(nb_active_filter-1, WID(treeviewActive), stores[0], action_parameter-1);
        break;

    case A_PARTIAL:
        if (!action_parameter) break;
        AVDMGenericVideoStream *replace;
        CONFcouple *conf;
        conf = videofilters[action_parameter].conf;
        if (videofilters[action_parameter].tag == VF_PARTIAL_FILTER)	// cannot recurse
        {
            GUI_Error_HIG (QT_TR_NOOP("The filter is already partial"), NULL);
            break;
	    }
        replace =
		new ADMVideoPartial (videofilters[action_parameter - 1].
				     filter,
				     videofilters[action_parameter].tag,
				     conf);
        if(replace->configure (videofilters[action_parameter - 1].filter))
        {
            delete videofilters[action_parameter].filter;
			if (conf) delete conf;
			videofilters[action_parameter].filter = replace;
			replace->getCoupledConf (&conf);
			videofilters[action_parameter].conf = conf;
			videofilters[action_parameter].tag = VF_PARTIAL_FILTER;
			getFirstVideoFilter ();
			updateFilterList ();
			setSelectionNumber(nb_active_filter-1, WID(treeviewActive), stores[0], action_parameter-1);
        }
        else delete replace;
        break;

    case A_UP:
        if (action_parameter < 2) break;
        // swap action parameter & action parameter -1
        FILTER tmp;
        memcpy (&tmp, &videofilters[action_parameter - 1], sizeof (FILTER));
        memcpy (&videofilters[action_parameter - 1],
            &videofilters[action_parameter], sizeof (FILTER));
        memcpy (&videofilters[action_parameter], &tmp, sizeof (FILTER));
        getFirstVideoFilter ();
        // select action_parameter -1
        updateFilterList ();
        setSelectionNumber (nb_active_filter - 1,
			      WID(treeviewActive),
			      stores[0], action_parameter - 2);
        break;

    case A_DOWN:
        if (((int) action_parameter < (int) (nb_active_filter - 1)) && (action_parameter))
        {
            // swap action parameter & action parameter -1
            FILTER tmp;
            memcpy (&tmp, &videofilters[action_parameter + 1], sizeof (FILTER));
            memcpy (&videofilters[action_parameter + 1],
                        &videofilters[action_parameter], sizeof (FILTER));
            memcpy (&videofilters[action_parameter], &tmp, sizeof (FILTER));
            getFirstVideoFilter ();
            updateFilterList ();
            setSelectionNumber (nb_active_filter - 1,
			      WID(treeviewActive),
			      stores[0], action_parameter);
        }
        break;

    case A_REMOVE:
		VF_FILTERS tag;
		AVDMGenericVideoStream *old;
		// we store the one we will delete
		if (action_parameter < 1) break;
		if (videofilters[action_parameter].conf)
		{
			delete videofilters[action_parameter].conf;
			videofilters[action_parameter].conf = NULL;
		}
		// recreate derivated filters
		for (uint32_t i = action_parameter + 1; i < nb_active_filter; i++)
	    {
			delete videofilters[i - 1].filter;
			videofilters[i - 1].filter = filterCreateFromTag(videofilters[i].tag,
															 videofilters[i].conf,
															 videofilters[i - 2].filter);
			videofilters[i - 1].conf = videofilters[i].conf;
			videofilters[i - 1].tag = videofilters[i].tag;
	    }
		delete videofilters[nb_active_filter - 1].filter;
		videofilters[nb_active_filter - 1].filter = NULL;
		nb_active_filter--;
        updateFilterList ();
        if(!setSelectionNumber(nb_active_filter-1, WID(treeviewActive), stores[0], action_parameter-1))
            setSelectionNumber(nb_active_filter-1, WID(treeviewActive), stores[0], action_parameter-2);
		break;

    case A_DONE:

        break;

    case A_PREVIEW:
        if (!action_parameter) break;

        extern uint32_t curframe;
        DIA_filterPreview(QT_TR_NOOP("Preview"), videofilters[action_parameter].filter, curframe);

        break;

    case A_LOAD:
#ifdef USE_LIBXML2
        GUI_FileSelRead (QT_TR_NOOP("Load set of filters"), filterLoadXml);
#else
        GUI_FileSelRead (QT_TR_NOOP("Load set of filters"), filterLoad);
#endif
        updateFilterList ();
        setSelectionNumber(nb_active_filter-1, WID(treeviewActive), stores[0], 0);
        break;
    case A_CLOSE:
        //gtk_widget_destroy(dialog);
      gtk_signal_emit_by_name(GTK_OBJECT(dialog),"delete-event");
        
        break;
    case A_SAVE:
        if (nb_active_filter < 2)
        {
            GUI_Error_HIG (QT_TR_NOOP("Nothing to save"), NULL);
        }
        else
#ifdef USE_LIBXML2
            GUI_FileSelWrite (QT_TR_NOOP("Save set of filters"), filterSaveXml);
#else
            GUI_FileSelWrite (QT_TR_NOOP("Save set of filters"), filterSave);
#endif
        break;
#if 0
    default:
        printf ("Unknown action :%d, action param %d\n", action, action_parameter);
        ADM_assert (0);
#endif
    } //end of switch
}
/*
 	\fn getFilterFromSelection
	\brief returns the tag of the selected filter
*/
VF_FILTERS getFilterFromSelection (void)
{
    uint32_t sel = 0, category = 0;
	uint8_t ret = 0;
    VF_FILTERS tag = VF_INVALID;
    // 1- identify the current tab/treeview we are in
    ret = getSelectionNumber (VF_MAX+1, treeCategories, storeCategories, &category);
    // then get the selection
    category++;
    if (ret == 1)
    {
        if ((ret = getSelectionNumber (max, treeAvailable, stores[category], &sel)))
        {
            tag = filterCategories[category-1][sel]->tag;
        }
    }
    return tag;
}

/**
 * 	\fn createTree
 *  \brief Set up a TreeView for the Available and Active Filters list
 */
GtkWidget *createTree (const gchar *ref)
{
	GtkWidget *tree = lookup_widget (dialog, ref); 
	GtkCellRenderer *renderer = gtk_cell_renderer_text_new ();
	GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes ("", renderer, "markup", (GdkModifierType) 0, NULL);
	gtk_widget_set_size_request (tree, tree_width, -1);
	g_object_set (renderer, "ypad", 6, NULL);
	g_object_set (renderer, "wrap-width", tree_width-8, NULL);
	g_object_set (renderer, "wrap-mode", PANGO_WRAP_WORD_CHAR, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW(tree), column);
	g_signal_connect (G_OBJECT(tree), "size-allocate", G_CALLBACK(on_tree_size_allocate), renderer);
	return tree;
}

/**
 * 	\fn createFilterDialog
 *  \brief Create the dialog including list of all filters available on the left.
 * 
 */
GtkWidget *
createFilterDialog (void)
{
	dialog = create_dialog1();
	GtkTreeIter iter;

	//Connect buttons from the Available and Active Filters list
	#define CONNECT(button, action) g_signal_connect (G_OBJECT(WID(button)), "clicked", G_CALLBACK(button_clicked), (void*)action);

	CONNECT (buttonAdd, A_ADD)
	CONNECT (buttonRemove, A_REMOVE)
	CONNECT (buttonProperties, A_CONFIGURE)
	CONNECT (buttonUp, A_UP)
	CONNECT (buttonDown, A_DOWN)
	CONNECT (buttonPreview, A_PREVIEW)
	CONNECT (buttonPartial, A_PARTIAL)
	CONNECT (buttonOpen, A_LOAD)
	CONNECT (buttonSave, A_SAVE)

	//Create TreeView with the filter categories
	enum
	{
		ICON_COLUMN,
		CATEGORY_COLUMN,
		N_COLUMNS
	};

	storeCategories = gtk_list_store_new (N_COLUMNS, GDK_TYPE_PIXBUF, G_TYPE_STRING);
	GdkPixbuf *pb;

	#define ADD_CATEGORY(icon, category) \
		gtk_list_store_append (storeCategories, &iter); \
		pb = create_pixbuf (#icon); \
		gtk_list_store_set (storeCategories, &iter, ICON_COLUMN, pb, CATEGORY_COLUMN, QT_TR_NOOP(#category), -1);

	ADD_CATEGORY (1.png, Transform)
	ADD_CATEGORY (2.png, Interlacing)
	ADD_CATEGORY (4.png, Colors)
	ADD_CATEGORY (5.png, Noise)
	ADD_CATEGORY (3.png, Sharpness)
	ADD_CATEGORY (7.png, Subtitles)
	ADD_CATEGORY (6.png, Miscellaneous)
	ADD_CATEGORY (film1.xpm, External)

	treeCategories = lookup_widget (dialog, "mainTree");
	gtk_tree_view_set_model (GTK_TREE_VIEW(treeCategories), GTK_TREE_MODEL(storeCategories));
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW(treeCategories), FALSE);

	GtkCellRenderer *renderer;
	renderer = gtk_cell_renderer_pixbuf_new();

	GtkTreeViewColumn *column;
	column = gtk_tree_view_column_new ();
	gtk_tree_view_append_column (GTK_TREE_VIEW(treeCategories), column);
	gtk_tree_view_column_pack_start (column, renderer, TRUE);
	gtk_tree_view_column_add_attribute (column, renderer, "pixbuf", ICON_COLUMN);

	column = gtk_tree_view_column_new ();
	gtk_tree_view_append_column (GTK_TREE_VIEW(treeCategories), column);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (column, renderer, TRUE);
	gtk_tree_view_column_add_attribute (column, renderer, "text", CATEGORY_COLUMN);
	g_object_set (renderer, "ypad", 6, NULL);

	//Create ListStores with filters, store zero is the Active Filters list
	stores[0]=gtk_list_store_new (3, G_TYPE_STRING, G_TYPE_INT, G_TYPE_POINTER);

	#define CREATE_STORE(x) stores[x] = gtk_list_store_new (1, G_TYPE_STRING);

	CREATE_STORE (1)
	CREATE_STORE (2)
	CREATE_STORE (3)
	CREATE_STORE (4)
	CREATE_STORE (5)
	CREATE_STORE (6)
	CREATE_STORE (7)
	CREATE_STORE (8)

	//load stores with filter names, get start filter for each page
	char *str=NULL;

	// Dispatch each category to the matching tree
	for(int current_tree=0;current_tree< VF_MAX;current_tree++)
	{
		std::vector <FilterDescriptor *> vec=filterCategories[current_tree];
		for (uint32_t i = 0; i < vec.size(); i++)
		{
			str = g_strconcat(
				"<span weight=\"bold\">", vec[i]->name, "</span>\n",
				"<span size=\"smaller\">", vec[i]->description, "</span>", NULL);

			gtk_list_store_append (stores[current_tree+1], &iter);
			gtk_list_store_set (stores[current_tree+1], &iter, 0, str ,-1);
			g_free(str);
			max++;
		}
	}

	//Create TreeViews with available and active filters
	treeAvailable = createTree ("treeviewAvailable");
	g_signal_connect (G_OBJECT(treeAvailable), "row-activated", G_CALLBACK(on_action_double_click), (void *) dialog);
	g_signal_connect (G_OBJECT(treeCategories), "cursor-changed", G_CALLBACK(on_action_change_category), NULL);

	treeActive = createTree ("treeviewActive");
	gtk_tree_view_set_model (GTK_TREE_VIEW(treeActive), GTK_TREE_MODEL(stores[0]));
	gtk_tree_view_set_reorderable (GTK_TREE_VIEW(treeActive), true);
	g_signal_connect (G_OBJECT(treeActive), 
		"row-activated", G_CALLBACK(on_action_double_click_1), 
		(void *)NULL);
	row_inserted_id = g_signal_connect (G_OBJECT(stores[0]), 
		"row-inserted", 
		G_CALLBACK(on_treeActive_row_inserted), 
		(void *)NULL);
	row_deleted_id = g_signal_connect (G_OBJECT(stores[0]), 
		"row-deleted", 
		G_CALLBACK(on_treeActive_row_deleted), 
		(void *)NULL);

	//Select the first category in treeCategories and show its filters in treeAvailable
	gtk_widget_grab_focus (treeCategories);
	GtkTreePath *gp = gtk_tree_path_new_first ();
	gtk_tree_view_set_cursor (GTK_TREE_VIEW(treeCategories), gp, NULL, TRUE);
    return dialog;
}

/**
 * 	\fn updateFilterList
 *  \brief Update the list of activated filters
 */
void updateFilterList (void)
{
    g_signal_handler_block(stores[0], row_inserted_id);
    g_signal_handler_block(stores[0], row_deleted_id);
    GtkTreeIter iter;
    char *str;
    VF_FILTERS fil;
    gtk_list_store_clear (stores[0]);
    for (uint32_t i = 1; i < nb_active_filter; i++)
    {
		fil = videofilters[i].tag;

		if (fil != VF_INTERNAL)
		{
			gtk_list_store_append (stores[0], &iter);

			const char * name = filterGetNameFromTag(fil);
			const char * conf = videofilters[i].filter->printConf ();
			int namelen = strlen (name);
			while (*conf == ' ')
				++conf;
			if (strncasecmp (name, conf, namelen) == 0)
			{
				conf += namelen;
				while (*conf == ' ' || *conf == ':')
					++conf;
			}
			const char * smallstart = "";
			const char * smallend = "";
			const char * namesmallstart = "";
			const char * namesmallend = "";
			int conflen = strlen (conf);
			if (conflen > 120)
			{
				smallstart = "<small>";
				smallend = "</small>";
				if (conflen > 180)
				{
					namesmallstart = smallstart;
					namesmallend = smallend;
				}
			}

			str = g_strconcat("<span  weight=\"bold\">",
				namesmallstart,
				name,
				namesmallend,
				"</span>\n",
				"<span size=\"smaller\">",
				smallstart,
				conf,
				smallend,
				"</span>",  NULL);

			gtk_list_store_set (stores[0], &iter,
				0, str,
				1, videofilters[i].tag,
				2, videofilters[i].conf,
				-1);
			g_free(str);
		}
    }
    g_signal_handler_unblock(stores[0], row_inserted_id);
    g_signal_handler_unblock(stores[0], row_deleted_id);
}

void
on_action_double_click (GtkButton * button, gpointer user_data)
{
    on_action(A_ADD);
}

void
on_action_change_category (GtkWidget * tree, gpointer user_data)
{
	GtkAdjustment *adj = gtk_scrolled_window_get_vadjustment (GTK_SCROLLED_WINDOW(WID(scrolledwindowAvailable)));
	gtk_adjustment_set_value (adj, 0);

	uint32_t number = 0;
	if (getSelectionNumber (VF_MAX+1, tree, storeCategories, &number))
		gtk_tree_view_set_model (GTK_TREE_VIEW(treeAvailable), GTK_TREE_MODEL(stores[number+1]));
}

void
on_action_double_click_1 (GtkButton * button, gpointer user_data)
{
    on_action(A_DOUBLECLICK);
}

void
on_tree_size_allocate(GtkWidget *widget, GtkAllocation *allocation, GtkCellRenderer *cell)
{
	g_object_set (cell, "wrap-width", allocation->width-8, NULL);
}

void
on_treeActive_row_deleted(GtkTreeModel *treemodel, GtkTreePath *arg1, gpointer user_data)
{
    GtkTreeIter iter;
    VF_FILTERS				tag;
	CONFcouple				*conf = 0;

        gtk_tree_model_get_iter_first(GTK_TREE_MODEL(stores[0]), &iter);
		AVDMGenericVideoStream 	*prevfilter = videofilters[0].filter;
		for (uint32_t i = 1; i < nb_active_filter; i++)
	    {
            gtk_tree_model_get (GTK_TREE_MODEL(stores[0]), &iter,
                            1, &tag,
                            2, &conf,
                            -1);
			delete videofilters[i].filter;
			videofilters[i].filter = filterCreateFromTag(tag,
                                                        conf,
                                                        prevfilter);
			videofilters[i].conf = conf;
			videofilters[i].tag = tag;
			prevfilter = videofilters[i].filter;
            gtk_tree_model_iter_next (GTK_TREE_MODEL(stores[0]), &iter);
	    }
    updateFilterList();
    //on_action(A_REORDERED);
}
void
on_treeActive_row_inserted(GtkTreeModel *treemodel, GtkTreePath *arg1, GtkTreeIter *arg2, gpointer user_data)
{


}

static void button_clicked(GtkWidget * wid, gpointer user_data)
{
	gui_act action;
#ifdef ADM_CPU_64BIT
#define TPE long long int
	long long int dummy;
#else
	int dummy;
#define TPE int
#endif
	dummy=(TPE)user_data;
	action=(gui_act) dummy;

	on_action(action);
}
