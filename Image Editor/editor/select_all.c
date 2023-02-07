#include "image_editor.h"

//  Select_all image (pbm)
static void pbm_selectAll(pbm *pbm_img) {
  (*pbm_img).x1 = (*pbm_img).width_start;
  (*pbm_img).y1 = (*pbm_img).height_start;
  (*pbm_img).x2 = (*pbm_img).width;
  (*pbm_img).y2 = (*pbm_img).height;
  printf("Selected ALL\n");
}

//  Select_all image (pgm)
static void pgm_selectAll(pgm *pgm_img) {
  (*pgm_img).x1 = (*pgm_img).width_start;
  (*pgm_img).y1 = (*pgm_img).height_start;
  (*pgm_img).x2 = (*pgm_img).width;
  (*pgm_img).y2 = (*pgm_img).height;
  printf("Selected ALL\n");
}

//  Select_all image (ppm)
static void ppm_selectAll(ppm *ppm_img) {
  (*ppm_img).x1 = (*ppm_img).width_start;
  (*ppm_img).y1 = (*ppm_img).height_start;
  (*ppm_img).x2 = (*ppm_img).width;
  (*ppm_img).y2 = (*ppm_img).height;
  printf("Selected ALL\n");
}

//  The switch for "select_all" routes
int select_all(pbm *pbm_img, pgm *pgm_img, ppm *ppm_img, uint focus) {
  //  Verifies if there is any image loaded
  if (no_image_loaded(*pbm_img, *pgm_img, *ppm_img, focus) == -1) return -1;

  switch (focus) {
    case 1:
      pbm_selectAll(pbm_img);
      break;

    case 2:
      pgm_selectAll(pgm_img);
      break;

    case 3:
      ppm_selectAll(ppm_img);
      break;

    default:
      printf("\"Focus is invalid\"\n");
      break;
  }

  return 1;
}
