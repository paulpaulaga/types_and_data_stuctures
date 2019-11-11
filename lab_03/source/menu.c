//
// Created by Павел Чеклин on 21.10.2019.
//

#include "menu.h"

#include <stdio.h>

#include "err.h"

void print_menu()
{
	printf("\n\n\n**********************************************************************************\n\n"
		   "Программа для выполнения действия с разреженными матрицами\n"
		   "Возможные действия:\n\n"
		   "Ввод:\n"
		   "1 - Ввести первую матрицу\n" // ввод
		   "2 - Ввести вторую матрицу\n"
		   "3 - Ввести вектор-стобец\n\n"
		   "Вывод:\n"
		   "4 - Вывести первую матрицу в полном  виде\n" // вывод
		   "5 - Вывести вторую матрицу\n"
		   "6 - Вывести вектор-столбец\n\n"
		   "Действия с матрицами:\n"
		   "7 - Вывести первую матрицу в разреженном виде (три массива)\n" // разреженный вывод
		   "8 - Вывести вектор-стобец в разреженном виде\n"
		   "9 - Перемножить матрицу и вектор-стобец (в разреженном виде)\n" // операции
		   "10 - Перемножить первую матрицу на вторую матрицу (с переводом матриц в полный вид)\n\n"
		   "**********************************************************************************\n\n"
		   "q - Выход из программы\n"
		   "i - Вывести информацию о программе\n\n"
		   "**********************************************************************************\n\n"
		   "Введите действие: ");
}

void print_info()
{
	"\n\n\n**********************************************************************************\n\n"
		   "Программа для выполнения действия с разреженными матрицами\n"
		   "Возможные действия:\n\n"
		   "Ввод:\n"
		   "1 - Ввести первую матрицу\n" // ввод
		   "2 - Ввести вторую матрицу\n"
		   "3 - Ввести вектор-стобец\n\n"
		   "Вывод:\n"
		   "4 - Вывести первую матрицу в полном  виде\n" // вывод
		   "5 - Вывести вторую матрицу\n"
		   "6 - Вывести вектор-столбец\n\n"
		   "Действия с матрицами:\n"
		   "7 - Вывести первую матрицу в разреженном виде (три массива)\n" // разреженный вывод
		   "8 - Вывести вектор-стобец в разреженном виде\n"
		   "9 - Перемножить матрицу и вектор-стобец (в разреженном виде)\n" // операции
		   "10 - Перемножить первую матрицу на вторую матрицу (с переводом матриц в полный вид)\n\n"
		   "**********************************************************************************\n\n"
		   "q - Выход из программы\n"
		   "i - Вывести информацию о программе\n\n"
		   "**********************************************************************************\n\n"
		   "Введите действие: ");
}

int handle_action(int action)
{
	switch (action)
	{
	case 'q':
		return MENU_EXIT;
		break;
	case 'i':
		print_info();
		break;
	case '\n':
		break;
	default:
		return MENU_UNDEFINED;
	}
}

int menu_mainloop()
{
	int err = OK, action;

	while (err == OK)
	{
		print_menu();
		action = getchar();
		err = handle_action(action);
	}
	return err;
}