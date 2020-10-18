#pragma once

#ifdef AUDIO_OAL
#include "eax.h"
#include "AL/efx.h"


void EFXInit();
void EAX3_Set(ALuint effect, const EAXLISTENERPROPERTIES *props);
void EFX_Set(ALuint effect, const EAXLISTENERPROPERTIES *props);
void EAX3_SetReverbMix(ALuint filter, float mix);
void SetEffectsLevel(ALuint uiFilter, float level);

extern LPALGENEFFECTS reAlGenEffects;
extern LPALDELETEEFFECTS reAlDeleteEffects;
extern LPALISEFFECT reAlIsEffect;
extern LPALEFFECTI reAlEffecti;
extern LPALEFFECTIV reAlEffectiv;
extern LPALEFFECTF reAlEffectf;
extern LPALEFFECTFV reAlEffectfv;
extern LPALGETEFFECTI reAlGetEffecti;
extern LPALGETEFFECTIV reAlGetEffectiv;
extern LPALGETEFFECTF reAlGetEffectf;
extern LPALGETEFFECTFV reAlGetEffectfv;
extern LPALGENAUXILIARYEFFECTSLOTS reAlGenAuxiliaryEffectSlots;
extern LPALDELETEAUXILIARYEFFECTSLOTS reAlDeleteAuxiliaryEffectSlots;
extern LPALISAUXILIARYEFFECTSLOT reAlIsAuxiliaryEffectSlot;
extern LPALAUXILIARYEFFECTSLOTI reAlAuxiliaryEffectSloti;
extern LPALAUXILIARYEFFECTSLOTIV reAlAuxiliaryEffectSlotiv;
extern LPALAUXILIARYEFFECTSLOTF reAlAuxiliaryEffectSlotf;
extern LPALAUXILIARYEFFECTSLOTFV reAlAuxiliaryEffectSlotfv;
extern LPALGETAUXILIARYEFFECTSLOTI reAlGetAuxiliaryEffectSloti;
extern LPALGETAUXILIARYEFFECTSLOTIV reAlGetAuxiliaryEffectSlotiv;
extern LPALGETAUXILIARYEFFECTSLOTF reAlGetAuxiliaryEffectSlotf;
extern LPALGETAUXILIARYEFFECTSLOTFV reAlGetAuxiliaryEffectSlotfv;
extern LPALGENFILTERS reAlGenFilters;
extern LPALDELETEFILTERS reAlDeleteFilters;
extern LPALISFILTER reAlIsFilter;
extern LPALFILTERI reAlFilteri;
extern LPALFILTERIV reAlFilteriv;
extern LPALFILTERF reAlFilterf;
extern LPALFILTERFV reAlFilterfv;
extern LPALGETFILTERI reAlGetFilteri;
extern LPALGETFILTERIV reAlGetFilteriv;
extern LPALGETFILTERF reAlGetFilterf;
extern LPALGETFILTERFV reAlGetFilterfv;
#endif
