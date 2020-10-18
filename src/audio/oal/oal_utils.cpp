#include "common.h"
#include "oal_utils.h"

#ifdef AUDIO_OAL

LPALGENEFFECTS reAlGenEffects;
LPALDELETEEFFECTS reAlDeleteEffects;
LPALISEFFECT reAlIsEffect;
LPALEFFECTI reAlEffecti;
LPALEFFECTIV reAlEffectiv;
LPALEFFECTF reAlEffectf;
LPALEFFECTFV reAlEffectfv;
LPALGETEFFECTI reAlGetEffecti;
LPALGETEFFECTIV reAlGetEffectiv;
LPALGETEFFECTF reAlGetEffectf;
LPALGETEFFECTFV reAlGetEffectfv;
LPALGENAUXILIARYEFFECTSLOTS reAlGenAuxiliaryEffectSlots;
LPALDELETEAUXILIARYEFFECTSLOTS reAlDeleteAuxiliaryEffectSlots;
LPALISAUXILIARYEFFECTSLOT reAlIsAuxiliaryEffectSlot;
LPALAUXILIARYEFFECTSLOTI reAlAuxiliaryEffectSloti;
LPALAUXILIARYEFFECTSLOTIV reAlAuxiliaryEffectSlotiv;
LPALAUXILIARYEFFECTSLOTF reAlAuxiliaryEffectSlotf;
LPALAUXILIARYEFFECTSLOTFV reAlAuxiliaryEffectSlotfv;
LPALGETAUXILIARYEFFECTSLOTI reAlGetAuxiliaryEffectSloti;
LPALGETAUXILIARYEFFECTSLOTIV reAlGetAuxiliaryEffectSlotiv;
LPALGETAUXILIARYEFFECTSLOTF reAlGetAuxiliaryEffectSlotf;
LPALGETAUXILIARYEFFECTSLOTFV reAlGetAuxiliaryEffectSlotfv;
LPALGENFILTERS reAlGenFilters;
LPALDELETEFILTERS reAlDeleteFilters;
LPALISFILTER reAlIsFilter;
LPALFILTERI reAlFilteri;
LPALFILTERIV reAlFilteriv;
LPALFILTERF reAlFilterf;
LPALFILTERFV reAlFilterfv;
LPALGETFILTERI reAlGetFilteri;
LPALGETFILTERIV reAlGetFilteriv;
LPALGETFILTERF reAlGetFilterf;
LPALGETFILTERFV reAlGetFilterfv;


void EFXInit()
{
	/* Define a macro to help load the function pointers. */
	/* Prefix all the function pointers with re* to avoid linking issue with the library functions */
#define LOAD_PROC(T, x, y)  ((x) = (T)alGetProcAddress(#y))
	LOAD_PROC(LPALGENEFFECTS, reAlGenEffects, alGenEffects);
	LOAD_PROC(LPALDELETEEFFECTS, reAlDeleteEffects, alDeleteEffects);
	LOAD_PROC(LPALISEFFECT, reAlIsEffect, alIsEffect);
	LOAD_PROC(LPALEFFECTI, reAlEffecti, alEffecti);
	LOAD_PROC(LPALEFFECTIV, reAlEffectiv, alEffectiv);
	LOAD_PROC(LPALEFFECTF, reAlEffectf, alEffectf);
	LOAD_PROC(LPALEFFECTFV, reAlEffectfv, alEffectfv);
	LOAD_PROC(LPALGETEFFECTI, reAlGetEffecti, alGetEffecti);
	LOAD_PROC(LPALGETEFFECTIV, reAlGetEffectiv, alGetEffectiv);
	LOAD_PROC(LPALGETEFFECTF, reAlGetEffectf, alGetEffectf);
	LOAD_PROC(LPALGETEFFECTFV, reAlGetEffectfv, alGetEffectfv);
	
	LOAD_PROC(LPALGENFILTERS, reAlGenFilters, alGenFilters);
	LOAD_PROC(LPALDELETEFILTERS, reAlDeleteFilters, alDeleteFilters);
	LOAD_PROC(LPALISFILTER, reAlIsFilter, alIsFilter);
	LOAD_PROC(LPALFILTERI, reAlFilteri, alFilteri);
	LOAD_PROC(LPALFILTERIV, reAlFilteriv, alFilteriv);
	LOAD_PROC(LPALFILTERF, reAlFilterf, alFilterf);
	LOAD_PROC(LPALFILTERFV, reAlFilterfv, alFilterfv);
	LOAD_PROC(LPALGETFILTERI, reAlGetFilteri, alGetFilteri);
	LOAD_PROC(LPALGETFILTERIV, reAlGetFilteriv, alGetFilteriv);
	LOAD_PROC(LPALGETFILTERF, reAlGetFilterf, alGetFilterf);
	LOAD_PROC(LPALGETFILTERFV, reAlGetFilterfv, alGetFilterfv);
	
	LOAD_PROC(LPALGENAUXILIARYEFFECTSLOTS, reAlGenAuxiliaryEffectSlots, alGenAuxiliaryEffectSlots);
	LOAD_PROC(LPALDELETEAUXILIARYEFFECTSLOTS, reAlDeleteAuxiliaryEffectSlots, alDeleteAuxiliaryEffectSlots);
	LOAD_PROC(LPALISAUXILIARYEFFECTSLOT, reAlIsAuxiliaryEffectSlot, alIsAuxiliaryEffectSlot);
	LOAD_PROC(LPALAUXILIARYEFFECTSLOTI, reAlAuxiliaryEffectSloti, alAuxiliaryEffectSloti);
	LOAD_PROC(LPALAUXILIARYEFFECTSLOTIV, reAlAuxiliaryEffectSlotiv, alAuxiliaryEffectSlotiv);
	LOAD_PROC(LPALAUXILIARYEFFECTSLOTF, reAlAuxiliaryEffectSlotf, alAuxiliaryEffectSlotf);
	LOAD_PROC(LPALAUXILIARYEFFECTSLOTFV, reAlAuxiliaryEffectSlotfv, alAuxiliaryEffectSlotfv);
	LOAD_PROC(LPALGETAUXILIARYEFFECTSLOTI, reAlGetAuxiliaryEffectSloti, alGetAuxiliaryEffectSloti);
	LOAD_PROC(LPALGETAUXILIARYEFFECTSLOTIV, reAlGetAuxiliaryEffectSlotiv, alGetAuxiliaryEffectSlotiv);
	LOAD_PROC(LPALGETAUXILIARYEFFECTSLOTF, reAlGetAuxiliaryEffectSlotf, alGetAuxiliaryEffectSlotf);
	LOAD_PROC(LPALGETAUXILIARYEFFECTSLOTFV, reAlGetAuxiliaryEffectSlotfv, alGetAuxiliaryEffectSlotfv);
#undef LOAD_PROC
}

void SetEffectsLevel(ALuint uiFilter, float level)
{
	reAlFilteri(uiFilter, AL_FILTER_TYPE, AL_FILTER_LOWPASS);
	reAlFilterf(uiFilter, AL_LOWPASS_GAIN, 1.0f);
	reAlFilterf(uiFilter, AL_LOWPASS_GAINHF, level);
}

static inline float gain_to_mB(float gain)
{
    return (gain > 1e-5f) ? (float)(log10f(gain) * 2000.0f) : -10000l;
}

static inline float mB_to_gain(float millibels)
{
    return (millibels > -10000.0f) ? powf(10.0f, millibels/2000.0f) : 0.0f;
}

static inline float clampF(float val, float minval, float maxval)
{
    if(val >= maxval) return maxval;
    if(val <= minval) return minval;
    return val;
}

void EAX3_Set(ALuint effect, const EAXLISTENERPROPERTIES *props)
{
	reAlEffecti (effect, AL_EFFECT_TYPE,                     AL_EFFECT_EAXREVERB);
	reAlEffectf (effect, AL_EAXREVERB_DENSITY,               clampF(powf(props->flEnvironmentSize, 3.0f) / 16.0f, 0.0f, 1.0f));
	reAlEffectf (effect, AL_EAXREVERB_DIFFUSION,             props->flEnvironmentDiffusion);
	reAlEffectf (effect, AL_EAXREVERB_GAIN,                  mB_to_gain((float)props->lRoom));
	reAlEffectf (effect, AL_EAXREVERB_GAINHF,                mB_to_gain((float)props->lRoomHF));
	reAlEffectf (effect, AL_EAXREVERB_GAINLF,                mB_to_gain((float)props->lRoomLF));
	reAlEffectf (effect, AL_EAXREVERB_DECAY_TIME,            props->flDecayTime);
	reAlEffectf (effect, AL_EAXREVERB_DECAY_HFRATIO,         props->flDecayHFRatio);
	reAlEffectf (effect, AL_EAXREVERB_DECAY_LFRATIO,         props->flDecayLFRatio);
	reAlEffectf (effect, AL_EAXREVERB_REFLECTIONS_GAIN,      clampF(mB_to_gain((float)props->lReflections), AL_EAXREVERB_MIN_REFLECTIONS_GAIN, AL_EAXREVERB_MAX_REFLECTIONS_GAIN));
	reAlEffectf (effect, AL_EAXREVERB_REFLECTIONS_DELAY,     props->flReflectionsDelay);
	reAlEffectfv(effect, AL_EAXREVERB_REFLECTIONS_PAN,       &props->vReflectionsPan.x);
	reAlEffectf (effect, AL_EAXREVERB_LATE_REVERB_GAIN,      clampF(mB_to_gain((float)props->lReverb), AL_EAXREVERB_MIN_LATE_REVERB_GAIN, AL_EAXREVERB_MAX_LATE_REVERB_GAIN));
	reAlEffectf (effect, AL_EAXREVERB_LATE_REVERB_DELAY,     props->flReverbDelay);
	reAlEffectfv(effect, AL_EAXREVERB_LATE_REVERB_PAN,       &props->vReverbPan.x);
	reAlEffectf (effect, AL_EAXREVERB_ECHO_TIME,             props->flEchoTime);
	reAlEffectf (effect, AL_EAXREVERB_ECHO_DEPTH,            props->flEchoDepth);
	reAlEffectf (effect, AL_EAXREVERB_MODULATION_TIME,       props->flModulationTime);
	reAlEffectf (effect, AL_EAXREVERB_MODULATION_DEPTH,      props->flModulationDepth);
	reAlEffectf (effect, AL_EAXREVERB_AIR_ABSORPTION_GAINHF, clampF(mB_to_gain(props->flAirAbsorptionHF), AL_EAXREVERB_MIN_AIR_ABSORPTION_GAINHF, AL_EAXREVERB_MAX_AIR_ABSORPTION_GAINHF));
	reAlEffectf (effect, AL_EAXREVERB_HFREFERENCE,           props->flHFReference);
	reAlEffectf (effect, AL_EAXREVERB_LFREFERENCE,           props->flLFReference);
	reAlEffectf (effect, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR,   props->flRoomRolloffFactor);
	reAlEffecti (effect, AL_EAXREVERB_DECAY_HFLIMIT,         (props->ulFlags&EAXLISTENERFLAGS_DECAYHFLIMIT) ? AL_TRUE : AL_FALSE);
}

void EFX_Set(ALuint effect, const EAXLISTENERPROPERTIES *props)
{
	reAlEffecti(effect, AL_EFFECT_TYPE, AL_EFFECT_REVERB);
	
	reAlEffectf(effect, AL_REVERB_DENSITY,               clampF(powf(props->flEnvironmentSize, 3.0f) / 16.0f, 0.0f, 1.0f));
	reAlEffectf(effect, AL_REVERB_DIFFUSION,             props->flEnvironmentDiffusion);
	reAlEffectf(effect, AL_REVERB_GAIN,                  mB_to_gain((float)props->lRoom));
	reAlEffectf(effect, AL_REVERB_GAINHF,                mB_to_gain((float)props->lRoomHF));
	reAlEffectf(effect, AL_REVERB_DECAY_TIME,            props->flDecayTime);
	reAlEffectf(effect, AL_REVERB_DECAY_HFRATIO,         props->flDecayHFRatio);
	reAlEffectf(effect, AL_REVERB_REFLECTIONS_GAIN,      clampF(mB_to_gain((float)props->lReflections), AL_EAXREVERB_MIN_REFLECTIONS_GAIN, AL_EAXREVERB_MAX_REFLECTIONS_GAIN));
	reAlEffectf(effect, AL_REVERB_REFLECTIONS_DELAY,     props->flReflectionsDelay);
	reAlEffectf(effect, AL_REVERB_LATE_REVERB_GAIN,      clampF(mB_to_gain((float)props->lReverb), AL_EAXREVERB_MIN_LATE_REVERB_GAIN, AL_EAXREVERB_MAX_LATE_REVERB_GAIN));
	reAlEffectf(effect, AL_REVERB_LATE_REVERB_DELAY,     props->flReverbDelay);
	reAlEffectf(effect, AL_REVERB_AIR_ABSORPTION_GAINHF, clampF(mB_to_gain(props->flAirAbsorptionHF), AL_EAXREVERB_MIN_AIR_ABSORPTION_GAINHF, AL_EAXREVERB_MAX_AIR_ABSORPTION_GAINHF));
	reAlEffectf(effect, AL_REVERB_ROOM_ROLLOFF_FACTOR,   props->flRoomRolloffFactor);
	reAlEffecti(effect, AL_REVERB_DECAY_HFLIMIT,         (props->ulFlags&EAXLISTENERFLAGS_DECAYHFLIMIT) ? AL_TRUE : AL_FALSE);
}

void EAX3_SetReverbMix(ALuint filter, float mix)
{
	//long vol=(long)linear_to_dB(mix);
	//DSPROPERTY_EAXBUFFER_ROOMHF,
	//DSPROPERTY_EAXBUFFER_ROOM,
	//DSPROPERTY_EAXBUFFER_REVERBMIX,
	
	long mbvol = gain_to_mB(mix);
	float mb   = mbvol;
	float mbhf = mbvol;
	
	reAlFilteri(filter, AL_FILTER_TYPE, AL_FILTER_LOWPASS);
	reAlFilterf(filter, AL_LOWPASS_GAIN,   mB_to_gain(Min(mb, 0.0f)));
	reAlFilterf(filter, AL_LOWPASS_GAINHF, mB_to_gain(mbhf));
}

#endif