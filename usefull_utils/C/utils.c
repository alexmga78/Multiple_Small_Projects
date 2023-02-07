#include "utils.h"

void swap_int(int number_list[], int a, int b)
{
    int temp = number_list[a];
    number_list[a] = number_list[b];
    number_list[b] = temp;
}


void sort_ascending(int number_list[], int low, int high)
{
    int pivot = low, value1 = low, value2 = high;

    if (low < high)
    {
        while(value1 < value2)
        {
            while(number_list[value1] <= number_list[pivot] && value1 <= high)
            {
                value1++;
            }
            while(number_list[value2] > number_list[pivot] && value2 >= low)
            {
                value2--;
            }
            if (value1 < value2)
            {
                swap_int (number_list, value1, value2);
            }

        }
    swap_int (number_list, value2, pivot);

    sort_ascending(number_list, low, value2 - 1);
    sort_ascending(number_list, value2 + 1, high);
    }
}


/*
size_t sizeof_int(int number_list[])
{
    //printf("%d %d\n\n", (&number_list)[1], number_list);
    size_t number_length = (&number_list)[1] - number_list;
    return number_length;
}
*/

int search_binary(int number_list[], int number_search, int number_length)
{
    int low = 0, i = number_length / 2, high = number_length;    
    while (number_list[i] != number_search && i>0 && i<number_length-1)
    {
        i = (low + high) / 2;
        if (number_list[i] > number_search)
        {   
            high = i;
            continue;
        }
        if(number_list[i] < number_search)
        {
            low = i;
            continue;
        }
    }
    if (number_list[i] == number_search)
        return i + 1;   
    else
        return -1;
}


void merging_arrays(int array1[], int array1_length, int array2[], int array2_length, int array_final[], int array_final_length) {
    int i = 0, j = 0, k = 0;
    while (k < array_final_length) {
        if (array1[i] > array2[j]) {
            array_final[k] = array2[j];
            j++;
        }else {
            array_final[k] = array1[i];
            i++;
        }
        k++;
    }
}

char* inmultire_char(char numar1[], char numar2[]) {
    static char rezultat[MAX];
    char intermediar[MAX], c[MAX];
    int lungime_numar1 = strlen(numar1)-1, lungime_numar2 = strlen(numar2)-1;
    int i, j, k = 0, x = 0, y;
    long int rest;
    long suma;

    // memset(rezultat, 0, strlen(rezultat));
    // memset(intermediar, 0, strlen(intermediar));
    // memset(c, 0, strlen(c));

    for (i = 0; i <= lungime_numar1; i++) {
        numar1[i] -= 48;
    }
    for (i = 0; i <= lungime_numar2; i++) {
        numar2[i] -= 48;
    }

    for (i = lungime_numar2; i >= 0; i--) {
        rest = 0;
        for (j = lungime_numar1; j >= 0; j--) {
            intermediar[k++] = (numar2[i]*numar1[j] + rest)%10;
            rest = (numar2[i]*numar1[j] + rest)/10;
        }
        intermediar[k++] = rest;
        x++;
        for (y = 0; y < x; y++) {
            intermediar[k++] = 0;
        }
    }

    k = 0;
    rest = 0;
    
    for (i = 0; i < lungime_numar1 + lungime_numar2 + 2; i++) {
        suma = 0;
        y = 0;
        for (j = 1; j <= lungime_numar2 + 1; j++) {
            if (i <= lungime_numar1 + j) {
                suma += intermediar[y+i];
            }
            y += j + lungime_numar1 + 1;
        }
        c[k++] = (suma + rest)%10;
        rest = (suma + rest)/10;
    }
    c[k] = rest;

    i = k-1;
    while (c[i] == 0){
        k--;
        i--;
    }

    j = 0;
    for (i = k-1; i>= 0; i--) {
        if (i != k-1 || c[i] != 0) {
            rezultat[j++] = c[i] + 48;
        }
    }
    rezultat[j] = '\0';

    for (i = 0; i <= lungime_numar1; i++) {
        numar1[i] += 48;
    }
    for (i = 0; i <= lungime_numar2; i++) {
        numar2[i] += 48;
    }

    return rezultat;
}

char* cat_impartire_fara_rest_char(char numar1[], unsigned long long numar2) {
    static char rezultat[MAX];
    int lungime_numar1 = strlen(numar1);
    int i, k = 0, prima_cifra_atasata = 0;
    unsigned long temp = 1, rest;

    // memset(rezultat, 0, strlen(rezultat));

    for (i = 0; i <= lungime_numar1; i++) {
        numar1[i] -= 48;
    }

    temp = numar1[0];
    rest = numar1[0];
    for (i = 1; i <= lungime_numar1; i++) {
        if (temp >= numar2) {
            rezultat[k++] = temp/numar2;
            rest = temp%numar2;
            temp = (temp%numar2)*10 + numar1[i];
            prima_cifra_atasata = 1;
        }else {
            rest = temp;
            temp = temp*10 + numar1[i];
            if (prima_cifra_atasata == 1) {
                rezultat[k++] = 0;
            }
        }
    }

    for (i = 0; i < k; i++) {
        rezultat[i] += 48;
    }

    rezultat[i] = '\0';

    for (i = 0; i <= lungime_numar1; i++) {
        numar1[i] += 48;
    }

    if (!rest) {
        return rezultat;
    }else {
        return 0;
    }
}

char* cat_impartire_char(char numar1[], unsigned long long numar2) {
    static char rezultat[MAX];
    int lungime_numar1 = strlen(numar1);
    int i, k = 0, prima_cifra_atasata = 0;
    unsigned long temp = 1, rest;

    // memset(rezultat, 0, strlen(rezultat));

    for (i = 0; i <= lungime_numar1; i++) {
        numar1[i] -= 48;
    }

    temp = numar1[0];
    rest = numar1[0];
    for (i = 1; i <= lungime_numar1; i++) {
        if (temp >= numar2) {
            rezultat[k++] = temp/numar2;
            rest = temp%numar2;
            temp = (temp%numar2)*10 + numar1[i];
            prima_cifra_atasata = 1;
        }else {
            rest = temp;
            temp = temp*10 + numar1[i];
            if (prima_cifra_atasata == 1) {
                rezultat[k++] = 0;
            }
        }
    }

    for (i = 0; i < k; i++) {
        rezultat[i] += 48;
    }

    rezultat[i] = '\0';

    for (i = 0; i <= lungime_numar1; i++) {
        numar1[i] += 48;
    }

    return rezultat;
}

int rest_impartire_char(char numar1[], unsigned long long numar2) {
    int lungime_numar1 = strlen(numar1);
    int i, k = 0;
    unsigned long temp = 1, rest;

    // memset(rezultat, 0, strlen(rezultat));

    for (i = 0; i <= lungime_numar1; i++) {
        numar1[i] -= 48;
    }

    temp = numar1[0];
    rest = numar1[0];
    for (i = 1; i <= lungime_numar1; i++) {
        if (temp >= numar2) {
            rest = temp%numar2;
            temp = (temp%numar2)*10 + numar1[i];
        }else {
            rest = temp;
            temp = temp*10 + numar1[i];
        }
    }

    for (i = 0; i <= lungime_numar1; i++) {
        numar1[i] += 48;
    }

    return rest;
}

char* adunare_char(char numar1[], char numar2[]) {
    static char rezultat[MAX];
    char intermediar[MAX];
    int lungime_numar1 = strlen(numar1)-1, lungime_numar2 = strlen(numar2)-1, lungime_maxima_util = -1;
    int i, j, k = 0, rest = 0, n1 = lungime_numar1, n2 = lungime_numar2;

    // memset(rezultat, 0, strlen(rezultat));
    // memset(intermediar, 0, strlen(intermediar));

    for (i = 0; i <= lungime_numar1; i++) {
        numar1[i] -= 48;
    }
    for (i = 0; i <= lungime_numar2; i++) {
        numar2[i] -= 48;
    }

    if (lungime_numar1 > lungime_numar2) {
        lungime_maxima_util = lungime_numar2;
    }else {
        lungime_maxima_util = lungime_numar1;
    }

    for (i = 0; i <= lungime_maxima_util; i++) {
        intermediar[k++] = (numar2[n2] + numar1[n1] + rest)%10;
        rest = (numar2[n2--] + numar1[n1--] + rest)/10;
    }

    if (n2 < 0 && lungime_numar1 != lungime_numar2) {
        intermediar[k++] = numar1[n1--] + rest;
        rest = 0;
        for (i = 0; i < lungime_numar1-lungime_maxima_util-1; i++) {
            intermediar[k++] = numar1[n1--];
        }
    }else if (lungime_numar1 != lungime_numar2) {
        intermediar[k++] = numar2[n2--] + rest;
        rest = 0;
        for (i = 0; i < lungime_numar2-lungime_maxima_util-1; i++) {
            intermediar[k++] = numar2[n2--];
        }
    }

    if (rest != 0) {
        intermediar[k++] += rest;
    }

    j = 0;
    for (i = k-1; i >= 0; i--) {
        rezultat[j++] = intermediar[i] + 48;
    }

    rezultat[j] = '\0';

    for (i = 0; i <= lungime_numar1; i++) {
        numar1[i] += 48;
    }
    for (i = 0; i <= lungime_numar2; i++) {
        numar2[i] += 48;
    }

    return rezultat;
}

char* ridicare_la_putere_char(char numar[], int putere) {
    static char rezultat[MAX];
    int i;

    memset(rezultat, 0, strlen(rezultat));

    rezultat[0] = 49;
    
    for (i = 0; i < putere; i++) {
        memcpy(rezultat, inmultire_char(rezultat, numar), strlen(inmultire_char(rezultat, numar)) + 1);
    }

    return rezultat;
}

int maximul_dintre_2_numere_char(char numar1[], char numar2[]) {
    int i, lungime_numar1 = strlen(numar1)-1, lungime_numar2 = strlen(numar2)-1;
        
    if (lungime_numar1 > lungime_numar2) {
        return 1;
    }else if (lungime_numar1 < lungime_numar2) {
        return 0;
    }

    for (i = 0; i <= lungime_numar1; i++) {
        if (numar1[i] > numar2[i]) {
            return 1;
        }else if (numar1[i] < numar2[i]) {
            return 0;
        }
    }
    return -1;
}

char* conversion_baseY_to_base10_char(char numar_in_bazaY[], int baseY) {
    static char numar_in_baza10[MAX];
    char intermediar[MAX], valoare_baza_la_putere[MAX];
    int lungime_numar_in_bazaY = strlen(numar_in_bazaY)-1;
    int i, j;

    char cifra[MAX], bazaY[MAX];

    memset(intermediar, 0, strlen(intermediar));
    memset(bazaY, 0, strlen(bazaY));
    
    for (i = 0; i <= lungime_numar_in_bazaY; i++) {
        numar_in_bazaY[i] -= 48;
    }

    valoare_baza_la_putere[0] = 1 + '0';
    bazaY[0] = baseY + '0';
    intermediar[0] = numar_in_bazaY[lungime_numar_in_bazaY] + '0';

    for (i = lungime_numar_in_bazaY - 1; i >= 0; i--) {
        cifra[0] = numar_in_bazaY[i] + '0';

        memcpy(valoare_baza_la_putere, ridicare_la_putere_char(bazaY, lungime_numar_in_bazaY - i), strlen(ridicare_la_putere_char(bazaY, lungime_numar_in_bazaY - i)) + 1);

        memcpy(intermediar, adunare_char(intermediar, inmultire_char(cifra, valoare_baza_la_putere)), strlen(adunare_char(intermediar, inmultire_char(cifra, valoare_baza_la_putere))) + 1);
    }

    j = 0;
    for (i = 0; i < strlen(intermediar); i++) {
        numar_in_baza10[j++] = intermediar[i];
    }

    for (i = 0; i <= lungime_numar_in_bazaY; i++) {
        numar_in_bazaY[i] += 48;
    }

    return numar_in_baza10;
}

char* conversion_base10_to_baseY_char(char numar_in_baza10[], int bazaY) {
    static char numar_in_bazaY[MAX];
    char intermediar[MAX];
    int lungime_numar_in_baza10 = strlen(numar_in_baza10)-1;
    int i, j, k = 0;
    int rest;

    rest = rest_impartire_char(numar_in_baza10, bazaY);
    memcpy(numar_in_baza10, cat_impartire_char(numar_in_baza10, bazaY), strlen(cat_impartire_char(numar_in_baza10, bazaY)) + 1);

    while(rest > 0) {
        intermediar[k++] = rest;
        rest = rest_impartire_char(numar_in_baza10, bazaY);
        memcpy(numar_in_baza10, cat_impartire_char(numar_in_baza10, bazaY), strlen(cat_impartire_char(numar_in_baza10, bazaY)) + 1);
    }

    j = 0;
    for (i = k-1; i >= 0; i--) {
        numar_in_bazaY[j++] = intermediar[i] + '0';
    }

    return numar_in_bazaY;
}

void setFileCursor_xLines_fromEnd(FILE* file, int number_of_lines) {
  fseek(file, 0, SEEK_END);
  char line[400];
  int setCursor_toStart = 0, bytes = 0, file_size = ftell(file);

  for (int i = 0; i < number_of_lines; i++) {
    fseek(file, bytes, SEEK_CUR);
    bytes = -2;
    while (line[0] != 10) {
      fseek(file, bytes, SEEK_CUR);
      fgets(line, 400, file);
      bytes--;

      if (abs(bytes) > file_size) {
        setCursor_toStart = 1;
        break;
      }
    }
    fgets(line, 400, file);
  }
  fseek(file, bytes + 2, SEEK_CUR);

  if (setCursor_toStart) fseek(file, 0, SEEK_SET);
}

void setFileCursor_xCharacters_fromEnd(FILE* file, int number_of_characters) {
  char character;
  int lineCounter = 0;

  fseek(file, -number_of_characters, SEEK_END);
  while (fscanf(file, "%c", &character) != EOF)
    if (character == 10) lineCounter++;
  fseek(file, -(number_of_characters + lineCounter), SEEK_END);
}

void setFileCursor_xBlocks512_fromEnd(FILE* file, int number_of_blocks512) {
  char character;
  int number_of_characters = 512, lineCounter = 0;

  fseek(file, -(number_of_blocks512 * number_of_characters), SEEK_END);
  while (fscanf(file, "%c", &character) != EOF)
    if (character == 10) lineCounter++;
  fseek(file, -(number_of_blocks512 * number_of_characters + lineCounter),
        SEEK_END);
}

int toArabic(char *number) {
  int year = 0;

  for (unsigned int i = 0; i < strlen(number); i++) switch (number[i]) {
      case 77: //M
        year += 1000;
        break;

      case 67: //C
        if (number[i + 1] != 77)
          year += 100;
        else {
          year += 900;
          i++;
        }
        break;

      case 88: //X
        if (number[i + 1] != 67)
          year += 10;
        else {
          year += 90;
          i++;
        }
        break;

      case 86: //V
        year += 5;
        break;

      case 73: //I
        if (number[i + 1] != 86 && number[i + 1] != 88)
          year += 1;
        else if (number[i + 1] == 88) {
          year += 9;
          i++;
        } else if (number[i + 1] == 86) {
          year += 4;
          i++;
        }
        break;

      default:
        break;
    }

  return year;
}
