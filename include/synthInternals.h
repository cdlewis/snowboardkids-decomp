#ifndef __SYNTH_INTERNALS__
#define __SYNTH_INTERNALS__

#include <PR/abi.h>
#include <PR/libaudio.h>

#define AL_MAX_RSP_SAMPLES 160
#define AL_DECODER_IN 0
#define AL_RESAMPLER_OUT 0
#define AL_TEMP_0 0
#define AL_DECODER_OUT 320
#define AL_TEMP_1 320
#define AL_TEMP_2 640
#define AL_MAIN_L_OUT 1088
#define AL_MAIN_R_OUT 1408
#define AL_AUX_L_OUT 1728
#define AL_AUX_R_OUT 2048

enum { AL_ADPCM, AL_RESAMPLE, AL_BUFFER, AL_SAVE, AL_ENVMIX, AL_FX, AL_AUXBUS, AL_MAINBUS };

enum {
    AL_FILTER_FREE_VOICE,
    AL_FILTER_SET_SOURCE,
    AL_FILTER_ADD_SOURCE,
    AL_FILTER_ADD_UPDATE,
    AL_FILTER_RESET,
    AL_FILTER_SET_WAVETABLE,
    AL_FILTER_SET_DRAM,
    AL_FILTER_SET_PITCH,
    AL_FILTER_SET_UNITY_PITCH,
    AL_FILTER_START,
    AL_FILTER_SET_STATE,
    AL_FILTER_SET_VOLUME,
    AL_FILTER_SET_PAN,
    AL_FILTER_START_VOICE_ALT,
    AL_FILTER_START_VOICE,
    AL_FILTER_STOP_VOICE,
    AL_FILTER_SET_FXAMT
};

typedef struct ALParam_s {
    struct ALParam_s *next;
    s32 delta;
    s16 type;
    union {
        f32 f;
        s32 i;
    } data;
    union {
        f32 f;
        s32 i;
    } moredata;
    union {
        f32 f;
        s32 i;
    } stillmoredata;
    union {
        f32 f;
        s32 i;
    } yetstillmoredata;
} ALParam;

typedef struct {
    struct ALParam_s *next;
    s32 delta;
    s16 type;
    s16 unity;
    f32 pitch;
    s16 volume;
    ALPan pan;
    u8 fxMix;
    s32 samples;
    struct ALWaveTable_s *wave;
} ALStartParamAlt;

typedef Acmd *(*ALCmdHandler)(void *, s16 *, s32, s32, Acmd *);
typedef s32 (*ALSetParam)(void *, s32, void *);

typedef struct ALFilter_s {
    struct ALFilter_s *source;
    ALCmdHandler handler;
    ALSetParam setParam;
    s16 inp;
    s16 outp;
    s32 type;
} ALFilter;

typedef struct {
    struct ALParam_s *next;
    s32 delta;
    s16 type;
    s16 unity;
    ALWaveTable *wave;
} ALStartParam;

typedef struct {
    struct ALParam_s *next;
    s32 delta;
    s16 type;
    struct PVoice_s *pvoice;
} ALFreeParam;

typedef struct ALSave_s {
    ALFilter filter;
    s32 dramout;
    s32 first;
} ALSave;

void alSaveNew(ALSave *r);
Acmd *alSavePull(void *f, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p);
s32 alSaveParam(void *f, s32 paramID, void *param);

typedef struct ALMainBus_s {
    ALFilter filter;
    s32 sourceCount;
    s32 maxSources;
    ALFilter **sources;
} ALMainBus;

void alMainBusNew(ALMainBus *m, void *ptr, s32 len);
Acmd *alMainBusPull(void *f, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p);
s32 alMainBusParam(void *filter, s32 paramID, void *param);

void alFilterNew(ALFilter *f, ALCmdHandler h, ALSetParam s, s32 type);

typedef struct {
    ALFilter filter;
    ADPCM_STATE *state;
    ADPCM_STATE *lstate;
    ALRawLoop loop;
    ALWaveTable *table;
    s32 bookSize;
    ALDMAproc dma;
    void *dmaState;
    s32 sample;
    s32 lastsam;
    s32 first;
    s32 memin;
} ALLoadFilter;

void alLoadNew(ALLoadFilter *f, ALDMANew dma, ALHeap *hp);
Acmd *alAdpcmPull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p);
s32 alLoadParam(void *filter, s32 paramID, void *param);

typedef struct ALResampler_s {
    ALFilter filter;
    RESAMPLE_STATE *state;
    f32 ratio;
    s32 upitch;
    f32 delta;
    s32 first;
    ALParam *ctrlList;
    ALParam *ctrlTail;
    s32 motion;
} ALResampler;

typedef struct ALEnvMixer_s {
    ALFilter filter;
    ENVMIX_STATE *state;
    s16 pan;
    s16 volume;
    s16 cvolL;
    s16 cvolR;
    s16 dryamt;
    s16 wetamt;
    u16 lratl;
    s16 lratm;
    s16 ltgt;
    u16 rratl;
    s16 rratm;
    s16 rtgt;
    s32 delta;
    s32 segEnd;
    s32 first;
    ALParam *ctrlList;
    ALParam *ctrlTail;
    ALFilter **sources;
    s32 motion;
} ALEnvMixer;

void alEnvmixerNew(ALEnvMixer *e, ALHeap *hp);
Acmd *alEnvmixerPull(void *f, s16 *outp, s32 out, s32 sampleOffset, Acmd *p);
s32 alEnvmixerParam(void *filter, s32 paramID, void *param);

typedef struct {
    s16 fc;
    s16 fgain;
    union {
        s16 fccoef[16];
        s64 force_aligned;
    } fcvec;
    POLEF_STATE *fstate;
    s32 first;
} ALLowPass;

typedef struct {
    u32 input;
    u32 output;
    s16 ffcoef;
    s16 fbcoef;
    s16 gain;
    f32 rsinc;
    f32 rsval;
    s32 rsdelta;
    f32 rsgain;
    ALLowPass *lp;
    ALResampler *rs;
} ALDelay;

typedef s32 (*ALSetFXParam)(void *, s32, void *);

typedef struct {
    ALFilter filter;
    s16 *base;
    s16 *input;
    u32 length;
    ALDelay *delay;
    u8 section_count;
    ALSetFXParam paramHdl;
} ALFx;

void alFxNew(ALFx *r, ALSynConfig *c, ALHeap *hp);
Acmd *alFxPull(void *f, s16 *outp, s32 out, s32 sampleOffset, Acmd *p);
s32 alFxParam(void *filter, s32 paramID, void *param);
s32 alFxParamHdl(void *filter, s32 paramID, void *param);

typedef struct ALAuxBus_s {
    ALFilter filter;
    s32 sourceCount;
    s32 maxSources;
    ALFilter **sources;
    ALFx fx[1];
} ALAuxBus;

void alAuxBusNew(ALAuxBus *m, void *ptr, s32 len);
Acmd *alAuxBusPull(void *f, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p);
s32 alAuxBusParam(void *filter, s32 paramID, void *param);

void alResampleNew(ALResampler *r, ALHeap *hp);
Acmd *alResamplePull(void *f, s16 *outp, s32 out, s32 sampleOffset, Acmd *p);
s32 alResampleParam(void *f, s32 paramID, void *param);

typedef struct PVoice_s {
    ALLink node;
    struct ALVoice_s *vvoice;
    ALFilter *channelKnob;
    ALLoadFilter decoder;
    ALResampler resampler;
    ALEnvMixer envmixer;
    s32 offset;
} PVoice;

#define _init_lpfilter init_lpfilter
void _init_lpfilter(ALLowPass *lp);

ALParam *__allocParam(void);
void __freeParam(ALParam *param);
void _freePVoice(ALSynth *drvr, PVoice *pvoice);
void _collectPVoices(ALSynth *drvr);
s32 _timeToSamples(ALSynth *synth, s32 micros);

#endif
