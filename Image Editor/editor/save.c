#include "image_editor.h"

//  Frees memory and gives a message (successful)
static void footer(char **filename, FILE **image) {
  printf("Saved %s\n", *filename);
  free(*filename);
  fclose(*image);
}

//  Save image (pbm-text format)
static void pbm_saveText(FILE *image, pbm pbm_img) {
  pbm_img.width = pbm_img.x2 - pbm_img.x1;
  pbm_img.height = pbm_img.y2 - pbm_img.y1;
  fprintf(image, "%s\n%d %d\n", "P1", pbm_img.width, pbm_img.height);

  for (uint i = pbm_img.y1; i < pbm_img.y2; i++) {
    for (uint j = pbm_img.x1; j < pbm_img.x2; j++) {
      if (j + 1 < pbm_img.x2)
        fprintf(image, "%d ", pbm_img.matrix[i][j]);
      else
        fprintf(image, "%d", pbm_img.matrix[i][j]);
    }
    fprintf(image, "%s", "\n");
  }
}

//  Save image (pgm-text format)
static void pgm_saveText(FILE *image, pgm pgm_img) {
  pgm_img.width = pgm_img.x2 - pgm_img.x1;
  pgm_img.height = pgm_img.y2 - pgm_img.y1;
  fprintf(image, "%s\n%d %d\n%lld\n", "P2", pgm_img.width, pgm_img.height,
          pgm_img.max_intensity);

  for (uint i = pgm_img.y1; i < pgm_img.y2; i++) {
    for (uint j = pgm_img.x1; j < pgm_img.x2; j++) {
      if (j + 1 < pgm_img.x2 && pgm_img.matrix[i][j + 1] > 9)
        fprintf(image, "%d ", pgm_img.matrix[i][j]);
      else if (j + 1 < pgm_img.x2)
        fprintf(image, "%d  ", pgm_img.matrix[i][j]);
      else
        fprintf(image, "%d", pgm_img.matrix[i][j]);
    }
    fprintf(image, "%s", "\n");
  }
}

//  Save image (ppm-text format)
static void ppm_saveText(FILE *image, ppm ppm_img) {
  ppm_img.width = ppm_img.x2 - ppm_img.x1;
  ppm_img.height = ppm_img.y2 - ppm_img.y1;
  fprintf(image, "%s\n%d %d\n%lld\n", "P3", ppm_img.width, ppm_img.height,
          ppm_img.max_intensity);

  uint rgb = 3;
  for (uint i = ppm_img.y1; i < ppm_img.y2; i++) {
    for (uint k = ppm_img.x1; k < ppm_img.x2; k++) {
      for (uint j = 0; j < rgb; j++) {
        if ((j + 1 < rgb && ppm_img.matrix[k][i][j + 1] > 9) ||
            (k + 1 < ppm_img.x2 && ppm_img.matrix[k + 1][i][0] > 9 &&
             j + 1 == rgb))
          fprintf(image, "%d ", ppm_img.matrix[k][i][j]);
        else if (j + 1 < rgb || k + 1 < ppm_img.x2)
          fprintf(image, "%d  ", ppm_img.matrix[k][i][j]);
        else
          fprintf(image, "%d", ppm_img.matrix[k][i][j]);
      }
      if (k + 1 < ppm_img.x2) fprintf(image, "%s", "  ");
    }
    fprintf(image, "%s", "\n");
  }
}

//  Save image (pbm-binary format)
static void pbm_saveBin(FILE *image, pbm pbm_img) {
  pbm_img.width = pbm_img.x2 - pbm_img.x1;
  pbm_img.height = pbm_img.y2 - pbm_img.y1;
  fprintf(image, "%s\n%d %d\n", "P4", pbm_img.width, pbm_img.height);

  uch aux_write = 0;
  for (ull i = 0; i < pbm_img.height; i++)
    for (ull j = 0; j < pbm_img.width; j++) {
      aux_write = aux_write | (((pbm_img.matrix[i][j]) & 1) << (7 - (j % 8)));

      if ((j + 1) % 8 == 0 || j == (pbm_img.width - 1)) {
        fwrite(&aux_write, sizeof(aux_write), 1, image);
        aux_write = 0;
      }
    }
}

//  Save image (pgm-binary format)
static void pgm_saveBin(FILE *image, pgm pgm_img) {
  pgm_img.width = pgm_img.x2 - pgm_img.x1;
  pgm_img.height = pgm_img.y2 - pgm_img.y1;
  fprintf(image, "%s\n%d %d\n%lld\n", "P5", pgm_img.width, pgm_img.height,
          pgm_img.max_intensity);

  for (uint i = pgm_img.y1; i < pgm_img.y2; i++)
    for (uint j = pgm_img.x1; j < pgm_img.x2; j++)
      fwrite(&pgm_img.matrix[i][j], sizeof(pgm_img.matrix[i][j]), 1, image);
}

//  Save image (ppm-binary format)
static void ppm_saveBin(FILE *image, ppm ppm_img) {
  ppm_img.width = ppm_img.x2 - ppm_img.x1;
  ppm_img.height = ppm_img.y2 - ppm_img.y1;
  fprintf(image, "%s\n%d %d\n%lld\n", "P6", ppm_img.width, ppm_img.height,
          ppm_img.max_intensity);

  uint rgb = 3;
  for (uint i = ppm_img.y1; i < ppm_img.y2; i++)
    for (uint k = ppm_img.x1; k < ppm_img.x2; k++)
      for (uint j = 0; j < rgb; j++)
        fwrite(&ppm_img.matrix[k][i][j], sizeof(ppm_img.matrix[k][i][j]), 1,
               image);
}

//  Route save (binary format)
int save_bin(char **filename, FILE **image, pbm pbm_img, pgm pgm_img,
             ppm ppm_img, uint focus) {
  switch (focus) {
    case 1:
      pbm_saveBin(*image, pbm_img);
      break;

    case 2:
      pgm_saveBin(*image, pgm_img);
      break;

    case 3:
      ppm_saveBin(*image, ppm_img);
      break;

    default:
      printf("\"Focus is invalid\"");
      break;
  }

  footer(filename, image);

  return 1;
}

//  Route save (text format)
int save_text(char **filename, FILE **image, pbm pbm_img, pgm pgm_img,
              ppm ppm_img, uint focus) {
  switch (focus) {
    case 1:
      pbm_saveText(*image, pbm_img);
      break;

    case 2:
      pgm_saveText(*image, pgm_img);
      break;

    case 3:
      ppm_saveText(*image, ppm_img);
      break;

    default:
      printf("\"Focus is invalid\"");
      break;
  }

  footer(filename, image);

  return 1;
}
