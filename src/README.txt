
JPEG XR reference software 1.32 (November 2011)
-------------------------------------------------------------------

Only minor changes again:

- The TIF tag that indicates whether an alpha channel is premultiplied
or not was not honoured by the previous releases, this has been fixed,
and while the "-M" flag workaround still does work, the codec can
now perfectly detect images with premultiplied alpha correctly.
- Setting the premultiplied flag correctly on decoding did not work
for images with a separate alpha plane, fixed.
- The default photometric tag for four channel generic output is now
always CMYK.

JPEG XR reference software 1.31 (November 2011)
-------------------------------------------------------------------

This is only a minor correction of version 1.30 which fixes one
issue of the previous release:

- For 20bpp 422 YUV, the previous software did use the wrong output
color transformation, and hence caused .
- The TIFF output formatting and input parsing for 20bpp 422 YUV 
was broken. This release fixes it.

JPEG XR reference software 1.30 (November 2011)
-------------------------------------------------------------------

Several issues were addressed in this release, namely:

- Specifying window parameters with "-s" did not work and created
incorrect results and inconsistencies between the container and
the codestream format. Specifically, it remains unclear how this
command line flag is supposed to work. It currently clips of the
indicated boundaries from the input image, though this does not mean
that the window bounds in the codestream actually reflect these
parameters. This is because the window sizes in the codestream must
be selected such that the extended image = image + boundaries has
dimensions that are divisible by 16.
- The previous version of the reference software automatically clipped
the extended image size such that the bottom and right border are
between 0 and 15. The current version no longer does that, but 
introduces a check whether the window dimensions are correct.
- The reference software now includes the latest changes of the
codestream specifications and honours now in the 555,565 and 101010
modes the "swap red and blue" flag in the decoder. The encoder writes
images now according to the modified coding specifications. Raw 
input and output is now expected in RGB order with R in the MSB.
All formats, including raw formats, are now RGB formats with R in
the MSB and B in the LSB.
- Additional checks for profile conformance were integrated into
the code, specifically tile dimensions are now checked correctly.
- Fixed a bug in padding the image data for internal 422 and 420
color formats if the image dimension was not a multiple of 16.
- Quantizer DC values were not correct in the SEPARATE mode if the
image contained more than three channels, fixed.
- The software did not decode images in 565 mode correctly if the
internal color format was set to YONLY.
- If the software now finds a tile with an invalid TILE_STARTCODE,
it will consistently report an error in all cases now.
- The software is more careful when attempting to read the profile
information; specifically, cases where less than four additional
bytes are present now no longer attempt to read the profile.
- The code no longer attempts to read flexbits in NOFLEXBITS mode.
- CMYK encoding no longer automatically selects the CMYKDIRECT mode,
instead the user must now request this mode with the "-M" flag.
- Fixed a memory management problem where the tile dimensions provided
by the user were tried to be released by the code.
- Fixed an issue on encoding RGB images with padding channels where the
padding channel was actually, due to an oversight, clipped off twice and
hence left an unsuable image.
- The reference software now sets the (new) premultiplied alpha flag
in the image header when appropriate.
- The reference software does not detect whether a TIFF alpha plane is
premultiplied or not. Please use the "-M" command line option to
indicate this.


JPEG XR reference software 1.20 (April 2011)
-------------------------------------------------------------------

This release fixes missing initializations in the quantizer; the
previous releases also occasionally used the wrong quantizer
tables for some quantizer configurations. This has been fixed.


JPEG XR reference software 1.19 (March 2011)
-------------------------------------------------------------------

Fixed one superfluous assert that prevented the usage of YUV420
internal color format with YUV444 as external format.

JPEG XR reference software 1.18 (March 2011)
-------------------------------------------------------------------

The previous release did not allow the selection of YUV420 or
YUV422 as internal color format if the external color format is
YUV444 and raw encoding was selected.

BGR555,565 and 101010 encoding was broken with the raw format.
RGBE was likely broken, too.


JPEG XR reference software 1.17 (March 2011)
-------------------------------------------------------------------

This release includes more fixes concerning YOnly support and
subsampling support, and includes a fix for CMYKA. In specific:

*) YUV444 output with YOnly was not supported correctly. Including
an interleaved alpha channel did not work properly.

*) The external color format of YUV444, combined with an
internal YUV420 or YUV422 representation did not work. The reference
software did not upsample the U and V planes on decoding, and did
not downsample the planes on encoding. Note that for an external
YUV420 or YUV422 format, upsampling and downsampling is *intentionally*
not included. This only happens if the external and internal color
formats differ in respect to their subsampling factors.

*) Tiff encoding from CMYKA did not work. The reference software now
defaults to the CMYKDirect modes if the source file is TIFF, unless
this choice is overridden with "-M". Note that this is the one and only
case where the external color space cannot be derived completely from
the TIFF file.

NOTE: TIFF support for CMYK and YUV encodings requires a TIFF file
using a plane-separate representation for all components; neither 
striping nor tiles are supported. The software will may not warn for
incorrectly organized input files, so be careful.


JPEG XR reference software 1.16 (March 2011)
-------------------------------------------------------------------

This release fixes proper handling of RGB type pixel formats with
a YOnly codestream underneath. Previous releases decoded such images
as one-component images - or if an alpha channel was present - as
a two component image with the second channel being the alpha channel.

Additionally, the encoder includes now a YOnly flag, to be specified
with -f YOnly, which chooses this color format for the codestream.
Due to limitations of the architecture, it then encodes the *red*
channel of the input RGB image as grey channel. It does not implement
a color transformation from RGB to luma as it should.


JPEG XR reference software 1.15 (October 2010)
-------------------------------------------------------------------

When compressing 32bpp images, the software did not set any pre-
shift (pre scaler), causing potential out-of-range errors in the
actual coding back-end. The pre-shift for 32bpp images is now set
to ten, similar to that of the DPK.

YUV420 and YUV422 explicitly now disables the 444 subsampling
command line argument.

In case the codestream container contained an odd number of bytes,
the reference software correctly includes a pad-byte, but it set
the IFD entry size such that this pad-byte is formally part of the
codestream, which it is not. Fixed.

For separate alpha channels, an additional quantizer parameter in
the -q command line option, is now used to specify the alpha
quantization. Note that this parameter is not used for interleaved
alpha where the software simply copies the image alpha over to the
alpha plane. 


JPEG XR reference software 1.14 (September 2010)
-------------------------------------------------------------------

This release fixes the handling of images with the BlackOnWhite
photometric setting and now writes the correct photometric
tag on decoding.

JPEG XR reference software 1.13 (September 2010)
-------------------------------------------------------------------

The image extension to the next block boundary for images whose
dimensions are not multiples of the block sizes were incorrect
and did not took subsampling into account. This release fixes
the problem.

JPEG XR reference software 1.12 (September 2010)
-------------------------------------------------------------------

On writing raw output, the software did not preserve the padding
channel, fixed.
Note that when writing tiff output, padding is by default not
reproduced because the tiff container only represents image data,
not its memory layout. To enforce writing tiff data with a dummy
padding channel, use the command line option "-p".

JPEG XR reference software 1.11 (August 2010)
-------------------------------------------------------------------

Previous versions of the CMYKDIRECT pixel formats did not implement
the (rather trivial, though) transposition of components correctly,
this release fixes it.

This release also supports CMYKDIRECT pixel formats through tiff
input. For that, specify -M 27 or -M 28 on the command line as 
additional parameters - even though the input is not raw.


JPEG XR reference software 1.10 (July 2010)
-------------------------------------------------------------------

This version adds tiff support for most pixel formats, namely:
     BGR in 555, 565 and 101010,
     YCC with 444,422 and 420 subsampling,
     CMYK and CMYKA.

Further modifications made for tiff is the proper reproduction of
the alpha channel, including information whether it requires
premultiplied components or not, proper ordering of the tiff tags
and inclusion of a tiff resolution.

This release now allows all but the RGBE pixel format to be
decompressed and represented as tiff, even though some of the generated
tiff formats might be quite unusual and may be unsupported by the
average tiff reader. The RGBE pixel format can now, alternatively,
represented in the .rgbe format defined by Greg Ward Larson, see:

http://www.graphics.cornell.edu/online/formats/rgbe/


The following encoder options have been added:

-S selects the chroma centering. It expects two parameters, separated
by a colon ":", the first the horizontal chroma position, follwed by
the vertical chroma positon. Both parameters can range from 0 to 4,
where zero is co-sited, and 4 is centered.

-y turns the encoder into a stripe-by-stripe encoding algorithm. Note that
this is an experimental switch.

The following decoder options have been added:

-p preserves the padding channel on tiff images. If -p is not given, the
padding channel is not preserved and only raw image data is written for
tiff files. If -p is given, a dummy padding channel is created for those
pixel formats that include a padding channel.

-y enables the experimental stripe decoding mode. In this mode, images
are decoded stripe by stripe and data is returned as soon as possible.

JPEG XR reference software 1.9 (May 2010)
-------------------------------------------------------------------

This version addresses a couple of issues on the decoder and adds more encoder options:

1.   The encoder supports a new command line option -S that allows the user to
     set the chroma centering for YUV420 and YUV422 files. It takes one single
     argument in the form of x:y where x and y are numbers between 0 and 4 specifying
     the relative location of the chroma samples relative to luma. 0:0 is the default
     and denotes co-located chroma and luma samples.
2.   On encoding the tiff photometry tag MIN_IS_WHITE was misinterpreted and bilevel
     images were encoded inverted.
3.   On decoding, the TIFF tags were ordered incorrectly, violating the tiff specs.
4.   On decoding, BGR packed formats were unsupported in TIFF form. The 1.9 release
     adds support for them. Note that the *encoder* does not support these fields.
5.   By default, the decoder no longer writes a padding channel for TIFF. If you want pad bytes
     represented in the TIFF output, specify the "-p" option.
6.   When writing padding channels, the older releases did not write the proper bitdepths for
     all channels and missed the padding channel.
7.   The sample format tag was written incorrectly and contained only a scalar value though
     it should be a vector of the dimension given by the samples per pixel.
8.   When padding channels are included on decoding, the TIFF EXTRASAMPLES tag is now used
     to announce them.

This software may or may not have issues in ordering BGR channels correctly when writing
them out to TIFF. This issue is currently under investigation.


JPEG XR reference software 1.8 (September 2009)
-------------------------------------------------------------------

This version constitutes changes to the encoder only. Two bugs were fixed:
1.	Fix a crashing bug for tiling.
2.	Fix a bug in the overlap operators. Overlap code was using row indices relative to the tile instead of relative to the image.

No changes were made to the usage text.

JPEG XR reference software 1.7 (July 2009)
-------------------------------------------------------------------

This version constitutes a change to the encoder only. A bug related to using a detailed quantization information file 
without specifying tile sizes (or without tiling) was fixed.

No changes were made to the usage text.

JPEG XR reference software 1.6 (29 May 2009)
-------------------------------------------------------------------

This version constitutes several improvements to both the sample 
encoder and reference decoder.


Changes related to the decoder are as follows:
1.	Add support for upsampling for non zero values of CHROMA_CENTERING_X and CHROMA_CENTERING_Y
2.	Enhance container handling to test conformance relationship of tag based container format and codestream
3.	Add fix by Thomas Richter for issue caused by some compilers treating unsigned long as 64 bit data
4.	Fix a bug in decoding of interleaved YCC/CMYKDIRECT formats

Changes related to the encoder are as follows:
1.	Add support for separate alpha image plane
2.	Add support for YCC OUTPUT_CLR_FMT (YUV444, YUV422, YUV420)
3.	Add support for CMYKDIRECT
4.	Add support for WINDOWING_FLAG
5.	Add support for the following pixel formats - 24bppBGR, 32bppBGRA, 32bppBGR, 32bppPGBGRA, 64bppPRGBA, 128bppPRGBA
6.	Add code to write IFD tags in the image container. The code for optional tags are in a macro which is disabled by default.
7.      Fix a bug related to encoding 40bppCMYKA and 80bppCMYKA using interleaved alpha mode



New usage text:
Usage: Debug\jpegxr.exe <flags> <input_file.jxr>
Usage: e:\rjsourcedepot\multimedia\dmd\CodecDSP\jxrRefCode\current\Debug\jpegxr.exe <flags> <input_file.jxr>
  DECODER FLAGS:
    [-o <path>] [-w] [-P 44|55|66|111] [-L 4|8|16|32|64|128|255]

        -o: selects output file name (.raw/.tif/.pnm)
            (PNM output can be used only for 24bpp RGB and 8bpp gray Output)
            (TIF output can be used for all formats except the following:
             N channels, BGR, RGBE, YCC, CMYKDirect & Premultiplied RGB)
            (RAW output can be used for all formats)
        -w: tests whether LONG_WORD_FLAG needs to be equal to TRUE
             (will still decode the image)
        -P: selects the maximum accepted profile value
             (44:Sub-Baseline|55:Baseline|66:Main|111:Advanced)
        -L: selects the maximum accepted level value
             (4|8|16|32|64|128)

  ENCODER FLAGS: (Temporary (.tmp) files may be used in encoding)
    -c [-o <path>] [-b ALL|NOFLEXBITS|NOHIGHPASS|DCONLY] [-a 0|1|2] [-p]
    [-f YUV444|YUV422|YUV420] [-F bits] [-h] [-m] [-l 0|1|2]
    [-q q1[:q2[:q3]]] [-Q <path>] [-d] [-w] [-U rows:columns]
    [-C width1[:width2>[:width3...]]] [-R height1[:height2[:height3...]]]
    [-P 44|55|66|111] [-L 4|8|16|32|64|128|255] [-s top|left|bottom|right]
    [-r -W width -H height -M 3|4|...|18 [-B 8|16]]

        -c: selects encoding instead of decoding
             this flag is necessary for encoding
        -o: selects the output file name (.jxr)
        -b: selects the bands to encode
             (ALL<Default>|NOFLEXBITS|NOHIGHPASS|DCONLY)
        -a: selects encoder alpha mode
             (0: no alpha|1:interleaved alpha|2:separate alpha)
             Default: For tif input files, based on the information in the
             PhotometricInterpretation and SamplesPerPixel tags in the container,
             the encoder chooses an input pixel format. If the number
             of components is 4 and photometric is 2, RGBA input is inferred and
             the encoder assumes the presence of an alpha channel while encoding.
             If the number of components is 5 and photometric is 5, CMYKA input is
             inferred. In both these cases, the encoder infers a pixel format with
             an alpha channel. In such cases, the default alpha encoder mode is 2.
             For raw input files, when the -M parameter specified by the user is
             9, 10, 11, 12 13, 14, 23, 24, 25, 26 or 28,
             the default alpha encoder mode is 2
             In all other cases, the default alpha encoder mode is 0.
        -p:  selects an input pixel format with a padding channel
             With tif input, when the encoder infers that the input file has an
             alpha channel (see explanation for -a), this flag causes the encoder
             to treat the alpha channel as a padding channel instead
        -f: selects the internal color format
             (YUV444<Default>|YUV422|YUV420)
        -F: selects the number of flexbits to trim
             (0<default> - 15)
        -h: selects hard tile boundaries
             (soft tile boundaries by default)
        -m: encode in frequency order codestream
             (spatial order by default)
        -l: selects the overlapped block filtering
             (0:off|1:HP only<Default>|2:all)

        -q: sets the quantization values separately, or one per band
             (0<default, lossless> - 255)
        -Q: specifies a file containing detailed quantization information
             See sample.qp
        -d: selects quantization for U/V channels derived from Y channel

        -U: selects uniform tile sizes
        -C: selects the number of tile columns and the width of each
        -R: selects the number of tile rows and the height of each

        -w: sets LONG_WORD_FLAG equal to FALSE
        -P: selects the profile value
             (44:Sub-Baseline|55:Baseline|66:Main|111:Advanced)
        -L: selects the level value
             (4|8|16|32|64|128)
        -s: sets the top, left, bottom, and right margins

        -r: selects encoding with RAW images
             must also specify -W, -H and -M, optional -B
        -W: RAW image width when encoding with RAW images
        -H: RAW image height when encoding with RAW images
        -M: RAW image format when encoding with RAW images
            3: 3-channel
            4: 4-channel
            5: 5-channel
            6: 6-channel
            7: 7-channel
            8: 8-channel
            9: 3-channel Alpha
            10: 4-channel Alpha
            11: 5-channel Alpha
            12: 6-channel Alpha
            13: 7-channel Alpha
            14: 8-channel Alpha
            15: 32bppRGBE
            16: 16bppBGR555
            17: 16bppBGR565
            18: 32bppBGR101010
            19: YCC420
            20: YCC422
            21: YCC444
            22: YCC444 Fixed Point
            23: YCC420 Alpha
            24: YCC422 Alpha
            25: YCC444 Alpha
            26: YCC444 Fixed Point Alpha
            27: CMYKDIRECT
            28: CMYKDIRECT Alpha
            29: 24bppBGR
            30: 32bppBGR
            31: 32bppBGRA
            32: 32bppPBGRA
            33: 64bppPRGBA
            34: 128bppPRGBAFloat
        -B: RAW image bit/channel when encoding with RAW images
            8: 8-bit/channel (default)
            10: 10-bit/channel
            16: 16-bit/channel


Raw File Description:
The raw  file makes it possible to store the results of the output formatting process . The raw file output consists of either interleaved or sequential data from each of the channels in the image, without any header information. Buffers are stored in a raster scan order.
If OUTPUT_CLR_FMT is equal to YUV_420, YUV_422, YUV_444 or CMYKDirect, the buffer for each channel is stored sequentially. For example, when OUTPUT_CLR_FMT equals YUV_444 all Y samples are stored in the output file, followed by all U samples, followed by all V samples. If OUTPUT_CLR_FMT is equal to YUV422, half as many bytes used for the Y channel will be used to store the U and V channels. Otherwise, uf OUTPUT_CLR_FMT is equal to YUV420, one quarter as many bytes as used for the Y channel will be used to store the U and V channels. If OUTPUT_CLR_FMT is equal to YUV444 or YUV422 and OUTPUT_BITDEPTH is equal to BD10, 2 bytes are used per sample, and the 10 bits are stored in the LSBs of each 2 byte pair. Samples decoded from the alpha image plane (when present), are concatenated with the output obtained from the primary image.
Otherwise, if OUTPUT_CLR_FMT is not equal to YUV_420, YUV_422, YUV_444 or CMYKDirect, the raw file output consists of interleaved data from each channel. The number of bytes used to store each sample is contingent upon the value of OUTPUT_BITDEPTH and whether the format is a packed output format or not. The samples decoded from the image alpha plane (when present), are interleaved with the samples decoded from the primary image.


JPEG XR reference software 1.5 (13 Apr 2009)
-------------------------------------------------------------------

This version constitutes several improvements to both the sample 
encoder and reference decoder.

Decoder changes:
-Add support for fixed point formats
-Add support for floating point formats
-Add support for half formats
-Add support for separate alpha image plane
-Add support for interleaved alpha image plane
-Add support for YCC OUTPUT_CLR_FMT (YUV444, YUV422, YUV420)
-Add support for CMYKDIRECT
-Add support for n-Component formats
-Add support for RGBE format
-Add support for packed output formats
-Add support for LONG_WORD_FLAG
-Add support for Profiles/Levels
-Fix a bug related to USE_DC_QP_FLAG and USE_LP_QP_FLAG
-Add support for raw image buffer output file format
-Add support for WINDOWING_FLAG
-Fix a bug with respect to the index table and spatial ordered
 bitstreams
-Improve usage text

Encoder changes:
-Fix a bug with CMYK encoding
-Add support for frequency mode
-Add support for tiling
-Add support for fixed point formats
-Add support for floating point formats
-Add support for half formats
-Add support for interleaved alpha image plane
-Add support for n-Component formats
-Add support for RGBE format
-Add support for packed output formats
-Add support for LONG_WORD_FLAG
-Add support for Profiles/Levels
-Improve usage text

New usage text:
Usage: Release\jpegxr.exe <flags> <input_file.jxr>
  DECODER FLAGS:
    [-o <path>] [-w] [-P 44|55|66|111] [-L 4|8|16|32|64|128|255]

        -o: selects output file name (.raw/.tif/.pnm)
        -w: tests whether LONG_WORD_FLAG needs to be equal to TRUE
             (will still decode the image)
        -P: selects the maximum accepted profile value
             (44:Sub-Baseline|55:Baseline|66:Main|111:Advanced)
        -L: selects the maximum accepted level value
             (4|8|16|32|64|128)

  ENCODER FLAGS: (Temporary (.tmp) files may be used in encoding)
    -c [-o <path>] [-b ALL|NOFLEXBITS|NOHIGHPASS|DCONLY] [-a 0|1|2]
    [-f YUV444|YUV422|YUV420] [-F bits] [-h] [-m] [-l 0|1|2]
    [-q q1[:q2[:q3]]] [-Q <path>] [-d] [-w] [-U rows:columns]
    [-C width1[:width2>[:width3...]]] [-R height1[:height2[:height3...]]]
    [-P 44|55|66|111] [-L 4|8|16|32|64|128|255]
    [-r -W width -H height -M 3|4|5|6|7|8|15|16|17|18 [-B 8|16]]

        -c: selects encoding instead of decoding
             this flag is necessary for encoding
        -o: selects the output file name (.jxr)
        -b: selects the bands to encode
             (ALL<Default>|NOFLEXBITS|NOHIGHPASS|DCONLY)
        -a: selects encoder alpha mode
             (0:no alpha|1:interleaved alpha|2:separate alpha)
        -f: selects the internal color format
             (YUV444<Default>|YUV422|YUV420)
        -F: selects the number of flexbits to trim
             (0<default> - 15)
        -h: selects hard tile boundaries
             (soft tile boundaries by default)
        -m: encode in frequency order codestream
             (spatial order by default)
        -l: selects the overlapped block filtering
             (0:off|1:HP only<Default>|2:all)

        -q: sets the quantization values separately, or one per band
             (0<default, lossless> - 255)
        -Q: specifies a file containing detailed quantization information
             See sample.qp
        -d: selects quantization for U/V channels derived from Y channel

        -U: selects uniform tile sizes
        -C: selects the number of tile columns and the width of each
        -R: selects the number of tile rows and the height of each

        -w: sets LONG_WORD_FLAG equal to FALSE
        -P: selects the profile value
             (44:Sub-Baseline|55:Baseline|66:Main|111:Advanced)
        -L: selects the level value
             (4|8|16|32|64|128)

        -r: selects encoding with RAW images
             must also specify -W, -H and -M, optional -B
        -W: RAW image width when encoding with RAW images
        -H: RAW image height when encoding with RAW images
        -M: RAW image format when encoding with RAW images
            3: 3-channel
            4: 4-channel
            5: 5-channel
            6: 6-channel
            7: 7-channel
            8: 8-channel
            9: 3-channel Alpha
            10: 4-channel Alpha
            11: 5-channel Alpha
            12: 6-channel Alpha
            13: 7-channel Alpha
            14: 8-channel Alpha
            15: 32bppRGBE
            16: 16bppBGR555
            17: 16bppBGR565
            18: 32bppBGR101010
        -B: RAW image bit/channel when encoding with RAW images
            8: 8-bit/channel (default)
            16: 16-bit/channel

Raw File Description:
The raw  file makes it possible to store the results of the output formatting process . The raw file output consists of either interleaved or sequential data from each of the channels in the image, without any header information. Buffers are stored in a raster scan order.
If OUTPUT_CLR_FMT is equal to YUV_420, YUV_422, YUV_444 or CMYKDirect, the buffer for each channel is stored sequentially. For example, when OUTPUT_CLR_FMT equals YUV_444 all Y samples are stored in the output file, followed by all U samples, followed by all V samples. If OUTPUT_CLR_FMT is equal to YUV422, half as many bytes used for the Y channel will be used to store the U and V channels. Otherwise, uf OUTPUT_CLR_FMT is equal to YUV420, one quarter as many bytes as used for the Y channel will be used to store the U and V channels. If OUTPUT_CLR_FMT is equal to YUV444 or YUV422 and OUTPUT_BITDEPTH is equal to BD10, 2 bytes are used per sample, and the 10 bits are stored in the LSBs of each 2 byte pair. Samples decoded from the alpha image plane (when present), are concatenated with the output obtained from the primary image.
Otherwise, if OUTPUT_CLR_FMT is not equal to YUV_420, YUV_422, YUV_444 or CMYKDirect, the raw file output consists of interleaved data from each channel. The number of bytes used to store each sample is contingent upon the value of OUTPUT_BITDEPTH and whether the format is a packed output format or not. The samples decoded from the image alpha plane (when present), are interleaved with the samples decoded from the primary image.


JPEG XR reference software 1.4 (31 Dec 2008)
-------------------------------------------------------------------

This version contains a bug fix and integrates the hard tile proposal.
Indentations have been reformatted. It fixes a bug related to decoding of 
multiple tile images with quantization step sizes varying across tiles.

JPEG XR reference software 1.3 (30 Sept 2008)
-------------------------------------------------------------------

This version was circulated for CD ballot.  In has the same
technical design as the 1.2 version.  Some renaming of
variables and strings was performed to replace references to
"HD Photo" (and "hdp") with references to "JPEG XR" (and "jxr").
Some minor header reformatting was performed.

JPEG XR reference software 1.2
-------------------------------------------------------------------

This version integrates the fixed POT filter proposed by
Microsoft. To enable or disable this filter, edit the
line setting FIXED_POT in jxr_priv.h. It is by default enabled
now.

JPEG XR reference software 1.1
-------------------------------------------------------------------

This is an updated/modified version of the 1.0 release of the
reference software, originally provided by Microsoft and Picture
Elements. Modifications for release 1.1 were made by the University
of Stuttgart (Thomas Richter), and are provided under the same
licence terms as release 1.0.

The following modifications have been made for this release:

- The main program prints a summary of its command line arguments
if an input file is missing.

- A new flag "-d" has been added that derives the quantization
parameter for the color planes in the same way the DPK software
derived its quantization parameters from the Y plane. This makes the
reference software backwards compatible to the DPK.



JPEG XR reference software 1.0
-------------------------------------------------------------------


SUMMARY

The jpegxr program is an example program that uses the jpegxr.h
compression/decompression API.

USAGE

jpegxr <flags> <input-file>

FLAGS

* -c

Compress.  Normally, jpegxr decompresses the input file. This flag
requests compression instead.

* -b ALL|NOFLEXBITS|NOHIGHPASS|DCONLY

When compressing, this flag selects the subbands to use. The names are
taken from the JPEG XR standard. The default is '-b ALL'. This is
only useful for compression.

* -f <flag>

Set various encode/decode options.

* -F <bits>

Enable FLEXBIT trimming. If FLEXBITS are enabled (-b ALL) then the
<bits> value is the number of bits to trim. The useful value ranges
from 0 to 15 inclusive. -F 0 keeps the most FLEXBITS, and -F 15 keeps
the fewest bits. This is only useful for compression.

* -l [0|1|2]

Enable overlap pre-filtering. The default is '-l 0' which turns off
filtering. Values 1 and 2 enable first pass only or first and second
pass filters respectively.

* -o <path>

Give the output file destination. If comression is enabled (-c) then
this is the path to the output compressed file. Otherwise, this is the
path where image data is written.

* -q q1:q2:q3...

Give the quantization code for each channel. If only 1 value is
given, then use the same quantization value for all channels. if
multiple values are given, separated by a ':', then each is assigned
to a channel. If fewer values are given then there are channels, then
the last value is used for all the remaining channels. For example,
one can use '-q 2:4' to specify Q=2 for Y and Q=4 for U and V channels
of a color image. The default is '-q 0', which along with '-b ALL'
makes compression lossless. The useful range for this value is 0..255.

* -d

Derived quantization. If this parameter is specified, only one argument
for -q is required and the code derives more suitable quantization values
for the Cb and Cr chroma components itself, similar to the DPK code.
This option improves coding efficency noticably whenever the color
transformation is run.

* -Q <path>

Give an ASCII source file that contains detailed information about the
quantization to use, down to a macroblock level.


ENCODE/DECODE OPTIONS

The -f argument can take any of a variety of options that control
encode decode. The -f flag can appear as often as needed, with one
flag at a time. The currently supported flags are:

   YUV444  -- Encode full color, with no subsampling of UV planes.

   YUV422  -- Encode color by subsampling the UV planes of YUV444.

   YUV420  -- Encode color by subsampling the UV planes of YUV444.

QP_FILE SYNTAX

Comments start with a '#' character and continue to the end of the
line. A comment can start anywhere in the line.

The keywords are:

    DC, LP, HP
    channel
    independent
    separate
    tile
    uniform

A NUMBER is an unsigned decimal value.

A file consists of a number of tile descriptors, one tile descriptor
for each expected tile in the image. A tile descripter is:

   tile ( <n>, <n> ) { tile_comp_mode tile_body }

A tile_comp_mode is one of the keywords: uniform, separate, or
independent. The tile_body is an unordered list of tile_items:

   channel <n> { channel_body }
   LP [ map_list ]
   HP [ map_list ]

The channel_body gives channel-specific information. How many channels
depends on the number of channels and the tile_comp_mode. A channel
body is one each of these channel items:

   DC { <n> }
   LP { <n>... }
   HP { <n>... }

