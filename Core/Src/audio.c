#include "audio.h"

static uint8_t flg_dma_done;
static uint8_t fileBuffer[BUFSIZE];
static uint8_t dmaBuffer[2][BUFSIZE];
static uint8_t dmaBank = 0;

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac)
{
	//HAL_DAC_Stop_DMA(hdac, DAC_CHANNEL_1);
	flg_dma_done = 1;
}

uint16_t val2Dac8(int32_t v)
{
  uint16_t out = v << 3;
  return out;
}

uint16_t val2Dac16(int32_t v)
{
  v >>= 4;
  v += 2047;
  return v & 0xfff;
}

void prepareDACBuffer_16Bit(uint8_t channels, uint16_t numSamples, void *pIn, uint16_t *pOutput)
{
  int16_t *pInput = (int16_t *)pIn;

  for (int i=0; i<numSamples; i++) {
    int32_t val = 0;

    for(int j=0; j<channels; j++) {
      val += *pInput++;
    }
    val /= channels;
    *pOutput++ = val2Dac16(val);
  }
}

void prepareDACBuffer_8Bit(uint8_t channels, uint16_t numSamples, void *pIn, uint16_t *pOutput)
{
  uint8_t *pInput = (uint8_t *)pIn;

  for (int i=0; i<numSamples; i++) {
    int32_t val = 0;

    for(int j=0; j<channels; j++) {
      val += *pInput++;
    }
    val /= channels;
    *pOutput++ = val2Dac8(val);
  }
}

void outputSamplesH(FIL *fil, struct Wav_Header *header)
{
  const uint8_t channels = header->channels;
  const uint8_t bytesPerSample = header->bitsPerSample / 8;

  funcP prepareData = (header->bitsPerSample == 8)? prepareDACBuffer_8Bit : prepareDACBuffer_16Bit;

  flg_dma_done = 1;
  dmaBank = 0;

  uint32_t bytes_last = header->listChunkLength;

  while(0 < bytes_last) {

    int blksize = (header->bitsPerSample == 8)? MIN(bytes_last, BUFSIZE / 2) : MIN(bytes_last, BUFSIZE);

    UINT bytes_read;
    FRESULT res;

    res = f_read(fil, fileBuffer, blksize, &bytes_read);
    if (res != FR_OK || bytes_read == 0)
      break;

    uint16_t numSamples = bytes_read / bytesPerSample / channels;
    int16_t     *pInput = (int16_t *)fileBuffer;
    uint16_t   *pOutput = (uint16_t *)dmaBuffer[dmaBank];

    prepareData(channels, numSamples, pInput, pOutput);

    // wait for DMA complete
    while(flg_dma_done == 0) {
      __NOP();
    }

    HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);
    flg_dma_done = 0;
    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)dmaBuffer[dmaBank], numSamples, DAC_ALIGN_12B_R);
    //HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)fileBuffer, numSamples, DAC_ALIGN_8B_R);

    dmaBank = !dmaBank;
    bytes_last -= blksize;
  };

  while(flg_dma_done == 0) {
    __NOP();
  }

  HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);
}

void Audio_Card(char string[]) {
	UINT count = 0;
	struct Wav_Header header;

	f_mount(&fs, "", 0);
	f_open(&fil, string, FA_READ);
    f_read(&fil, &header, sizeof(struct Wav_Header), &count);
    outputSamplesH(&fil, &header);
    f_close(&fil);
}
