#include "image_editor.h"

static int pbm_select(pbm *pbm_img, int x1) {
  int y1, x2, y2;

  scanf(" %d %d %d", &y1, &x2, &y2);

  //  Sort coordinates
  if (x1 > x2) swap(&x1, &x2);
  if (y1 > y2) swap(&y1, &y2);

  //  Verify if coordinates are valid
  if (x1 < 0 || (uint)x1 < (*pbm_img).width_start ||
      (uint)x1 > (*pbm_img).width || (uint)x2 > (*pbm_img).width || y1 < 0 ||
      (uint)y1 < (*pbm_img).height_start || (uint)y1 > (*pbm_img).height ||
      (uint)y2 > (*pbm_img).height) {
    printf("Invalid set of coordinates\n");
    return -1;
  }

  (*pbm_img).x1 = x1;
  (*pbm_img).y1 = y1;
  (*pbm_img).x2 = x2;
  (*pbm_img).y2 = y2;
  printf("Selected %d %d %d %d\n", x1, y1, x2, y2);

  return 1;
}

//  Select image (pgm)
static int pgm_select(pgm *pgm_img, int x1) {
  int y1, x2, y2;

  scanf(" %d %d %d", &y1, &x2, &y2);

  //  Sort coordinates
  if (x1 > x2) swap(&x1, &x2);
  if (y1 > y2) swap(&y1, &y2);

  //  Verify if coordinates are valid
  if (x1 < 0 || (uint)x1 < (*pgm_img).width_start ||
      (uint)x1 > (*pgm_img).width || (uint)x2 > (*pgm_img).width || y1 < 0 ||
      (uint)y1 < (*pgm_img).height_start || (uint)y1 > (*pgm_img).height ||
      (uint)y2 > (*pgm_img).height) {
    printf("Invalid set of coordinates\n");
    return -1;
  }

  (*pgm_img).x1 = x1;
  (*pgm_img).y1 = y1;
  (*pgm_img).x2 = x2;
  (*pgm_img).y2 = y2;
  printf("Selected %d %d %d %d\n", x1, y1, x2, y2);

  return 1;
}

//  Select image (ppm)
static int ppm_select(ppm *ppm_img, int x1) {
  int y1, x2, y2;

  scanf(" %d %d %d", &y1, &x2, &y2);

  //  Sort coordinates
  if (x1 > x2) swap(&x1, &x2);
  if (y1 > y2) swap(&y1, &y2);

  //  Verify if coordinates are valid
  if (x1 < 0 || (uint)x1 < (*ppm_img).width_start ||
      (uint)x1 > (*ppm_img).width || (uint)x2 > (*ppm_img).width || y1 < 0 ||
      (uint)y1 < (*ppm_img).height_start || (uint)y1 > (*ppm_img).height ||
      (uint)y2 > (*ppm_img).height) {
    printf("Invalid set of coordinates\n");
    return -1;
  }

  (*ppm_img).x1 = x1;
  (*ppm_img).y1 = y1;
  (*ppm_img).x2 = x2;
  (*ppm_img).y2 = y2;
  printf("Selected %d %d %d %d\n", x1, y1, x2, y2);

  return 1;
}

//  The switch for "select" routes
int select(pbm *pbm_img, pgm *pgm_img, ppm *ppm_img, uint focus, int x1) {
  //  Verifies if there is any image loaded
  if (no_image_loaded(*pbm_img, *pgm_img, *ppm_img, focus) == -1) {
    int aux;
    scanf(" %d %d %d", &aux, &aux, &aux);
    return -1;
  }

  switch (focus) {
    case 1:
      pbm_select(pbm_img, x1);
      break;

    case 2:
      pgm_select(pgm_img, x1);
      break;

    case 3:
      ppm_select(ppm_img, x1);
      break;

    default:
      printf("\"Focus is invalid\"\n");
      break;
  }

  return 1;
}
