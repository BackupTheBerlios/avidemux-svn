#define LIST_OF_OBJECTS     \
PROCESS(actionOpen,ACT_OpenAvi) \
PROCESS(actionAppend,ACT_AppendAvi) \
PROCESS(actionQuit,ACT_Exit) \
PROCESS(actionCut,ACT_Cut) \
PROCESS(actionCopy,ACT_Copy) \
PROCESS(actionPaste,ACT_Paste) \
PROCESS(actionDelete,ACT_Delete) \
PROCESS(actionSet_marker_A,ACT_MarkA) \
PROCESS(actionSet_marker_B,ACT_MarkB) \
PROCESS(actionPreferences,ACT_Pref) \
PROCESS(actionProperties,ACT_AviInfo) \
PROCESS(actionSave_video,ACT_SaveAvi) \
PROCESS(actionSave_BMP,ACT_SaveImg) \
PROCESS(actionSave_jpeg,ACT_SaveJPG) \
PROCESS(actionLoad_run_project,ACT_RunScript) \
PROCESS(actionSave_project,ACT_DUMMY) \
PROCESS(actionSave_project_as,ACT_SaveWork) \
PROCESS(actionMain_toolbar,ACT_SaveCurrentWork) \
PROCESS(actionA_V_toolbar,ACT_DUMMY) \
PROCESS(actionZoom_1_4,ACT_ZOOM_1_4) \
PROCESS(actionZoom_1_2,ACT_ZOOM_1_2) \
PROCESS(actionZoom_1_1,ACT_ZOOM_1_1) \
PROCESS(actionZoom_2_1,ACT_ZOOM_2_1) \
PROCESS(actionZoom_4_1,ACT_ZOOM_4_1) \
PROCESS(actionDecoder_options,ACT_DecoderOption) \
PROCESS(actionPostprocessing,ACT_SetPostProcessing) \
PROCESS(actionPreview,ACT_PreviewToggle) \
PROCESS(actionDisplay_output,ACT_OuputToggle) \
PROCESS(actionFrame_rate,ACT_ChangeFPS) \
PROCESS(actionEncoder,ACT_SelectEncoder) \
PROCESS(actionFilters,ACT_VideoParameter) \
PROCESS(actionMain_Track,ACT_SelectTrack1) \
PROCESS(actionSecondary_Track,ACT_SecondAudioTrack) \
PROCESS(actionBuild_VBR_time_map,ACT_DUMMY) \
PROCESS(actionSave_2,ACT_DUMMY) \
PROCESS(actionEncoder_2,ACT_DUMMY) \
PROCESS(actionFilters_2,ACT_DUMMY) \
PROCESS(actionCalculator,ACT_DUMMY) \
PROCESS(actionRebuild_I_B_Frames,ACT_DUMMY) \
PROCESS(actionBitrate_histogram,ACT_DUMMY) \
PROCESS(actionScan_for_black_frames,ACT_DUMMY) \
PROCESS(actionVob_to_vobsub,ACT_DUMMY) \
PROCESS(actionOCR,ACT_PlayAvi) 

#define LIST_OF_BUTTONS     \
PROCESS(toolButtonPlay , ACT_PlayAvi  ) \
PROCESS(toolButtonBPrev ,ACT_PrevBlackFrame ) \
PROCESS(toolButtonA ,    ACT_MarkA) \
PROCESS(toolButtonKNext ,ACT_NextKFrame ) \
PROCESS(toolButtonEnd ,  ACT_End ) \
PROCESS(toolButtonPrev,  ACT_PreviousFrame ) \
PROCESS(toolButtonNext , ACT_NextFrame ) \
PROCESS(toolButtonBegin ,ACT_Begin ) \
PROCESS(toolButtoBNext,  ACT_NextBlackFrame ) \
PROCESS(toolButtonB ,    ACT_MarkB) \
PROCESS(toolButtonKPrev ,ACT_PreviousKFrame ) \
PROCESS(toolButtonStop , ACT_StopAvi) 
