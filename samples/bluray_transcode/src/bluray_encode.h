#ifndef __BLURAY_ENCODE_H__
#define __BLURAY_ENCODE_H__

mfxStatus SetupEncoder(int argc, msdk_char *argv[], int argPos, mfxFrameInfo *pFrameInfo, CEncodingPipeline*& pPipeline, CFrameFifo *pFrameFifo);

#endif
