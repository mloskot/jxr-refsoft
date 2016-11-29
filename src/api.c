
/*************************************************************************
*
* This software module was originally contributed by Microsoft
* Corporation in the course of development of the
* ITU-T T.832 | ISO/IEC 29199-2 ("JPEG XR") format standard for
* reference purposes and its performance may not have been optimized.
*
* This software module is an implementation of one or more
* tools as specified by the JPEG XR standard.
*
* ITU/ISO/IEC give You a royalty-free, worldwide, non-exclusive
* copyright license to copy, distribute, and make derivative works
* of this software module or modifications thereof for use in
* products claiming conformance to the JPEG XR standard as
* specified by ITU-T T.832 | ISO/IEC 29199-2.
*
* ITU/ISO/IEC give users the same free license to this software
* module or modifications thereof for research purposes and further
* ITU/ISO/IEC standardization.
*
* Those intending to use this software module in products are advised
* that its use may infringe existing patents. ITU/ISO/IEC have no
* liability for use of this software module or modifications thereof.
*
* Copyright is not released for products that do not conform to
* to the JPEG XR standard as specified by ITU-T T.832 |
* ISO/IEC 29199-2.
*
******** Section to be removed when the standard is published ************
*
* Assurance that the contributed software module can be used
* (1) in the ITU-T "T.JXR" | ISO/IEC 29199 ("JPEG XR") standard once the
* standard has been adopted; and
* (2) to develop the JPEG XR standard:
*
* Microsoft Corporation and any subsequent contributors to the development
* of this software grant ITU/ISO/IEC all rights necessary to include
* the originally developed software module or modifications thereof in the
* JPEG XR standard and to permit ITU/ISO/IEC to offer such a royalty-free,
* worldwide, non-exclusive copyright license to copy, distribute, and make
* derivative works of this software module or modifications thereof for
* use in products claiming conformance to the JPEG XR standard as
* specified by ITU-T T.832 | ISO/IEC 29199-2, and to the extent that
* such originally developed software module or portions of it are included
* in an ITU/ISO/IEC standard. To the extent that the original contributors
* may own patent rights that would be required to make, use, or sell the
* originally developed software module or portions thereof included in the
* ITU/ISO/IEC standard in a conforming product, the contributors will
* assure ITU/ISO/IEC that they are willing to negotiate licenses under
* reasonable and non-discriminatory terms and conditions with
* applicants throughout the world and in accordance with their patent
* rights declarations made to ITU/ISO/IEC (if any).
*
* Microsoft, any subsequent contributors, and ITU/ISO/IEC additionally
* gives You a free license to this software module or modifications
* thereof for the sole purpose of developing the JPEG XR standard.
*
******** end of section to be removed when the standard is published *****
*
* Microsoft Corporation retains full right to modify and use the code
* for its own purpose, to assign or donate the code to a third party,
* and to inhibit third parties from using the code for products that
* do not conform to the JPEG XR standard as specified by ITU-T T.832 |
* ISO/IEC 29199-2.
*
* This copyright notice must be included in all copies or derivative
* works.
*
* Copyright (c) ITU-T/ISO/IEC 2008, 2009.
***********************************************************************/

#ifdef _MSC_VER
#pragma comment (user,"$Id: api.c,v 1.14 2011-11-11 17:13:50 thor Exp $")
#endif

# include "jxr_priv.h"
# include <assert.h>
# include <stdlib.h>

void _jxr_send_mb_to_output(jxr_image_t image, int mx, int my, int*data)
{
    if (image->out_fun)
        image->out_fun(image, mx, my, data);
}

void jxr_set_block_output(jxr_image_t image, block_fun_t fun)
{
    image->out_fun = fun;
}

void _jxr_get_mb_from_input(jxr_image_t image, int mx, int my, int*data)
{
    if (image->inp_fun)
        image->inp_fun(image, mx, my, data);
}

void jxr_set_block_input(jxr_image_t image, block_fun_t fun)
{
    image->inp_fun = fun;
}

void jxr_set_user_data(jxr_image_t image, void*data)
{
    image->user_data = data;
}

void* jxr_get_user_data(jxr_image_t image)
{
    return image->user_data;
}

/* WARNING: This call returns the number of channels in the codestream,
** which is *likely* not the information you care about. Instead, the
** number of components is defined through the pixel format in the
** container.
*/
int jxr_get_IMAGE_CHANNELS(jxr_image_t image)
{
    return image->num_channels;
}

/* While the above returns the number of channels encoded in the codestream,
** the following returns the nominal number of channels indicated in the
** component. Note that this might be different because Y-Only is enabled
** and thus everything but the first channel is missing. Bummer!
*/
int jxr_get_CONTAINER_CHANNELS(jxr_image_t image)
{
  return image->container_nc;
}

/*
** Return an indicator whether we are currently decoding a separate alpha
** channel. Returns 0 in case the primary channels are decoded, returns 1
** in case the separate alpha channel is decoded.
*/
int jxr_is_DECODING_SEPARATE_ALPHA(jxr_image_t image)
{
  return image->container_current_separate_alpha?1:0;
}

void jxr_set_INTERNAL_CLR_FMT(jxr_image_t image, jxr_color_fmt_t fmt, int channels)
{
    switch (fmt) {
        case JXR_YONLY:
            image->use_clr_fmt  = fmt;
            image->num_channels = 1;
	    break;
        case JXR_YUV420:
        case JXR_YUV422:
        case JXR_YUV444:
            image->use_clr_fmt = fmt;
            image->num_channels = 3;
            break;
        case JXR_YUVK:
            image->use_clr_fmt = fmt;
            image->num_channels = 4;
            break;
        case JXR_OCF_NCOMPONENT:
            image->use_clr_fmt = fmt;
            image->num_channels = channels; 
            break;
        default:
            image->use_clr_fmt = fmt;
            image->num_channels = channels;
            break;
    }
    image->container_nc = channels;            
}

void jxr_set_CHROMA_CENTERING(jxr_image_t image,unsigned x,unsigned y)
{
  image->chroma_centering_x = x;
  image->chroma_centering_y = y;
}

void jxr_set_OUTPUT_CLR_FMT(jxr_image_t image, jxr_output_clr_fmt_t fmt)
{
    image->output_clr_fmt = fmt;
    
    switch (fmt) {
        case JXR_OCF_YONLY: /* YONLY */
            image->header_flags_fmt |= 0x00; /* OUTPUT_CLR_FMT==0 */
            break;
        case JXR_OCF_YUV420: /* YUV420 */
            image->header_flags_fmt |= 0x10; /* OUTPUT_CLR_FMT==1 */
            break;
        case JXR_OCF_YUV422: /* YUV422 */
            image->header_flags_fmt |= 0x20; /* OUTPUT_CLR_FMT==2 */
            break;
        case JXR_OCF_YUV444: /* YUV444 */
            image->header_flags_fmt |= 0x30; /* OUTPUT_CLR_FMT==3 */
            break;
        case JXR_OCF_CMYK: /* CMYK */
            image->header_flags_fmt |= 0x40; /* OUTPUT_CLR_FMT=4 (CMYK) */
            break;
        case JXR_OCF_CMYKDIRECT: /* CMYKDIRECT */
            image->header_flags_fmt |= 0x50; /* OUTPUT_CLR_FMT=5 (CMYKDIRECT) */
            break;
        case JXR_OCF_RGB: /* RGB color */
            image->header_flags_fmt |= 0x70; /* OUTPUT_CLR_FMT=7 (RGB) */
            break;
        case JXR_OCF_RGBE: /* RGBE color */
            image->header_flags_fmt |= 0x80; /* OUTPUT_CLR_FMT=8 (RGBE) */
            break;
        case JXR_OCF_NCOMPONENT: /* N-channel color */
            image->header_flags_fmt |= 0x60; /* OUTPUT_CLR_FMT=6 (NCOMPONENT) */
            break;
        default:
            fprintf(stderr, "Unsupported external color format (%d)! \n", fmt); 
            break;
    }
}

jxr_output_clr_fmt_t jxr_get_OUTPUT_CLR_FMT(jxr_image_t image)
{
    return SOURCE_CLR_FMT(image); // JNB fix, was: image->output_clr_fmt;
}


jxr_bitdepth_t jxr_get_OUTPUT_BITDEPTH(jxr_image_t image)
{
    return (jxr_bitdepth_t) SOURCE_BITDEPTH(image);
}

void jxr_set_OUTPUT_BITDEPTH(jxr_image_t image, jxr_bitdepth_t bd)
{
    image->header_flags_fmt &= ~0x0f;
    image->header_flags_fmt |= bd;
}

void jxr_set_BANDS_PRESENT(jxr_image_t image, jxr_bands_present_t bp)
{
    image->bands_present = bp;
    image->bands_present_of_primary = image->bands_present;
}

void jxr_set_FREQUENCY_MODE_CODESTREAM_FLAG(jxr_image_t image, int flag)
{
    assert(flag >= 0 && flag <= 1);
    image->header_flags1 &= ~0x40;
    image->header_flags1 |= (flag << 6);

    /* Enable INDEX_TABLE_PRESENT_FLAG */
    if (flag) {
        jxr_set_INDEX_TABLE_PRESENT_FLAG(image, 1);
    }
}

void jxr_set_INDEX_TABLE_PRESENT_FLAG(jxr_image_t image, int flag)
{
    assert(flag >= 0 && flag <= 1);
    image->header_flags1 &= ~0x04;
    image->header_flags1 |= (flag << 2);
}

void jxr_set_ALPHA_IMAGE_PLANE_FLAG(jxr_image_t image, int flag)
{
    assert(flag == 0 || flag == 1);
    if(flag == 1)
        image->header_flags2 |= 0x01;
    else
        image->header_flags2 &= 0xfe;
}

void jxr_set_PROFILE_IDC(jxr_image_t image, int profile_idc)
{
    assert(profile_idc >= 0 && profile_idc <= 255);
    image->profile_idc = (uint8_t) profile_idc;
}

void jxr_set_LEVEL_IDC(jxr_image_t image, int level_idc)
{
    assert(level_idc >= 0 && level_idc <= 255);
    image->level_idc = (uint8_t) level_idc;
}

void jxr_set_LONG_WORD_FLAG(jxr_image_t image, int flag)
{
    assert(flag >= 0 && flag <= 1);
    image->header_flags2 &= ~0x40;
    image->header_flags2 |= (flag << 6);
}

int jxr_test_PROFILE_IDC(jxr_image_t image, int flag)
{
    /* 
    ** flag = 0 - encoder
    ** flag = 1 - decoder
    */
    jxr_bitdepth_t obd = jxr_get_OUTPUT_BITDEPTH(image);
    jxr_output_clr_fmt_t ocf = jxr_get_OUTPUT_CLR_FMT(image);

    unsigned char profile = image->profile_idc;
    /* 
    * For forward compatability 
    * Though only specified profiles are currently applicable, decoder shouldn't reject other profiles.
    */
    if (flag) {
        if (profile <= 44)
            profile = 44;
        else if (profile <= 55)
            profile = 55;
        else if (profile <= 66)
            profile = 66;
        else if (profile <= 111)
            profile = 111;
    }

    switch (profile) {
        case 44:
            if (OVERLAP_INFO(image) >= 2)
                return JXR_EC_BADFORMAT;
            if (LONG_WORD_FLAG(image))
                return JXR_EC_BADFORMAT;
            if (image->num_channels != 1 && image->num_channels != 3)
                return JXR_EC_BADFORMAT;
            if (image->alpha)
                return JXR_EC_BADFORMAT;
            if ((obd == JXR_BD16) || (obd == JXR_BD16S) || (obd == JXR_BD16F) || (obd == JXR_BD32S) || (obd == JXR_BD32F))
                return JXR_EC_BADFORMAT;
            if ((ocf != JXR_OCF_RGB) && (ocf != JXR_OCF_YONLY))
                return JXR_EC_BADFORMAT;
            break;
        case 55:
            if (image->num_channels != 1 && image->num_channels != 3)
                return JXR_EC_BADFORMAT;
            if (image->alpha)
                return JXR_EC_BADFORMAT;
            if ((obd == JXR_BD16F) || (obd == JXR_BD32S) || (obd == JXR_BD32F))
                return JXR_EC_BADFORMAT;
            if ((ocf != JXR_OCF_RGB) && (ocf != JXR_OCF_YONLY))
                return JXR_EC_BADFORMAT;
            break;
        case 66:
            if ((ocf == JXR_OCF_CMYKDIRECT) || (ocf == JXR_OCF_YUV420) || (ocf == JXR_OCF_YUV422) || (ocf == JXR_OCF_YUV444))
                return JXR_EC_BADFORMAT;
            break;
        case 111:
            break;
        default:
            return JXR_EC_BADFORMAT;
            break;
    }

    return JXR_EC_OK;
}

int jxr_test_LEVEL_IDC(jxr_image_t image, int flag)
{
    /* 
    ** flag = 0 - encoder
    ** flag = 1 - decoder
    */
    unsigned tr = image->tile_rows - 1;
    unsigned tc = image->tile_columns - 1;
    uint64_t h = (uint64_t) image->extended_height - 1;
    uint64_t w = (uint64_t) image->extended_width - 1;
    uint64_t n = (uint64_t) image->num_channels;
    uint64_t buf_size = 1;

    unsigned i;
    uint64_t max_tile_width = 0, max_tile_height = 0;
    unsigned *tdim;

    tdim = (image->tile_column_width)?(image->tile_column_width):(image->tile_column_width_input);

    if (tdim) {
      for (i = 0; i < image->tile_columns; i++)
        max_tile_width = (uint64_t) (tdim[i] > max_tile_width ? tdim[i] : max_tile_width);
    } else {
      max_tile_width = image->extended_width >> 4;
    }

    tdim = (image->tile_row_height)?(image->tile_row_height):(image->tile_row_height_input);
    
    if (tdim) {
      for (i = 0; i < image->tile_rows; i++)
        max_tile_height = (uint64_t) (tdim[i] > max_tile_height ? tdim[i] : max_tile_height);
    } else {
      max_tile_height = image->extended_height >> 4;
    }

    /* JNB fix: convert the tile size to a pixel size */
    max_tile_width  *= 16;
    max_tile_height *= 16;

    if (image->alpha)
        n += 1;

    switch (SOURCE_BITDEPTH(image)) {
        case 1: /* BD8 */
            buf_size = n * (h + 1) * (w + 1);
            break;
        case 2: /* BD16 */
        case 3: /* BD16S */
        case 4: /* BD16F */
            buf_size = 2 * n * (h + 1) * (w + 1);
            break;
        case 6: /* BD32S */
        case 7: /* BD32F */
            buf_size = 4 * n * (h + 1) * (w + 1);
            break;
        case 0: /* BD1WHITE1 */
        case 15: /* BD1BLACK1 */
            buf_size = ((h + 8) >> 3) * ((w + 8) >> 3) * 8; 
            break;
        case 8: /* BD5 */
        case 10: /* BD565 */
            buf_size = 2 * (h + 1) * (w + 1);
            break;
        case 9: /* BD10 */
            if (image->output_clr_fmt == JXR_OCF_RGB) 
                buf_size = 4 * (h + 1) * (w + 1);
            else
                buf_size = 2 * n * (h + 1) * (w + 1);
            break;
        default: /* RESERVED */
            return JXR_EC_BADFORMAT;
            break;
    }

    unsigned char level = image->level_idc;
    /* 
    * For forward compatability 
    * Though only specified levels are currently applicable, decoder shouldn't reject other levels.
    */
    if (flag) {
      /* JNB fix: level adjustment was wrong - adjust to the next higher level. */
      if (level <= 4)
	level = 4;
      else if (level <= 8)
	level = 8;
      else if (level <= 16)
	level = 16;
      else if (level <= 32)
	level = 32;
      else if (level <= 64)
	level = 64;
      else if (level <= 128)
	level = 128;
      else
	level = 255;
    }

    switch (level) {
        case 4:
            if ((w >> 10) || (h >> 10) || (tc >> 4) || (tr >> 4) || (max_tile_width >> 10) || (max_tile_height >> 10) || (buf_size >> 22))
                return JXR_EC_BADFORMAT;
            break;
        case 8:
            if ((w >> 11) || (h >> 11) || (tc >> 5) || (tr >> 5) || (max_tile_width >> 11) || (max_tile_height >> 11) || (buf_size >> 24))
                return JXR_EC_BADFORMAT;
            break;
        case 16:
            if ((w >> 12) || (h >> 12) || (tc >> 6) || (tr >> 6) || (max_tile_width >> 12) || (max_tile_height >> 12) || (buf_size >> 26))
                return JXR_EC_BADFORMAT;
            break;
        case 32:
            if ((w >> 13) || (h >> 13) || (tc >> 7) || (tr >> 7) || (max_tile_width >> 12) || (max_tile_height >> 12) || (buf_size >> 28))
                return JXR_EC_BADFORMAT;
            break;
        case 64:
            if ((w >> 14) || (h >> 14) || (tc >> 8) || (tr >> 8) || (max_tile_width >> 12) || (max_tile_height >> 12) || (buf_size >> 30))
                return JXR_EC_BADFORMAT;
            break;
        case 128:
            if ((w >> 16) || (h >> 16) || (tc >> 10) || (tr >> 10) || (max_tile_width >> 12) || (max_tile_height >> 12) || (buf_size >> 32))
                return JXR_EC_BADFORMAT;
            break;
        case 255: /* width and height restriction is 2^32 */
            if ((w >> 32) || (h >> 32) || (tc >> 12) || (tr >> 12) || (max_tile_width >> 32) || (max_tile_height >> 32))
                return JXR_EC_BADFORMAT;
            break;
        default:
            return JXR_EC_BADFORMAT;
            break;
    }

    return JXR_EC_OK;
}

void jxr_set_container_parameters(jxr_image_t image, jxrc_t_pixelFormat pixel_format, unsigned wid, unsigned hei, unsigned separate, unsigned char image_presence, unsigned char alpha_presence, unsigned char alpha) 
{
    image->container_width = wid;
    image->container_height = hei;
    image->container_separate_alpha = separate ? 1 : 0;
    image->container_image_band_presence = image_presence;
    image->container_alpha_band_presence = alpha_presence;
    image->container_current_separate_alpha = alpha ? 1 : 0;

    switch (pixel_format) {
        case JXRC_FMT_24bppRGB:
        case JXRC_FMT_24bppBGR:
        case JXRC_FMT_32bppBGR:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_RGB;
            image->container_nc = 3;
            break;
        case JXRC_FMT_48bppRGB:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_RGB;
            image->container_nc = 3;
            break;
        case JXRC_FMT_48bppRGBFixedPoint:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD16S;
            image->container_color = JXR_OCF_RGB;
            image->container_nc = 3;
            break;
        case JXRC_FMT_48bppRGBHalf:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD16F;
            image->container_color = JXR_OCF_RGB;
            image->container_nc = 3;
            break;
        case JXRC_FMT_96bppRGBFixedPoint:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD32S;
            image->container_color = JXR_OCF_RGB;
            image->container_nc = 3;
            break;
        case JXRC_FMT_64bppRGBFixedPoint:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD16S;
            image->container_color = JXR_OCF_RGB;
            image->container_nc = 3;
            break;
        case JXRC_FMT_64bppRGBHalf:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD16F;
            image->container_color = JXR_OCF_RGB;
            image->container_nc = 3;
            break;
        case JXRC_FMT_128bppRGBFixedPoint:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD32S;
            image->container_color = JXR_OCF_RGB;
            image->container_nc = 3;
            break;
        case JXRC_FMT_128bppRGBFloat:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD32F;
            image->container_color = JXR_OCF_RGB;
            image->container_nc = 3;
            break;
        case JXRC_FMT_32bppBGRA:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_RGB;
            image->container_nc = 4;
            break;
        case JXRC_FMT_64bppRGBA:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_RGB;
            image->container_nc = 4;
            break;
        case JXRC_FMT_64bppRGBAFixedPoint:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD16S;
            image->container_color = JXR_OCF_RGB;
            image->container_nc = 4;
            break;
        case JXRC_FMT_64bppRGBAHalf:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD16F;
            image->container_color = JXR_OCF_RGB;
            image->container_nc = 4;
            break;
        case JXRC_FMT_128bppRGBAFixedPoint:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD32S;
            image->container_color = JXR_OCF_RGB;
            image->container_nc = 4;
            break;
        case JXRC_FMT_128bppRGBAFloat:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD32F;
            image->container_color = JXR_OCF_RGB;
            image->container_nc = 4;
            break;
        case JXRC_FMT_32bppPBGRA:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_RGB;
            image->container_nc = 4;
            break;
        case JXRC_FMT_64bppPRGBA:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_RGB;
            image->container_nc = 4;
            break;
        case JXRC_FMT_128bppPRGBAFloat:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD32F;
            image->container_color = JXR_OCF_RGB;
            image->container_nc = 4;
            break;
        case JXRC_FMT_32bppCMYK:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_CMYK;
            image->container_nc = 4;
            break;
        case JXRC_FMT_40bppCMYKAlpha:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_CMYK;
            image->container_nc = 5;
            break;
        case JXRC_FMT_64bppCMYK:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_CMYK;
            image->container_nc = 4;
            break;
        case JXRC_FMT_80bppCMYKAlpha:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_CMYK;
            image->container_nc = 5;
            break;
        case JXRC_FMT_24bpp3Channels:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 3;
            break;
        case JXRC_FMT_32bpp4Channels:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 4;
            break;
        case JXRC_FMT_40bpp5Channels:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 5;
            break;
        case JXRC_FMT_48bpp6Channels:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 6;
            break;
        case JXRC_FMT_56bpp7Channels:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 7;
            break;
        case JXRC_FMT_64bpp8Channels:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 8;
            break;
        case JXRC_FMT_32bpp3ChannelsAlpha:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 4;
            break;
        case JXRC_FMT_40bpp4ChannelsAlpha:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 5;
            break;
        case JXRC_FMT_48bpp5ChannelsAlpha:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 6;
            break;
        case JXRC_FMT_56bpp6ChannelsAlpha:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 7;
            break;
        case JXRC_FMT_64bpp7ChannelsAlpha:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 8;
            break;
        case JXRC_FMT_72bpp8ChannelsAlpha:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 9;
            break;
        case JXRC_FMT_48bpp3Channels:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 3;
            break;
        case JXRC_FMT_64bpp4Channels:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 4;
            break;
        case JXRC_FMT_80bpp5Channels:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 5;
            break;
        case JXRC_FMT_96bpp6Channels:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 6;
            break;
        case JXRC_FMT_112bpp7Channels:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 7;
            break;
        case JXRC_FMT_128bpp8Channels:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 8;
            break;
        case JXRC_FMT_64bpp3ChannelsAlpha:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 4;
            break;
        case JXRC_FMT_80bpp4ChannelsAlpha:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 5;
            break;
        case JXRC_FMT_96bpp5ChannelsAlpha:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 6;
            break;
        case JXRC_FMT_112bpp6ChannelsAlpha:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 7;
            break;
        case JXRC_FMT_128bpp7ChannelsAlpha:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 8;
            break;
        case JXRC_FMT_144bpp8ChannelsAlpha:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_NCOMPONENT;
            image->container_nc = 9;
            break;
        case JXRC_FMT_8bppGray:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_YONLY;
            image->container_nc = 1;
            break;
        case JXRC_FMT_16bppGray:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_YONLY;
            image->container_nc = 1;
            break;
        case JXRC_FMT_16bppGrayFixedPoint:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD16S;
            image->container_color = JXR_OCF_YONLY;
            image->container_nc = 1;
            break;
        case JXRC_FMT_16bppGrayHalf:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD16F;
            image->container_color = JXR_OCF_YONLY;
            image->container_nc = 1;
            break;
        case JXRC_FMT_32bppGrayFixedPoint:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD32S;
            image->container_color = JXR_OCF_YONLY;
            image->container_nc = 1;
            break;
        case JXRC_FMT_32bppGrayFloat:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD32F;
            image->container_color = JXR_OCF_YONLY;
            image->container_nc = 1;
            break;
        case JXRC_FMT_BlackWhite:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD1WHITE1; /* or JXR_BD1BLACK1 */
            image->container_color = JXR_OCF_YONLY;
            image->container_nc = 1;
            break;
        case JXRC_FMT_16bppBGR555:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD5;
            image->container_color = JXR_OCF_RGB;
            image->container_nc = 3;
            break;
        case JXRC_FMT_16bppBGR565:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD565;
            image->container_color = JXR_OCF_RGB;
            image->container_nc = 3;
            break;
        case JXRC_FMT_32bppBGR101010:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD10;
            image->container_color = JXR_OCF_RGB;
            image->container_nc = 3;
            break;
        case JXRC_FMT_32bppRGBE:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_RGBE;
            image->container_nc = 3;
            break;
        case JXRC_FMT_32bppCMYKDIRECT:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_CMYKDIRECT;
            image->container_nc = 4;
            break;
        case JXRC_FMT_64bppCMYKDIRECT:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_CMYKDIRECT;
            image->container_nc = 4;
            break;
        case JXRC_FMT_40bppCMYKDIRECTAlpha:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_CMYKDIRECT;
            image->container_nc = 5;
            break;
        case JXRC_FMT_80bppCMYKDIRECTAlpha:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_CMYKDIRECT;
            image->container_nc = 5;
            break;
        case JXRC_FMT_12bppYCC420:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_YUV420;
            image->container_nc = 3;
            break;
        case JXRC_FMT_16bppYCC422:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_YUV422;
            image->container_nc = 3;
            break;
        case JXRC_FMT_20bppYCC422:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD10;
            image->container_color = JXR_OCF_YUV422;
            image->container_nc = 3;
            break;
        case JXRC_FMT_32bppYCC422:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_YUV422;
            image->container_nc = 3;
            break;
        case JXRC_FMT_24bppYCC444:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_YUV444;
            image->container_nc = 3;
            break;
        case JXRC_FMT_30bppYCC444:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD10;
            image->container_color = JXR_OCF_YUV444;
            image->container_nc = 3;
            break;
        case JXRC_FMT_48bppYCC444:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_YUV444;
            image->container_nc = 3;
            break;
        case JXRC_FMT_48bppYCC444FixedPoint:
            image->container_alpha = 0;
            image->container_bpc = JXR_BD16S;
            image->container_color = JXR_OCF_YUV444;
            image->container_nc = 3;
            break;
        case JXRC_FMT_20bppYCC420Alpha:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_YUV420;
            image->container_nc = 4;
            break;
        case JXRC_FMT_24bppYCC422Alpha:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_YUV422;
            image->container_nc = 4;
            break;
        case JXRC_FMT_30bppYCC422Alpha:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD10;
            image->container_color = JXR_OCF_YUV422;
            image->container_nc = 4;
            break;
        case JXRC_FMT_48bppYCC422Alpha:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_YUV422;
            image->container_nc = 4;
            break;
        case JXRC_FMT_32bppYCC444Alpha:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD8;
            image->container_color = JXR_OCF_YUV444;
            image->container_nc = 4;
            break;
        case JXRC_FMT_40bppYCC444Alpha:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD10;
            image->container_color = JXR_OCF_YUV444;
            image->container_nc = 4;
            break;
        case JXRC_FMT_64bppYCC444Alpha:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD16;
            image->container_color = JXR_OCF_YUV444;
            image->container_nc = 4;
            break;
        case JXRC_FMT_64bppYCC444AlphaFixedPoint:
            image->container_alpha = 1;
            image->container_bpc = JXR_BD16S;
            image->container_color = JXR_OCF_YUV444;
            image->container_nc = 4;
            break;
        default:
            /* all Guids listed above*/
            assert(0); 
            break;
    }
}

void jxr_set_NUM_VER_TILES_MINUS1(jxr_image_t image, unsigned num)
{
    assert( num > 0 && num < 4097 );
    image->tile_columns = num;

    if (num > 1)
        jxr_set_TILING_FLAG(image, 1);
}

void jxr_set_TILE_WIDTH_IN_MB(jxr_image_t image, unsigned* list)
{
  if (list == 0 || list[0] == 0) {
    unsigned idx, total_width = 0;
    image->tile_column_width = calloc(2*image->tile_columns, sizeof(unsigned));
    image->tile_column_position = image->tile_column_width + image->tile_columns;
    for ( idx = 0 ; idx < image->tile_columns - 1 ; idx++ ) {
      image->tile_column_width[idx] = (image->extended_width >> 4) / image->tile_columns;
      image->tile_column_position[idx] = total_width;
      total_width += image->tile_column_width[idx];
    }
    image->tile_column_width[image->tile_columns - 1] = (image->extended_width >> 4) - total_width;
    image->tile_column_position[image->tile_columns - 1] = total_width;
  } else {
    image->tile_column_width_input = list;
    image->tile_column_width       = NULL;
    image->tile_column_position    = NULL;
  }
}

void jxr_set_NUM_HOR_TILES_MINUS1(jxr_image_t image, unsigned num)
{
    assert( num > 0 && num < 4097 );
    image->tile_rows = num;

    if (num > 1)
        jxr_set_TILING_FLAG(image, 1);
}

void jxr_set_TILE_HEIGHT_IN_MB(jxr_image_t image, unsigned* list)
{
  if (list == 0 || list[0] == 0) { 
    unsigned idx, total_height = 0;
    image->tile_row_height     = calloc(2*image->tile_rows, sizeof(unsigned));
    image->tile_row_position   = image->tile_row_height + image->tile_rows;

    total_height = 0;
    for ( idx = 0 ; idx < image->tile_rows - 1 ; idx++ ) {
      image->tile_row_height[idx] = (image->extended_height >> 4) / image->tile_rows;
      image->tile_row_position[idx] = total_height;
      total_height += image->tile_row_height[idx];
    }
    image->tile_row_height[image->tile_rows - 1] = (image->extended_height >> 4) - total_height;
    image->tile_row_position[image->tile_rows - 1] = total_height;
  } else {
    image->tile_row_height_input = list;
    image->tile_row_height       = NULL;
    image->tile_row_position     = NULL;
  }
}

void jxr_set_TILING_FLAG(jxr_image_t image, int flag)
{
    assert(flag >= 0 && flag <= 1);
    image->header_flags1 &= ~0x80;
    image->header_flags1 |= (flag << 7);

    /* Enable INDEX_TABLE_PRESENT_FLAG */
    if (flag) {
        jxr_set_INDEX_TABLE_PRESENT_FLAG(image, 1);
    }
}

void jxr_set_TRIM_FLEXBITS(jxr_image_t image, int trim_flexbits)
{
    assert(trim_flexbits >= 0 && trim_flexbits < 16);
    image->trim_flexbits = trim_flexbits;
}

void jxr_set_OVERLAP_FILTER(jxr_image_t image, int flag)
{
    assert(flag >= 0 && flag <= 3);
    image->header_flags1 &= ~0x03;
    image->header_flags1 |= flag&0x03;
}

void jxr_set_DISABLE_TILE_OVERLAP(jxr_image_t image, int disable_tile_overlap)
{
    image->disableTileOverlapFlag = disable_tile_overlap;
}

void jxr_set_QP_LOSSLESS(jxr_image_t image)
{
    unsigned char q[MAX_CHANNELS];
    int idx;
    for (idx = 0 ; idx < MAX_CHANNELS ; idx += 1)
        q[idx] = 0;

    jxr_set_QP_INDEPENDENT(image, q);

    if (image->num_channels == 1) {
        image->dc_component_mode = JXR_CM_UNIFORM;
        image->lp_component_mode = JXR_CM_UNIFORM;
        image->hp_component_mode = JXR_CM_UNIFORM;
    } else if (image->num_channels == 3) {
        image->dc_component_mode = JXR_CM_SEPARATE;
        image->lp_component_mode = JXR_CM_SEPARATE;
        image->hp_component_mode = JXR_CM_SEPARATE;
    }
}

void jxr_set_QP_INDEPENDENT(jxr_image_t image, unsigned char*quant_per_channel)
{
    /*
    * SCALED_FLAG MUST be set false if lossless compressing.
    * SCALED_FLAG SHOULD be true otherwise.
    *
    * So assume that we are setting up for lossless compression
    * until we find a QP flag that indicates otherwlse. If that
    * happens, turn SCALED_FLAG on.
    */

    image->scaled_flag = 0;

    if (image->bands_present != JXR_BP_ALL)
        image->scaled_flag = 1;

    if (image->num_channels == 1) {
        image->dc_component_mode = JXR_CM_UNIFORM;
        image->lp_component_mode = JXR_CM_UNIFORM;
        image->hp_component_mode = JXR_CM_UNIFORM;
    } else {
        image->dc_component_mode = JXR_CM_INDEPENDENT;
        image->lp_component_mode = JXR_CM_INDEPENDENT;
        image->hp_component_mode = JXR_CM_INDEPENDENT;
    }

    image->dc_frame_uniform = 1;
    image->lp_frame_uniform = 1;
    image->hp_frame_uniform = 1;
    image->lp_use_dc_qp = 0;
    image->hp_use_lp_qp = 0;

    image->num_lp_qps = 1;
    image->num_hp_qps = 1;

    int idx;
    for (idx = 0 ; idx < image->num_channels ; idx += 1) {
        if (quant_per_channel[idx] >= 1)
            image->scaled_flag = 1;

        image->dc_quant_ch[idx] = quant_per_channel[idx];
        image->lp_quant_ch[idx][0] = quant_per_channel[idx];
        image->hp_quant_ch[idx][0] = quant_per_channel[idx];
    }
}

void jxr_set_QP_UNIFORM(jxr_image_t image, unsigned char quant)
{
    image->scaled_flag = 0;

    image->dc_component_mode = JXR_CM_UNIFORM;
    image->lp_component_mode = JXR_CM_UNIFORM;
    image->hp_component_mode = JXR_CM_UNIFORM;

    image->dc_frame_uniform = 1;
    image->lp_frame_uniform = 1;
    image->hp_frame_uniform = 1;
    image->lp_use_dc_qp = 0;
    image->hp_use_lp_qp = 0;

    image->num_lp_qps = 1;
    image->num_hp_qps = 1;

    if (quant >= 1)
        image->scaled_flag = 1;
    if (image->bands_present != JXR_BP_ALL)
        image->scaled_flag = 1;

    int idx;
    for (idx = 0 ; idx < image->num_channels ; idx += 1) {
        image->dc_quant_ch[idx] = quant;
        image->lp_quant_ch[idx][0] = quant;
        image->hp_quant_ch[idx][0] = quant;
    }
}

void jxr_set_QP_SEPARATE(jxr_image_t image, unsigned char*quant_per_channel)
{
    /*
    * SCALED_FLAG MUST be set false if lossless compressing.
    * SCALED_FLAG SHOULD be true otherwise.
    *
    * So assume that we are setting up for lossless compression
    * until we find a QP flag that indicates otherwlse. If that
    * happens, turn SCALED_FLAG on.
    */
    image->scaled_flag = 0;

    if (image->bands_present != JXR_BP_ALL)
        image->scaled_flag = 1;

    /* XXXX Only thought out how to handle 1 channel. */
    assert(image->num_channels >= 3);

    image->dc_component_mode = JXR_CM_SEPARATE;
    image->lp_component_mode = JXR_CM_SEPARATE;
    image->hp_component_mode = JXR_CM_SEPARATE;

    image->dc_frame_uniform = 1;
    image->lp_frame_uniform = 1;
    image->hp_frame_uniform = 1;
    image->lp_use_dc_qp = 0;
    image->hp_use_lp_qp = 0;

    if (quant_per_channel[0] >= 1)
        image->scaled_flag = 1;

    image->dc_quant_ch[0] = quant_per_channel[0];
    image->lp_quant_ch[0][0] = quant_per_channel[0];
    image->hp_quant_ch[0][0] = quant_per_channel[0];

    if (quant_per_channel[1] >= 1)
        image->scaled_flag = 1;

    int ch;
    for (ch = 1 ; ch < image->num_channels ; ch += 1) {
        image->dc_quant_ch[ch] = quant_per_channel[1];
        image->lp_quant_ch[ch][0] = quant_per_channel[1];
        image->hp_quant_ch[ch][0] = quant_per_channel[1];
    }
}

void jxr_set_QP_DISTRIBUTED(jxr_image_t image, struct jxr_tile_qp*qp)
{
    image->dc_frame_uniform = 0;
    image->lp_frame_uniform = 0;
    image->hp_frame_uniform = 0;
    image->lp_use_dc_qp = 0;
    image->hp_use_lp_qp = 0;

    image->tile_quant = qp;
}

void jxr_set_pixel_format(jxr_image_t image, jxrc_t_pixelFormat pixelFormat)
{
    image->ePixelFormat = pixelFormat;    
}

void jxr_set_SHIFT_BITS(jxr_image_t image, unsigned char shift_bits)
{
    image->shift_bits = shift_bits;
}

void jxr_set_FLOAT(jxr_image_t image, unsigned char len_mantissa, char exp_bias)
{
    image->len_mantissa = len_mantissa;
    image->exp_bias = exp_bias;
}

/*
* $Log: api.c,v $
* Revision 1.14  2011-11-11 17:13:50  thor
* Fixed a memory bug, fixed padding channel on encoding bug.
* Fixed window sizes (again).
*
* Revision 1.13  2011-11-08 20:17:29  thor
* Merged a couple of fixes from the JNB.
*
* Revision 1.12  2011-04-28 08:45:42  thor
* Fixed compiler warnings, ported to gcc 4.4, removed obsolete files.
*
* Revision 1.11  2011-03-04 12:12:12  thor
* Bumped to 1.16. Fixed RGB-YOnly handling, including the handling of
* YOnly for which a new -f flag has been added.
*
* Revision 1.10  2011-02-26 10:24:39  thor
* Fixed bugs for alpha and separate alpha.
*
* Revision 1.9  2010-06-19 11:48:35  thor
* Fixed memory leaks.
*
* Revision 1.8  2010-05-13 16:30:03  thor
* Added options to set the chroma centering. Fixed writing of BGR565.
* Made the "-p" output option nicer.
*
* Revision 1.7  2010-03-31 07:50:58  thor
* Replaced by the latest MS version.
*
* Revision 1.20 2009/05/29 12:00:00 microsoft
* Reference Software v1.6 updates.
*
* Revision 1.19 2009/04/13 12:00:00 microsoft
* Reference Software v1.5 updates.
*
* Revision 1.18 2008/03/21 18:05:53 steve
* Proper CMYK formatting on input.
*
* Revision 1.17 2008/03/06 02:05:48 steve
* Distributed quantization
*
* Revision 1.16 2008/03/05 04:04:30 steve
* Clarify constraints on USE_DC_QP in image plane header.
*
* Revision 1.15 2008/03/05 01:27:15 steve
* QP_UNIFORM may use USE_DC_LP optionally.
*
* Revision 1.14 2008/03/05 00:31:17 steve
* Handle UNIFORM/IMAGEPLANE_UNIFORM compression.
*
* Revision 1.13 2008/03/04 23:01:28 steve
* Cleanup QP API in preparation for distributed QP
*
* Revision 1.12 2008/03/02 19:56:27 steve
* Infrastructure to read write BD16 files.
*
* Revision 1.11 2008/02/26 23:52:44 steve
* Remove ident for MS compilers.
*
* Revision 1.10 2008/02/01 22:49:52 steve
* Handle compress of YUV444 color DCONLY
*
* Revision 1.9 2008/01/08 01:06:20 steve
* Add first pass overlap filtering.
*
* Revision 1.8 2008/01/07 16:19:10 steve
* Properly configure TRIM_FLEXBITS_FLAG bit.
*
* Revision 1.7 2008/01/06 01:29:28 steve
* Add support for TRIM_FLEXBITS in compression.
*
* Revision 1.6 2008/01/04 17:07:35 steve
* API interface for setting QP values.
*
* Revision 1.5 2007/11/30 01:50:58 steve
* Compression of DCONLY GRAY.
*
* Revision 1.4 2007/11/26 01:47:15 steve
* Add copyright notices per MS request.
*
* Revision 1.3 2007/11/08 02:52:32 steve
* Some progress in some encoding infrastructure.
*
* Revision 1.2 2007/09/08 01:01:43 steve
* YUV444 color parses properly.
*
* Revision 1.1 2007/06/06 17:19:12 steve
* Introduce to CVS.
*
*/

