#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdint.h>

#define APP_mask 0xE0 // app markers with metadata can be skipped for decoding

#define SOI 0xD8 //start of image
#define EOI 0xD9 //end of image

#define SOS 0xDA //start of search (start of compressed image data)
#define DQT 0xDB //used for quantization in image construction with the DCT
#define DRI 0xDD //how many MCUs between restart markers

#define SOF0 0xC0//extract image params like size 
#define SOF2 0xC2

#define DHT 0xC4//huffman coding table for compression format

#define u8 uint8_t;
#define u16 uint16_t;

#endif 
