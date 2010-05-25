/***************************************************************************
 avsfilter.cpp  -  description
 -------------------
 begin                : 28-04-2008
 copyright            : (C) 2008 by fahr
 email                : fahr at inbox dot ru
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//#include "config.h"
//#include "prefs.h"

#ifdef VERSION_2_5
#include "ADM_default.h"
#include "ADM_videoFilterDynamic.h"
#include "ADM_videoFilter.h"
#include "ADM_videoFilter_internal.h"
#include "DIA_factory.h"
#include "errno.h"
//#include "ADM_vidComputeAverage.h"
#else
#include "fourcc.h"
#include "avio.hxx"
#include "errno.h"
#include "avi_vars.h"
#include "ADM_toolkit/toolkit.hxx"
#include "ADM_editor/ADM_edit.hxx"
#include "ADM_video/ADM_genvideo.hxx"
#include "ADM_userInterfaces/ADM_commonUI/DIA_factory.h"
#include "ADM_filter/video_filters.h"
#include "ADM_video/ADM_cache.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>

#include "strnew.h"
#include "avspipecomm.h"
#include "avsfilter.h"
#include "cdebug.h"
#include "ADM_plugin_translate.h"

static FILTER_PARAM avsParam={3,{"avs_script", "avs_loader","pipe_timeout"}};
static WINE_LOADER *first_loader = NULL;
static AVSTerminate term;

#define MAXPATHLEN 512

/**
 * several functions for realize link of objects
 */
WINE_LOADER *find_object(int order,
                         char *avs_loader, char *avs_script,
                         time_t script_ctime, time_t script_mtime,
                         ADV_Info *input_info,
                         bool *full)
{
  WINE_LOADER *res = first_loader;

  while (res != NULL)
  {
    // check order first
    if (res->order == order)
    {
      // after we check other params
      if (!strcmp((char*)res->_param.avs_loader, avs_loader) &&
          (!avs_script ||
           !strcmp((char*)res->_param.avs_script, avs_script)) &&
          res->input_info.width == input_info->width &&
          res->input_info.height == input_info->height &&
          res->_param.script_ctime == script_ctime &&
          res->_param.script_mtime == script_mtime &&
          res->input_info.nb_frames == input_info->nb_frames &&
          res->input_info.orgFrame == input_info->orgFrame)
      {
        printf("find_object : find %s %s\n",
               (char*)res->_param.avs_loader,
               (char*)res->_param.avs_script);
        if (full) *full = true;
      }
      else
      {
        printf("find_object fail: %s %s %dx%d [%d - %d] ftime %lX:%lX != %s %s %dx%d [%d - %d] ftime %lX:%lX\n",
               (char*)res->_param.avs_loader,
               (char*)res->_param.avs_script,
               res->input_info.width,
               res->input_info.height,
               res->input_info.orgFrame, res->input_info.orgFrame + res->input_info.nb_frames,
               res->_param.script_ctime, res->_param.script_mtime,
               avs_loader, avs_script, input_info->width, input_info->height,
               input_info->orgFrame, input_info->orgFrame + input_info->nb_frames,
               script_ctime, script_mtime);
        if (full) *full = false;
      }
      break;
    }
    res = (WINE_LOADER *)res->next_wine_loader;
  }
  return res;
}

void print_objects(void)
{
  WINE_LOADER *res = first_loader;

  while (res != NULL)
  {
    printf("print_objects : %s %s %dx%d [%d - %d]\n",
           (char*)res->_param.avs_loader,
           (char*)res->_param.avs_script,
           res->input_info.width,
           res->input_info.height,
           res->input_info.orgFrame, res->input_info.orgFrame + res->input_info.nb_frames);
    res = (WINE_LOADER *)res->next_wine_loader;
  }
  return;
}

void delete_object(WINE_LOADER *obj)
{
  WINE_LOADER *res = first_loader;

  if (res == obj)
  {
    first_loader = (WINE_LOADER *) obj->next_wine_loader;
    return;
  }

  while (res != NULL)
  {
    if (res->next_wine_loader == obj)
    {
      res->next_wine_loader = obj->next_wine_loader;
      break;
    }
    res = (WINE_LOADER *)res->next_wine_loader;
  }
}

void add_object(WINE_LOADER *obj)
{
  WINE_LOADER *res = first_loader;
  DEBUG_PRINTF("avsfilter : add_object start, res = %X\n", res);
  if (!res)
  {
    first_loader = obj;
    obj->next_wine_loader = NULL;
    return;
  }

  while (res != NULL)
  {
    if (res->next_wine_loader == NULL)
    {
      res->next_wine_loader = obj;
      obj->next_wine_loader = NULL;
      break;
    }
    res = (WINE_LOADER *) res->next_wine_loader;
  }
  DEBUG_PRINTF("avsfilter : add_object end\n");
}

//********************************************

void deinit_pipe(AVS_PIPES *avsp)
{
  if (avsp->hpipe != -1)
  {
    close(avsp->hpipe);
    avsp->hpipe = -1;
  }

  // if both link to pipename and pipename string exist
  DEBUG_PRINTF("avsfilter : deinit_pipe %X\n", avsp->pipename);
  DEBUG_PRINTF("avsfilter : deinit_pipe %s\n", avsp->pipename);
  remove(avsp->pipename);
  if (avsp->pipename && *avsp->pipename)
  {
    void *ptr = avsp->pipename;
    avsp->pipename = NULL;
    ADM_dealloc (ptr);
  }
}

void deinit_pipes(AVS_PIPES *avsp, int num)
{
  int i;
  for (i = 0; i < num; i++) deinit_pipe(&avsp[i]);
}

#define MAX_PATH 1024

bool init_pipes (AVS_PIPES *avsp, int num, FILE *pfile)
{
  int i;
  for (i = 0; i < num; i++)
  {
    char sname[MAX_PATH];

    if (fscanf(pfile, "%s\n", sname) != 1) DEBUG_PRINTF("fscanf error\n");
    else if (!(avsp[i].pipename = strnew(sname))) DEBUG_PRINTF("strnew error\n");
    else if (remove(avsp[i].pipename)) DEBUG_PRINTF("error remove file\n");
    else if (mkfifo(avsp[i].pipename, 0600))
     DEBUG_PRINTF("mkfifo error create fifo file %s, errno %d\n",
	         avsp[i].pipename, errno);
    else continue;

    deinit_pipes(avsp, i);
    return false;
  }

  return true;
}

bool open_pipes(AVS_PIPES *avsp, int num)
{
  int i;
  for (i = 0; i < num; i++)
  {
    DEBUG_PRINTF("avsfilter : try to open %s fifo\n", avsp[i].pipename);
    if ((avsp[i].hpipe = open(avsp[i].pipename, avsp[i].flags)) == -1)
    {
      DEBUG_PRINTF("avsfilter : failed open errno %d\n", errno);
      deinit_pipe(&avsp[i]);
      deinit_pipes(avsp, i);
      return false;
    }
  }

  DEBUG_PRINTF("all pipes open ok\n");

  return true;
}

bool pipe_test_filter(int hr, int hw)
{

  uint32_t test_send = (uint32_t) time(NULL);
  uint32_t test_r1 = 0;

  int sz1;

  DEBUG_PRINTF("avsfilter : pipe_test_filter prewrite\n");

  sz1 = write(hw, &test_send, sizeof(uint32_t));

  if (sz1 != sizeof(uint32_t)) return false;

  DEBUG_PRINTF("avsfilter : pipe_test_filter preread\n");

  sz1 = read(hr, &test_r1, sizeof(uint32_t));

  if (sz1 != sizeof(uint32_t) ||
      test_r1 != test_send) return false;

  return true;
}

bool open_pipes_ok, wine_loader_down = false;

AVSTerminate::~AVSTerminate()
{
  WINE_LOADER *cur_loader = first_loader;
  int i = 0;
  printf("Call terminate!!!\n");

  if (cur_loader)
    do {
      printf("Count %d\n", i++);

      if (cur_loader->avs_pipes[PIPE_LOADER_WRITE].hpipe != -1)
      {
        send_cmd(cur_loader->avs_pipes[PIPE_LOADER_WRITE].hpipe,
                 UNLOAD_AVS_SCRIPT, NULL, 0);
        printf("UNLOAD_AVS_SCRIPT try\n");
      }

      if (cur_loader->avs_pipes[PIPE_LOADER_WRITE].hpipe != -1)
      {
        send_cmd(cur_loader->avs_pipes[PIPE_LOADER_WRITE].hpipe,
                 UNLOAD_AVS_LOADER, NULL, 0);
        printf("UNLOAD_AVS_LOADER try\n");
      }

      deinit_pipes(cur_loader->avs_pipes, CMD_PIPE_NUM);
    } while((cur_loader = (WINE_LOADER*)cur_loader->next_wine_loader) != NULL);
}

void *parse_wine_stdout(void *arg)
{
  char sname[MAX_PATH];
  TPARSER *tp = (TPARSER *)arg;
  FILE *pfile = tp->pfile;
  AVS_PIPES copy_pipes [CMD_PIPE_NUM];
  int i;
  for (i = 0; i < CMD_PIPE_NUM; i++)
  {
    memcpy (&copy_pipes[i], &tp->avs_pipes[i], sizeof(AVS_PIPES));
    if ((copy_pipes[i].flags & O_ACCMODE) == O_RDONLY)
      copy_pipes[i].flags = (copy_pipes[i].flags & ~O_ACCMODE) | O_WRONLY;
    else
      if ((copy_pipes[i].flags & O_ACCMODE) == O_WRONLY)
        copy_pipes[i].flags = (copy_pipes[i].flags & ~O_ACCMODE) | O_RDONLY;

    DEBUG_PRINTF("avsfilter : new.flags %X, old.flags %X\n",
           copy_pipes[i].flags, tp->avs_pipes[i].flags);
  }

  wine_loader_down = false;

  if (pfile)
  {
    time_t t = time(NULL);
    DEBUG_PRINTF("avsfilter : pthread time %s\n",
           ctime(&t));
    DEBUG_PRINTF("pthread start ok\n");
    while(fgets(sname, MAX_PATH, pfile) != NULL)
#ifdef DEBUGMSG
      printf("%s", sname);
#else
    ;
#endif
    DEBUG_PRINTF("End parse\n");
    pclose(pfile);

    wine_loader_down = true;

    // if pipes not open completely, then simple open from thread for close
    if (!open_pipes_ok)
    {
      DEBUG_PRINTF("avsfilter : loader down, try to close waiting (for open) main thread\n");
      if (open_pipes((AVS_PIPES*)&copy_pipes, CMD_PIPE_NUM))
      {
        DEBUG_PRINTF("avsfilter : open ok, try to deinit\n");
        //deinit_pipes((AVS_PIPES*)&copy_pipes, CMD_PIPE_NUM);
        DEBUG_PRINTF("avsfilter : deinit done\n");
      }
    }

  }
}

bool wine_start(char *avsloader, AVS_PIPES *avs_pipes, int pipe_timeout)
{
  char sname[MAX_PATH];
  struct stat st;
  sprintf(sname, "wine %s %d", avsloader, pipe_timeout);

  FILE *pfile = popen(sname, "r");
  if (!pfile)
  {
    DEBUG_PRINTF("avsfilter : popen failed, errno %d\n", errno);
    return false;
  }

  if (fscanf(pfile, "%s\n", sname) != 1 ||
      stat(sname, &st) ||
      !S_ISDIR(st.st_mode))
  {
    DEBUG_PRINTF("avsfilter : tmpdirname failed, errno %d[stat %d isdir %d]\n", errno, stat(sname, &st), S_ISDIR(st.st_mode));
    pclose(pfile);
    return false;
  }

  DEBUG_PRINTF("avsfilter : good tmpdirname %s\n", sname);

  if (!init_pipes(avs_pipes, CMD_PIPE_NUM, pfile))
  {
    DEBUG_PRINTF("init_pipes failed\n");
    pclose(pfile);
    return false;
  }

  time_t t = time(NULL);
  DEBUG_PRINTF("avsfilter : precreate thread time %s\n",
         ctime(&t));
  pthread_t thread;
  TPARSER tp = { avs_pipes, pfile };

  open_pipes_ok = false;

  if (pthread_create(&thread, NULL, parse_wine_stdout, &tp))
  {
    DEBUG_PRINTF("Cannot pthread started...Errno %d\n",errno);
    deinit_pipes(avs_pipes, CMD_PIPE_NUM);
    return false;
  }

  t = time(NULL);
  DEBUG_PRINTF("avsfilter : preopen time %s\n",
         ctime(&t));

  if (!open_pipes(avs_pipes, CMD_PIPE_NUM) || wine_loader_down)
  {
    open_pipes_ok = true;
    DEBUG_PRINTF("open_pipes failed\n");
    deinit_pipes(avs_pipes, CMD_PIPE_NUM);
    return false;
  }

  open_pipes_ok = true;

  if (pipe_test_filter (avs_pipes[PIPE_LOADER_READ].hpipe,
                        avs_pipes[PIPE_FILTER_WRITE].hpipe))
  {
    DEBUG_PRINTF("avsfilter : test pipe to filter ok\n");

    if (pipe_test_filter (avs_pipes[PIPE_LOADER_READ].hpipe,
                          avs_pipes[PIPE_LOADER_WRITE].hpipe))
    {
      DEBUG_PRINTF("avsfilter : test pipe to loader ok\n");
    }
    else
      goto error_pipe_test;
  }
  else
  {
    error_pipe_test:
    DEBUG_PRINTF("Error test read/write pipes\n");
    deinit_pipes(avs_pipes, CMD_PIPE_NUM);
    return false;
  }

  DEBUG_PRINTF("wine start is ok\n");
  return true;
}

bool avs_start(ADV_Info *info, ADV_Info *avisynth_info,
               char *fname, AVS_PIPES *avs_pipes)
{
  if (!send_cmd(avs_pipes[PIPE_LOADER_WRITE].hpipe,
                LOAD_AVS_SCRIPT, fname,
                strlen(fname) + sizeof("\0")) ||
      !send_cmd(avs_pipes[PIPE_FILTER_WRITE].hpipe,
                SET_CLIP_PARAMETER, info,
                sizeof(ADV_Info)))
  {
    DEBUG_PRINTF("avsfilter : cannot set script name or set clip parameters\n");
    deinit_pipes(avs_pipes, CMD_PIPE_NUM);
    return false;
  }

  // get avisynth frame info
  PIPE_MSG_HEADER msg;
  if (!receive_cmd(avs_pipes[PIPE_LOADER_READ].hpipe,
                   &msg) ||
      msg.avs_cmd != SET_CLIP_PARAMETER ||
      !receive_data(avs_pipes[PIPE_LOADER_READ].hpipe,
                    &msg, avisynth_info))
  {
    DEBUG_PRINTF("avsfilter : cannot receive avisynth clip parameters\n");
    deinit_pipes(avs_pipes, CMD_PIPE_NUM);
    return false;
  }

  // correct avisynth_info for span of frames, calculate fps change metrics
  float k_fps;
  k_fps = float(avisynth_info->nb_frames + avisynth_info->orgFrame) / float(info->nb_frames + info->orgFrame);
  DEBUG_PRINTF("avsfilter : FPS change metrics %f\n", k_fps);
  avisynth_info->nb_frames = int (info->nb_frames * k_fps);
  avisynth_info->orgFrame = int (info->orgFrame * k_fps);
  DEBUG_PRINTF("avsfilter : Calculate new span for avisynth script [%d - %d]\n",
               avisynth_info->orgFrame,avisynth_info->orgFrame + avisynth_info->nb_frames);

  return true;
}
#ifdef VERSION_2_5

VF_DEFINE_FILTER(ADMVideoAVSfilter, avsParam,
                 avsfilter,
                 QT_TR_NOOP("Avisynth script filter (avsfilter), ver 0.7a(internal)"),
                 1,
                 VF_MISC,
                 QT_TR_NOOP("Use avisynth script as video filter."));

#else
extern "C"
{

  SCRIPT_CREATE(FILTER_create_fromscript,ADMVideoAVSfilter,avsParam);
  BUILD_CREATE(FILTER_create,ADMVideoAVSfilter);

  char *FILTER_getName(void)
  {
    return "AvsFilter, ver 0.7a";
  }

  char *FILTER_getDesc(void)
  {
    return "This filter do intermediate processing via avisynth script";
  }

  uint32_t FILTER_getVersion(void)
  {
    return 1;
  }
  uint32_t FILTER_getAPIVersion(void)
  {
    return ADM_FILTER_API_VERSION;
  }
}
#endif

char *ADMVideoAVSfilter::printConf( void )
{
  static char buf[MAXPATHLEN];

  sprintf((char *)buf, "loader : %s\n script : %s\npipe timeout %d",
          _param->avs_loader ,_param->avs_script,
          _param->pipe_timeout);
  return buf;
}

uint8_t ADMVideoAVSfilter::configure(AVDMGenericVideoStream *in)
{
  DEBUG_PRINTF("avsfilter : before dialog init\n");
  print_objects();

#define PX(x) &(_param->x)
  diaElemFile loaderfile(0,(char**)PX(avs_loader),
                         QT_TR_NOOP("_loader file:"), NULL,
                         QT_TR_NOOP("Select loader filename[avsload.exe]"));
  diaElemFile avsfile(0,(char**)PX(avs_script),
                      QT_TR_NOOP("_avs file:"), NULL,
                      QT_TR_NOOP("Select avs filename[*.avs]"));
  diaElemUInteger pipe_timeout(PX(pipe_timeout),QT_TR_NOOP("_pipe timeout:"),1,30);

  diaElem *elems[3]={&loaderfile, &avsfile, &pipe_timeout};

  if( diaFactoryRun(QT_TR_NOOP("AvsFilter config"), 3, elems))
  {
    bool res = false;

    DEBUG_PRINTF("avsfilter : configure before SetParameters\n");

    // if script/loader names are exist, then taste config
    if (_param->avs_loader && strlen((const char*)_param->avs_loader) &&
        _param->avs_script && strlen((const char*)_param->avs_script))
    {
      struct stat st;
      if (stat((char*)_param->avs_script, &st) != 0)
      {
        DEBUG_PRINTF("avsfilter : cannot stat script file\n");
        return 0;
      }

      _param->script_mtime = st.st_mtime; // store timestamp
      _param->script_ctime = st.st_ctime;

      print_objects();
      res = SetParameters(_param);
#ifdef PREFSINCHANGE
      DEBUG_PRINTF("avsfilter : configure before save prefs [%s][%s]\n",
                   _param->avs_script, _param->avs_loader);
      // if setparameters are ok and (therefore) avs_script and avs_loader exist
      // we store this parameters in filter preferences
      if (res && _param->avs_script && _param->avs_loader)
      {
        prefs->set(FILTERS_AVSFILTER_AVS_SCRIPT, (ADM_filename*)_param->avs_script);
        prefs->set(FILTERS_AVSFILTER_AVS_LOADER, (ADM_filename*)_param->avs_loader);
        prefs->set(FILTERS_AVSFILTER_PIPE_TIMEOUT, _param->pipe_timeout);
      }
      DEBUG_PRINTF("avsfilter : configure exit ok\n");
#endif
      return res;
    }
  }
  return 0;
}

//#define SET_AVS(i,x,y,z) {wine_loader->avs_pipes[i].pipename = x; wine_loader->avs_pipes[i].hpipe = y; wine_loader->avs_pipes[i].flags = z;}

bool ADMVideoAVSfilter::SetParameters(AVS_PARAM *newparam)
{
  bool full_exact = false;
  DEBUG_PRINTF("avsfilter : SetParameters\n");

  // find corresponding loader/script
  WINE_LOADER *loader = find_object(order,
                                    (char*)newparam->avs_loader,
                                    (char*)newparam->avs_script,
                                    newparam->script_ctime, newparam->script_mtime,
                                    &_info,
                                    &full_exact);
  // if loader not found
  if (!loader)
  {
    DEBUG_PRINTF("avsfilter : SetParameters no loader found\n");
    loader = new (WINE_LOADER);
    loader->avs_pipes[0].flags = O_RDONLY;
    loader->avs_pipes[1].flags = O_WRONLY;
    loader->avs_pipes[2].flags = O_WRONLY;
    loader->RefCounter = 0;
    loader->_param.avs_script = NULL;
    loader->_param.avs_loader = NULL;

    if (!wine_start((char*)newparam->avs_loader, loader->avs_pipes, newparam->pipe_timeout))
    {
      DEBUG_PRINTF("avsfilter : wine_start unsuccessful start!\n");
      delete loader;
      deref_wine_loader:
      if (wine_loader)
      {
        wine_loader->RefCounter--;
        wine_loader = NULL;
      }
      return false;
    }

    DEBUG_PRINTF("avsfilter : SetParameters success start wine\n");
    loader->order = order;
    add_object(loader);
  }

  // all parameters are NOT matched [order only]
  if (!full_exact)
  {
    DEBUG_PRINTF("avsfilter : SetParameters !full_exact\n");

    // matched only order (need reload with new script/geometry/etc)
    if (!avs_start(&_info, &loader->output_info, (char*)newparam->avs_script, loader->avs_pipes))
    {
      DEBUG_PRINTF("avsfilter : SetParameters fail avs_start\n");
      delete_object(loader);
      goto deref_wine_loader;
    }

    DEBUG_PRINTF("avsfilter : SetParameters avs_start ok\n");
    loader->RefCounter = 0;
    memcpy(&loader->input_info, &_info, sizeof(_info));
    loader->_param.avs_loader = (ADM_filename*)ADM_strdup ((char*)newparam->avs_loader);
    loader->_param.avs_script = (ADM_filename*)ADM_strdup ((char*)newparam->avs_script);
    loader->_param.script_ctime = newparam->script_ctime; // store timestamp
    loader->_param.script_mtime = newparam->script_mtime;
  }

  if (wine_loader && wine_loader != loader) wine_loader->RefCounter--;
  wine_loader = loader;
  wine_loader->RefCounter++;
  out_frame_sz = ((loader->output_info.width * loader->output_info.height) * 3) >>1;
  // 22.11 fix size of output filter with fullexact found loader
  _info.width = loader->output_info.width;
  _info.height = loader->output_info.height;
  _info.fps1000 = loader->output_info.fps1000;
  _info.nb_frames = loader->output_info.nb_frames;
  _info.orgFrame = loader->output_info.orgFrame;
  DEBUG_PRINTF("avsfilter : clip info : geom %d:%d fps1000 %d num_frames %d\n",
               _info.width, _info.height, _info.fps1000, _info.nb_frames);

  DEBUG_PRINTF("avsfilter : SetParameters return Ok\n");
  return true;

#if 0
  // if not change parameters
/*  if (oldparam &&
      !strcmp((char*)oldparam->avs_loader, (char*)newparam->avs_loader) &&
      !strcmp((char*)oldparam->avs_script, (char*)newparam->avs_script))
    return true;*/

  avs_load = false;
  wine_load = false;

  // find corresponding loader/script
  WINE_LOADER *loader = find_object((char*)newparam->avs_loader,
                                    (char*)newparam->avs_script,
                                    (time_t*)&newparam->script_ftime,
                                    &wine_loader->input_info);
  // if find itself
  if (loader == wine_loader)
  {
    avs_load = true;
    wine_load = true;
    return true;
  }

  // replace old wine_loader with founded
  if (loader)
  {
    DEBUG_PRINTF("avsfilter : SetParameters find loader!!!\n");

    // copy pipe handles
    SET_AVS(0,&pipe_loader_read,loader->avs_pipes[0].hpipe,O_RDONLY);
    SET_AVS(1,&pipe_loader_write,loader->avs_pipes[1].hpipe,O_WRONLY);
    SET_AVS(2,&pipe_filter_write,loader->avs_pipes[2].hpipe,O_WRONLY);

    // set pipe names to NULL
    pipe_loader_read = NULL;
    pipe_loader_write = NULL;
    pipe_filter_write = NULL;

    // cleanup old names etc.
    int i;
    for (i = 0; i < CMD_PIPE_NUM; i++)
      *wine_loader->avs_pipes->pipename = NULL;

    memcpy(&wine_loader->temp_info, &loader->temp_info, sizeof(ADV_Info));
    avs_load = true;
    wine_load = true;
  }
  else
  {
    struct stat st;
    DEBUG_PRINTF("avsfilter : SetParameters try to start new loader and script\n");

    if (stat((char*)newparam->avs_script, &st) != 0)
    {
      DEBUG_PRINTF("avsfilter : cannot stat script file\n");
      return false;
    }

    // try start wine (if exist loader filename)
    wine_load = wine_start((char*)newparam->avs_loader, wine_loader->avs_pipes, newparam->pipe_timeout);
    if (wine_load)
      avs_load = avs_start(&wine_loader->input_info, &wine_loader->temp_info, (char*)newparam->avs_script, wine_loader->avs_pipes);
    if (wine_load && avs_load)
    {
      wine_loader->_param.avs_script = (ADM_filename*)ADM_strdup ((char*)newparam->avs_script);
      wine_loader->_param.avs_loader = (ADM_filename*)ADM_strdup ((char*)newparam->avs_loader);
      wine_loader->_param.script_ftime[0] = st.st_mtime; // store timestamp
      wine_loader->_param.script_ftime[1] = st.st_ctime;
      add_object(wine_loader);
      print_objects();
    }
  }

  // if all is ok, set corresponding structures and vars
  if (avs_load)
  {
    _info.width = wine_loader->temp_info.width;
    _info.height = wine_loader->temp_info.height;
    _info.fps1000 = wine_loader->temp_info.fps1000;
    _info.nb_frames = wine_loader->temp_info.nb_frames;
    _info.orgFrame = wine_loader->temp_info.orgFrame;
    DEBUG_PRINTF("avsfilter : clip info : geom %d:%d fps1000 %d num_frames %d\n",
           _info.width, _info.height, _info.fps1000, _info.nb_frames);
    out_frame_sz = ((_info.width * _info.height) * 3) >>1;
  }

  return wine_load && avs_load;
#else
  DEBUG_PRINTF("avsfilter : SetParameters return false!\n");
  return false;
#endif
}

ADMVideoAVSfilter::ADMVideoAVSfilter(AVDMGenericVideoStream *in,
                                     CONFcouple *couples)
{
  /*
   * DEBUG CODE : to determine ORDER of filter in filter chain
   */
  uint32_t fcount = 0;
  order = -1;
  FILTER *z = getCurrentVideoFilterList (&fcount);
  printf("fcount = %d\n", fcount);
  if (z && fcount)
  {
    for (int i = 0; i < fcount ; i++)
      // printf("Filter %X\n", z[i].filter);
      if (z[i].filter == in)
      {
        printf("avsfilter : this filter is %d in list\n", i + 1);
        order = i + 1;
        break;
      }
  }
  /*
   * END DEBUG CODE
   */

  ADM_assert(in);
//  ADM_assert(order == -1);

  _in=in;
  DEBUG_PRINTF("Create AVSfilter(%X), AVDMGenericVideoStream %X\n", this, in);

  wine_loader = NULL;
  _param = new (AVS_PARAM);
  memcpy(&_info,_in->getInfo(),sizeof(_info));
  _info.encoding=1;
  vidCache = NULL;

  // if parameters set
  if(couples)
  {
    GET (avs_script);
    GET (avs_loader);
    GET (pipe_timeout);
    GET (script_ctime);
    GET (script_mtime);
    DEBUG_PRINTF("avsfilter : avsloader %s avsscript %s\n",
                 _param->avs_loader, _param->avs_script);

    if (!SetParameters(_param))
    {
      DEBUG_PRINTF("avsfilter : SetParameters return false\n");
      return;
    }
  }
  else // default value (or value from config)
  {
    char *tmp_str;
    _param->avs_script = NULL;
    _param->avs_loader = NULL;
    _param->pipe_timeout = 10;
    _param->script_ctime = 0;
    _param->script_mtime = 0;

#ifdef PREFSINCHANGE
    if (prefs->get(FILTERS_AVSFILTER_AVS_SCRIPT, &tmp_str) == RC_OK &&
        strlen(tmp_str) > 0)
    {
      _param->avs_script = (ADM_filename*)ADM_strdup (tmp_str);
      DEBUG_PRINTF("avsfilter : avsscript from config is %s\n", _param->avs_script);
      ADM_dealloc(tmp_str);
    }

    if (prefs->get(FILTERS_AVSFILTER_AVS_LOADER, &tmp_str) == RC_OK &&
        strlen(tmp_str) > 0)
    {
      _param->avs_loader = (ADM_filename*)ADM_strdup (tmp_str);
      DEBUG_PRINTF("avsfilter : avsloader from config is %s\n", _param->avs_loader);
      ADM_dealloc(tmp_str);
    }
    prefs->get(FILTERS_AVSFILTER_PIPE_TIMEOUT, &_param->pipe_timeout);

    struct stat st;
    if (_param->avs_script)
      if (stat((char*)_param->avs_script, &st) != 0)
      {
        DEBUG_PRINTF("avsfilter : cannot stat script file\n");
        return;
      }
      else
      {
        _param->script_mtime = st.st_mtime; // store timestamp
        _param->script_ctime = st.st_ctime;
      }
#endif
  }
  _uncompressed=new ADMImage(_in->getInfo()->width,_in->getInfo()->height);
  ADM_assert(_uncompressed);
  in_frame_sz = ((_uncompressed->_width * _uncompressed->_height) * 3) >>1;

#if 0
  // if param is not empty
  if (_param->avs_loader && strlen((const char*)_param->avs_loader) &&
      _param->avs_script && strlen((const char*)_param->avs_script))
  {
    struct stat st;
    if (stat((char*)_param->avs_script, &st) != 0)
    {
      DEBUG_PRINTF("avsfilter : cannot stat script file\n");
      return;
    }

    _param->script_ftime[0] = st.st_mtime; // store timestamp
    _param->script_ftime[1] = st.st_ctime;

    if (!SetParameters(_param))
    {
      DEBUG_PRINTF("avsfilter : SetParameters return false\n");
      return;
    }
  }
#endif

  vidCache=new VideoCache(16,_in);
}

ADMVideoAVSfilter::~ADMVideoAVSfilter()
{
#if 0
  DEBUG_PRINTF("avsfilter : delete AVSFilter(%X)\n", this);

  /**
   * if loader work incorrectly (may be wrong parameter or other case),
   * then simple down both loader and avisynth library
   */
  if (!wine_load || !avs_load)
  {
    DEBUG_PRINTF("avsfilter : partially UNLOAD check-> %d %d\n",
                 avs_load, wine_load);
    if (avs_load)
      send_cmd(wine_loader->avs_pipes[PIPE_LOADER_WRITE].hpipe,
               UNLOAD_AVS_SCRIPT, NULL, 0);
    if (wine_load)
      send_cmd(wine_loader->avs_pipes[PIPE_LOADER_WRITE].hpipe,
               UNLOAD_AVS_LOADER, NULL, 0);

    delete_object(wine_loader);

    ADM_dealloc(_param->avs_loader);
    ADM_dealloc(_param->avs_script);
    ADM_dealloc(wine_loader);
  }
  else
  {
    // all is ok, delete internal link from object
    wine_loader->avs_pipes[0].pipename = NULL;
    wine_loader->avs_pipes[1].pipename = NULL;
    wine_loader->avs_pipes[2].pipename = NULL;
  }

#endif

  if (wine_loader)
  {
    wine_loader->RefCounter--;
    if (!wine_loader->RefCounter) wine_loader = NULL;
  }

  if (vidCache)
  {
    delete vidCache;
    vidCache = NULL;
  }
}

uint8_t ADMVideoAVSfilter::getCoupledConf( CONFcouple **couples)
{
  *couples=new CONFcouple(5);

//#define CSET(x)  (*couples)->setCouple((char *)#x,(_param.x))
  DEBUG_PRINTF("avsfilter : getCoupledConf\n");
  CSET(avs_script);
  CSET(avs_loader);
  CSET(pipe_timeout);
  CSET(script_mtime);
  CSET(script_ctime);
  DEBUG_PRINTF("avsfilter : getCoupledConf end\n");
  return 1;
}

uint8_t ADMVideoAVSfilter::getFrameNumberNoAlloc(uint32_t iframe,
                                                 uint32_t *len,
                                                 ADMImage *data,
                                                 uint32_t *flags)
{
  ADMImage *src;
  int frame = iframe + _info.orgFrame, tmpframe;

  DEBUG_PRINTF("avsfilter : receive getFrameNumberNoAlloc %d [nb_frames %d], wine_loader %X\n",
               frame, _info.nb_frames, wine_loader);

  // check framenumber
  if(iframe > _info.nb_frames || !wine_loader) return 0;

  FRAME_DATA fd = {frame};

  // send command to get filtered data
  if (!send_cmd(wine_loader->avs_pipes[PIPE_LOADER_WRITE].hpipe,
                GET_FRAME, (void*)&fd,
                sizeof(FRAME_DATA)))
  {
    DEBUG_PRINTF("avsfilter : error send GET_FRAME to avsloader\n");
    return 0;
  }

  // read all data from avsloader and pipe dll
  PIPE_MSG_HEADER msg;
  while (receive_cmd(wine_loader->avs_pipes[PIPE_LOADER_READ].hpipe,
                     &msg))
  {
    switch(msg.avs_cmd)
    {
      case GET_FRAME: // this request from pipe_source for input frame(s) to avisynth core
        DEBUG_PRINTF("avsfilter : receive GET_FRAME\n");
        if (!receive_data(wine_loader->avs_pipes[PIPE_LOADER_READ].hpipe,
                          &msg, &fd))
        {
          DEBUG_PRINTF("\navsfilter : error receive data\n");
          return 0;
        }

        DEBUG_PRINTF("avsfilter : GET_FRAME number %d\n", fd.frame);
        tmpframe = ((fd.frame >= wine_loader->input_info.orgFrame) ? (fd.frame - wine_loader->input_info.orgFrame) : 0);
        DEBUG_PRINTF("avsfilter : %d but really get %d\n", fd.frame, tmpframe);
        src=vidCache->getImage(tmpframe);

        DEBUG_PRINTF("avsfilter : in frame size %lu\n", in_frame_sz);
        // send frame to pipe_source
        if (!send_cmd_by_two_part(wine_loader->avs_pipes[PIPE_FILTER_WRITE].hpipe,
                                  PUT_FRAME,
                                  (void*)&fd, sizeof(FRAME_DATA),
                                  src->data, in_frame_sz))

        {
          DEBUG_PRINTF("avsfilter : error send uncompressed frame to dll\n");
          return 0;
        }

        DEBUG_PRINTF("avsfilter : send data ok for frame %d\n", fd.frame);
        break;

      case PUT_FRAME: // this request from avsload.exe with filtering data after avisynth
        DEBUG_PRINTF("avsfilter : receive PUT_FRAME, msg.sz %d\n", msg.sz);
        if (msg.sz != out_frame_sz + sizeof(FRAME_DATA))
        {
          DEBUG_PRINTF("avsfilter : PUT_FRAME msg.sz [%lu] != out_frame_sz+sizeof(FRAME_DATA) [%lu,%d]\n",
                 msg.sz, out_frame_sz, sizeof(FRAME_DATA));
          return 0;
        }

        DEBUG_PRINTF("avsfilter : read 1\n");
        if (!receive_data_by_size(wine_loader->avs_pipes[PIPE_LOADER_READ].hpipe,
                                  &fd, sizeof(FRAME_DATA)))
        {
          DEBUG_PRINTF("avsfilter : receive data error#1\n");
          return 0;
        }

        ADM_assert(fd.frame == (iframe + _info.orgFrame));

        DEBUG_PRINTF("avsfilter : read %d frame number\n", fd.frame);

        if (!receive_data_by_size(wine_loader->avs_pipes[PIPE_LOADER_READ].hpipe,
                                  data->data, msg.sz - sizeof(FRAME_DATA)))
        {
          DEBUG_PRINTF("avsfilter : receive data error#2\n");
          return 0;
        }

        *len = out_frame_sz;
        DEBUG_PRINTF("avsfilter : copy data\n");
        DEBUG_PRINTF("avsfilter : data parameters %d:%d\n",
               data->_width, data->_height);
        data->copyInfo(_uncompressed);
        vidCache->unlockAll();
        return 1;
        break;
    }
  }
  return 0;
}
