#ifndef SPEECHSYNTHESIS_H
#define SPEECHSYNTHESIS_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>

typedef int SR_DWORD;
typedef short int SR_WORD;

typedef struct _wave_pcm_hdr {
    char riff[4];
    int size_8;
    char wave[4];
    char fmt[4];
    int fmt_size;
    short int format_tag;
    short int channels;
    int samples_per_sec;
    int avg_bytes_per_sec;
    short int block_align;
    short int bits_per_sample;
    char data[4];
    int data_size;
} wave_pcm_hdr;

class SpeechSynthesis {
public:
    SpeechSynthesis();
    ~SpeechSynthesis();

    int textToSpeech(const char* src_text, const char* des_path, const char* params);

private:
    wave_pcm_hdr default_wav_hdr;
};

#endif // SPEECHSYNTHESIS_H
