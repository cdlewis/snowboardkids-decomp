#include <PR/libaudio.h>
#include "synthInternals.h"

void alSynStartVoice(ALSynth *synth, ALVoice *v, ALWaveTable *table) {
    ALStartParam *update;
    ALFilter *f;

    if (v->pvoice) {
        update = (ALStartParam *)__allocParam();
        ALFailIf(update == 0, ERR_ALSYN_NO_UPDATE);

        update->delta = synth->paramSamples + v->pvoice->offset;
        update->type = AL_FILTER_START_VOICE;
        update->wave = table;
        update->next = 0;
        update->unity = v->unityPitch;

        f = v->pvoice->channelKnob;
        (*f->setParam)(f, AL_FILTER_ADD_UPDATE, update);
    }
}
