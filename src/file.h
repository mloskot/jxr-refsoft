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
#pragma comment (user,"$Id: file.h,v 1.10 2011-11-22 12:30:15 thor Exp $")
#endif

void *open_input_file(const char *name, const raw_info *raw_info_t, int *alpha_mode, int *padded_format);
void *open_output_file(const char *name,int write_padding_channel);
void get_file_parameters(void *handle, int *wid, int *hei, int *ncomp, int *bpi, short *sf, short *photometric, int *padBytes,int *ycc_format,int *premultiplied);
void read_file(jxr_image_t image, int mx, int my, int *data);
void write_file(jxr_image_t image, int mx, int my, int *data);
void write_file_combine_primary_alpha(jxr_image_t image, FILE *fpPrimary, FILE *fpAlpha);
void separate_primary_alpha(jxr_image_t image, void *input_handle, char *path_out, char * path_primary, char *path_alpha, jxr_container_t container);
void close_file(void *handle);
void set_ncomp(void *input_handle, int ncomp);

/*
* $Log: file.h,v $
* Revision 1.10  2011-11-22 12:30:15  thor
* The default color space for the 4 channel generic pixel format
* is now CMYK. The premultiplied alpha flag is now set correctly on
* decoding, even for images with a separate alpha plane, and it is
* honoured correctly on encoding from tif files.
*
* Revision 1.9  2011-04-28 08:45:42  thor
* Fixed compiler warnings, ported to gcc 4.4, removed obsolete files.
*
* Revision 1.8  2010-06-17 22:02:14  thor
* Fixed (partially) the YCC reader. Added type recognition for TIFF.
*
* Revision 1.7  2010-05-13 16:30:03  thor
* Added options to set the chroma centering. Fixed writing of BGR565.
* Made the "-p" output option nicer.
*
* Revision 1.6  2010-03-31 07:50:58  thor
* Replaced by the latest MS version.
*
* Revision 1.4 2009/05/29 12:00:00 microsoft
* Reference Software v1.6 updates.
*
* Revision 1.3 2009/04/13 12:00:00 microsoft
* Reference Software v1.5 updates.
*
* Revision 1.2 2008/02/26 23:52:44 steve
* Remove ident for MS compilers.
*
* Revision 1.1 2008/01/19 02:30:46 rick
* Re-implement and extend file interface.
*
*/

