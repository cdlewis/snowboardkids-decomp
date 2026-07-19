#include "game/audio/audio_engine_internal.h"

void updateAudioUnderrunState(s32 arg0) {
    s32 temp;

    if ((osAiGetLength() >> 2) == 0) {
        temp = 0;
        if (gAudioUnderrunState != temp) {
            return;
        }
        gAudioUnderrunState = temp;
    }
}
