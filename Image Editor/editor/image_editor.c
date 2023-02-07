#include "image_editor.h"

//  Opens the file in read mode (defensive)
static int open_file_read(char *filename, FILE **image) {
  *image = fopen(filename, "r");

  if (!*image) {
    fprintf(stderr, "Failed to load %s\n", filename);
    free(filename);
    return -1;
  }

  return 1;
}

//  Opens the file in write mode (defensive)
static int open_file_write(char *filename, FILE **image) {
  *image = fopen(filename, "w");

  if (!*image) {
    fprintf(stderr, "Failed to load %s\n", filename);
    free(filename);
    return -1;
  }

  return 1;
}

//  Initializes with 0 "loaded" object's variable (loaded keeps track if that
//  type of image has one allocated)
static void initialize_images(pbm *pbm_img, pgm *pgm_img, ppm *ppm_img) {
  (*pbm_img).loaded = 0;
  (*pgm_img).loaded = 0;
  (*ppm_img).loaded = 0;
}

//  Frees pbm/pgm/ppm _img memory
static void free_images(pbm *pbm_img, pgm *pgm_img, ppm *ppm_img) {
  //  Frees pbm image memory if it has been loaded
  if ((*pbm_img).loaded) {
    for (uint i = (*pbm_img).height_start; i < (*pbm_img).height; i++)
      free((*pbm_img).matrix[i]);
    free((*pbm_img).matrix);
    (*pbm_img).loaded = 0;
  }

  //  Frees pgm image memory if it has been loaded
  if ((*pgm_img).loaded) {
    for (uint i = (*pgm_img).height_start; i < (*pgm_img).height; i++)
      free((*pgm_img).matrix[i]);
    free((*pgm_img).matrix);
    (*pgm_img).loaded = 0;
  }

  //  Frees ppm image memory if it has been loaded
  if ((*ppm_img).loaded) {
    for (uint k = (*ppm_img).width_start; k < (*ppm_img).width; k++) {
      for (uint i = (*ppm_img).height_start; i < (*ppm_img).height; i++)
        free(((*ppm_img).matrix)[k][i]);
      free((*ppm_img).matrix[k]);
    }
    free((*ppm_img).matrix);
    (*ppm_img).loaded = 0;
  }
}

//  Removes comments from the image file
static void remove_comments(FILE *image) {
  char verify_comment[1024];
  fscanf(image, " %s", verify_comment);
  if (verify_comment[0] == '#') {
    fseek(image, 0L, SEEK_SET);
    while (verify_comment[0] == '#') {
      fgets(verify_comment, 1024, image);
    }
  }
  fseek(image, -strlen(verify_comment), SEEK_CUR);
}

//  Get the type of command (transform the command in integer)
static int get_type(char command[20]) {
  if (!strcmp(command, "LOAD")) return 1;
  if (!strcmp(command, "SELECT")) return 2;
  if (!strcmp(command, "HISTOGRAM")) return 3;
  if (!strcmp(command, "EQUALIZE")) return 4;
  if (!strcmp(command, "ROTATE")) return 5;
  if (!strcmp(command, "CROP")) return 6;
  if (!strcmp(command, "APPLY")) return 7;
  if (!strcmp(command, "SAVE")) return 8;
  if (!strcmp(command, "EXIT")) return 9;

  return 0;
}

//  The switch for "load" routes
static int load_routes(pbm *pbm_img, pgm *pgm_img, ppm *ppm_img, uint *focus) {
  uint filename_max_letters = 30, type = 0;
  char *filename = (char *)malloc(filename_max_letters * sizeof(char));

  free_images(pbm_img, pgm_img, ppm_img);

  if (!filename) {
    fprintf(stderr, "Failed to alloc memory for 'filename'\n");
    exit(500);
  }
  scanf("%s", filename);

  FILE *image = NULL;
  if (open_file_read(filename, &image) == -1) return -1;

  char type_aux[2];
  remove_comments(image);
  fscanf(image, "%s", type_aux);
  type = type_aux[1] - 48;

  switch (type) {
    case 1:
      *focus = 1;
      pbm_loadText(&filename, &image, pbm_img);
      break;
    case 2:
      *focus = 2;
      pgm_loadText(&filename, &image, pgm_img);
      break;

    case 3:
      *focus = 3;
      ppm_loadText(&filename, &image, ppm_img);
      break;

    case 4:
      *focus = 1;
      pbm_loadBin(&filename, &image, pbm_img);
      break;

    case 5:
      *focus = 2;
      pgm_loadBin(&filename, &image, pgm_img);
      break;

    case 6:
      *focus = 3;
      ppm_loadBin(&filename, &image, ppm_img);
      break;

    default:
      break;
  }

  return 1;
}

//  The switch for "select" routes
static int select_routes(pbm *pbm_img, pgm *pgm_img, ppm *ppm_img, uint focus) {
  char command_aux[20];

  scanf("%s", command_aux);

  if (!strcmp(command_aux, "ALL"))
    select_all(pbm_img, pgm_img, ppm_img, focus);
  else {
    int x1;

    if (command_aux[0] != '-')
      x1 = command_aux[0] - 48;
    else {
      x1 = command_aux[1] - 48;
      x1 *= -1;
    }

    select(pbm_img, pgm_img, ppm_img, focus, x1);
  }

  return 1;
}

//  The switch for "histrogam" routes
static int histogram_routes(pbm pbm_img, pgm pgm_img, ppm ppm_img, uint focus) {
  //  Verifies if there is any image loaded
  if (no_image_loaded(pbm_img, pgm_img, ppm_img, focus) == -1) {
    uint aux;
    scanf(" %d %d", &aux, &aux);
    return -1;
  }

  //  Verifies if the loaded image is black&white (pgm)
  if (black_and_white_image(focus) == -1) {
    uint aux;
    scanf(" %d %d", &aux, &aux);
    return -1;
  }

  histogram(pgm_img);

  return 1;
}

//  The switch for "equalize" routes
static int equalize_routes(pbm pbm_img, pgm *pgm_img, ppm ppm_img, uint focus) {
  //  Verifies if there is any image loaded
  if (no_image_loaded(pbm_img, *pgm_img, ppm_img, focus) == -1) return -1;

  //  Verifies if the loaded image is black&white (pgm)
  if (black_and_white_image(focus) == -1) return -1;

  equalize(pgm_img);

  return 1;
}

//  The switch for "apply" routes
static int apply_routes(pbm pbm_img, pgm *pgm_img, ppm *ppm_img, uint focus) {
  char parameter[15];
  scanf(" %s", parameter);

  //  Verifies if there is any image loaded
  if (no_image_loaded(pbm_img, *pgm_img, *ppm_img, focus) == -1) return -1;

  if (!strcmp(parameter, "BLUR")) {
    apply_blur(pgm_img, ppm_img, focus);
  } else if (!strcmp(parameter, "EDGE")) {
    apply_edge(pgm_img, ppm_img, focus);
  } else if (!strcmp(parameter, "GAUSSIAN_BLUR")) {
    apply_gaussian_blur(pgm_img, ppm_img, focus);
  } else if (!strcmp(parameter, "SHARPEN")) {
    apply_sharpen(pgm_img, ppm_img, focus);
  } else {
    printf("APPLY parameter invalid\n");
  }

  return 1;
}

//  The switch for "save" routes
static int save_routes(pbm pbm_img, pgm pgm_img, ppm ppm_img, uint focus) {
  char parameters[50] = {0};
  uint filename_max_letters = 30, ascii = 0;

  //  Verifies if there is any image loaded
  if (no_image_loaded(pbm_img, pgm_img, ppm_img, focus) == -1) return -1;

  //  Reads filename
  char *filename = (char *)malloc(filename_max_letters * sizeof(char));
  if (!filename) {
    fprintf(stderr, "Failed to alloc memory for 'filename'\n");
    free_images(&pbm_img, &pgm_img, &ppm_img);
    exit(500);
  }
  scanf("%s", filename);

  //  Opens the file in write mode
  FILE *image = NULL;
  if (open_file_write(filename, &image) == -1) return -1;

  //  Identifies if "ascii" attribute is true
  fgets(parameters, 20, stdin);
  parameters[strlen(parameters)] = 0;
  if (parameters[1] == 'a' && parameters[2] == 's' && parameters[3] == 'c' &&
      parameters[4] == 'i' && parameters[5] == 'i' && parameters[6] == '\n')
    ascii = 1;

  switch (ascii) {
    case 0:
      save_bin(&filename, &image, pbm_img, pgm_img, ppm_img, focus);
      break;

    case 1:
      save_text(&filename, &image, pbm_img, pgm_img, ppm_img, focus);
      break;

    default:
      break;
  }

  return 1;
}

int main() {
  pbm pbm_img;
  pgm pgm_img;
  ppm ppm_img;
  uint focus = 0;
  char command[20] = {0};

  initialize_images(&pbm_img, &pgm_img, &ppm_img);

  while (scanf("%s", command)) {
    switch (get_type(command)) {
      case 1:
        load_routes(&pbm_img, &pgm_img, &ppm_img, &focus);
        break;

      case 2:
        select_routes(&pbm_img, &pgm_img, &ppm_img, focus);
        break;

      case 3:
        histogram_routes(pbm_img, pgm_img, ppm_img, focus);
        break;

      case 4:
        equalize_routes(pbm_img, &pgm_img, ppm_img, focus);
        break;

      case 5:
        rotate(&pbm_img, &pgm_img, &ppm_img, focus);
        break;

      case 6:
        crop(&pbm_img, &pgm_img, &ppm_img, focus);
        break;

      case 7:
        apply_routes(pbm_img, &pgm_img, &ppm_img, focus);
        break;

      case 8:
        save_routes(pbm_img, pgm_img, ppm_img, focus);
        break;

      case 9:
        if (no_image_loaded(pbm_img, pgm_img, ppm_img, focus) == 1) {
          free_images(&pbm_img, &pgm_img, &ppm_img);
          exit(0);
        }
        break;

      default:
        printf("Invalid command\n");
        break;
    }
  }

  return 0;
