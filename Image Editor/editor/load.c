#include "image_editor.h"

//  Removes comments from the image file
static void remove_comments(FILE *image) {
  char verify_comment[1024];
  fscanf(image, " %s", verify_comment);
  if (verify_comment[0] == '#') {
    fseek(image, -strlen(verify_comment), SEEK_CUR);
    while (verify_comment[0] == '#') {
      fgets(verify_comment, 1024, image);
    }
  }
  fseek(image, -strlen(verify_comment), SEEK_CUR);
}

// static void remove_comments_binary(FILE *image) {
//   char verify_comment[1024];
//   // fscanf(image, " %s", verify_comment);
//   fread(verify_comment, sizeof(verify_comment), 1, image);
//   // fread(&pbm_img.matrix[i][j], sizeof(pbm_img.matrix[i][j]), 1, image);
//   if (verify_comment[0] == '#') {
//     fseek(image, -strlen(verify_comment), SEEK_CUR);
//     while (verify_comment[0] == '#') {
//       fgets(verify_comment, 1024, image);
//     }
//   }
//   fseek(image, -strlen(verify_comment), SEEK_CUR);
// }

//  Frees memory and gives a message (successful)
static void footer(char **filename, FILE **image) {
  printf("Loaded %s\n", *filename);
  free(*filename);
  fclose(*image);
}

//  Frees memory and closes the program when an allocations fails
static void free_and_exit(char **filename, FILE **image) {
  free(*filename);
  free(*image);
  exit(500);
}

//  Load image (pbm-text format)
static pbm loadText_pbm(char **filename, FILE **image) {
  pbm pbm_img;

  //  Reads image characteristics
  remove_comments(*image);
  fscanf(*image, " %d", &pbm_img.width);
  remove_comments(*image);
  fscanf(*image, " %d", &pbm_img.height);

  pbm_img.width_start = 0;
  pbm_img.height_start = 0;

  pbm_img.x1 = 0;
  pbm_img.y1 = 0;
  pbm_img.x2 = pbm_img.width;
  pbm_img.y2 = pbm_img.height;

  //  Alloc lines
  pbm_img.matrix = (uch **)malloc(pbm_img.height * sizeof(uch *));
  if (!pbm_img.matrix) {
    fprintf(stderr, "Failed to alloc memory for 'pbm_img.matrix'\n");
    free_and_exit(filename, image);
  }

  //  Alloc columns
  for (uint i = 0; i < pbm_img.height; i++) {
    pbm_img.matrix[i] = (uch *)malloc(pbm_img.width * sizeof(uch));
    if (!pbm_img.matrix[i]) {
      fprintf(stderr, "Failed to alloc memory for 'pbm_img.matrix[%d]'\n", i);
      while (i > 0) free(pbm_img.matrix[--i]);
      free(pbm_img.matrix);
      free_and_exit(filename, image);
    }
  }

  //  Read pixels
  remove_comments(*image);
  for (uint i = 0; i < pbm_img.height; i++)
    for (uint j = 0; j < pbm_img.width; j++)
      fscanf(*image, " %hhd", &pbm_img.matrix[i][j]);

  return pbm_img;
}

//  Load image (pgm-text format)
static pgm loadText_pgm(char **filename, FILE **image) {
  pgm pgm_img;

  //  Reads image characteristics
  remove_comments(*image);
  fscanf(*image, " %d", &pgm_img.width);
  remove_comments(*image);
  fscanf(*image, " %d", &pgm_img.height);
  remove_comments(*image);
  fscanf(*image, " %lld", &pgm_img.max_intensity);

  pgm_img.width_start = 0;
  pgm_img.height_start = 0;

  pgm_img.x1 = 0;
  pgm_img.y1 = 0;
  pgm_img.x2 = pgm_img.width;
  pgm_img.y2 = pgm_img.height;

  //  Alloc lines
  pgm_img.matrix = (uch **)malloc(pgm_img.height * sizeof(uch *));
  if (!pgm_img.matrix) {
    fprintf(stderr, "Failed to alloc memory for 'pgm_img.matrix'\n");
    free_and_exit(filename, image);
  }

  //  Alloc columns
  for (uint i = 0; i < pgm_img.height; i++) {
    pgm_img.matrix[i] = (uch *)malloc(pgm_img.width * sizeof(uch));
    if (!pgm_img.matrix[i]) {
      fprintf(stderr, "Failed to alloc memory for 'pgm_img.matrix[%d]'\n", i);
      while (i > 0) free(pgm_img.matrix[--i]);
      free(pgm_img.matrix);
      free_and_exit(filename, image);
    }
  }

  //  Read pixels
  remove_comments(*image);
  for (uint i = 0; i < pgm_img.height; i++)
    for (uint j = 0; j < pgm_img.width; j++)
      fscanf(*image, " %hhd", &pgm_img.matrix[i][j]);

  return pgm_img;
}

//  Load image (ppm-text format)
static ppm loadText_ppm(char **filename, FILE **image) {
  ppm ppm_img;

  //  Reads image characteristics
  remove_comments(*image);
  fscanf(*image, " %d", &ppm_img.width);
  remove_comments(*image);
  fscanf(*image, " %d", &ppm_img.height);
  remove_comments(*image);
  fscanf(*image, " %lld", &ppm_img.max_intensity);

  ppm_img.width_start = 0;
  ppm_img.height_start = 0;

  ppm_img.x1 = 0;
  ppm_img.y1 = 0;
  ppm_img.x2 = ppm_img.width;
  ppm_img.y2 = ppm_img.height;

  //  Alloc rgb matrices
  ppm_img.matrix = (uch ***)malloc(ppm_img.width * sizeof(uch **));
  if (!ppm_img.matrix) {
    fprintf(stderr, "Failed to alloc memory for 'ppm_img.matrix'\n");
    free_and_exit(filename, image);
  }

  //  Alloc lines
  for (uint k = 0; k < ppm_img.width; k++) {
    ppm_img.matrix[k] = (uch **)malloc(ppm_img.height * sizeof(uch *));
    if (!ppm_img.matrix[k]) {
      fprintf(stderr, "Failed to alloc memory for 'ppm_img.matrix[%d]'\n", k);
      while (k > 0) free(ppm_img.matrix[--k]);
      free(ppm_img.matrix);
      free_and_exit(filename, image);
    }
  }

  //  Alloc columns
  uint rgb = 3;
  for (uint k = 0; k < ppm_img.width; k++)
    for (uint i = 0; i < ppm_img.height; i++) {
      ppm_img.matrix[k][i] = (uch *)malloc(rgb * sizeof(uch));
      if (!ppm_img.matrix[k][i]) {
        fprintf(stderr, "Failed to aloc memory for 'ppm_img.matrix[%d][%d]'\n",
                k, i);
        while (i > 0) free(ppm_img.matrix[k][--i]);
        for (uint l = k; l > 0; --l)
          for (uint j = ppm_img.height + 1; j > 0; --j)
            free(ppm_img.matrix[l][j]);
        free_and_exit(filename, image);
      }
    }

  //  Read rgb pixels
  remove_comments(*image);
  for (uint i = 0; i < ppm_img.height; i++)
    for (uint k = 0; k < ppm_img.width; k++)
      for (uint j = 0; j < rgb; j++)
        fscanf(*image, " %hhd", &ppm_img.matrix[k][i][j]);

  return ppm_img;
}

//  Load image (pbm-binary format)
static pbm loadBin_pbm(char **filename, FILE **image) {
  pbm pbm_img;

  //  Reads image characteristics
  remove_comments(*image);
  fscanf(*image, " %d", &pbm_img.width);
  remove_comments(*image);
  fscanf(*image, " %d", &pbm_img.height);

  pbm_img.width_start = 0;
  pbm_img.height_start = 0;

  pbm_img.x1 = 0;
  pbm_img.y1 = 0;
  pbm_img.x2 = pbm_img.width;
  pbm_img.y2 = pbm_img.height;

  //  Alloc lines
  pbm_img.matrix = (uch **)malloc(pbm_img.height * sizeof(uch *));
  if (!pbm_img.matrix) {
    fprintf(stderr, "Failed to alloc memory for 'pbm_img.matrix'\n");
    free_and_exit(filename, image);
  }

  //  Alloc columns
  for (uint i = 0; i < pbm_img.height; i++) {
    pbm_img.matrix[i] = (uch *)malloc(pbm_img.width * sizeof(uch));
    if (!pbm_img.matrix[i]) {
      fprintf(stderr, "Failed to alloc memory for 'pbm_img.matrix[%d]'\n", i);
      while (i > 0) free(pbm_img.matrix[--i]);
      free(pbm_img.matrix);
      free_and_exit(filename, image);
    }
  }

  //  TODO: remove_comments() - between ascii/binary file part
  // remove_comments(image);
  //  Read pixels
  uch aux_read;
  fread(&aux_read, sizeof(aux_read), 1, *image);
  for (uint i = 0; i < pbm_img.height; i++)
    for (uint j = 0; j < pbm_img.width; j++) {
      if (j % 8 == 0) fread(&aux_read, sizeof(aux_read), 1, *image);
      pbm_img.matrix[i][j] = (aux_read >> (7 - (j % 8))) & 1;
    }

  return pbm_img;
}

//  Load image (pgm-binary format)
static pgm loadBin_pgm(char **filename, FILE **image) {
  pgm pgm_img;

  //  Reads image characteristics
  remove_comments(*image);
  fscanf(*image, " %d", &pgm_img.width);
  remove_comments(*image);
  fscanf(*image, " %d", &pgm_img.height);
  remove_comments(*image);
  fscanf(*image, " %lld", &pgm_img.max_intensity);

  pgm_img.width_start = 0;
  pgm_img.height_start = 0;

  pgm_img.x1 = 0;
  pgm_img.y1 = 0;
  pgm_img.x2 = pgm_img.width;
  pgm_img.y2 = pgm_img.height;

  //  Alloc lines
  pgm_img.matrix = (uch **)malloc(pgm_img.height * sizeof(uch *));
  if (!pgm_img.matrix) {
    fprintf(stderr, "Failed to alloc memory for 'pgm_img.matrix'\n");
    free_and_exit(filename, image);
  }

  //  Alloc columns
  for (uint i = 0; i < pgm_img.height; i++) {
    pgm_img.matrix[i] = (uch *)malloc(pgm_img.width * sizeof(uch));
    if (!pgm_img.matrix[i]) {
      fprintf(stderr, "Failed to alloc memory for 'pgm_img.matrix[%d]'\n", i);
      while (i > 0) free(pgm_img.matrix[--i]);
      free(pgm_img.matrix);
      free_and_exit(filename, image);
    }
  }

  //  TODO: remove_comments() - between ascii/binary file part
  // remove_comments(image);
  //  Read pixels
  fread(&pgm_img.matrix[0][0], sizeof(pgm_img.matrix[0][0]), 1, *image);
  for (uint i = 0; i < pgm_img.height; i++)
    for (uint j = 0; j < pgm_img.width; j++)
      fread(&pgm_img.matrix[i][j], sizeof(pgm_img.matrix[i][j]), 1, *image);

  return pgm_img;
}

//  Load image (ppm-binary format)
static ppm loadBin_ppm(char **filename,
                       FILE **image /*With register (x2) - 2:58*/) {
  ppm ppm_img;  // With register (x2) - 2:58

  //  Reads image characteristics
  remove_comments(*image);
  fscanf(*image, " %d", &ppm_img.width);
  remove_comments(*image);
  fscanf(*image, " %d", &ppm_img.height);
  remove_comments(*image);
  fscanf(*image, " %lld", &ppm_img.max_intensity);

  ppm_img.width_start = 0;
  ppm_img.height_start = 0;

  ppm_img.x1 = 0;
  ppm_img.y1 = 0;
  ppm_img.x2 = ppm_img.width;
  ppm_img.y2 = ppm_img.height;

  //  Alloc rgb matrices
  ppm_img.matrix = (uch ***)malloc(ppm_img.width * sizeof(uch **));
  if (!ppm_img.matrix) {
    fprintf(stderr, "Failed to alloc memory for 'ppm_img.matrix'\n");
    free_and_exit(filename, image);
  }

  //  Alloc lines
  for (uint k = 0; k < ppm_img.width; k++) {
    ppm_img.matrix[k] = (uch **)malloc(ppm_img.height * sizeof(uch *));
    if (!ppm_img.matrix[k]) {
      fprintf(stderr, "Failed to alloc memory for 'ppm_img.matrix[%d]'\n", k);
      while (k > 0) free(ppm_img.matrix[--k]);
      free(ppm_img.matrix);
      free_and_exit(filename, image);
    }
  }

  //  Alloc columns
  uint rgb = 3;
  for (uint k = 0; k < ppm_img.width; k++)
    for (uint i = 0; i < ppm_img.height; i++) {
      ppm_img.matrix[k][i] = (uch *)malloc(rgb * sizeof(uch));
      if (!ppm_img.matrix[k][i]) {
        fprintf(stderr, "Failed to aloc memory for 'ppm_img.matrix[%d][%d]'\n",
                k, i);
        while (i > 0) free(ppm_img.matrix[k][--i]);
        free(ppm_img.matrix[k]);
        for (uint l = k; l > 0; --l)
          for (uint j = ppm_img.height + 1; j > 0; --j)
            free(ppm_img.matrix[l][j]);
        free_and_exit(filename, image);
      }
    }

  //  TODO: remove_comments() - between ascii/binary file part
  // remove_comments_binary(image);
  //  Read rgb pixels
  fread(&ppm_img.matrix[0][0][0], sizeof(ppm_img.matrix[0][0][0]), 1, *image);
  for (uint i = 0; i < ppm_img.height; i++)
    for (uint k = 0; k < ppm_img.width; k++)
      for (uint j = 0; j < rgb; j++)
        fread(&ppm_img.matrix[k][i][j], sizeof(ppm_img.matrix[k][i][j]), 1,
              *image);

  return ppm_img;
}

//  Route load (pbm-text format)
int pbm_loadText(char **filename, FILE **image, pbm *pbm_img) {
  //  Frees the memory in case there has been another 'pbm' image loaded prior
  if ((*pbm_img).loaded) {
    for (uint i = 0; i < (*pbm_img).height; i++) free((*pbm_img).matrix[i]);
    free((*pbm_img).matrix);
  }

  *pbm_img = loadText_pbm(filename, image);
  (*pbm_img).loaded = 1;

  footer(filename, image);

  return 1;
}

//  Route load (pgm-text format)
int pgm_loadText(char **filename, FILE **image, pgm *pgm_img) {
  //  Frees the memory in case there has been another 'pgm' image loaded prior
  if ((*pgm_img).loaded) {
    for (uint i = 0; i < (*pgm_img).height; i++) free((*pgm_img).matrix[i]);
    free((*pgm_img).matrix);
  }

  *pgm_img = loadText_pgm(filename, image);
  (*pgm_img).loaded = 1;

  footer(filename, image);

  return 1;
}

//  Route load (ppm-text format)
int ppm_loadText(char **filename, FILE **image, ppm *ppm_img) {
  //  Frees the memory in case there has been another 'ppm' image loaded prior
  if ((*ppm_img).loaded) {
    for (uint k = 0; k < (*ppm_img).width; k++) {
      for (uint i = 0; i < (*ppm_img).height; i++)
        free(((*ppm_img).matrix)[k][i]);
      free((*ppm_img).matrix[k]);
    }
    free((*ppm_img).matrix);
  }

  *ppm_img = loadText_ppm(filename, image);
  (*ppm_img).loaded = 1;

  footer(filename, image);

  return 1;
}

//  Route load (pbm-binary format)
int pbm_loadBin(char **filename, FILE **image, pbm *pbm_img) {
  //  Frees the memory in case there has been another 'pbm' image loaded prior
  if ((*pbm_img).loaded) {
    for (uint i = 0; i < (*pbm_img).height; i++) free((*pbm_img).matrix[i]);
    free((*pbm_img).matrix);
  }

  *pbm_img = loadBin_pbm(filename, image);
  (*pbm_img).loaded = 1;

  footer(filename, image);

  return 1;
}

//  Route load (pgm-binary format)
int pgm_loadBin(char **filename, FILE **image, pgm *pgm_img) {
  //  Frees the memory in case there has been another 'pgm' image loaded prior
  if ((*pgm_img).loaded) {
    for (uint i = 0; i < (*pgm_img).height; i++) free((*pgm_img).matrix[i]);
    free((*pgm_img).matrix);
  }

  *pgm_img = loadBin_pgm(filename, image);
  (*pgm_img).loaded = 1;

  footer(filename, image);

  return 1;
}

//  Route load (ppm-binary format)
int ppm_loadBin(char **filename, FILE **image, ppm *ppm_img) {
  //  Frees the memory in case there has been another 'ppm' image loaded prior
  if ((*ppm_img).loaded) {
    for (uint k = 0; k < (*ppm_img).width; k++) {
      for (uint i = 0; i < (*ppm_img).height; i++)
        free(((*ppm_img).matrix)[k][i]);
      free((*ppm_img).matrix[k]);
    }
    free((*ppm_img).matrix);
  }

  *ppm_img = loadBin_ppm(filename, image);
  (*ppm_img).loaded = 1;

  footer(filename, image);

  return 1;
}
