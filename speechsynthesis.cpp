#include "speechsynthesis.h"
#include<QtNetwork>
#include<msp_errors.h>
#include<msp_cmn.h>
#include<qtts.h>

SpeechSynthesis::SpeechSynthesis() {
    default_wav_hdr = {
        { 'R', 'I', 'F', 'F' },
        0,
        {'W', 'A', 'V', 'E'},
        {'f', 'm', 't', ' '},
        16,
        1,
        1,
        16000,
        32000,
        2,
        16,
        {'d', 'a', 't', 'a'},
        0
    };
    // 初始化 MSP
        const char* login_params = "appid=055e05b0";  // 将应用ID传递给MSPLogin
        int ret = MSPLogin(NULL, NULL, login_params);
        if (MSP_SUCCESS != ret) {
            printf("MSPLogin failed, error code: %d.\n", ret);
        }
}

SpeechSynthesis::~SpeechSynthesis() {
    MSPLogout(); //退出登录

}

int SpeechSynthesis::textToSpeech(const char* src_text, const char* des_path, const char* params) {
    int ret = -1;
    FILE* fp = NULL;
    const char* sessionID = NULL;
    unsigned int audio_len = 0;
    wave_pcm_hdr wav_hdr = default_wav_hdr;
    int synth_status = MSP_TTS_FLAG_STILL_HAVE_DATA;

    if (NULL == src_text || NULL == des_path) {
        printf("params is error!\n");
        return ret;
    }

    fp = fopen(des_path, "wb");
    if (NULL == fp) {
        printf("open %s error.\n", des_path);
        return ret;
    }

    sessionID = QTTSSessionBegin(params, &ret);
    if (MSP_SUCCESS != ret) {
        printf("QTTSSessionBegin failed, error code: %d.\n", ret);
        fclose(fp);
        return ret;
    }

    ret = QTTSTextPut(sessionID, src_text, (unsigned int)strlen(src_text), NULL);
    if (MSP_SUCCESS != ret) {
        printf("QTTSTextPut failed, error code: %d.\n", ret);
        QTTSSessionEnd(sessionID, "TextPutError");
        fclose(fp);
        return ret;
    }

    printf("正在合成 ...\n");
    fwrite(&wav_hdr, sizeof(wav_hdr), 1, fp);

    while (1) {
        const void* data = QTTSAudioGet(sessionID, &audio_len, &synth_status, &ret);
        if (MSP_SUCCESS != ret)
            break;
        if (NULL != data) {
            fwrite(data, audio_len, 1, fp);
            wav_hdr.data_size += audio_len;
        }
        if (MSP_TTS_FLAG_DATA_END == synth_status)
            break;
    }

    printf("\n");
    if (MSP_SUCCESS != ret) {
        printf("QTTSAudioGet failed, error code: %d.\n", ret);
        QTTSSessionEnd(sessionID, "AudioGetError");
        fclose(fp);
        return ret;
    }

    wav_hdr.size_8 += wav_hdr.data_size + (sizeof(wav_hdr) - 8);

    fseek(fp, 4, 0);
    fwrite(&wav_hdr.size_8, sizeof(wav_hdr.size_8), 1, fp);
    fseek(fp, 40, 0);
    fwrite(&wav_hdr.data_size, sizeof(wav_hdr.data_size), 1, fp);
    fclose(fp);
    fp = NULL;

    ret = QTTSSessionEnd(sessionID, "Normal");
    if (MSP_SUCCESS != ret) {
        printf("QTTSSessionEnd failed, error code: %d.\n", ret);
    }

    return ret;
}
