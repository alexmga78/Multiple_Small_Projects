#include "image_editor.h"

//  Swaps 2 int numbers
void swap(int *number1, int *number2) {
  int aux = *number1;
  *number1 = *number2;
  *number2 = aux;
}

//  Swaps 2 unsigned int numbers
void swap_uint(uint *number1, uint *number2) {
  uint aux = *number1;
  *number1 = *number2;
  *number2 = aux;
}
