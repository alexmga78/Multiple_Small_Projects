#include "image_editor.h"

int equalize(pgm *pgm_img) {
  uint occurences[257] = {0}, area = (*pgm_img).width * (*pgm_img).height;

  //  Calculate vector of frequence (of image intensities)
  for (uint i = (*pgm_img).height_start; i < (*pgm_img).height; i++)
    for (uint j = (*pgm_img).width_start; j < (*pgm_img).width; j++)
      occurences[(*pgm_img).matrix[i][j]]++;

  for (uint i = (*pgm_img).height_start; i < (*pgm_img).height; i++) {
    for (uint j = (*pgm_img).width_start; j < (*pgm_img).width; j++) {
      uint sum = 0;
      for (uint l = 0; l < (*pgm_img).matrix[i][j]; l++) sum += occurences[l];
      (*pgm_img).matrix[i][j] = round((255 * sum) / area);
    }
  }
  
  printf("Equalize done\n");

  return 1;
}
