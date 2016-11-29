/*
** $Id: w_strip.h,v 1.5 2008-12-27 01:03:17 thor Exp $
**
*/

#ifndef JPEGXR_W_STRIP_H
#define JPEGXR_W_STRIP_H

extern int quantize_dc(int value, int quant);
extern int quantize_lphp(int value, int quant,int preshift);

#ifdef USE_RALLOC
extern void w_PredCBP(hdp_image_t image, unsigned tx, unsigned ty, unsigned mx);
extern void calculate_hpcbp_up1(hdp_image_t image, int tx, int ty, int mx);
extern void w_predict_up1_hp(hdp_image_t image, int ch, int tx, int mx, int mbhp_pred_mode);
#endif

#endif
