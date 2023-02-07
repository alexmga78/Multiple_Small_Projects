#include "image_editor.h"

//  Crop image (pbm)
static int pbm_crop(pbm *pbm_img) {
  //  Frees pbm image memory that is being cropped out
  for (uint i = (*pbm_img).height_start; i < (*pbm_img).y1; i++)
    free((*pbm_img).matrix[i]);
  for (uint i = (*pbm_img).y2; i < (*pbm_img).height; i++)
    free((*pbm_img).matrix[i]);

  //  Asign the new "hard" dimensions
  (*pbm_img).width_start = (*pbm_img).x1;
  (*pbm_img).height_start = (*pbm_img).y1;
  (*pbm_img).width = (*pbm_img).x2;
  (*pbm_img).height = (*pbm_img).y2;

  return 1;
}

//  Crop image (pgm)
static int pgm_crop(pgm *pgm_img) {
  //  Frees pgm image memory that is being cropped out
  for (uint i = (*pgm_img).height_start; i < (*pgm_img).y1; i++)
    free((*pgm_img).matrix[i]);
  for (uint i = (*pgm_img).y2; i < (*pgm_img).height; i++)
    free((*pgm_img).matrix[i]);

  //  Asign the new "hard" dimensions
  (*pgm_img).width_start = (*pgm_img).x1;
  (*pgm_img).height_start = (*pgm_img).y1;
  (*pgm_img).width = (*pgm_img).x2;
  (*pgm_img).height = (*pgm_img).y2;

  return 1;
}

//  Crop image (ppm)
static int ppm_crop(ppm *ppm_img) {
  //  Frees ppm image memory that is being cropped out (left cols)
  for (uint k = (*ppm_img).width_start; k < (*ppm_img).x1; k++) {
    for (uint i = (*ppm_img).height_start; i < (*ppm_img).height; i++)
      free((*ppm_img).matrix[k][i]);
    free((*ppm_img).matrix[k]);
  }
  //  Frees ppm image memory that is being cropped out (right cols)
  for (uint k = (*ppm_img).x2; k < (*ppm_img).width; k++) {
    for (uint i = (*ppm_img).height_start; i < (*ppm_img).height; i++)
      free((*ppm_img).matrix[k][i]);
    free((*ppm_img).matrix[k]);
  }
  //  Frees ppm image memory that is being cropped out (middle rows)
  for (uint k = (*ppm_img).x1; k < (*ppm_img).x2; k++) {
    for (uint i = (*ppm_img).height_start; i < (*ppm_img).y1; i++)
      free((*ppm_img).matrix[k][i]);
    for (uint i = (*ppm_img).y2; i < (*ppm_img).height; i++)
      free((*ppm_img).matrix[k][i]);
  }

  //  Asign the new "hard" dimensions
  (*ppm_img).width_start = (*ppm_img).x1;
  (*ppm_img).height_start = (*ppm_img).y1;
  (*ppm_img).width = (*ppm_img).x2;
  (*ppm_img).height = (*ppm_img).y2;

  return 1;
}

//  redim. an image with a lower size (crop)
int crop(pbm *pbm_img, pgm *pgm_img, ppm *ppm_img, uint focus) {
  if (no_image_loaded(*pbm_img, *pgm_img, *ppm_img, focus) == -1) return -1;

  switch (focus) {
    case 1:
      pbm_crop(pbm_img);
      break;

    case 2:
      pgm_crop(pgm_img);
      break;

    case 3:
      ppm_crop(ppm_img);
      break;

    default:
      printf("\"Focus is invalid\"\n");
      break;
  }

  printf("Image cropped\n");
  return 1;
}
