/******************************************************************************\
Copyright (c) 2005-2017, Intel Corporation
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

This sample was distributed or derived from the Intel's Media Samples package.
The original version of this sample may be obtained from https://software.intel.com/en-us/intel-media-server-studio
or https://software.intel.com/en-us/media-client-solutions-support.
\**********************************************************************************/

#include "mfx_samples_config.h"

#include "pipeline_decode.h"
#include "pipeline_encode.h"
#include "bluray_encode.h"
#include <sstream>
#include "version.h"

void PrintHelp(msdk_char *strAppName, const msdk_char *strErrorMessage)
{
    msdk_printf(MSDK_STRING("Bluray  Trasncode Version 1.0.0\n"));
    msdk_printf(MSDK_STRING("        Based on Intel Media SDK samples.  %s\n\n"), GetMSDKSampleVersion().c_str());
    msdk_printf(MSDK_STRING("        This aplication is free and open where possible.\n\n"));

    msdk_printf(MSDK_STRING("        NOTE: This application works best with 4th generation Intel® Core processor(codename Haswell) onward.\n\n"));

    if (strErrorMessage)
    {
        msdk_printf(MSDK_STRING("Error: %s\n"), strErrorMessage);
    }

    msdk_printf(MSDK_STRING("Usage: %s <codecid> [<decode options>] -i InputBitstream <codecid> [<encode options>] -o OutputBitstream \n"), strAppName);
    msdk_printf(MSDK_STRING("\n"));
    msdk_printf(MSDK_STRING("Supported codecs (<codecid>):\n"));
    msdk_printf(MSDK_STRING("   <codecid>=h264|mpeg2|vc1|mvc - built-in Media SDK codecs\n"));
    msdk_printf(MSDK_STRING("\n"));
    msdk_printf(MSDK_STRING("Decode & Encode Options:\n"));
    msdk_printf(MSDK_STRING("   [-hw]                     - use platform specific SDK implementation (default)\n"));
    msdk_printf(MSDK_STRING("   [-sw]                     - use software implementation, if not specified platform specific SDK implementation is used\n"));
    msdk_printf(MSDK_STRING("   [-f]                      - rendering framerate\n"));
    msdk_printf(MSDK_STRING("   [-async]                 - depth of asynchronous pipeline. default value is 8. must be between 1 and 20.\n"));
#if D3D_SURFACES_SUPPORT
    msdk_printf(MSDK_STRING("   [-d3d]                    - work with d3d9 surfaces\n"));
    msdk_printf(MSDK_STRING("   [-d3d11]                  - work with d3d11 surfaces\n"));
    msdk_printf(MSDK_STRING("\n"));

#endif
    msdk_printf(MSDK_STRING("   [-async]                  - depth of asynchronous pipeline. default value is 8. must be between 1 and 20\n"));
    msdk_printf(MSDK_STRING("   [-gpucopy::<on,off>] Enable or disable GPU copy mode\n"));
	msdk_printf(MSDK_STRING("   [-timeout]                - timeout in seconds\n"));
    msdk_printf(MSDK_STRING("\n"));
    msdk_printf(MSDK_STRING("\n"));
    
	msdk_printf(MSDK_STRING("Decode Options:\n"));
	msdk_printf(MSDK_STRING("   [-di bob/adi]             - enable deinterlacing BOB/ADI\n"));
    msdk_printf(MSDK_STRING("   [-w]                      - output width\n"));
    msdk_printf(MSDK_STRING("   [-h]                      - output height\n"));
	msdk_printf(MSDK_STRING("\n"));
    msdk_printf(MSDK_STRING("\n"));

	msdk_printf(MSDK_STRING("Encode Options:\n"));
    msdk_printf(MSDK_STRING("   [-tff|bff] - input stream is interlaced, top|bottom fielf first, if not specified progressive is expected\n"));
    msdk_printf(MSDK_STRING("   [-bref] - arrange B frames in B pyramid reference structure\n"));
    msdk_printf(MSDK_STRING("   [-nobref] -  do not use B-pyramid (by default the decision is made by library). enabled by default.\n"));
    msdk_printf(MSDK_STRING("   [-idr_interval size] - idr interval, default 0 means every I is an IDR, 1 means every other I frame is an IDR etc\n"));
    msdk_printf(MSDK_STRING("   [-n number] - number of frames to process\n"));
    msdk_printf(MSDK_STRING("   [-b bitRate] - encoded bit rate (Kbits per second), valid for H.264, H.265, MPEG2 and MVC encoders \n"));
    msdk_printf(MSDK_STRING("   [-u speed|quality|balanced] - target usage, valid for H.264, H.265, MPEG2 and MVC encoders.\n"));
    msdk_printf(MSDK_STRING("   [-r distance] - Distance between I- or P- key frames (1 means no B-frames) \n"));
    msdk_printf(MSDK_STRING("   [-g size] - GOP size (default 256)\n"));
    msdk_printf(MSDK_STRING("   [-x numRefs]   - number of reference frames\n"));
    msdk_printf(MSDK_STRING("   [-la] - use the look ahead bitrate control algorithm (LA BRC) (by default constant bitrate control method is used)\n"));
    msdk_printf(MSDK_STRING("           for H.264, H.265 encoder. Supported only with -hw option on 4th Generation Intel Core processors. \n"));
    msdk_printf(MSDK_STRING("   [-lad depth] - depth parameter for the LA BRC, the number of frames to be analyzed before encoding. In range [10,100].\n"));
    msdk_printf(MSDK_STRING("            may be 1 in the case when -mss option is specified \n"));
    msdk_printf(MSDK_STRING("   [-dstw width] - destination picture width, invokes VPP resizing\n"));
    msdk_printf(MSDK_STRING("   [-dsth height] - destination picture height, invokes VPP resizing\n"));
    msdk_printf(MSDK_STRING("   [-gpucopy::<on,off>] Enable or disable GPU copy mode\n"));
    msdk_printf(MSDK_STRING("   [-qvbr quality]          - quality controlled variable bitrate control, quality in range [11,51] where 11 is the highest quality.\n"));
    msdk_printf(MSDK_STRING("                              Bit rate (-b) and max bit rate (-MaxKbps) are used by qvbr bitrate control.\n"));
    msdk_printf(MSDK_STRING("                              This algorithm tries to achieve the subjective quality with minimum no. of bits while trying to keep\n"));
    msdk_printf(MSDK_STRING("                              the bitrate constant and HRD compliance is being followed. QVBR is supported from 4th generation\n"));
    msdk_printf(MSDK_STRING("                              Intel® Core processor(codename Haswell) onward.\n"));
    msdk_printf(MSDK_STRING("   [-vbr]                   - variable bitrate control\n"));
    msdk_printf(MSDK_STRING("   [-cqp]                   - constant quantization parameter (CQP BRC) bitrate control method\n"));
    msdk_printf(MSDK_STRING("                              (by default constant bitrate control method is used), should be used along with -qpi, -qpp, -qpb.\n"));
    msdk_printf(MSDK_STRING("   [-qpi]                   - constant quantizer for I frames (if bitrace control method is CQP). In range [1,51]. 0 by default, i.e.no limitations on QP.\n"));
    msdk_printf(MSDK_STRING("   [-qpp]                   - constant quantizer for P frames (if bitrace control method is CQP). In range [1,51]. 0 by default, i.e.no limitations on QP.\n"));
    msdk_printf(MSDK_STRING("   [-qpb]                   - constant quantizer for B frames (if bitrace control method is CQP). In range [1,51]. 0 by default, i.e.no limitations on QP.\n"));
    msdk_printf(MSDK_STRING("   [-qsv-ff]       Enable QSV-FF mode\n"));
    msdk_printf(MSDK_STRING("   [-gpb:<on,off>]          - Turn this option OFF to make HEVC encoder use regular P-frames instead of GPB\n"));
    msdk_printf(MSDK_STRING("   [-num_slice]             - number of slices in each video frame. 1 by default.\n"));
    msdk_printf(MSDK_STRING("                              If num_slice equals zero, the encoder may choose any slice partitioning allowed by the codec standard.\n"));
    msdk_printf(MSDK_STRING("   [-mss]                   - maximum slice size in bytes. Supported only with -hw and h264 codec. This option is not compatible with -num_slice option.\n"));
    msdk_printf(MSDK_STRING("   [-mfs]                   - maximum frame size in bytes. Supported only with h264 and hevc codec for VBR mode.\n"));
    msdk_printf(MSDK_STRING("   [-CodecProfile]          - specifies codec profile. HIGH by default.\n"));
    msdk_printf(MSDK_STRING("   [-CodecLevel]            - specifies codec level. 4.1 by default.\n"));
    msdk_printf(MSDK_STRING("   [-GopOptFlag:closed]     - closed gop. open gop by default\n"));
    msdk_printf(MSDK_STRING("   [-GopOptFlag:strict]     - strict gop\n"));
    msdk_printf(MSDK_STRING("   [-InitialDelayInKB]      - the decoder starts decoding after the buffer reaches the initial size InitialDelayInKB, \
                            which is equivalent to reaching an initial delay of InitialDelayInKB*8000/TargetKbps ms\n"));
    msdk_printf(MSDK_STRING("   [-BufferSizeInKB ]       - represents the maximum possible size of any compressed frames\n"));
    msdk_printf(MSDK_STRING("   [-MaxKbps ]              - for variable bitrate control, specifies the maximum bitrate at which \
                            the encoded data enters the Video Buffering Verifier buffer\n"));

    msdk_printf(MSDK_STRING("   [-timeout]               - encoding in cycle not less than specific time in seconds\n"));
    msdk_printf(MSDK_STRING("\n"));
	msdk_printf(MSDK_STRING("Example:\n"));
    msdk_printf(MSDK_STRING("  %s mvc -i in.264 mvc -o out.264\n"), strAppName);
}

mfxStatus ParseInputString(msdk_char* strInput[], mfxU8 nArgNum, int *nArgPos, sInputParams* pParams)
{
	bool complete = false;

    if (1 == nArgNum)
    {
        PrintHelp(strInput[0], NULL);
        return MFX_ERR_UNSUPPORTED;
    }

    MSDK_CHECK_POINTER(pParams, MFX_ERR_NULL_PTR);

    // set default implementation
	pParams->bUseHWLib = false;
    pParams->bUseFullColorRange = false;
#if defined(LIBVA_SUPPORT)
    pParams->libvaBackend = MFX_LIBVA_DRM;
#endif

    for (mfxU8 i = 1; i < nArgNum; i++)
    {
		*nArgPos = i;

		if (complete) {
			break;
		}

        if (MSDK_CHAR('-') != strInput[i][0])
        {
            mfxStatus sts = StrFormatToCodecFormatFourCC(strInput[i], pParams->videoType);
            if (sts != MFX_ERR_NONE)
            {
                PrintHelp(strInput[0], MSDK_STRING("Unknown codec"));
                return MFX_ERR_UNSUPPORTED;
            }
            if (!IsDecodeCodecSupported(pParams->videoType))
            {
                PrintHelp(strInput[0], MSDK_STRING("Unsupported codec"));
                return MFX_ERR_UNSUPPORTED;
            }
            if (pParams->videoType == CODEC_MVC)
            {
                pParams->videoType = MFX_CODEC_AVC;
                pParams->bIsMVC = true;
            }
            continue;
        }

        if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-sw")))
        {
            pParams->bUseHWLib = false;
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-hw")))
        {
            pParams->bUseHWLib = true;
        }
#if D3D_SURFACES_SUPPORT
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-d3d")))
        {
            pParams->memType = D3D9_MEMORY;
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-d3d11")))
        {
            pParams->memType = D3D11_MEMORY;
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-r")))
        {
            pParams->mode = MODE_RENDERING;
            // use d3d9 rendering by default
            if (SYSTEM_MEMORY == pParams->memType)
                pParams->memType = D3D9_MEMORY;
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-wall")))
        {
            if(i + 6 >= nArgNum)
            {
                PrintHelp(strInput[0], MSDK_STRING("Not enough parameters for -wall key"));
                return MFX_ERR_UNSUPPORTED;
            }
            // use d3d9 rendering by default
            if (SYSTEM_MEMORY == pParams->memType)
                pParams->memType = D3D9_MEMORY;

            pParams->mode = MODE_RENDERING;

            msdk_opt_read(strInput[++i], pParams->nWallW);
            msdk_opt_read(strInput[++i], pParams->nWallH);
            msdk_opt_read(strInput[++i], pParams->nWallCell);
            msdk_opt_read(strInput[++i], pParams->nWallMonitor);

            mfxU32 nTitle;
            msdk_opt_read(strInput[++i], nTitle);

            pParams->bWallNoTitle = 0 == nTitle;

            msdk_opt_read(strInput[++i], pParams->nTimeout);
        }
#endif
#if defined(LIBVA_SUPPORT)
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-vaapi")))
        {
            pParams->memType = D3D9_MEMORY;
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-r")))
        {
            pParams->memType = D3D9_MEMORY;
            pParams->mode = MODE_RENDERING;
            pParams->libvaBackend = MFX_LIBVA_X11;
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-rwld")))
        {
            pParams->memType = D3D9_MEMORY;
            pParams->mode = MODE_RENDERING;
            pParams->libvaBackend = MFX_LIBVA_WAYLAND;
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-perf")))
        {
            pParams->bPerfMode = true;
        }
        else if (0 == msdk_strncmp(strInput[i], MSDK_STRING("-rdrm"), 5))
        {
            pParams->memType = D3D9_MEMORY;
            pParams->mode = MODE_RENDERING;
            pParams->libvaBackend = MFX_LIBVA_DRM_MODESET;
            if (strInput[i][5]) {
                if (strInput[i][5] != '-') {
                    PrintHelp(strInput[0], MSDK_STRING("unsupported monitor type"));
                    return MFX_ERR_UNSUPPORTED;
                }
                pParams->monitorType = getMonitorType(&strInput[i][6]);
                if (pParams->monitorType >= MFX_MONITOR_MAXNUMBER) {
                    PrintHelp(strInput[0], MSDK_STRING("unsupported monitor type"));
                    return MFX_ERR_UNSUPPORTED;
                }
            } else {
                pParams->monitorType = MFX_MONITOR_AUTO; // that's case of "-rdrm" pure option
            }
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-window")))
        {
            if(i +4 >= nArgNum)
            {
                PrintHelp(strInput[0], MSDK_STRING("Not enough parameters for -window key"));
                return MFX_ERR_UNSUPPORTED;
            }
            msdk_opt_read(strInput[++i], pParams->nRenderWinX);
            msdk_opt_read(strInput[++i], pParams->nRenderWinY);
            msdk_opt_read(strInput[++i], pParams->Width);
            msdk_opt_read(strInput[++i], pParams->Height);

            if (0 == pParams->Width)
                pParams->Width = 320;
            if (0 == pParams->Height)
                pParams->Height = 240;

            pParams->bRenderWin = true;
        }
#endif
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-low_latency")))
        {
            switch (pParams->videoType)
            {
                case MFX_CODEC_HEVC:
                case MFX_CODEC_AVC:
                case MFX_CODEC_JPEG:
                {
                    pParams->bLowLat = true;
                    if (!pParams->bIsMVC)
                        break;
                }
                default:
                {
                     PrintHelp(strInput[0], MSDK_STRING("-low_latency mode is suppoted only for H.264 and JPEG codecs"));
                     return MFX_ERR_UNSUPPORTED;
                }
            }
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-jpeg_rotate")))
        {
            if(MFX_CODEC_JPEG != pParams->videoType)
                return MFX_ERR_UNSUPPORTED;

            if(i + 1 >= nArgNum)
            {
                PrintHelp(strInput[0], MSDK_STRING("Not enough parameters for -jpeg_rotate key"));
                return MFX_ERR_UNSUPPORTED;
            }

            msdk_opt_read(strInput[++i], pParams->nRotation);
            if((pParams->nRotation != 90)&&(pParams->nRotation != 180)&&(pParams->nRotation != 270))
            {
                PrintHelp(strInput[0], MSDK_STRING("-jpeg_rotate is supported only for 90, 180 and 270 angles"));
                return MFX_ERR_UNSUPPORTED;
            }
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-calc_latency")))
        {
            switch (pParams->videoType)
            {
                case MFX_CODEC_HEVC:
                case MFX_CODEC_AVC:
                case MFX_CODEC_JPEG:
                {
                    pParams->bCalLat = true;
                    if (!pParams->bIsMVC)
                        break;
                }
                default:
                {
                     PrintHelp(strInput[0], MSDK_STRING("-calc_latency mode is suppoted only for H.264 and JPEG codecs"));
                     return MFX_ERR_UNSUPPORTED;
                }
            }
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-async")))
        {
            if(i + 1 >= nArgNum)
            {
                PrintHelp(strInput[0], MSDK_STRING("Not enough parameters for -async key"));
                return MFX_ERR_UNSUPPORTED;
            }
            if (MFX_ERR_NONE != msdk_opt_read(strInput[++i], pParams->nAsyncDepth))
            {
                PrintHelp(strInput[0], MSDK_STRING("async is invalid"));
                return MFX_ERR_UNSUPPORTED;
            }
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-timeout")))
        {
            if(i + 1 >= nArgNum)
            {
                PrintHelp(strInput[0], MSDK_STRING("Not enough parameters for -timeout key"));
                return MFX_ERR_UNSUPPORTED;
            }
            if (MFX_ERR_NONE != msdk_opt_read(strInput[++i], pParams->nTimeout))
            {
                PrintHelp(strInput[0], MSDK_STRING("timeout is invalid"));
                return MFX_ERR_UNSUPPORTED;
            }
        }

        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-di")))
        {
            if(i + 1 >= nArgNum)
            {
                PrintHelp(strInput[0], MSDK_STRING("Not enough parameters for -di key"));
                return MFX_ERR_UNSUPPORTED;
            }
            msdk_char diMode[32] = {};
            if (MFX_ERR_NONE != msdk_opt_read(strInput[++i], diMode))
            {
                PrintHelp(strInput[0], MSDK_STRING("deinterlace value is not set"));
                return MFX_ERR_UNSUPPORTED;
            }

            if (0 == msdk_strcmp(diMode, MSDK_CHAR("bob")))
            {
                pParams->eDeinterlace = MFX_DEINTERLACING_BOB;
            }
            else if (0 == msdk_strcmp(diMode, MSDK_CHAR("adi")))
            {
                pParams->eDeinterlace = MFX_DEINTERLACING_ADVANCED;
            }
            else
            {
                PrintHelp(strInput[0], MSDK_STRING("deinterlace value is invalid"));
                return MFX_ERR_UNSUPPORTED;
            }
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-gpucopy::on")))
        {
            pParams->gpuCopy = MFX_GPUCOPY_ON;
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-gpucopy::off")))
        {
            pParams->gpuCopy = MFX_GPUCOPY_OFF;
        }
#if !defined(_WIN32) && !defined(_WIN64)
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-threads_num")))
        {
            if(i + 1 >= nArgNum)
            {
                PrintHelp(strInput[0], MSDK_STRING("Not enough parameters for -threads_num key"));
                return MFX_ERR_UNSUPPORTED;
            }
            if (MFX_ERR_NONE != msdk_opt_read(strInput[++i], pParams->nThreadsNum))
            {
                PrintHelp(strInput[0], MSDK_STRING("threads_num is invalid"));
                return MFX_ERR_UNSUPPORTED;
            }
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-threads_schedtype")))
        {
            if(i + 1 >= nArgNum)
            {
                PrintHelp(strInput[0], MSDK_STRING("Not enough parameters for -threads_schedtype key"));
                return MFX_ERR_UNSUPPORTED;
            }
            if (MFX_ERR_NONE != msdk_thread_get_schedtype(strInput[++i], pParams->SchedulingType))
            {
                PrintHelp(strInput[0], MSDK_STRING("threads_schedtype is invalid"));
                return MFX_ERR_UNSUPPORTED;
            }
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-threads_priority")))
        {
            if(i + 1 >= nArgNum)
            {
                PrintHelp(strInput[0], MSDK_STRING("Not enough parameters for -threads_priority key"));
                return MFX_ERR_UNSUPPORTED;
            }
            if (MFX_ERR_NONE != msdk_opt_read(strInput[++i], pParams->Priority))
            {
                PrintHelp(strInput[0], MSDK_STRING("threads_priority is invalid"));
                return MFX_ERR_UNSUPPORTED;
            }
        }
#endif // #if !defined(_WIN32) && !defined(_WIN64)
#if _MSDK_API >= MSDK_API(1,22)
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-dec_postproc")))
        {
            if(i + 1 >= nArgNum)
            {
                PrintHelp(strInput[0], MSDK_STRING("Not enough parameters for \"-dec_postproc\", right is  \"--dec_postproc force//auto\""));
                return MFX_ERR_UNSUPPORTED;
            }
            msdk_char postProcMode[32] = {};
            if (MFX_ERR_NONE != msdk_opt_read(strInput[++i], postProcMode))
            {
                PrintHelp(strInput[0], MSDK_STRING("dec_postproc value is not set"));
                return MFX_ERR_UNSUPPORTED;
            }
            if (0 == msdk_strcmp(postProcMode, MSDK_STRING("auto")))
            {
                pParams->nDecoderPostProcessing = MODE_DECODER_POSTPROC_AUTO;
            }
            else if (0 == msdk_strcmp(postProcMode, MSDK_STRING("force")))
            {
                pParams->nDecoderPostProcessing = MODE_DECODER_POSTPROC_FORCE;
            }
            else
            {
                PrintHelp(strInput[0], MSDK_STRING("dec_postproc is invalid"));
                return MFX_ERR_UNSUPPORTED;
            }
        }
#endif //_MSDK_API >= MSDK_API(1,22)
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-f")))
        {
            if(i + 1 >= nArgNum)
            {
                PrintHelp(strInput[0], MSDK_STRING("Not enough parameters for -f key"));
                return MFX_ERR_UNSUPPORTED;
            }
            if (MFX_ERR_NONE != msdk_opt_read(strInput[++i], pParams->nMaxFPS))
            {
                PrintHelp(strInput[0], MSDK_STRING("rendering frame rate is invalid"));
                return MFX_ERR_UNSUPPORTED;
            }
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-scr:w")))
        {
            if (i + 1 >= nArgNum)
            {
                PrintHelp(strInput[0], MSDK_STRING("Not enough parameters for -scr:w key"));
                return MFX_ERR_UNSUPPORTED;
            }
            if (MFX_ERR_NONE != msdk_opt_read(strInput[++i], pParams->scrWidth))
            {
                PrintHelp(strInput[0], MSDK_STRING("screen width rate is invalid"));
                return MFX_ERR_UNSUPPORTED;
            }
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-scr:h")))
        {
            if (i + 1 >= nArgNum)
            {
                PrintHelp(strInput[0], MSDK_STRING("Not enough parameters for -scr:h key"));
                return MFX_ERR_UNSUPPORTED;
            }
            if (MFX_ERR_NONE != msdk_opt_read(strInput[++i], pParams->scrHeight))
            {
                PrintHelp(strInput[0], MSDK_STRING("screen height is invalid"));
                return MFX_ERR_UNSUPPORTED;
            }
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-w")))
        {
            if (i + 1 >= nArgNum)
            {
                PrintHelp(strInput[0], MSDK_STRING("Not enough parameters for -w key"));
                return MFX_ERR_UNSUPPORTED;
            }
            if (MFX_ERR_NONE != msdk_opt_read(strInput[++i], pParams->Width))
            {
                PrintHelp(strInput[0], MSDK_STRING("width is invalid"));
                return MFX_ERR_UNSUPPORTED;
            }
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-h")))
        {
            if (i + 1 >= nArgNum)
            {
                PrintHelp(strInput[0], MSDK_STRING("Not enough parameters for -h key"));
                return MFX_ERR_UNSUPPORTED;
            }
            if (MFX_ERR_NONE != msdk_opt_read(strInput[++i], pParams->Height))
            {
                PrintHelp(strInput[0], MSDK_STRING("height is invalid"));
                return MFX_ERR_UNSUPPORTED;
            }
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-n")))
        {
            if(i + 1 >= nArgNum)
            {
                PrintHelp(strInput[0], MSDK_STRING("Not enough parameters for -n key"));
                return MFX_ERR_UNSUPPORTED;
            }
            if (MFX_ERR_NONE != msdk_opt_read(strInput[++i], pParams->nFrames))
            {
                PrintHelp(strInput[0], MSDK_STRING("rendering frame rate is invalid"));
                return MFX_ERR_UNSUPPORTED;
            }
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-jpeg_rgb")))
        {
            if(MFX_CODEC_JPEG == pParams->videoType)
            {
               pParams->chromaType = MFX_JPEG_COLORFORMAT_RGB;
            }
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-i420")))
        {
            pParams->fourcc = MFX_FOURCC_NV12;
            pParams->outI420 = true;
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-nv12")))
        {
            pParams->fourcc = MFX_FOURCC_NV12;
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-rgb4")))
        {
            pParams->fourcc = MFX_FOURCC_RGB4;
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-rgb4_fcr")))
        {
            pParams->fourcc = MFX_FOURCC_RGB4;
            pParams->bUseFullColorRange = true;
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-p010")))
        {
            pParams->fourcc = MFX_FOURCC_P010;
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-a2rgb10")))
        {
            pParams->fourcc = MFX_FOURCC_A2RGB10;
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-path")))
        {
            i++;
#if defined(_WIN32) || defined(_WIN64)
            msdk_char wchar[MSDK_MAX_FILENAME_LEN];
            msdk_opt_read(strInput[i], wchar);
            std::wstring wstr(wchar);
            std::string str(wstr.begin(), wstr.end());

            strcpy_s(pParams->pluginParams.strPluginPath, str.c_str());
#else
            msdk_opt_read(strInput[i], pParams->pluginParams.strPluginPath);
#endif
            pParams->pluginParams.type = MFX_PLUGINLOAD_TYPE_FILE;
        }
        else if (0 == msdk_strcmp(strInput[i], MSDK_STRING("-i:null")))
        {
            ;
        }
        else // 1-character options
        {
            switch (strInput[i][1])
            {
            case MSDK_CHAR('p'):
                if (++i < nArgNum) {
                   if (MFX_ERR_NONE == ConvertStringToGuid(strInput[i], pParams->pluginParams.pluginGuid))
                    {
                        if(pParams->pluginParams.type != MFX_PLUGINLOAD_TYPE_FILE)
                        {
                            pParams->pluginParams.type = MFX_PLUGINLOAD_TYPE_GUID;
                        }
                    }
                    else
                    {
                        PrintHelp(strInput[0], MSDK_STRING("Unknown options"));
                    }
                 }
                else {
                    msdk_printf(MSDK_STRING("error: option '-p' expects an argument\n"));
                }
                break;
            case MSDK_CHAR('i'):
                if (++i < nArgNum) {
                    msdk_opt_read(strInput[i], pParams->strSrcFile);
					pParams->mode = MODE_FILE_DUMP;
					complete = true;
                }
                else {
                    msdk_printf(MSDK_STRING("error: option '-i' expects an argument\n"));
                }
                break;
            case MSDK_CHAR('o'):
                if (++i < nArgNum) {
                    pParams->mode = MODE_FILE_DUMP;
                    msdk_opt_read(strInput[i], pParams->strDstFile);
                }
                else {
                    msdk_printf(MSDK_STRING("error: option '-o' expects an argument\n"));
                }
                break;
            case MSDK_CHAR('?'):
                PrintHelp(strInput[0], NULL);
                return MFX_ERR_UNSUPPORTED;
            default:
                {
                    std::basic_stringstream<msdk_char> stream;
                    stream << MSDK_STRING("Unknown option: ") << strInput[i];
                    PrintHelp(strInput[0], stream.str().c_str());
                    return MFX_ERR_UNSUPPORTED;
                }
            }
        }
    }

    if (0 == msdk_strlen(pParams->strSrcFile) && MFX_CODEC_CAPTURE != pParams->videoType)
    {
        msdk_printf(MSDK_STRING("error: source file name not found"));
        return MFX_ERR_UNSUPPORTED;
    }

    if (MFX_CODEC_CAPTURE == pParams->videoType)
    {
        if (!pParams->scrWidth || !pParams->scrHeight)
        {
            msdk_printf(MSDK_STRING("error: for screen capture, width and height must be specified manually (-scr:w and -scr:h)"));
            return MFX_ERR_UNSUPPORTED;
        }
    }
    else if (pParams->scrWidth || pParams->scrHeight)
    {
        msdk_printf(MSDK_STRING("error: width and height parameters are supported only by screen capture decoder"));
        return MFX_ERR_UNSUPPORTED;
    }

/*
    if ((pParams->mode == MODE_FILE_DUMP) && (0 == msdk_strlen(pParams->strDstFile)))
    {
        msdk_printf(MSDK_STRING("error: destination file name not found"));
        return MFX_ERR_UNSUPPORTED;
    }
*/

    if (MFX_CODEC_MPEG2   != pParams->videoType &&
        MFX_CODEC_AVC     != pParams->videoType &&
        MFX_CODEC_HEVC    != pParams->videoType &&
        MFX_CODEC_VC1     != pParams->videoType &&
        MFX_CODEC_JPEG    != pParams->videoType &&
        MFX_CODEC_CAPTURE != pParams->videoType &&
        MFX_CODEC_VP8     != pParams->videoType &&
        MFX_CODEC_VP9     != pParams->videoType)
    {
        PrintHelp(strInput[0], MSDK_STRING("Unknown codec"));
        return MFX_ERR_UNSUPPORTED;
    }

    if (pParams->nAsyncDepth == 0)
    {
        pParams->nAsyncDepth = 8; //set by default;
    }

    return MFX_ERR_NONE;
}

DWORD WINAPI EncodeThreadRun( LPVOID lpParam )
{
    mfxStatus sts = MFX_ERR_NONE; // return value check
	CEncodingPipeline   *pPipelineEncode = (CEncodingPipeline*)lpParam;

	for (;;)
    {
        sts = pPipelineEncode->Run();

        if (MFX_ERR_DEVICE_LOST == sts || MFX_ERR_DEVICE_FAILED == sts)
        {
            msdk_printf(MSDK_STRING("\nERROR: Hardware device was lost or returned an unexpected error. Recovering...\n"));
            sts = pPipelineEncode->ResetDevice();
            MSDK_CHECK_STATUS(sts, "pPipeline->ResetDevice failed");
			break;
			/*
            sts = pPipelineEncode->ResetMFXComponents(&Params);
            MSDK_CHECK_STATUS(sts, "pPipeline->ResetMFXComponents failed");
            continue;
			*/
        }
        else
        {
            MSDK_CHECK_STATUS(sts, "pPipeline->Run failed");
            break;
        }
    }

    pPipelineEncode->CaptureStopV4L2Pipeline();

    pPipelineEncode->Close();

    msdk_printf(MSDK_STRING("\nProcessing finished\n"));
	return 0;
}

#if defined(_WIN32) || defined(_WIN64)
int _tmain(int argc, TCHAR *argv[])
#else
int main(int argc, char *argv[])
#endif
{

    sInputParams        Params;   // input parameters from command line
    CDecodingPipeline   Pipeline; // pipeline for decoding, includes input file reader, decoder and output file writer
	CEncodingPipeline   *pPipelineEncode;
	CFrameFifo MemFrames;
	HANDLE  hEncodeThread;
	DWORD   dwEncodeThreadId;

	int argPos;

    mfxStatus sts = MFX_ERR_NONE; // return value check

    sts = ParseInputString(argv, (mfxU8)argc, &argPos, &Params);
    MSDK_CHECK_PARSE_RESULT(sts, MFX_ERR_NONE, 1);

    if (Params.bIsMVC)
        Pipeline.SetMultiView();

	sts = Pipeline.Init(&Params, &MemFrames);
    MSDK_CHECK_STATUS(sts, "Pipeline.Init failed");

    // print stream info
    Pipeline.PrintInfo();

	mfxFrameInfo frameInfo = Pipeline.GetFrameInfo();
	SetupEncoder(argc, argv, argPos, &frameInfo, pPipelineEncode, &MemFrames);

    // Create the thread to begin execution on its own.

    hEncodeThread = CreateThread( 
        NULL,                   // default security attributes
        0,                      // use default stack size  
        EncodeThreadRun,       // thread function name
        (void*)pPipelineEncode,          // argument to thread function 
        0,                      // use default creation flags 
        &dwEncodeThreadId);   // returns the thread identifier 


    // Check the return value for success.
    // If CreateThread fails, terminate execution. 
    // This will automatically clean up threads and memory. 

    if (hEncodeThread == NULL) 
    {
        msdk_printf(MSDK_STRING("\nERROR: Create thread failed!\n"));
        ExitProcess(3);
    }

    msdk_printf(MSDK_STRING("Decoding started\n"));

    for (;;)
    {
        sts = Pipeline.RunDecoding(pPipelineEncode);

        if (MFX_ERR_INCOMPATIBLE_VIDEO_PARAM == sts || MFX_ERR_DEVICE_LOST == sts || MFX_ERR_DEVICE_FAILED == sts)
        {
            if (MFX_ERR_INCOMPATIBLE_VIDEO_PARAM == sts)
            {
                msdk_printf(MSDK_STRING("\nERROR: Incompatible video parameters detected. Recovering...\n"));
            }
            else
            {
                msdk_printf(MSDK_STRING("\nERROR: Hardware device was lost or returned unexpected error. Recovering...\n"));
                sts = Pipeline.ResetDevice();
                MSDK_CHECK_STATUS(sts, "Pipeline.ResetDevice failed");
            }

            sts = Pipeline.ResetDecoder(&Params);
            MSDK_CHECK_STATUS(sts, "Pipeline.ResetDecoder failed");
            continue;
        }
        else
        {
            MSDK_CHECK_STATUS(sts, "Pipeline.RunDecoding failed");
            break;
        }
    }

    msdk_printf(MSDK_STRING("\nDecoding finished\n"));

    WaitForSingleObject(hEncodeThread, 20000);

    // Close all thread handles and free memory allocations.

    CloseHandle(hEncodeThread);

    return 0;
}
