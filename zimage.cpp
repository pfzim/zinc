#include "zimage.h"									// pf_ziminski [2008]
#include <stdio.h>
#include "libjpeg/jpeglib.h"
#include "zalloc.h"
#include <memory.h>

/*
	RGB 24bit - array(R,G,B, R,G,B, ..., R,G,B)
	RGB 32bit - array(R,G,B,unused, R,G,B,unused, ..., R,G,B,unused)

	BGR -> RGB {
		B ^= R;
		R ^= B;
		B ^= R;
	}

	CMYK -> RGB {
		R = C*K/255;
		G = M*K/255;
		B = Y*K/255;
	}
*/

void jpeg_init_source(j_decompress_ptr cinfo)
{
}

// Fill JPEG input buffer
boolean jpeg_fill_input_buffer (j_decompress_ptr cinfo)
{
	static JOCTET jpeg_eoi[] = { 0xFF, JPEG_EOI };

	if(cinfo && cinfo->src)
	{
		cinfo->src->next_input_byte = jpeg_eoi;
		cinfo->src->bytes_in_buffer = 2;
	}

	return TRUE;
}

// Skip data in JPEG buffer
void jpeg_skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
	if(cinfo && cinfo->src)
	{
	  	cinfo->src->bytes_in_buffer -= num_bytes;
		cinfo->src->next_input_byte += num_bytes;

		if(cinfo->src->bytes_in_buffer < 0) jpeg_fill_input_buffer(cinfo);
	}
}

// Destroy JPEG buffer
void jpeg_term_source(j_decompress_ptr cinfo)
{
}

unsigned char *zimage_load_jpeg(const unsigned char *data, unsigned long size, unsigned long *w, unsigned long *h, unsigned long *bpl)
{
	// always return 32 bit RGB bits map

	unsigned char *bits = NULL;
	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;

	memset(&cinfo, 0, sizeof(cinfo));
	memset(&jerr, 0, sizeof(jerr));

	/*
		Create error handler
	*/
	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_decompress(&cinfo);

	/*
		Fill in callback functions
	*/
	cinfo.src = (struct jpeg_source_mgr *)cinfo.mem->alloc_small((j_common_ptr)&cinfo, JPOOL_PERMANENT, sizeof(struct jpeg_source_mgr));
	//if (!IsValidWritePtr(cinfo.src)) longjmp(jerr.setjmp_buffer, 1);
	cinfo.src->next_input_byte = data;
	cinfo.src->bytes_in_buffer = size;
	cinfo.src->init_source = jpeg_init_source;
	cinfo.src->fill_input_buffer = jpeg_fill_input_buffer;
	cinfo.src->skip_input_data = jpeg_skip_input_data;
	cinfo.src->resync_to_restart = jpeg_resync_to_restart;
	cinfo.src->term_source = jpeg_term_source;

	/*
		Fill in values from JPEG header
	*/
	jpeg_read_header(&cinfo, TRUE);
	cinfo.do_fancy_upsampling = TRUE;
	cinfo.dct_method = JDCT_FLOAT;

	/*
		Start decompressor
	*/
	jpeg_start_decompress(&cinfo);

	/*
		Allocate buffer to hold bitmap data
	*/
	unsigned char *row, *row32;
	unsigned long bytes_per_line; // = ((w+1)*3) & (~3); //((((w*c*8)+7)/8)+ 3 & ~3);
	unsigned long offset;
	unsigned long offset32;
	JSAMPARRAY buffer = &row;

	//bytes_per_line = (cinfo.image_width*cinfo.output_components +3) & ~3; // make divisible by 4
	bytes_per_line = (cinfo.image_width*4 +3) & ~3; // make divisible by 4
	//bytes_per_line = (cinfo.image_width*4 +1) & ~2; // or may be???

	bits = (unsigned char *) zalloc(cinfo.image_height*bytes_per_line);
	if(bits)
	{
		row = (unsigned char *) zalloc(cinfo.image_width*cinfo.output_components);
		if(row)
		{
			while(cinfo.output_scanline < cinfo.image_height)
			{
				/*
					Decode each scanline storing the decoded data in our bitmap buffer
				*/

				row32 = &bits[(cinfo.image_height - cinfo.output_scanline -1)*bytes_per_line]; //for Windows
				//row32 = &bits[cinfo.output_scanline*bytes_per_line]; //for Windows
				//row = &bits[cinfo.output_scanline*cinfo.image_width*cinfo.output_components]; // original

				jpeg_read_scanlines(&cinfo, buffer, 1); // buffer must be divisible by 4 ???

				/*
					Convert row to 32 bit RGB bits set
				*/
				if(cinfo.out_color_space == JCS_RGB && cinfo.output_components >= 3)
				{
					offset = 0;
					offset32 = 0;
					while(offset < cinfo.output_components*cinfo.image_width)
					{
						row32[offset32+2] = row[offset];
						row32[offset32+1] = row[offset+1];
						row32[offset32] = row[offset+2];

						offset += cinfo.output_components;
						offset32 += 4;
					}
				}
				else if(cinfo.out_color_space == JCS_CMYK && cinfo.output_components >= 4)
				{
					offset = 0;
					offset32 = 0;
					while(offset < cinfo.output_components*cinfo.image_width)
					{
						row32[offset32] = (row[offset+3]*row[offset+2])/255;
						row32[offset32+1] = (row[offset+3]*row[offset+1])/255;
						row32[offset32+2] = (row[offset+3]*row[offset])/255;

						offset += cinfo.output_components;
						offset32 += 4;
					}
				}
				else if(cinfo.out_color_space == JCS_GRAYSCALE/* && cinfo.output_components == 1*/)
				{
					offset = 0;
					offset32 = 0;
					while(offset < cinfo.output_components*cinfo.image_width)
					{
						row32[offset32] = row[offset];
						row32[offset32+1] = row[offset];
						row32[offset32+2] = row[offset];

						offset += cinfo.output_components;
						offset32 += 4;
					}
				}
			}

			zfree(row);
		}
	}

	/*
		Destroy JPEG objects
	*/
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	/* Assemble bitmap header+bitmap data into a HBITMAP

	BITMAP bm;
	bm.bmBitsPixel = cinfo.output_components*8;
	bm.bmHeight = cinfo.image_height;
	bm.bmWidth = cinfo.image_width;
	bm.bmPlanes = 1;
	bm.bmType = 0;
	bm.bmWidthBytes = cinfo.image_width*cinfo.output_components; // must be divisible by 2
	bm.bmBits = lpBits;
	hbmImage = CreateBitmapIndirect(&bm);
	//*/

	*w = cinfo.image_width;
	*h = cinfo.image_height;
	*bpl = bytes_per_line;

	return bits;
}

int zimage_load_jpeg(char *filename)
{
  /* This struct contains the JPEG decompression parameters and pointers to
   * working space (which is allocated as needed by the JPEG library).
   */
  struct jpeg_decompress_struct cinfo;
  /* We use our private extension JPEG error handler.
   * Note that this struct must live as long as the main JPEG parameter
   * struct, to avoid dangling-pointer problems.
   */
  struct jpeg_error_mgr jerr;
  /* More stuff */
  FILE * infile;		/* source file */
  JSAMPARRAY buffer;		/* Output row buffer */
  int row_stride;		/* physical row width in output buffer */

  /* In this example we want to open the input file before doing anything else,
   * so that the setjmp() error recovery below can assume the file is open.
   * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
   * requires it in order to read binary files.
   */

  if ((infile = fopen(filename, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    return 0;
  }

  /* Step 1: allocate and initialize JPEG decompression object */

  /* We set up the normal JPEG error routines, then override error_exit. */
  cinfo.err = jpeg_std_error(&jerr);
  /* Now we can initialize the JPEG decompression object. */
  jpeg_create_decompress(&cinfo);

  /* Step 2: specify data source (eg, a file) */

  jpeg_stdio_src(&cinfo, infile);

  /* Step 3: read file parameters with jpeg_read_header() */

  (void) jpeg_read_header(&cinfo, TRUE);
  /* We can ignore the return value from jpeg_read_header since
   *   (a) suspension is not possible with the stdio data source, and
   *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
   * See libjpeg.doc for more info.
   */

  /* Step 4: set parameters for decompression */

  /* In this example, we don't need to change any of the defaults set by
   * jpeg_read_header(), so we do nothing here.
   */

  /* Step 5: Start decompressor */

  (void) jpeg_start_decompress(&cinfo);
  /* We can ignore the return value since suspension is not possible
   * with the stdio data source.
   */

  /* We may need to do some setup of our own at this point before reading
   * the data.  After jpeg_start_decompress() we have the correct scaled
   * output image dimensions available, as well as the output colormap
   * if we asked for color quantization.
   * In this example, we need to make an output work buffer of the right size.
   */
  /* JSAMPLEs per row in output buffer */
  row_stride = cinfo.output_width * cinfo.output_components;
  /* Make a one-row-high sample array that will go away when done with image */
  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  /* Step 6: while (scan lines remain to be read) */
  /*           jpeg_read_scanlines(...); */

  /* Here we use the library's state variable cinfo.output_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   */
  while (cinfo.output_scanline < cinfo.output_height) {
    /* jpeg_read_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could ask for
     * more than one scanline at a time if that's more convenient.
     */
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    /* Assume put_scanline_someplace wants a pointer and sample count. */
    //put_scanline_someplace(buffer[0], row_stride);
	for(int i = 0; i < row_stride; i++)
		printf("%c", buffer[0][i]);
  }

  /* Step 7: Finish decompression */

  (void) jpeg_finish_decompress(&cinfo);
  /* We can ignore the return value since suspension is not possible
   * with the stdio data source.
   */

  /* Step 8: Release JPEG decompression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_decompress(&cinfo);

  /* After finish_decompress, we can close the input file.
   * Here we postpone it until after no more JPEG errors are possible,
   * so as to simplify the setjmp error logic above.  (Actually, I don't
   * think that jpeg_destroy can do an error exit, but why assume anything...)
   */
  fclose(infile);

  /* At this point you may want to check to see whether any corrupt-data
   * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
   */

  /* And we're done! */
  return 1;
}

int zimage_load_jpeg2(char *filename)
{
  /* This struct contains the JPEG decompression parameters and pointers to
   * working space (which is allocated as needed by the JPEG library).
   */
  struct jpeg_decompress_struct cinfo;
  /* We use our private extension JPEG error handler.
   * Note that this struct must live as long as the main JPEG parameter
   * struct, to avoid dangling-pointer problems.
   */
  struct jpeg_error_mgr jerr;
  /* More stuff */
  FILE * infile;		/* source file */
  JSAMPARRAY buffer;		/* Output row buffer */
  int row_stride;		/* physical row width in output buffer */

  /* In this example we want to open the input file before doing anything else,
   * so that the setjmp() error recovery below can assume the file is open.
   * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
   * requires it in order to read binary files.
   */

  if ((infile = fopen(filename, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    return 0;
  }

  /* Step 1: allocate and initialize JPEG decompression object */

  /* We set up the normal JPEG error routines, then override error_exit. */
  cinfo.err = jpeg_std_error(&jerr);
  /* Now we can initialize the JPEG decompression object. */
  jpeg_create_decompress(&cinfo);

  /* Step 2: specify data source (eg, a file) */

  jpeg_stdio_src(&cinfo, infile);

  /* Step 3: read file parameters with jpeg_read_header() */

  (void) jpeg_read_header(&cinfo, TRUE);
  /* We can ignore the return value from jpeg_read_header since
   *   (a) suspension is not possible with the stdio data source, and
   *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
   * See libjpeg.doc for more info.
   */

  /* Step 4: set parameters for decompression */

  /* In this example, we don't need to change any of the defaults set by
   * jpeg_read_header(), so we do nothing here.
   */

  /* Step 5: Start decompressor */

  (void) jpeg_start_decompress(&cinfo);
  /* We can ignore the return value since suspension is not possible
   * with the stdio data source.
   */

  /* We may need to do some setup of our own at this point before reading
   * the data.  After jpeg_start_decompress() we have the correct scaled
   * output image dimensions available, as well as the output colormap
   * if we asked for color quantization.
   * In this example, we need to make an output work buffer of the right size.
   */
  /* JSAMPLEs per row in output buffer */
  row_stride = cinfo.output_width * cinfo.output_components;
  /* Make a one-row-high sample array that will go away when done with image */
  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  /* Step 6: while (scan lines remain to be read) */
  /*           jpeg_read_scanlines(...); */

  /* Here we use the library's state variable cinfo.output_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   */
  while (cinfo.output_scanline < cinfo.output_height) {
    /* jpeg_read_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could ask for
     * more than one scanline at a time if that's more convenient.
     */
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    /* Assume put_scanline_someplace wants a pointer and sample count. */
    //put_scanline_someplace(buffer[0], row_stride);
	for(int i = 0; i < row_stride; i++)
		printf("%c", buffer[0][i]);
  }

  /* Step 7: Finish decompression */

  (void) jpeg_finish_decompress(&cinfo);
  /* We can ignore the return value since suspension is not possible
   * with the stdio data source.
   */

  /* Step 8: Release JPEG decompression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_decompress(&cinfo);

  /* After finish_decompress, we can close the input file.
   * Here we postpone it until after no more JPEG errors are possible,
   * so as to simplify the setjmp error logic above.  (Actually, I don't
   * think that jpeg_destroy can do an error exit, but why assume anything...)
   */
  fclose(infile);

  /* At this point you may want to check to see whether any corrupt-data
   * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
   */

  /* And we're done! */
  return 1;
}
