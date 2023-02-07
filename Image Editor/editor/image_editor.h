//  #pragma once ×
#ifndef __image_editor_h__
#define __image_editor_h__

#define uint unsigned int
#define ull unsigned long long
#define uch unsigned char

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  .pbm image
typedef struct pbm {
  uint x1, x2, y1, y2;
  uint loaded;
  uint width_start;
  uint height_start;
  uint width;
  uint height;
  uch **matrix;
} pbm;

//  .pgm image
typedef struct pgm {
  uint x1, x2, y1, y2;
  uint loaded;
  uint width_start;
  uint height_start;
  uint width;
  uint height;
  ull max_intensity;
  uch **matrix;
} pgm;

//  .ppm image
typedef struct ppm {
  uint x1, x2, y1, y2;
  uint loaded;
  uint width_start;
  uint height_start;
  uint width;
  uint height;
  ull max_intensity;
  uch ***matrix;
} ppm;

//  apply.c
int apply_blur();
int apply_edge();
int apply_gaussian_blur();
int apply_sharpen();

//  crop.c
int crop();

//  exit_program.c
void exit_program();

//  equalize.c
int equalize();

//  histrogram.c
int histogram();

//  load.c
int pbm_loadText();
int pgm_loadText();
int ppm_loadText();
int pbm_loadBin();
int pgm_loadBin();
int ppm_loadBin();

//  rotate.c
int rotate();

//  save.c
int save_bin();
int save_text();

//  select.c
int select();

//  select_all.c
int select_all();

//  utils.c
void swap();
void swap_uint();

//  warnings.c
int black_and_white_image();
int invalid_apply_parameter();
int invalid_coordinates();
int no_image_loaded();
int unsupported_angle();

#endif
