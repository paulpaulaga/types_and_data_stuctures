#ifndef _SMATR_INPUT_H_
#define _SMATR_INPUT_H_

#include "smatrix.h"

int s_matr_full_input(s_matr *m, FILE *f);

int s_matr_input_my();

int s_matr_column_input(s_matr *c, int rows );

int s_matr_matrix_input(s_matr *m);

int s_matr_add_elem(s_matr *m, int buf, int row, int column, int new_line);

int s_matr_add_line(s_matr *m);

int s_matr_close(s_matr *m);

void s_matr_noutput(s_matr m);

void print_zeros(FILE *f, int c);

int input_row(FILE *f, int c);

#endif
