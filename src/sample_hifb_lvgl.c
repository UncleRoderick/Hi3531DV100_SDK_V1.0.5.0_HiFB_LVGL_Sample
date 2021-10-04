/******************************************************************************

  Copyright (C), 2011-2018, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : sample_hifb.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/7/1
  Description   :
  History       :
  1.Date        : 2013/7/1
    Author      :
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <stdint.h>
#include "sample_comm.h"

#include <linux/fb.h>
#include "hifb.h"
#include "loadbmp.h"
#include "hi_tde_api.h"
#include "hi_tde_type.h"
#include "hi_tde_errcode.h"
#include "lvgl/lvgl.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_demos/lv_demo.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#define DISP_BUF_SIZE (128 * 1024)

//extern int lvgl_main(void);
//extern uint32_t custom_tick_get(void);

static VO_DEV VoDev = SAMPLE_VO_DEV_DHD0;

#define WIDTH                  1920
#define HEIGHT                 1080
#define GRAPHICS_LAYER_HC0 3

HI_VOID SAMPLE_HIFB_HandleSig(HI_S32 signo)
{
    static int sig_handled = 0;
    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);

    if (!sig_handled && (SIGINT == signo || SIGTERM == signo))
    {
        sig_handled = 1;

	#ifndef HI_FPGA
		SAMPLE_COMM_VO_HdmiStop();
	#endif

        SAMPLE_COMM_SYS_Exit();

        printf("\033[0;31mprogram exit abnormally!\033[0;39m\n");
    }

    exit(-1);
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}

HI_S32 SAMPLE_HIFB_LVGL(HI_VOID)
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32PicWidth         = WIDTH;
    HI_U32 u32PicHeight        = HEIGHT;
    SIZE_S  stSize;

	VO_LAYER VoLayer = 0;
    VO_PUB_ATTR_S stPubAttr;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
    HI_U32 u32VoFrmRate;

    VB_CONF_S       stVbConf;
	HI_U32 u32BlkSize;

    /******************************************
     step  1: init variable
    ******************************************/
    memset(&stVbConf,0,sizeof(VB_CONF_S));

    u32BlkSize = CEILING_2_POWER(u32PicWidth,SAMPLE_SYS_ALIGN_WIDTH)\
        * CEILING_2_POWER(u32PicHeight,SAMPLE_SYS_ALIGN_WIDTH) *2;

    stVbConf.u32MaxPoolCnt = 128;

    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt =  6;

    /******************************************
     step 2: mpp system init.
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with 0x%x!\n", s32Ret);
        goto SAMPLE_HIFB_NoneBufMode_0;
    }

    /******************************************
     step 3:  start vo hd0.
    *****************************************/
    s32Ret = HI_MPI_VO_UnBindGraphicLayer(GRAPHICS_LAYER_HC0, VoDev);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("UnBindGraphicLayer failed with 0x%x!\n", s32Ret);
        goto SAMPLE_HIFB_NoneBufMode_0;
    }

    s32Ret = HI_MPI_VO_BindGraphicLayer(GRAPHICS_LAYER_HC0, VoDev);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("BindGraphicLayer failed with 0x%x!\n", s32Ret);
        goto SAMPLE_HIFB_NoneBufMode_0;
    }
    stPubAttr.enIntfSync = VO_OUTPUT_1080P60;
    stPubAttr.enIntfType = VO_INTF_HDMI;
	stPubAttr.u32BgColor = 0x000000;

    stLayerAttr.bClusterMode = HI_FALSE;
	stLayerAttr.bDoubleFrame = HI_FALSE;
	stLayerAttr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;

    s32Ret = SAMPLE_COMM_VO_GetWH(stPubAttr.enIntfSync,&stSize.u32Width,\
        &stSize.u32Height,&u32VoFrmRate);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("get vo wh failed with %d!\n", s32Ret);
        goto SAMPLE_HIFB_NoneBufMode_0;
    }
    memcpy(&stLayerAttr.stImageSize,&stSize,sizeof(stSize));

	stLayerAttr.u32DispFrmRt = 30 ;
	stLayerAttr.stDispRect.s32X = 0;
	stLayerAttr.stDispRect.s32Y = 0;
	stLayerAttr.stDispRect.u32Width = stSize.u32Width;
	stLayerAttr.stDispRect.u32Height = stSize.u32Height;

    s32Ret = SAMPLE_COMM_VO_StartDev(VoDev, &stPubAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vo dev failed with %d!\n", s32Ret);
        goto SAMPLE_HIFB_NoneBufMode_0;
	}

    s32Ret = SAMPLE_COMM_VO_StartLayer(VoLayer, &stLayerAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vo layer failed with %d!\n", s32Ret);
        goto SAMPLE_HIFB_NoneBufMode_1;
	}

    if (stPubAttr.enIntfType & VO_INTF_HDMI)
    {
		#ifndef HI_FPGA
        s32Ret = SAMPLE_COMM_VO_HdmiStart(stPubAttr.enIntfSync);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("start HDMI failed with %d!\n", s32Ret);
            goto SAMPLE_HIFB_NoneBufMode_2;
    	}
		#endif
    }

    /*LittlevGL init*/
    lv_init();

    /*Linux frame buffer device init*/
    fbdev_init();

    /*A small buffer for LittlevGL to draw the screen's content*/
    static lv_color_t buf[DISP_BUF_SIZE];

    /*Initialize a descriptor for the buffer*/
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    /*Initialize and register a display driver*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &disp_buf;
    disp_drv.flush_cb   = fbdev_flush;
    disp_drv.hor_res    = 1920;
    disp_drv.ver_res    = 1080;
    lv_disp_drv_register(&disp_drv);

    /*Create a Demo*/
    lv_demo_widgets();

    /*Handle LitlevGL tasks (tickless mode)*/
    while(1) {
        lv_task_handler();
        usleep(5000);
    }

	if (stPubAttr.enIntfType & VO_INTF_HDMI)
	{
#ifndef HI_FPGA
		SAMPLE_COMM_VO_HdmiStop();
#endif
	}

SAMPLE_HIFB_NoneBufMode_2:
    SAMPLE_COMM_VO_StopLayer(VoLayer);
SAMPLE_HIFB_NoneBufMode_1:
    SAMPLE_COMM_VO_StopDev(VoDev);
SAMPLE_HIFB_NoneBufMode_0:
    SAMPLE_COMM_SYS_Exit();

    return s32Ret;
}

int main(int argc, char *argv[])
{
	HI_S32 s32Ret = HI_SUCCESS;
	HI_CHAR ch;
    HI_BOOL bExit = HI_FALSE;

    signal(SIGINT, SAMPLE_HIFB_HandleSig);
    signal(SIGTERM, SAMPLE_HIFB_HandleSig);

	return SAMPLE_HIFB_LVGL();
}

