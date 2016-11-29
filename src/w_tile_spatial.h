/*
** $Id: w_tile_spatial.h,v 1.2 2008/06/18 17:26:11 thor Exp $
**
*/

#ifndef JPEGXR_W_TILE_SPATIAL
#define JPEGXR_W_TILE_SPATIAL

extern int w_DECODE_BLOCK_ADAPTIVE(hdp_image_t image, struct wbitstream*str,
				   unsigned tx, unsigned mx,
				   int cbp_flag, int chroma_flag,
				   int channel, int block, int mbhp_pred_mode,
				   unsigned model_bits);

extern void w_BLOCK_FLEXBITS(hdp_image_t image, struct wbitstream*str,
			     unsigned tx, unsigned ty,
			     unsigned mx, unsigned my,
			     unsigned ch, unsigned bl, unsigned model_bits);

extern void w_MB_CBP(hdp_image_t image, struct wbitstream*str,
		     int alpha_flag,
		     unsigned tx, unsigned ty,
		     unsigned mx, unsigned my);

extern int w_MB_HP(hdp_image_t image, struct wbitstream*str,
		   int alpha_flag,
		   unsigned tx, unsigned ty,
		   unsigned mx, unsigned my);
#endif
