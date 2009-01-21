#include <stdio.h>
#include <jpeglib.h>
#include <stdlib.h>

int read_jpeg_file( char *filename, unsigned char **raw_image, struct jpeg_decompress_struct &cinfo);

int write_jpeg_file( char *filename, unsigned char *raw_image );
