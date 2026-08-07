#include <PR/libaudio.h>
#include "synthInternals.h"

void alSynSetPitch(ALSynth *synth, ALVoice *v, f32 pitch) {
    ALParam *update;
    ALFilter *f;

    if (v->pvoice) {
        update = __allocParam();
        ALFailIf(update == 0, ERR_ALSYN_NO_UPDATE);

        update->delta = synth->paramSamples + v->pvoice->offset;
        update->type = AL_FILTER_SET_PITCH;
        update->data.f = pitch;
        update->next = 0;

        f = v->pvoice->channelKnob;
        (*f->setParam)(f, AL_FILTER_ADD_UPDATE, update);
    }
}
