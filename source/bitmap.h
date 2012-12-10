/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * bitmap.h                                            *
 * Structures for .bmp images handling on Nintendo DS  *
 *                                                     *
 * 2012 (c) David Barozzini (Marval13)                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef struct
{
	char signature[2];
	unsigned int fileSize;
	unsigned int reserved;
	unsigned int offset;
}__attribute__ ((packed)) BmpHeader;

typedef struct
{
	unsigned int headerSize;
	unsigned int width;
	unsigned int height;
	unsigned short planeCount;
	unsigned short bitDepth;
	unsigned int compression;
	unsigned int compressedImageSize;
	unsigned int horizontalResolution;
	unsigned int verticalResolution;
	unsigned int numColors;
	unsigned int importantColors;
	
}__attribute__ ((packed)) BmpImageInfo;

typedef struct
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char reserved;
}__attribute__ ((packed)) Rgb;
 
typedef struct
{
	BmpHeader header;
	BmpImageInfo info;
	Rgb colors[256];
	unsigned short image[1];
}__attribute__ ((packed)) BmpFile;
