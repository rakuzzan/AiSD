/* 6. В общежитии живет  N  студентов.При  поселении  каждый
студент  представил список своих знакомых.Каждое воскресение
организуется вечер знакомств, когда знакомые любого  студента
знакомятся   между   собой.Выяснить, через  сколько  недель
познакомятся два указанных студента(8).

Работу выполнил студент ПС-21
Подоплелов Дмитрий Алексеевич*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

vector<vector<int>> Matrix;
vector<string> StudentsList;

bool uniqName(string Name) {
    for (int i = 0; i < StudentsList.size(); i++)
        if (StudentsList[i] == Name)
            return false;
    return true;
}

void printNames() {
    for (int i = 0; i < StudentsList.size(); i++)
        cout << i + 1 << " - " << StudentsList[i] << '\n';
}

int getIndex(string Name) {
    for (int i = 0; i < StudentsList.size(); i++)
        if (StudentsList[i] == Name)
            return i;
}

void searchPath(int First, int Second) {

    int week = 0;

    vector<vector<int>> FirstMatrix = Matrix;
    vector<vector<int>> SecondMatrix = Matrix;

    while (FirstMatrix[First][Second] != 1) {
        for (int i = 0; i < Matrix.size(); i++)
            for (int j = 0; j < Matrix.size(); j++)
                if (FirstMatrix[i][j] == 1)
                    for (int k = 0; k < Matrix.size(); k++)
                        if ((FirstMatrix[j][k] == 1) && (i != k)) {
                            SecondMatrix[i][k] = 1;
                            SecondMatrix[k][i] = 1;
                        }
        if (week == Matrix.size()) {
            cout << "The file is not set correctly students will not get to know each other\n\n";
            break;
        }
        FirstMatrix = SecondMatrix;
        week++;
    }
    if (week != Matrix.size()) {
        if (week == 0)
            cout << StudentsList[First] << " and " << StudentsList[Second] << " already know each other\n\n";
        else if (week == 1)
            cout << StudentsList[First] << " and " << StudentsList[Second] << " will get know each other in 1 week\n\n";
        else
            cout << StudentsList[First] << " and " << StudentsList[Second] << " will get know each other in " << week << " weeks\n\n";
    }
}

void getPath() {
    string firstName, secondName;

    cout << "Enter the name of the first student: ";
    cin >> firstName;

    if (!uniqName(firstName)) {
        cout << "Enter the name of the second student: ";
        cin >> secondName;

        if (!uniqName(secondName))
            searchPath(getIndex(firstName), getIndex(secondName));
        else
            cout << "No student found\n";
    }
    else
        cout << "No student found\n";
}

void readGraph() {
    string fileName;
    cout << "Enter a filename: ";
    cin >> fileName;
    ifstream Fin;
    Fin.open(fileName);
    if (Fin.is_open()) {
        int students;
        string fromStudent, toStudent;
        while (Fin >> students) {

            Matrix.resize(students);
            for (int i = 0; i < students; i++)
                Matrix[i].resize(students);

            while (Fin >> fromStudent) {
                Fin >> toStudent;

                if (uniqName(fromStudent))
                    StudentsList.push_back(fromStudent);
                if (uniqName(toStudent))
                    StudentsList.push_back(toStudent);

                Matrix[getIndex(fromStudent)][getIndex(toStudent)] = 1;
                Matrix[getIndex(toStudent)][getIndex(fromStudent)] = 1;
            }
        }
        printNames();
        getPath();
    }
    else
        cout << "File doesn't exist\n";
}

int main() {

    bool a = true;
    int option;

    while (a) {
        cout << "Choose an option(1 - run program 2 - exit): ";
        cin >> option;
        switch (option) {
            case 1: {
                readGraph();
                break;
            }
            case 2: {
                a = false;
                break;
            }
            default:
                cout << "Choose one of the options!\n\n";
                break;
        }
    }
}