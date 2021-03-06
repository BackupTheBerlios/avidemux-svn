#ifndef AUDM_LAME_PARAM_H
#define AUDM_LAME_PARAM_H

typedef enum ADM_LAME_PRESET
{
  ADM_LAME_PRESET_CBR,
  ADM_LAME_PRESET_ABR,
  ADM_LAME_PRESET_EXTREME
};

typedef struct ADM_PRESET_DEFINITION
{
  ADM_LAME_PRESET preset;
  const char	*name;
}ADM_PRESET_DEFINITION;
static const ADM_PRESET_DEFINITION      presetDefinition[]=
{
  {ADM_LAME_PRESET_CBR,"CBR"},
  {ADM_LAME_PRESET_ABR,"ABR"},
  {ADM_LAME_PRESET_EXTREME,"Extreme"}
};    

typedef struct LAME_encoderParam
{
  ADM_LAME_PRESET preset;
  ADM_mode        mode;
  uint32_t        quality;
};

#endif
