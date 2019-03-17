#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include <fstream>
#include <map>
using namespace std;


int main()
{
	setlocale(LC_ALL, "Russian"); // устанавливаем русскую локализацию
	map <char, int> SymbolMap; //массив для хранения символов и их количества в тексте 
	char otv; //переменная для хранения ответа оператора о читабельности текста
	int vh; //переменная для хранения процента вхдимости русских букв в текст
	DWORD dwNum; //число символов после преобразования кодировки
	DWORD num_symbol;//переменная для хранения количества символов русского алфавита
	ifstream fp("text.txt", ios::in); //открытие файла
	cout << "Please, wait..." << endl;
	if (fp.is_open()) 
	{
		// Если файл открыт успешно
		fp.seekg(0, fp.end);
		int length = fp.tellg();// Получение размера файла
		char *TextBuff = new char[length + 1]; //создаем текстовый массив
		fp.seekg(0, fp.beg);
		// Считывание данных из файла
		fp.read(TextBuff, length);
		fp.close();// Закрытие файла
		TextBuff[length - 1] = 0; //записываем признак конца строки

		wchar_t *pwText;
		pwText = new wchar_t[length + 1];//переменная для хранения переделанного текста из неизвестной кодировки в кодировку консоли
		for (int code_i = 1; code_i < 65002; code_i++) 
		{//в цикле перебираем кодовые страницы с 1 по 65001, которая отвечает за UTF-8
			MultiByteToWideChar(code_i, 0, TextBuff, -1, pwText, length);//Преобразуем текст из предполагаемой кодировки code_i
			dwNum = WideCharToMultiByte(1251, 0, pwText, -1, NULL, 0, NULL, NULL);//получаем количество символов в pwText
			char *textbuffnew = new char[dwNum + 1]; //переменная для хранения текста после преобразования в кодировку консоли и анализа
			WideCharToMultiByte(1251, 0, pwText, -1, textbuffnew, dwNum, NULL, NULL);//преобразуем текст из pwText в textbuffernew указывая кодировку преобразования 1251		
			for (int si = 0; si < dwNum; si++) // c начала и до последнего символа
			{
				SymbolMap[textbuffnew[si]]++;// считаем каждый символ в  textbuffnew
			}
			num_symbol = 0;
			for (auto it = SymbolMap.begin(); it != SymbolMap.end(); ++it) 
			{ // В цикле проходим по SymbolMap и считаем сумму количества всех русских букв
				if (((*it).first >= 'А') and ((*it).first <= 'я'))  // для значений от "А" до "я" в char массиве map
					num_symbol += (*it).second; // записывает сумму количества всех русских букв
				(*it).second = 0;
			}
			vh = num_symbol * 100 / dwNum; //Считаем процент входимости символов русского алфавита в текст
			if (vh >= 70) 
			{              //Если процент большее 70, то выводим оператору предложение о читабельности	
				cout << "Кодировка :" << code_i;
				cout << endl << "Результаты анализа: входимость символов русского алфавита :" << vh << endl<<"---------------------------" << endl;
				cout << textbuffnew << endl << endl<<"---------------------------"<<endl;
				cout << "Читабельно? (y/n): ";
				cin >> otv; //считываем ответ оператора о читабельности
				cin.get();
				if (otv == 'y') 
				{ //если ответ утвердительный
					ofstream op("output.txt", ios::out); //открываем файл output.txt для записи
					if (op) 
					{
						op.write(textbuffnew, length); //записываем преобразованный текст в файл 
						cout << endl << "Информация сохранена в файл output.txt";
						break; //прерываем цикл перебора кодировок
					}
					else {
						cerr << "output.txt open error" << endl; //если не удалось открыть файл output.txt выдаем сообщеение об ошибке
						return 2;
					}
				}
			}
			else
			{
				if (code_i > 65000)
				{
					cout << "Sorry, no matches found" << endl;
				}
			}
			delete[] textbuffnew;// Удаление буфера, в котором хранится преобразованный текст входного файла
		}
		delete[] TextBuff;// Удаление буфера, в котором сохранен текст входного файла
		delete[] pwText; //Удаляем массив для хранения промежуточного текста
	}
	else {
		cerr << "Ошибка открытия файла text.txt" << endl; //если файл text.txt не удалось открыть, выдается сообщение об ошибке
	}

	cin.get();

	return 0;
}
