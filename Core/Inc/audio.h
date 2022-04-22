#ifndef AUDIO_H
#define AUDIO_H

#include "stm32l1xx_hal.h"
#include "fatfs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 512

#define MIN(a,b) (((a)<(b))? (a):(b))
typedef void (*funcP)(uint8_t channels, uint16_t numSamples, void *pIn, uint16_t *pOutput);

extern DAC_HandleTypeDef hdac;
extern DMA_HandleTypeDef hdma_dac_ch1;
extern FATFS fs;
extern FIL fil;

struct Wav_Header {
  char     riff[4];
  uint32_t fileSize;
  char     fileTypeHeader[4];
  char     formatChunkMarker[4];
  uint32_t formatChunkLength;
  uint16_t vfmt;
  uint16_t channels;
  uint32_t sampleFreq;
  uint32_t sampleBytesPerSecond;
  uint16_t blkSize;
  uint16_t bitsPerSample;
  char     listChunkHeader[4];
  uint32_t listChunkLength;
};

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac);
uint16_t val2Dac8(int32_t v);
uint16_t val2Dac16(int32_t v);
void prepareDACBuffer_16Bit(uint8_t channels, uint16_t numSamples, void *pIn, uint16_t *pOutput);
void prepareDACBuffer_8Bit(uint8_t channels, uint16_t numSamples, void *pIn, uint16_t *pOutput);
void outputSamplesH(FIL *fil, struct Wav_Header *header);
void Audio_Card(char string[]);

#endif
