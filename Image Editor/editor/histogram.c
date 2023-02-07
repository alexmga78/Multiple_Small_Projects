#include "image_editor.h"

int histogram(pgm pgm_img) {
  uint stars_max, bins;
  uint occurences[257] = {0};
  uint bin_histogram[257] = {0};
  uint max_occurences = 0;

  scanf(" %d %d", &stars_max, &bins);

  uint bins_inside = 256 / bins;

  //  Calculate vector of frequence (of image intensities)
  for (uint i = pgm_img.y1; i < pgm_img.y2; i++)
    for (uint j = pgm_img.x1; j < pgm_img.x2; j++)
      occurences[pgm_img.matrix[i][j]]++;

  //  Calculates every bin's intensity sum
  for (uint i = 0; i < 256; i++)
    bin_histogram[i / bins_inside] += occurences[i];

  //  Gets the highest number of occurences of intensity
  for (uint i = 0; i < bins; i++)
    if (bin_histogram[i] > max_occurences) max_occurences = bin_histogram[i];

  //  If the input (maximum stars) exceeds the maximum number of occurences
  //  set the "stars_max" as maximum number of occurences
  if (stars_max > max_occurences) stars_max = max_occurences;

  //  Calculate number of stars and print histogram
  for (uint i = 0; i < bins; i++) {
    bin_histogram[i] = (bin_histogram[i] * stars_max) / max_occurences;
    printf("%d\t|\t", bin_histogram[i]);
    for (uint j = 0; j < bin_histogram[i]; j++) printf("*");
    printf("\n");
  }

  return 1;
}
