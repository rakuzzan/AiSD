/*   18. Дана  шахматная  доска,  состоящая   из   NxN   клеток,
несколько из них занято другими фигурами.  Провести ходом коня
через незанятые клетки путь  минимальной  длины  из  начальной
клетки в конечную.
   Ограничения: 2 <= N <= 500, время 1 с.
   Ввод из файла input.txt. В  первой строке задано число N. В
следующих  N  строках  содержится  по  N символов.  Символом #
обозначена занятая клетка,  точкой -  незанятая  клетка,  @  -
исходное и конечное положение коня (таких символов два).
   Вывод в файл output.txt.  Если  путь построить  невозможно,
вывести No,  в противном случае вывести такую же карту,  как и
на входе, но пометить символом @  все промежуточные  положения 
коня (11).

Работу выполнил студент ПС-21
Подоплелов Дмитрий Алексеевич*/
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <string>

using namespace std;

int main() {
	int n;
	char input[] = "input.txt";
	ifstream inFile(input, ios_base::in);
	const pair <int, int> UNDEF = make_pair(-1, -1);
	pair<int, int> start = UNDEF, finish = UNDEF;
	inFile >> n;
	vector<vector<char>> field(n, vector<char>(n));
	vector<vector<pair<int, int>>> from(n, vector<pair<int, int>>(n, UNDEF));
	queue<pair<int, int>> q;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			inFile >> field[i][j];
			if (field[i][j] == '@') {
				if (start == UNDEF) {
					start = make_pair(i, j);
				}
				else {
					if (finish == UNDEF) {
						finish = make_pair(i, j);
						field[i][j] = '.';
					}
				}
			}
		}
	}

	q.push(start);
	while (!q.empty()) {
		pair<int, int> cur = q.front();
		q.pop();
		for (int di = -2; di <= 2; di++) {
			for (int dj = -2; dj <= 2; dj++) {
				if (di * di + dj * dj == 5) {
					int ni = cur.first + di;
					int nj = cur.second + dj;
					if (
						0 <= ni && ni < n && 0 <= nj && nj < n &&
						field[ni][nj] == '.' && from[ni][nj] == UNDEF
					) {
						q.push(pair<int, int>(ni, nj));
						from[ni][nj] = cur;
					}
				}
			}
		}
	}
	
	if (from[finish.first][finish.second] == UNDEF) {
		cout << "No" << endl;
		return 0;
	}

	pair<int, int> cur = finish;
	do {
		field[cur.first][cur.second] = '@';
		cur = from[cur.first][cur.second];
	} while (cur != start);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << field[i][j];
		}
		cout << "\n";
	}
	return 0;
}
