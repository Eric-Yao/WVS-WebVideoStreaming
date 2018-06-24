
#include "jpegwrite.h"


/*
	This a custom destination manager for jpeglib that
	enables the use of memory to memory compression.

	See IJG documentation for details.
*/
typedef struct {
	struct jpeg_destination_mgr pub;	/* base class */
	JOCTET*	buffer;						/* buffer start address */
	int		bufsize;					/* size of buffer */
	size_t	datasize;					/* final size of compressed data */
	int*	outsize;					/* user pointer to datasize */
	int		errcount;					/* counts up write errors due to 
										   buffer overruns */	
} memory_destination_mgr;

typedef memory_destination_mgr* mem_dest_ptr;

/* ------------------------------------------------------------- */
/*			MEMORY DESTINATION INTERFACE METHODS				 */
/* ------------------------------------------------------------- */


/* This function is called by the library before any data gets written */
METHODDEF(void)
init_destination (j_compress_ptr cinfo)
{
	mem_dest_ptr dest = (mem_dest_ptr)cinfo->dest;
	
	dest->pub.next_output_byte  = dest->buffer;		/* set destination buffer */
	dest->pub.free_in_buffer	= dest->bufsize;	/* input buffer size */
	dest->datasize = 0;								/* reset output size */
	dest->errcount = 0;								/* reset error count */
}

/* This function is called by the library if the buffer fills up 
 
   I just reset destination pointer and buffer size here.
   Note that this behavior, while preventing seg faults
   will lead to invalid output streams as data is over-
   written.
 */
METHODDEF(boolean)
empty_output_buffer (j_compress_ptr cinfo)
{
	mem_dest_ptr dest = (mem_dest_ptr)cinfo->dest;
	dest->pub.next_output_byte = dest->buffer;
	dest->pub.free_in_buffer = dest->bufsize;
	++dest->errcount;	/* need to increase error count */

	return TRUE;
}

/* Usually the library wants to flush output here.

   I will calculate output buffer size here.
   Note that results become incorrect, once
   empty_output_buffer was called.
   This situation is notified by errcount.
*/
METHODDEF(void)
term_destination (j_compress_ptr cinfo)
{
	mem_dest_ptr dest = (mem_dest_ptr)cinfo->dest;
	dest->datasize = dest->bufsize - dest->pub.free_in_buffer;
	if (dest->outsize) *dest->outsize += (int)dest->datasize;
}

/* Override the default destination manager initialization
   provided by jpeglib. Since we want to use memory-to-memory
   compression, we need to use our own destination manager.
*/
GLOBAL(void)
jpeg_memory_dest (j_compress_ptr cinfo, JOCTET* buffer, int bufsize, int* outsize)
{
	mem_dest_ptr dest;

	/* first call for this instance - need to setup */
	if (cinfo->dest == 0) {
		cinfo->dest = (struct jpeg_destination_mgr *)
			(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
			sizeof (memory_destination_mgr));
	}

	dest = (mem_dest_ptr) cinfo->dest;
	dest->bufsize = bufsize;
	dest->buffer  = buffer;
	dest->outsize = outsize;
	/* set method callbacks */
	dest->pub.init_destination = init_destination;
	dest->pub.empty_output_buffer = empty_output_buffer;
	dest->pub.term_destination = term_destination;
}

/* ------------------------------------------------------------- */
/*				 MEMORY SOURCE INTERFACE METHODS				 */
/* ------------------------------------------------------------- */

/* Called before data is read */
METHODDEF(void)
init_source (j_decompress_ptr dinfo)
{
	/* nothing to do here, really. I mean. I'm not lazy or something, but...
	   we're actually through here. */	
}

/* Called if the decoder wants some bytes that we cannot provide... */
METHODDEF(boolean)
fill_input_buffer (j_decompress_ptr dinfo)
{
	/* we can't do anything about this. This might happen if the provided
	   buffer is either invalid with regards to its content or just a to
	   small bufsize has been given. */

	/* fail. */
	return FALSE;
}

/* From IJG docs: "it's not clear that being smart is worth much trouble"
	So I save myself some trouble by ignoring this bit.
*/
METHODDEF(void)
skip_input_data (j_decompress_ptr dinfo, INT32 num_bytes)
{
	/*	There might be more data to skip than available in buffer.
		This clearly is an error, so screw this mess. */
	if ((size_t)num_bytes > dinfo->src->bytes_in_buffer) {
		dinfo->src->next_input_byte = 0;	/* no buffer byte */
		dinfo->src->bytes_in_buffer = 0;	/* no input left */
	} else {
		dinfo->src->next_input_byte += num_bytes;
		dinfo->src->bytes_in_buffer -= num_bytes;
	}
}

/* Finished with decompression */
METHODDEF(void)
term_source (j_decompress_ptr dinfo)
{
	/* Again. Absolute laziness. Nothing to do here. Boring. */
}

GLOBAL(void)
jpeg_memory_src (j_decompress_ptr dinfo, unsigned char* buffer, size_t size)
{
	struct jpeg_source_mgr* src;

	/* first call for this instance - need to setup */
	if (dinfo->src == 0) {
		dinfo->src = (struct jpeg_source_mgr *)
			(*dinfo->mem->alloc_small) ((j_common_ptr) dinfo, JPOOL_PERMANENT,
			sizeof (struct jpeg_source_mgr));
	}

	src = dinfo->src;
	src->next_input_byte = buffer;
	src->bytes_in_buffer = size;
	src->init_source = init_source;
	src->fill_input_buffer	= fill_input_buffer;
	src->skip_input_data	= skip_input_data;
	src->term_source		= term_source;
	/* IJG recommend to use their function - as I don't know ****
		about how to do better, I follow this recommendation */
	src->resync_to_restart	= jpeg_resync_to_restart;
}



////////////////////////////////////////////////////////////////////////////
struct JpegImageInfo {
  int width;
  int height;
  int pixels;
  int num_bytes;
  JSAMPLE * original_buffer;
  JSAMPLE * flip_buffer;
};

JpegImageInfo gJpegObj;

void jpeg_write_init(int width, int height) {
  gJpegObj.width = width;
  gJpegObj.height = height;
  gJpegObj.pixels = width * height *3;

  gJpegObj.original_buffer = new JSAMPLE[gJpegObj.pixels];
  gJpegObj.flip_buffer  = new JSAMPLE[gJpegObj.pixels];

}

void jpeg_write_destroy() {
  delete[] gJpegObj.original_buffer;
  delete[] gJpegObj.flip_buffer ;
  gJpegObj.original_buffer = NULL;
  gJpegObj.flip_buffer  = NULL;
}

void jpeg_write_reset() {
  memset(gJpegObj.original_buffer,0x0,sizeof(JSAMPLE)*gJpegObj.pixels);
  memset(gJpegObj.flip_buffer,0x0,sizeof(JSAMPLE)*gJpegObj.pixels);

  glReadPixels(0.0, 0.0, gJpegObj.width, gJpegObj.height, GL_RGB, GL_BYTE, gJpegObj.original_buffer);
  
  for (int x = 0; x < gJpegObj.width; x++) {
    for (int y = 0; y < gJpegObj.height; y++) {
      gJpegObj.flip_buffer[(y*gJpegObj.height+x)*3] = gJpegObj.original_buffer[((gJpegObj.width-1-y)*gJpegObj.height+x)*3];
      gJpegObj.flip_buffer[(y*gJpegObj.height+x)*3+1] = gJpegObj.original_buffer[((gJpegObj.width-1-y)*gJpegObj.height+x)*3+1];
      gJpegObj.flip_buffer[(y*gJpegObj.height+x)*3+2] = gJpegObj.original_buffer[((gJpegObj.width-1-y)*gJpegObj.height+x)*3+2];
    }
  }

}

void jpeg_write_file (char * filename, int quality)
{
  /* This struct contains the JPEG compression parameters and pointers to
   * working space (which is allocated as needed by the JPEG library).
   * It is possible to have several such structures, representing multiple
   * compression/decompression processes, in existence at once.  We refer
   * to any one struct (and its associated working data) as a "JPEG object".
   */
  struct jpeg_compress_struct cinfo;
  /* This struct represents a JPEG error handler.  It is declared separately
   * because applications often want to supply a specialized error handler
   * (see the second half of this file for an example).  But here we just
   * take the easy way out and use the standard error handler, which will
   * print a message on stderr and call exit() if compression fails.
   * Note that this struct must live as long as the main JPEG parameter
   * struct, to avoid dangling-pointer problems.
   */
  struct jpeg_error_mgr jerr;
  /* More stuff */
  FILE * outfile;		/* target file */
  JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
  int row_stride;		/* physical row width in image buffer */

  /* Step 1: allocate and initialize JPEG compression object */

  /* We have to set up the error handler first, in case the initialization
   * step fails.  (Unlikely, but it could happen if you are out of memory.)
   * This routine fills in the contents of struct jerr, and returns jerr's
   * address which we place into the link field in cinfo.
   */
  cinfo.err = jpeg_std_error(&jerr);
  /* Now we can initialize the JPEG compression object. */
  jpeg_create_compress(&cinfo);

  /* Step 2: specify data destination (eg, a file) */
  /* Note: steps 2 and 3 can be done in either order. */

  /* Here we use the library-supplied code to send compressed data to a
   * stdio stream.  You can also write your own code to do something else.
   * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
   * requires it in order to write binary files.
   */
  if ((outfile = fopen(filename, "wb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    exit(1);
  }
  jpeg_stdio_dest(&cinfo, outfile);

  /* Step 3: set parameters for compression */

  /* First we supply a description of the input image.
   * Four fields of the cinfo struct must be filled in:
   */
  cinfo.image_width = gJpegObj.width; 	/* image width and height, in pixels */
  cinfo.image_height = gJpegObj.height;
  cinfo.input_components = 3;		/* # of color components per pixel */
  cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
  /* Now use the library's routine to set default compression parameters.
   * (You must set at least cinfo.in_color_space before calling this,
   * since the defaults depend on the source color space.)
   */
  jpeg_set_defaults(&cinfo);
  /* Now you can set any non-default parameters you wish to.
   * Here we just illustrate the use of quality (quantization table) scaling:
   */
  jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

  /* Step 4: Start compressor */

  /* TRUE ensures that we will write a complete interchange-JPEG file.
   * Pass TRUE unless you are very sure of what you're doing.
   */
  jpeg_start_compress(&cinfo, TRUE);

  /* Step 5: while (scan lines remain to be written) */
  /*           jpeg_write_scanlines(...); */

  /* Here we use the library's state variable cinfo.next_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   * To keep things simple, we pass one scanline per call; you can pass
   * more if you wish, though.
   */
  row_stride = gJpegObj.width * 3;	/* JSAMPLEs per row in image_buffer */

  while (cinfo.next_scanline < cinfo.image_height) {
    /* jpeg_write_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could pass
     * more than one scanline at a time if that's more convenient.
     */
    row_pointer[0] = & gJpegObj.flip_buffer[cinfo.next_scanline * row_stride];
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  /* Step 6: Finish compression */

  jpeg_finish_compress(&cinfo);
  /* After finish_compress, we can close the output file. */
  fclose(outfile);

  /* Step 7: release JPEG compression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_compress(&cinfo);

  /* And we're done! */
}

int jpeg_write_buffer (const char * filename, int quality, char* content, int content_len)
{
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  /* More stuff */
  JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
  int row_stride;		/* physical row width in image buffer */

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);

  gJpegObj.num_bytes = 0; //size of jpeg after compression
  jpeg_memory_dest(&cinfo, (JOCTET*)content, content_len, &gJpegObj.num_bytes);

  cinfo.image_width = gJpegObj.width; 	/* image width and height, in pixels */
  cinfo.image_height = gJpegObj.height;
  cinfo.input_components = 3;		/* # of color components per pixel */
  cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */

  jpeg_set_defaults(&cinfo);

  jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

  jpeg_start_compress(&cinfo, TRUE);

  row_stride = gJpegObj.width * 3;	/* JSAMPLEs per row in image_buffer */

//printf("gImageWidth: %d \n", gImageWidth);
  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer[0] = & gJpegObj.flip_buffer[cinfo.next_scanline * row_stride];
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  jpeg_finish_compress(&cinfo);

  if (filename != NULL) {
    file_create(filename);
    file_write(filename, 0, gJpegObj.num_bytes, content);
  }
  jpeg_destroy_compress(&cinfo);
  return gJpegObj.num_bytes;
}



