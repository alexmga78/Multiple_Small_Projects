#include "image_editor.h"

//  Rotate the image/selected part of the image 90 degrees to the right
int rotate_90_r(uch ***matrix, uint *height, uint *width, uint x1, uint y1,
                uint *x2, uint *y2) {
  uch matrix_copy[*y2 - y1 + 1][*x2 - x1 + 1];

  if (x1 != 0 || y1 != 0 || *x2 != *width || *y2 != *height) {
    //  Clone image (selected part)
    for (uint i = y1; i < *y2; i++)
      for (uint j = x1; j < *x2; j++)
        matrix_copy[i - y1][j - x1] = matrix[0][i][j];

    //  Rotate to right (selected part)
    for (uint i = 0; i < *y2 - y1; i++)
      for (int j = *x2 - x1 - 1; j >= 0; j--)
        matrix[0][i + y1][*x2 - j - 1] = matrix_copy[j][i];
  } else {
    //  Clone image (whole image)
    for (uint i = 0; i < *height; i++) {
      for (uint j = 0; j < *width; j++) matrix_copy[i][j] = matrix[0][i][j];
      free(matrix[0][i]);
    }

    //  Swap dimensions
    swap_uint(height, width);

    *x2 = *width;
    *y2 = *height;

    //  Alloc lines
    *matrix = (uch **)realloc(*matrix, *height * sizeof(uch *));
    if (!*matrix) {
      fprintf(stderr, "Failed to alloc memory for '*matrix'\n");
      free(matrix);
      exit(500);
    }

    //  Alloc columns
    for (uint i = 0; i < *height; i++) {
      matrix[0][i] = (uch *)malloc(*width * sizeof(uch));
      if (!matrix[0][i]) {
        fprintf(stderr, "Failed to alloc memory for '*matrix[%d]\n'", i);
        while (i > 0) free(matrix[0][--i]);
        free(matrix[0]);
        free(matrix);
      }
    }

    //  Rotate to right (whole image)
    for (uint i = 0; i < *height; i++)
      for (uint j = 0; j < *width; j++)
        matrix[0][i][j] = matrix_copy[*width - j - 1][i];
  }

  return 1;
}

//  Rotate the image/selected part of the image 90 degrees to the left
int rotate_90_l(uch ***matrix, uint *height, uint *width, uint x1, uint y1,
                uint *x2, uint *y2) {
  uch matrix_copy[*y2 - y1 + 1][*x2 - x1 + 1];

  if (x1 != 0 || y1 != 0 || *x2 != *width || *y2 != *height) {
    //  Clone image (selected part)
    for (uint i = y1; i < *y2; i++)
      for (uint j = x1; j < *x2; j++)
        matrix_copy[i - y1][j - x1] = matrix[0][i][j];

    //  Rotate to left (selected part)
    for (int i = *y2 - y1 - 1; i >= 0; i--)
      for (uint j = 0; j < *x2 - x1; j++)
        matrix[0][*y2 - i - 1][j + x1] = matrix_copy[j][i];
  } else {
    //  Clone image (whole image)
    for (uint i = 0; i < *height; i++) {
      for (uint j = 0; j < *width; j++) matrix_copy[i][j] = matrix[0][i][j];
      free(matrix[0][i]);
    }

    swap_uint(height, width);

    *x2 = *width;
    *y2 = *height;

    //  Alloc lines
    *matrix = (uch **)realloc(*matrix, *height * sizeof(uch *));
    if (!*matrix) {
      fprintf(stderr, "Failed to alloc memory for '*matrix'\n");
      free(matrix);
      exit(500);
    }

    //  Alloc columns
    for (uint i = 0; i < *height; i++) {
      matrix[0][i] = (uch *)malloc(*width * sizeof(uch));
      if (!matrix[0][i]) {
        fprintf(stderr, "Failed to alloc memory for '*matrix[%d]\n'", i);
        while (i > 0) free(matrix[0][--i]);
        free(matrix[0]);
        free(matrix);
      }
    }

    //  Rotate to left (whole image)
    for (uint i = 0; i < *height; i++)
      for (uint j = 0; j < *width; j++)
        matrix[0][i][j] = matrix_copy[j][*height - i - 1];
  }

  return 1;
}

//  Rotate the image/selected part of the image 180 degrees
int rotate_180(uch ***matrix, uint x1, uint y1, uint x2, uint y2) {
  uch matrix_copy[y2][x2];

  //  Clone image (selected part)
  for (uint i = y1; i < y2; i++)
    for (uint j = x1; j < x2; j++) matrix_copy[i][j] = matrix[0][i][j];

  //  Rotate upside-down
  for (uint i = y1; i < y2; i++)
    for (uint j = x1; j < x2; j++)
      matrix[0][i][j] = matrix_copy[(y2 + y1) - i - 1][(x2 + x1) - j - 1];

  return 1;
}

//  Rotate the image/selected part of the image 90 degrees to the right (ppm)
int rotate_90_r_ppm(uch ****matrix, uint *height, uint *width, uint x1, uint y1,
                    uint *x2, uint *y2) {
  uint rgb = 3;
  uch matrix_copy[*y2 - y1 + 1][*x2 - x1 + 1][rgb + 1];

  if (x1 != 0 || y1 != 0 || *x2 != *width || *y2 != *height) {
    //  Clone image (selected part)
    for (uint i = y1; i < *y2; i++)
      for (uint k = x1; k < *x2; k++)
        for (uint j = 0; j < rgb; j++)
          matrix_copy[i - y1][k - x1][j] = matrix[0][k][i][j];

    // //  Rotate to right (selected part)
    for (uint i = 0; i < *y2 - y1; i++)
      for (int k = *x2 - x1 - 1; k >= 0; k--)
        for (uint j = 0; j < rgb; j++)
          matrix[0][*x2 - k - 1][i + y1][j] = matrix_copy[k][i][j];

  } else {
    //  Clone image (whole image)
    for (uint k = x1; k < *x2; k++) {
      for (uint i = y1; i < *y2; i++) {
        for (uint j = 0; j < rgb; j++)
          matrix_copy[i][k][j] = matrix[0][k][i][j];
        free(matrix[0][k][i]);
      }
      free(matrix[0][k]);
    }

    //  Swap dimensions
    swap_uint(height, width);

    *x2 = *width;
    *y2 = *height;

    //  Alloc columns
    *matrix = (uch ***)realloc(*matrix, *width * sizeof(uch **));
    if (!*matrix) {
      fprintf(stderr, "Failed to alloc memory for '*matrix'\n");
      free(matrix);
    }

    //  Alloc lines
    for (uint k = x1; k < *x2; k++) {
      matrix[0][k] = (uch **)malloc(*height * sizeof(uch *));
      if (!matrix[0][k]) {
        fprintf(stderr, "Failed to alloc memory for 'matrix[0][%d]'\n", k);
        while (k > x1) free(matrix[0][--k]);
        free(matrix[0]);
        free(matrix);
        exit(500);
      }
    }

    //  Alloc rgb
    for (uint k = x1; k < *x2; k++)
      for (uint i = y1; i < *y2; i++) {
        matrix[0][k][i] = (uch *)malloc(rgb * sizeof(uch));
        if (!matrix[0][k][i]) {
          fprintf(stderr, "Failed to alloc memory for 'matrix[0][%d][%d]'\n", k,
                  i);
          while (i > y1) free(matrix[0][k][--i]);
          for (uint l = k; l > x1; --l)
            for (uint j = *y2; j > y1; --j) free(matrix[0][l][j]);
        }
      }

    //  Rotate to right (whole image)
    for (uint i = y1; i < *y2; i++)
      for (uint k = x1; k < *x2; k++)
        for (uint j = 0; j < rgb; j++)
          matrix[0][k][i][j] = matrix_copy[*width - k - 1][i][j];
  }

  return 1;
}

//  Rotate the image/selected part of the image 90 degrees to the left (ppm)
int rotate_90_l_ppm(uch ****matrix, uint *height, uint *width, uint x1, uint y1,
                    uint *x2, uint *y2) {
  uint rgb = 3;
  uch matrix_copy[*y2 - y1 + 1][*x2 - x1 + 1][rgb + 1];

  if (x1 != 0 || y1 != 0 || *x2 != *width || *y2 != *height) {
    //  Clone image (selected part)
    for (uint i = y1; i < *y2; i++)
      for (uint k = x1; k < *x2; k++)
        for (uint j = 0; j < rgb; j++)
          matrix_copy[i - y1][k - x1][j] = matrix[0][k][i][j];

    // //  Rotate to left (selected part)
    for (int i = *y2 - y1 - 1; i >= 0; i--)
      for (uint k = 0; k < *x2 - x1; k++)
        for (uint j = 0; j < rgb; j++)
          matrix[0][k + x1][*y2 - i - 1][j] = matrix_copy[k][i][j];
  } else {
    //  Clone image (whole image)
    for (uint k = x1; k < *x2; k++) {
      for (uint i = y1; i < *y2; i++) {
        for (uint j = 0; j < rgb; j++)
          matrix_copy[i][k][j] = matrix[0][k][i][j];
        free(matrix[0][k][i]);
      }
      free(matrix[0][k]);
    }

    //  Swap dimensions
    swap_uint(height, width);

    *x2 = *width;
    *y2 = *height;

    //  Alloc columns
    *matrix = (uch ***)realloc(*matrix, *width * sizeof(uch **));
    if (!*matrix) {
      fprintf(stderr, "Failed to alloc memory for '*matrix'\n");
      free(matrix);
    }

    //  Alloc lines
    for (uint k = x1; k < *x2; k++) {
      matrix[0][k] = (uch **)malloc(*height * sizeof(uch *));
      if (!matrix[0][k]) {
        fprintf(stderr, "Failed to alloc memory for 'matrix[0][%d]'\n", k);
        while (k > x1) free(matrix[0][--k]);
        free(matrix[0]);
        free(matrix);
        exit(500);
      }
    }

    //  Alloc rgb
    for (uint k = x1; k < *x2; k++)
      for (uint i = y1; i < *y2; i++) {
        matrix[0][k][i] = (uch *)malloc(rgb * sizeof(uch));
        if (!matrix[0][k][i]) {
          fprintf(stderr, "Failed to alloc memory for 'matrix[0][%d][%d]'\n", k,
                  i);
          while (i > y1) free(matrix[0][k][--i]);
          for (uint l = k; l > x1; --l)
            for (uint j = *y2; j > y1; --j) free(matrix[0][l][j]);
        }
      }

    //  Rotate to left (whole image)
    for (uint i = y1; i < *y2; i++)
      for (uint k = x1; k < *x2; k++)
        for (uint j = 0; j < rgb; j++)
          matrix[0][k][i][j] = matrix_copy[k][*height - i - 1][j];
  }

  return 1;
}

//  Rotate the image/selected part of the image 180 degrees (ppm)
int rotate_180_ppm(uch ****matrix, uint x1, uint y1, uint x2, uint y2) {
  uint rgb = 3;
  uch matrix_copy[y2][x2][rgb + 1];

  //  Clone image (selected part)
  for (uint i = y1; i < y2; i++)
    for (uint k = x1; k < x2; k++)
      for (uint j = 0; j < rgb; j++) matrix_copy[i][k][j] = matrix[0][k][i][j];

  //  Rotate upside-down
  for (uint i = y1; i < y2; i++)
    for (uint k = x1; k < x2; k++)
      for (uint j = 0; j < rgb; j++)
        matrix[0][k][i][j] =
            matrix_copy[(y2 + y1) - i - 1][(x2 + x1) - k - 1][j];

  return 1;
}

//  Rotates a pbm image
int pbm_rotate(pbm *pbm_img) {
  int angle;

  scanf(" %d", &angle);

  switch (angle) {
    case 90:
      rotate_90_r(&pbm_img->matrix, &pbm_img->height, &pbm_img->width,
                  (*pbm_img).x1, (*pbm_img).y1, &pbm_img->x2, &pbm_img->y2);
      break;

    case 180:
      rotate_180(&pbm_img->matrix, (*pbm_img).x1, (*pbm_img).y1, (*pbm_img).x2,
                 (*pbm_img).y2);
      break;

    case 270:
      rotate_90_l(&pbm_img->matrix, &pbm_img->height, &pbm_img->width,
                  (*pbm_img).x1, (*pbm_img).y1, &pbm_img->x2, &pbm_img->y2);
      break;

    case 360:
      break;

    case -90:
      rotate_90_l(&pbm_img->matrix, &pbm_img->height, &pbm_img->width,
                  (*pbm_img).x1, (*pbm_img).y1, &pbm_img->x2, &pbm_img->y2);
      break;

    case -180:
      rotate_180(&pbm_img->matrix, (*pbm_img).x1, (*pbm_img).y1, (*pbm_img).x2,
                 (*pbm_img).y2);
      break;

    case -270:
      rotate_90_r(&pbm_img->matrix, &pbm_img->height, &pbm_img->width,
                  (*pbm_img).x1, (*pbm_img).y1, &pbm_img->x2, &pbm_img->y2);
      break;

    case -360:
      break;

    default:
      printf("Unsupported rotation angle\n");
      return 0;
      break;
  }

  printf("Rotated %d\n", angle);
  return 1;
}

//  Rotates a pgm image
int pgm_rotate(pgm *pgm_img) {
  int angle;

  scanf(" %d", &angle);

  switch (angle) {
    case 90:
      rotate_90_r(&pgm_img->matrix, &pgm_img->height, &pgm_img->width,
                  (*pgm_img).x1, (*pgm_img).y1, &pgm_img->x2, &pgm_img->y2);
      break;

    case 180:
      rotate_180(&pgm_img->matrix, (*pgm_img).x1, (*pgm_img).y1, (*pgm_img).x2,
                 (*pgm_img).y2);
      break;

    case 270:
      rotate_90_l(&pgm_img->matrix, &pgm_img->height, &pgm_img->width,
                  (*pgm_img).x1, (*pgm_img).y1, &pgm_img->x2, &pgm_img->y2);
      break;

    case 360:
      break;

    case -90:
      rotate_90_l(&pgm_img->matrix, &pgm_img->height, &pgm_img->width,
                  (*pgm_img).x1, (*pgm_img).y1, &pgm_img->x2, &pgm_img->y2);
      break;

    case -180:
      rotate_180(&pgm_img->matrix, (*pgm_img).x1, (*pgm_img).y1, (*pgm_img).x2,
                 (*pgm_img).y2);
      break;

    case -270:
      rotate_90_r(&pgm_img->matrix, &pgm_img->height, &pgm_img->width,
                  (*pgm_img).x1, (*pgm_img).y1, &pgm_img->x2, &pgm_img->y2);
      break;

    case -360:
      break;

    default:
      printf("Unsupported rotation angle\n");
      return 0;
      break;
  }

  printf("Rotated %d\n", angle);
  return 1;
}

//  Rotates a ppm image
int ppm_rotate(ppm *ppm_img) {
  int angle;

  scanf(" %d", &angle);

  switch (angle) {
    case 90:
      rotate_90_r_ppm(&ppm_img->matrix, &ppm_img->height, &ppm_img->width,
                      (*ppm_img).x1, (*ppm_img).y1, &ppm_img->x2, &ppm_img->y2);
      break;

    case 180:
      rotate_180_ppm(&ppm_img->matrix, (*ppm_img).x1, (*ppm_img).y1,
                     (*ppm_img).x2, (*ppm_img).y2);
      break;

    case 270:
      rotate_90_l_ppm(&ppm_img->matrix, &ppm_img->height, &ppm_img->width,
                      (*ppm_img).x1, (*ppm_img).y1, &ppm_img->x2, &ppm_img->y2);
      break;

    case 360:
      break;

    case -90:
      rotate_90_l_ppm(&ppm_img->matrix, &ppm_img->height, &ppm_img->width,
                      (*ppm_img).x1, (*ppm_img).y1, &ppm_img->x2, &ppm_img->y2);
      break;

    case -180:
      rotate_180_ppm(&ppm_img->matrix, (*ppm_img).x1, (*ppm_img).y1,
                     (*ppm_img).x2, (*ppm_img).y2);
      break;

    case -270:
      rotate_90_r_ppm(&ppm_img->matrix, &ppm_img->height, &ppm_img->width,
                      (*ppm_img).x1, (*ppm_img).y1, &ppm_img->x2, &ppm_img->y2);
      break;

    case -360:
      break;

    default:
      printf("Unsupported rotation angle\n");
      return 0;
      break;
  }

  printf("Rotated %d\n", angle);
  return 1;
}

//  The switch for "rotate" routes
int rotate(pbm *pbm_img, pgm *pgm_img, ppm *ppm_img, uint focus) {
  switch (focus) {
    case 1:
      if ((*pbm_img).x2 - (*pbm_img).x1 == (*pbm_img).y2 - (*pbm_img).y1 ||
          ((*pbm_img).x2 - (*pbm_img).x1 ==
               (*pbm_img).width - (*pbm_img).width_start &&
           (*pbm_img).y2 - (*pbm_img).y1 ==
               (*pbm_img).height - (*pbm_img).height_start))
        pbm_rotate(pbm_img);
      else {
        printf("The selection must be square\n");
        int aux;
        scanf(" %d", &aux);
      }
      break;

    case 2:
      if ((*pgm_img).x2 - (*pgm_img).x1 == (*pgm_img).y2 - (*pgm_img).y1 ||
          ((*pgm_img).x2 - (*pgm_img).x1 ==
               (*pgm_img).width - (*pgm_img).width_start &&
           (*pgm_img).y2 - (*pgm_img).y1 ==
               (*pgm_img).height - (*pgm_img).height_start))
        pgm_rotate(pgm_img);
      else {
        printf("The selection must be square\n");
        int aux;
        scanf(" %d", &aux);
      }
      break;

    case 3:
      if ((*ppm_img).x2 - (*ppm_img).x1 == (*ppm_img).y2 - (*ppm_img).y1 ||
          ((*ppm_img).x2 - (*ppm_img).x1 ==
               (*ppm_img).width - (*ppm_img).width_start &&
           (*ppm_img).y2 - (*ppm_img).y1 ==
               (*ppm_img).height - (*ppm_img).height_start))
        ppm_rotate(ppm_img);
      else {
        printf("The selection must be square\n");
        int aux;
        scanf(" %d", &aux);
      }
      break;

    default:
      printf("\"Focus is invalid\"\n");
      return 0;
      break;
  }

  return 1;
}
