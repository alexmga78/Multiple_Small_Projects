// #pragma once
#ifndef __utils_h__
#define __utils_h__

#include <stdio.h>

void swap_int(int number_list[], int a, int b);
void sort_ascending(int number_list[], int low, int high);
int search_binary(int number_list[], int number_search, int number_length);
void merging_arrays(int array1[], int array1_length, int array2[], int array2_length, int array_final[], int array_final_length);
char* inmultire_char(char numar1[], char numar2[]);
char* cat_impartire_fara_rest_char(char numar1[], unsigned long long numar2);
char* cat_impartire_char(char numar1[], unsigned long long numar2);
int rest_impartire_char(char numar1[], unsigned long long numar2);
char* adunare_char(char numar1[], char numar2[]);
char* ridicare_la_putere_char(char numar[], int putere);
int maximul_dintre_2_numere_char(char numar1[], char numar2[]);
char* conversion_baseY_to_base10_char(char numar_in_bazaY[], int baseY);
char* conversion_base10_to_baseY_char(char numar_in_baza10[], int bazaY);
#endif
