/*********************
Author: Jiannan Wu
Date: 2019/05/13 11:18
File: ff_camera.c
Description: Display video of USB camera
**********************/

#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>    
#include <opencv2/highgui/highgui.hpp>   

extern "C"
{
	#include <libavformat/avformat.h>    
	#include <libavcodec/avcodec.h>    
	#include <libswscale/swscale.h>     
	#include <libavfilter/avfilter.h>
	#include <libavdevice/avdevice.h>
	#include <libavutil/avutil.h>
/*  
#include <libavutil/dict.h>  
#include <libavutil/opt.h>  
#include <libavutil/channel_layout.h>  
#include <libavutil/common.h>  
#include <libavutil/imgutils.h>  
#include <libavutil/mathematics.h>  
#include <libavutil/samplefmt.h>  
*/
}

using namespace std;
using namespace cv;


Mat avframe_to_cvmat(AVFrame *frame);

int main()
{

	//define varialbes
	int i, ret;
	AVCodecContext *pCodecCtx;
	AVCodec *pCodec;


	//////////////////////////////////////////////////
	// step 1: Initializing
	//register supporting files formats and codecs
	av_register_all();
	avdevice_register_all();

	// step 2: Connetct to camera
	AVFormatContext *pFormatCtx = avformat_alloc_context();
	AVInputFormat *pInputFmt = av_find_input_format("v4l2");

	if(NULL != pInputFmt)
		cout << "Input device name: " << pInputFmt->name << endl;
	else
	{
		cout << "Not found device!" << endl;
		return -1;
	}

	// step 3: open camera
	ret = avformat_open_input(&pFormatCtx, "/dev/video0", pInputFmt, NULL);
	if(0 == ret)
		cout << "Open device succeeded!" << endl;
	else
	{
		cout << "Open device failed!" << endl;
		return -1;
	}

	//print info
	if(avformat_find_stream_info(pFormatCtx, NULL) < 0)		
		return -1;
	av_dump_format(pFormatCtx, 0, "/dev/video0", 0);

	// step 4: Find video stream (sound not needed)
	int Videoindex = -1;
	for(int i = 0; i < pFormatCtx->nb_streams; i++)
	{
		if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			Videoindex = i;
			break;
		}
	}
	if(-1 == Videoindex)
	{
		cout << "Couldn't find a video stream!" << endl;
		return -1;
	}

	// step 5: Find decoder
	pCodecCtx = pFormatCtx->streams[Videoindex]->codec;
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if(NULL == pCodec)
	{
		cout << "Couldn't find decoder!" << endl;
		return -1;
	}

	if(avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
	{
		cout << "Couldn't open decoder!" << endl;
		return -1;
	}
	

	/////////////////////////////////////////////////////
	// step 6: Get video
	AVPacket *pPacket = (AVPacket *)av_malloc(sizeof(AVPacket));
	AVFrame *pFrame = av_frame_alloc();
	
	//Create img convert context
	//struct SwsContext *img_convert_ctx;

	cout << "width = " << pCodecCtx->width \
		<< ", height = " << pCodecCtx->height << endl;

	int unfinished = 1;
	i = 0;


	while(av_read_frame(pFormatCtx, pPacket) >= 0)		//get one frame
	{
		if(Videoindex == pPacket->stream_index)
		{
			ret = avcodec_decode_video2(pCodecCtx, pFrame, &unfinished, pPacket);
			if(ret < 0)
			{
				cout << "Fail to decode!" << endl;
				return -1;
			}

			if(unfinished)
			{
				i++;
				cout << i << endl;

				//Convert AVFrame to Mat
				Mat frame = avframe_to_cvmat(pFrame);

				// Display
				//Mat frame(pFrame->height, pFrame->width, CV_8UC3, picture.data[0]);
				namedWindow("camera", WINDOW_AUTOSIZE);
				imshow("camera", frame);
				waitKey(5);
			}

			av_free_packet(pPacket);
		}

	}

	// step 7: Destroy
	av_free_packet(pPacket);
	av_free(pFrame);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);


	return 0;
}



Mat avframe_to_cvmat(AVFrame *frame)
{
	AVFrame dst; 
	cv::Mat cvmat; 

	memset(&dst, 0, sizeof(dst)); 
	int w = frame->width, h = frame->height; 
	cvmat = cv::Mat(h, w, CV_8UC3); 
	dst.data[0] = (uint8_t *)cvmat.data; 
	avpicture_fill((AVPicture *)&dst, dst.data[0], AV_PIX_FMT_BGR24, w, h); 

	struct SwsContext *img_convert_ctx = NULL; 

	//enum PixelFormat src_pixfmt = (enum PixelFormat)frame->format; 
	AVPixelFormat src_pixfmt = (AVPixelFormat)frame->format; 
	//enum PixelFormat dst_pixfmt = AV_PIX_FMT_BGR24; 
	AVPixelFormat dst_pixfmt = AV_PIX_FMT_BGR24; 
	
	img_convert_ctx = sws_getContext(w, h, src_pixfmt, w, h, dst_pixfmt, SWS_FAST_BILINEAR, NULL, NULL, NULL); 
	sws_scale(img_convert_ctx, frame->data, frame->linesize, 0, h, dst.data, dst.linesize); 
	sws_freeContext(img_convert_ctx); 
	
	return cvmat;
}


