/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

    

#ifndef SWSCALE_H
#define SWSCALE_H    

#include "avutil.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AV_STRINGIFY(s)         AV_TOSTRING(s)
#define AV_TOSTRING(s) #s

#define LIBSWSCALE_VERSION_INT  ((0<<16)+(5<<8)+0)
#define LIBSWSCALE_VERSION      0.5.0
#define LIBSWSCALE_BUILD        LIBSWSCALE_VERSION_INT

#define LIBSWSCALE_IDENT        "SwS" AV_STRINGIFY(LIBSWSCALE_VERSION)  

#define SWS_FAST_BILINEAR 1
#define SWS_BILINEAR 2
#define SWS_BICUBIC  4
#define SWS_X        8
#define SWS_POINT    0x10
#define SWS_AREA     0x20
#define SWS_BICUBLIN 0x40
#define SWS_GAUSS    0x80
#define SWS_SINC     0x100
#define SWS_LANCZOS  0x200
#define SWS_SPLINE   0x400

#define SWS_SRC_V_CHR_DROP_MASK		0x30000
#define SWS_SRC_V_CHR_DROP_SHIFT	16

#define SWS_PARAM_DEFAULT		123456

#define SWS_PRINT_INFO		0x1000    

#define SWS_FULL_CHR_H_INT	0x2000

#define SWS_FULL_CHR_H_INP	0x4000
#define SWS_DIRECT_BGR		0x8000
#define SWS_ACCURATE_RND	0x40000

#define SWS_CPU_CAPS_MMX   0x80000000
#define SWS_CPU_CAPS_MMX2  0x20000000
#define SWS_CPU_CAPS_3DNOW 0x40000000
#define SWS_CPU_CAPS_ALTIVEC 0x10000000

#define SWS_MAX_REDUCE_CUTOFF 0.002

#define SWS_CS_ITU709		1
#define SWS_CS_FCC 		4
#define SWS_CS_ITU601		5
#define SWS_CS_ITU624		5
#define SWS_CS_SMPTE170M 	5
#define SWS_CS_SMPTE240M 	7
#define SWS_CS_DEFAULT 		5        

typedef struct {
	double *coeff;
	int length;
} SwsVector;  

typedef struct {
	SwsVector *lumH;
	SwsVector *lumV;
	SwsVector *chrH;
	SwsVector *chrV;
} SwsFilter;

struct SwsContext;

void sws_freeContext(struct SwsContext *swsContext);

struct SwsContext *sws_getContext(int srcW, int srcH, int srcFormat, int dstW, int dstH, int dstFormat, int flags,
			 SwsFilter *srcFilter, SwsFilter *dstFilter, double *param);
int sws_scale(struct SwsContext *context, uint8_t* src[], int srcStride[], int srcSliceY,
                           int srcSliceH, uint8_t* dst[], int dstStride[]);
int sws_scale_ordered(struct SwsContext *context, uint8_t* src[], int srcStride[], int srcSliceY,
                           int srcSliceH, uint8_t* dst[], int dstStride[]) attribute_deprecated;  

int sws_setColorspaceDetails(struct SwsContext *c, const int inv_table[4], int srcRange, const int table[4], int dstRange, int brightness, int contrast, int saturation);
int sws_getColorspaceDetails(struct SwsContext *c, int **inv_table, int *srcRange, int **table, int *dstRange, int *brightness, int *contrast, int *saturation);
SwsVector *sws_getGaussianVec(double variance, double quality);
SwsVector *sws_getConstVec(double c, int length);
SwsVector *sws_getIdentityVec(void);
void sws_scaleVec(SwsVector *a, double scalar);
void sws_normalizeVec(SwsVector *a, double height);
void sws_convVec(SwsVector *a, SwsVector *b);
void sws_addVec(SwsVector *a, SwsVector *b);
void sws_subVec(SwsVector *a, SwsVector *b);
void sws_shiftVec(SwsVector *a, int shift);
SwsVector *sws_cloneVec(SwsVector *a);

void sws_printVec(SwsVector *a);
void sws_freeVec(SwsVector *a);

SwsFilter *sws_getDefaultFilter(float lumaGBlur, float chromaGBlur, 
				float lumaSarpen, float chromaSharpen,
				float chromaHShift, float chromaVShift,
				int verbose);
void sws_freeFilter(SwsFilter *filter);

struct SwsContext *sws_getCachedContext(struct SwsContext *context,
                int srcW, int srcH, int srcFormat,
                int dstW, int dstH, int dstFormat, int flags,
                SwsFilter *srcFilter, SwsFilter *dstFilter, double *param);

#ifdef __cplusplus
}
#endif

#endif
