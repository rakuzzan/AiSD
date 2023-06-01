/*20. Текст программы на диалекте бейсика включает циклы вида
FOR ID =
...............
...............
NEXT  ID,
где ID - идентификатор параметра цикла.
Требуется:
1) проверить   правильность    идентификаторов, задающих
параметры  циклов(не более двух символов;  первый - латинская
	буква, а второй, если он имеется, -цифра);
2) с   помощью  стека  проверить  правильность  вложенности
циклов;
3) переписать файл с исходным текстом так, чтобы операторы
цикла каждого последующего уровня вложения  были  сдвинуты  на
две позиции вправо по сравнению с предыдущим уровнем(9).

 Работу выполнил студент ПС-21
 Подоплелов Дмитрий Алексеевич*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <string.h>
#include <Windows.h>
#include <conio.h>

using namespace std;

const string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
int tab = 0;
int curLine = 0;
int errorCycleLine = 0;

struct Stack
{
	string Value;
	int cycleLine;
	Stack* Next;
	Stack(const string& v, const int& l, Stack* tmp) {
		Value = v;
		cycleLine = l;
		Next = tmp;
	}
};

Stack* StackOfID;

void ErrorLog(string Message)
{
	cout << Message << "\n";
}

bool StackIsEmpty(Stack* Ptr)
{
	return Ptr == nullptr;
}

void Push(Stack*& Ptr, string Elem)
{
	Stack* NewPtr = new Stack(Elem, curLine, Ptr);
	Ptr = NewPtr;
	NewPtr = nullptr;
	delete NewPtr;
	tab += 1;
}

string Pop(Stack*& Ptr)
{
	if (StackIsEmpty(Ptr))
	{
		ErrorLog("You have cycle without begin");
		return "";
	}
	else
	{
		Stack* CurrPtr;
		string str;
		int number = Ptr->cycleLine;
		str = Ptr->Value;
		CurrPtr = Ptr;
		Ptr = Ptr->Next;
		CurrPtr->Next = nullptr;
		CurrPtr = nullptr;
		delete CurrPtr;
		tab -= 1;
		errorCycleLine = number;
		return str;
	}
}

bool CheckCycleBegin(string& line) {
	return (line[0] == 'F' && line[1] == 'O' && line[2] == 'R' && line[3] == ' ');
}

bool CheckCycleEnd(string& line) {
	if (line[0] == 'N' && line[1] == 'E' && line[2] == 'X' && line[3] == 'T' && line[4] == ' ') {
		if (alphabet.find(line[5]) && ((line[6] == char(0) || line[6] == ',') || (isdigit(line[6]) && (line[7] == char(0) || line[7] == ',')))) {
			if (line[6] == char(0) || line[6] == ',') {
				string str(1, line[5]);
				string popStr = Pop(StackOfID);
				return popStr == str;
			}
			else {
				string str;
				str.assign(line, 5, 2);
				string popStr = Pop(StackOfID);
				return popStr == str;
			}
		}
	}
	return true;
}

bool CheckID(string& line) {
	if (alphabet.find(line[4]) && (line[5] == '=' || (isdigit(line[5]) && line[6] == '='))) {
		if (line[5] == '=') {
			string str(1, line[4]);
			Push(StackOfID, str);

		}
		else {
			string str;
			str.assign(line, 4, 2);
			Push(StackOfID, str);
		}
		return true;
	}
	else return false;
}

int main()
{

	char input[1000];
	cout << "Enter a file path: ";
	cin >> input;
	cout << "\n";
	char output[] = "out.txt";

	ifstream inFile(input, ios_base::in);
	ofstream outFile(output, ios_base::app);
	string line;

	while (getline(inFile, line)) {
		curLine += 1;
		if (!CheckCycleEnd(line)) {
			cout << "Cycle doesn't closed: line " << errorCycleLine << "\n";
		}
		string t(2 * tab, ' ');
		t.append(line);
		outFile << t << "\n";
		if (line.length() >= 6) {
			if (CheckCycleBegin(line)) {
				CheckID(line);
			}
		}
	}

	if (!StackIsEmpty(StackOfID)) {
		ErrorLog("First Cycle doesn't closed");
	}

	inFile.close();
	outFile.close();

	_getch();
	return 0;
}