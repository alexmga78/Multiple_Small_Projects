#include "image_editor.h"

//  Find the coordinates in a way that will blur the image and ignore the
//  pixels on the edges
static void get_coordinates_pgm(pgm pgm_img, uint *apply_start_i,
                                uint *apply_start_j, uint *apply_fin_i,
                                uint *apply_fin_j) {
  if (pgm_img.y1 > pgm_img.height_start) *apply_start_i = pgm_img.y1;
  if (pgm_img.y2 < pgm_img.height) *apply_fin_i = pgm_img.y2;
  if (pgm_img.x1 > pgm_img.width_start) *apply_start_j = pgm_img.x1;
  if (pgm_img.x2 < pgm_img.width) *apply_fin_j = pgm_img.x2;
}

//  Find the coordinates in a way that the effect applies on image and ignore
//  the pixels on the edges
static void get_coordinates_ppm(ppm ppm_img, uint *apply_start_i,
                                uint *apply_start_j, uint *apply_fin_i,
                                uint *apply_fin_j) {
  if (ppm_img.y1 > ppm_img.height_start) *apply_start_i = ppm_img.y1;
  if (ppm_img.y2 < ppm_img.height) *apply_fin_i = ppm_img.y2;
  if (ppm_img.x1 > ppm_img.width_start) *apply_start_j = ppm_img.x1;
  if (ppm_img.x2 < ppm_img.width) *apply_fin_j = ppm_img.x2;
}

//  Alloc new_matrix (pgm type)
static int alloc_new_matrix_pgm(pgm pgm_img, uch ***new_matrix) {
  //  Alloc lines
  *new_matrix = (uch **)malloc(pgm_img.height * sizeof(uch *));
  if (!*new_matrix) {
    fprintf(stderr, "Failed to alloc memory for 'new_matrix'\n");
    return -1;
  }

  //  Alloc cols
  for (uint i = pgm_img.height_start; i < pgm_img.height; i++) {
    new_matrix[0][i] = (uch *)malloc(pgm_img.width * sizeof(uch));
    if (!new_matrix[0][i]) {
      fprintf(stderr, "Failed to alloc memory for 'new_matrix[%d]'\n", i);
      while (i > 0) free(new_matrix[0][--i]);
      free(*new_matrix);
      return -1;
    }
  }
  return 1;
}

//  Alloc new_matrix (ppm type)
static int alloc_new_matrix_ppm(ppm ppm_img, uch ****new_matrix) {
  uint rgb = 3;

  //  Alloc lines
  *new_matrix = (uch ***)malloc(ppm_img.height * sizeof(uch **));
  if (!*new_matrix) {
    fprintf(stderr, "Failed to alloc memory for 'new_matrix'\n");
    return -1;
  }

  //  Alloc cols
  for (uint i = ppm_img.height_start; i < ppm_img.height; i++) {
    new_matrix[0][i] = (uch **)malloc(ppm_img.width * sizeof(uch *));
    if (!new_matrix[0][i]) {
      fprintf(stderr, "Failed to alloc memory for 'new_matrix[%d]'\n", i);
      while (i > 0) free(new_matrix[0][--i]);
      free(*new_matrix);
      return -1;
    }
  }

  //  Alloc rgb
  for (uint i = ppm_img.height_start; i < ppm_img.height; i++)
    for (uint j = ppm_img.width_start; j < ppm_img.width; j++) {
      new_matrix[0][i][j] = (uch *)malloc(rgb * sizeof(uch));
      if (!new_matrix[0][i][j]) {
        fprintf(stderr, "Failed to alloc memory for 'new_matrix[%d][%d]'\n", i,
                j);
        while (j > 0) free(new_matrix[0][i][--j]);
        free(new_matrix[0][i]);
        for (uint l = i; l > 0; --l) {
          for (uint m = ppm_img.height_start; m < ppm_img.height; m++)
            free(new_matrix[0][l][m]);
          free(new_matrix[0][l]);
        }
        free(*new_matrix);
        return -1;
      }
    }

  return 1;
}

//  Free the auxiliary matrix (pgm type)
static void free_new_matrix_pgm(pgm pgm_img, uch ***new_matrix) {
  for (uint i = pgm_img.height_start; i < pgm_img.height; i++)
    free(new_matrix[0][i]);
  free(*new_matrix);
}

//  Free the auxiliary matrix (ppm type)
static void free_new_matrix_ppm(ppm ppm_img, uch ****new_matrix) {
  for (uint i = ppm_img.height_start; i < ppm_img.height; i++) {
    for (uint j = ppm_img.width_start; j < ppm_img.width; j++)
      free(new_matrix[0][i][j]);
    free(new_matrix[0][i]);
  }
  free(*new_matrix);
}

//  Copy the new_matrix into the original image (pgm type)
static void copy_new_matrix_to_image_pgm(pgm *pgm_img, uch **new_matrix,
                                         uint apply_start_i, uint apply_start_j,
                                         uint apply_fin_i, uint apply_fin_j) {
  for (uint i = apply_start_i; i < apply_fin_i; i++)
    for (uint j = apply_start_j; j < apply_fin_j; j++)
      (*pgm_img).matrix[i][j] = new_matrix[i][j];
}

//  Copy the new_matrix into the original image (ppm type)
static void copy_new_matrix_to_image_ppm(ppm *ppm_img, uch ***new_matrix,
                                         uint apply_start_i, uint apply_start_j,
                                         uint apply_fin_i, uint apply_fin_j) {
  uint rgb = 3;

  for (uint i = apply_start_i; i < apply_fin_i; i++)
    for (uint k = apply_start_j; k < apply_fin_j; k++)
      for (uint j = 0; j < rgb; j++)
        (*ppm_img).matrix[k][i][j] = new_matrix[i][k][j];
}

//  Applies blur on pgm image
static int pgm_apply_blur(pgm *pgm_img) {
  uint apply_start_i = (*pgm_img).height_start + 1,
       apply_start_j = (*pgm_img).width_start + 1,
       apply_fin_i = (*pgm_img).height - 1, apply_fin_j = (*pgm_img).width - 1;
  uch **new_matrix;

  if (alloc_new_matrix_pgm(*pgm_img, &new_matrix) == -1) return -1;

  get_coordinates_pgm(*pgm_img, &apply_start_i, &apply_start_j, &apply_fin_i,
                      &apply_fin_j);

  //  Blur image (selected part) -> into the auxiliary matrix
  for (uint i = apply_start_i; i < apply_fin_i; i++)
    for (uint j = apply_start_j; j < apply_fin_j; j++) {
      int new_pixel = 0;
      for (uint I = i - 1; I <= i + 1; I++)
        for (uint J = j - 1; J <= j + 1; J++)
          new_pixel += (*pgm_img).matrix[I][J];
      new_matrix[i][j] = new_pixel / 9;
    }

  copy_new_matrix_to_image_pgm(pgm_img, new_matrix, apply_start_i,
                               apply_start_j, apply_fin_i, apply_fin_j);

  free_new_matrix_pgm(*pgm_img, &new_matrix);

  printf("Easy, Charlie Chaplin\n");

  return 1;
}

//  Applies box blur on ppm image
static int ppm_apply_blur(ppm *ppm_img) {
  uint apply_start_i = (*ppm_img).height_start + 1,
       apply_start_j = (*ppm_img).width_start + 1,
       apply_fin_i = (*ppm_img).height - 1, apply_fin_j = (*ppm_img).width - 1,
       rgb = 3;
  uch ***new_matrix;

  if (alloc_new_matrix_ppm(*ppm_img, &new_matrix) == -1) return -1;

  get_coordinates_ppm(*ppm_img, &apply_start_i, &apply_start_j, &apply_fin_i,
                      &apply_fin_j);

  //  Blur image (selected part) -> into the auxiliary matrix
  for (uint j = 0; j < rgb; j++)
    for (uint i = apply_start_i; i < apply_fin_i; i++)
      for (uint k = apply_start_j; k < apply_fin_j; k++) {
        int new_pixel = 0;

        for (uint I = i - 1; I <= i + 1; I++)
          for (uint K = k - 1; K <= k + 1; K++)
            new_pixel += (*ppm_img).matrix[K][I][j];

        new_matrix[i][k][j] = new_pixel / 9;
      }

  copy_new_matrix_to_image_ppm(ppm_img, new_matrix, apply_start_i,
                               apply_start_j, apply_fin_i, apply_fin_j);

  free_new_matrix_ppm(*ppm_img, &new_matrix);

  printf("APPLY BLUR done\n");

  return 1;
}

//  Applies edge detetion on pgm image
static int pgm_apply_edge(pgm *pgm_img) {
  uint apply_start_i = (*pgm_img).height_start + 1,
       apply_start_j = (*pgm_img).width_start + 1,
       apply_fin_i = (*pgm_img).height - 1, apply_fin_j = (*pgm_img).width - 1;
  uch **new_matrix;

  if (alloc_new_matrix_pgm(*pgm_img, &new_matrix) == -1) return -1;

  get_coordinates_pgm(*pgm_img, &apply_start_i, &apply_start_j, &apply_fin_i,
                      &apply_fin_j);

  //  Edge image (selected part) -> into the auxiliary matrix
  for (uint i = apply_start_i; i < apply_fin_i; i++)
    for (uint j = apply_start_j; j < apply_fin_j; j++) {
      int new_pixel =
          (8 * (*pgm_img).matrix[i][j]) - (*pgm_img).matrix[i - 1][j - 1] -
          (*pgm_img).matrix[i - 1][j] - (*pgm_img).matrix[i - 1][j + 1] -
          (*pgm_img).matrix[i][j - 1] - (*pgm_img).matrix[i][j + 1] -
          (*pgm_img).matrix[i + 1][j - 1] - (*pgm_img).matrix[i + 1][j] -
          (*pgm_img).matrix[i + 1][j + 1];

      if (new_pixel < 0)
        new_pixel = 0;
      else if (new_pixel > 255)
        new_pixel = 255;

      new_matrix[i][j] = new_pixel;
    }

  copy_new_matrix_to_image_pgm(pgm_img, new_matrix, apply_start_i,
                               apply_start_j, apply_fin_i, apply_fin_j);

  free_new_matrix_pgm(*pgm_img, &new_matrix);

  printf("Easy, Charlie Chaplin\n");

  return 1;
}

//  Applies edge detection on ppm image
static int ppm_apply_edge(ppm *ppm_img) {
  uint rgb = 3, apply_start_i = (*ppm_img).height_start + 1,
       apply_start_j = (*ppm_img).width_start + 1,
       apply_fin_i = (*ppm_img).height - 1, apply_fin_j = (*ppm_img).width - 1;
  uch ***new_matrix;

  if (alloc_new_matrix_ppm(*ppm_img, &new_matrix) == -1) return -1;

  get_coordinates_ppm(*ppm_img, &apply_start_i, &apply_start_j, &apply_fin_i,
                      &apply_fin_j);

  //  Edge image (selected part) -> into the auxiliary matrix
  for (uint j = 0; j < rgb; j++)
    for (uint i = apply_start_i; i < apply_fin_i; i++)
      for (uint k = apply_start_j; k < apply_fin_j; k++) {
        int new_pixel =
            (8 * (*ppm_img).matrix[k][i][j]) - (*ppm_img).matrix[k][i - 1][j] -
            (*ppm_img).matrix[k - 1][i - 1][j] -
            (*ppm_img).matrix[k + 1][i - 1][j] -
            (*ppm_img).matrix[k - 1][i][j] - (*ppm_img).matrix[k + 1][i][j] -
            (*ppm_img).matrix[k - 1][i + 1][j] -
            (*ppm_img).matrix[k][i + 1][j] - (*ppm_img).matrix[k + 1][i + 1][j];

        if (new_pixel < 0)
          new_pixel = 0;
        else if (new_pixel > 255)
          new_pixel = 255;
        new_matrix[i][k][j] = new_pixel;
      }

  copy_new_matrix_to_image_ppm(ppm_img, new_matrix, apply_start_i,
                               apply_start_j, apply_fin_i, apply_fin_j);

  free_new_matrix_ppm(*ppm_img, &new_matrix);

  printf("APPLY EDGE done\n");

  return 1;
}

//  Applies gaussian blur on pgm image
static int pgm_apply_gaussian_blur(pgm *pgm_img) {
  uint apply_start_i = (*pgm_img).height_start + 1,
       apply_start_j = (*pgm_img).width_start + 1,
       apply_fin_i = (*pgm_img).height - 1, apply_fin_j = (*pgm_img).width - 1;
  uch **new_matrix;

  if (alloc_new_matrix_pgm(*pgm_img, &new_matrix) == -1) return -1;

  get_coordinates_pgm(*pgm_img, &apply_start_i, &apply_start_j, &apply_fin_i,
                      &apply_fin_j);

  //  Blur image (Gaussian) (selected part) -> into the auxiliary matrix
  for (uint i = apply_start_i; i < apply_fin_i; i++)
    for (uint j = apply_start_j; j < apply_fin_j; j++) {
      int new_pixel =
          (4 * (*pgm_img).matrix[i][j]) + (*pgm_img).matrix[i - 1][j - 1] +
          (2 * (*pgm_img).matrix[i - 1][j]) + (*pgm_img).matrix[i - 1][j + 1] +
          (2 * (*pgm_img).matrix[i][j - 1]) +
          (2 * (*pgm_img).matrix[i][j + 1]) + (*pgm_img).matrix[i + 1][j - 1] +
          (2 * (*pgm_img).matrix[i + 1][j]) + (*pgm_img).matrix[i + 1][j + 1];

      new_matrix[i][j] = new_pixel / 16;
    }

  copy_new_matrix_to_image_pgm(pgm_img, new_matrix, apply_start_i,
                               apply_start_j, apply_fin_i, apply_fin_j);

  free_new_matrix_pgm(*pgm_img, &new_matrix);

  printf("Easy, Charlie Chaplin\n");

  return 1;
}

//  Applies gaussian blur on ppm image
static int ppm_apply_gaussian_blur(ppm *ppm_img) {
  uint rgb = 3, apply_start_i = (*ppm_img).height_start + 1,
       apply_start_j = (*ppm_img).width_start + 1,
       apply_fin_i = (*ppm_img).height - 1, apply_fin_j = (*ppm_img).width - 1;
  uch ***new_matrix;

  if (alloc_new_matrix_ppm(*ppm_img, &new_matrix) == -1) return -1;

  get_coordinates_ppm(*ppm_img, &apply_start_i, &apply_start_j, &apply_fin_i,
                      &apply_fin_j);

  //  Blur image (Gaussian) (selected part) -> into the auxiliary matrix  for
  for (uint j = 0; j < rgb; j++)
    for (uint i = apply_start_i; i < apply_fin_i; i++)
      for (uint k = apply_start_j; k < apply_fin_j; k++) {
        int new_pixel = (4 * (*ppm_img).matrix[k][i][j]) +
                        (*ppm_img).matrix[k - 1][i - 1][j] +
                        (2 * (*ppm_img).matrix[k][i - 1][j]) +
                        (*ppm_img).matrix[k + 1][i - 1][j] +
                        (2 * (*ppm_img).matrix[k - 1][i][j]) +
                        (2 * (*ppm_img).matrix[k + 1][i][j]) +
                        (*ppm_img).matrix[k - 1][i + 1][j] +
                        (2 * (*ppm_img).matrix[k][i + 1][j]) +
                        (*ppm_img).matrix[k + 1][i + 1][j];

        new_matrix[i][k][j] = new_pixel / 16;
      }

  copy_new_matrix_to_image_ppm(ppm_img, new_matrix, apply_start_i,
                               apply_start_j, apply_fin_i, apply_fin_j);

  free_new_matrix_ppm(*ppm_img, &new_matrix);

  printf("APPLY GAUSSIAN_BLUR done\n");

  return 1;
}

//  Applies sharpen on pgm image
static int pgm_apply_sharpen(pgm *pgm_img) {
  uint apply_start_i = (*pgm_img).height_start + 1,
       apply_start_j = (*pgm_img).width_start + 1,
       apply_fin_i = (*pgm_img).height - 1, apply_fin_j = (*pgm_img).width - 1;
  uch **new_matrix;

  if (alloc_new_matrix_pgm(*pgm_img, &new_matrix) == -1) return -1;

  get_coordinates_pgm(*pgm_img, &apply_start_i, &apply_start_j, &apply_fin_i,
                      &apply_fin_j);

  //  Sharpen image (selected part) -> into the auxiliary matrix
  for (uint i = apply_start_i; i < apply_fin_i; i++)
    for (uint j = apply_start_j; j < apply_fin_j; j++) {
      int new_pixel = (5 * (*pgm_img).matrix[i][j]) -
                      (*pgm_img).matrix[i - 1][j] -
                      (*pgm_img).matrix[i][j - 1] -
                      (*pgm_img).matrix[i][j + 1] - (*pgm_img).matrix[i + 1][j];

      if (new_pixel < 0)
        new_pixel = 0;
      else if (new_pixel > 255)
        new_pixel = 255;

      new_matrix[i][j] = new_pixel;
    }

  copy_new_matrix_to_image_pgm(pgm_img, new_matrix, apply_start_i,
                               apply_start_j, apply_fin_i, apply_fin_j);

  free_new_matrix_pgm(*pgm_img, &new_matrix);

  printf("Easy, Charlie Chaplin\n");

  return 1;
}

//  Applies sharpen on ppm image
static int ppm_apply_sharpen(ppm *ppm_img) {
  uint rgb = 3, apply_start_i = (*ppm_img).height_start + 1,
       apply_start_j = (*ppm_img).width_start + 1,
       apply_fin_i = (*ppm_img).height - 1, apply_fin_j = (*ppm_img).width - 1;
  uch ***new_matrix;

  if (alloc_new_matrix_ppm(*ppm_img, &new_matrix) == -1) return -1;

  get_coordinates_ppm(*ppm_img, &apply_start_i, &apply_start_j, &apply_fin_i,
                      &apply_fin_j);

  //  Sharpen (Gaussian) (selected part) -> into the auxiliary matrix  for
  for (uint j = 0; j < rgb; j++)
    for (uint i = apply_start_i; i < apply_fin_i; i++)
      for (uint k = apply_start_j; k < apply_fin_j; k++) {
        int new_pixel =
            (5 * (*ppm_img).matrix[k][i][j]) - (*ppm_img).matrix[k][i - 1][j] -
            (*ppm_img).matrix[k - 1][i][j] - (*ppm_img).matrix[k + 1][i][j] -
            (*ppm_img).matrix[k][i + 1][j];

        if (new_pixel < 0)
          new_pixel = 0;
        else if (new_pixel > 255)
          new_pixel = 255;

        new_matrix[i][k][j] = new_pixel;
      }

  copy_new_matrix_to_image_ppm(ppm_img, new_matrix, apply_start_i,
                               apply_start_j, apply_fin_i, apply_fin_j);

  free_new_matrix_ppm(*ppm_img, &new_matrix);

  printf("APPLY SHARPEN done\n");

  return 1;
}

//  Redirect what image type to blur
int apply_blur(pgm *pgm_img, ppm *ppm_img, uint focus) {
  if (focus == 2 || focus == 5)
    pgm_apply_blur(pgm_img);
  else if (focus == 3 || focus == 6)
    ppm_apply_blur(ppm_img);

  return 1;
}

//  Redirect what image type to edge
int apply_edge(pgm *pgm_img, ppm *ppm_img, uint focus) {
  if (focus == 2 || focus == 5)
    pgm_apply_edge(pgm_img);
  else if (focus == 3 || focus == 6)
    ppm_apply_edge(ppm_img);

  return 1;
}

//  Redirect what image type to gaussian blur
int apply_gaussian_blur(pgm *pgm_img, ppm *ppm_img, uint focus) {
  if (focus == 2 || focus == 5)
    pgm_apply_gaussian_blur(pgm_img);
  else if (focus == 3 || focus == 6)
    ppm_apply_gaussian_blur(ppm_img);

  return 1;
}

//  Redirect what image type to sharpen
int apply_sharpen(pgm *pgm_img, ppm *ppm_img, uint focus) {
  if (focus == 2 || focus == 5)
    pgm_apply_sharpen(pgm_img);
  else if (focus == 3 || focus == 6)
    ppm_apply_sharpen(ppm_img);

  return 1;
}
