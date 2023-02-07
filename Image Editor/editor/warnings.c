#include "image_editor.h"

//  If the given image is not black and white
int black_and_white_image(uint focus) {
  if (focus != 2) {
    printf("Black and white image needed\n");
    return -1;
  }
  return 1;
}

//  If there isn't any image uploaded
int no_image_loaded(pbm pbm_img, pgm pgm_img, ppm ppm_img, uint focus) {
  if (focus == 0) {
    printf("No image loaded\n");
    return -1;
  } else if (focus == 1 && !pbm_img.loaded) {
    printf("No image loaded\n");
    return -1;
  } else if (focus == 2 && !pgm_img.loaded) {
    printf("No image loaded\n");
    return -1;
  } else if (focus == 3 && !ppm_img.loaded) {
    printf("No image loaded\n");
    return -1;
  }
  return 1;
  
