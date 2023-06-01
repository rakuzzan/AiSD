/* 21. В  листьях  И - ИЛИ  дерева, соответствующего некоторому
множеству  конструкций, заданы   значения   массы.Известно
максимально допустимое значение массы изделия.Требуется усечь
дерево   так, чтобы   дерево    включало    все    элементы,
соответствующие  допустимым  значениям массы, но не содержало
"лишних" вершин.Конечное дерево выдать на экран в  наглядном
виде(13). 

Работу выполнил студент ПС-21
Подоплелов Дмитрий Алексеевич*/

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <valarray>

using namespace std;

struct tree
{
    string name;
    char and_or_root = ' ';
    int level;
    int maxWeight;
    int minWeight;
    bool isChosen = true;
    tree* ptrDad;
    vector<tree*> sons;
};

void init_tree(string path, tree*& root)
{
    char ch;
    int prevLevel = -1;
    string data;
    tree* pinnacle, * ptr, * temp;
    temp = nullptr;
    ifstream fIn;

    fIn.open(path);

    while (fIn.get(ch))
    {
        int level = 0;
        pinnacle = new tree;

        while (ch == '.')
        {
            level++;
            fIn.get(ch);
        }
        pinnacle->level = level;
        pinnacle->minWeight = _I32_MAX;
        pinnacle->maxWeight = -_I32_MAX;
        while (ch != ' ')
        {
            pinnacle->name += ch;
            fIn.get(ch);
        }
        while (ch != '\n')
        {
            data += ch;
            fIn.get(ch);
            if (ch == 'a')
            {
                pinnacle->and_or_root = 'a';
            }
            else if (ch == 'o')
            {
                pinnacle->and_or_root = 'o';
            }
        }
        if (pinnacle->and_or_root == ' ')
        {
            pinnacle->maxWeight = stoi(data);
            pinnacle->minWeight = pinnacle->maxWeight;
        }
        data = "";
        if (level == 0)
        {
            root = pinnacle;
            temp = root;
            prevLevel = 0;
            continue;
        }
        if (level > prevLevel)
        {
            temp->sons.push_back(pinnacle);
            pinnacle->ptrDad = temp;
        }
        else if (level == prevLevel)
        {
            ptr = temp->ptrDad;
            ptr->sons.push_back(pinnacle);
            pinnacle->ptrDad = ptr;
        }
        else
        {
            ptr = temp;
            for (int i = 0; i <= prevLevel - level; i++)
            {
                ptr = ptr->ptrDad;
            }
            ptr->sons.push_back(pinnacle);
            pinnacle->ptrDad = ptr;
        }
        prevLevel = level;
        temp = pinnacle;
    }
    fIn.close();
    if (!root)
    {
        cout << "Tree is empty. Invalid input data" << endl;
    }
}

void calculate_weight(tree* root)
{
    if (!root->sons.empty())
    {
        for (int i = 0; i < root->sons.size(); i++)
        {
            calculate_weight(root->sons[i]);
        }

        if (root->and_or_root == 'o')
        {
            for (int j = 0; j < root->sons.size(); j++)
            {
                if (root->sons[j]->maxWeight > root->maxWeight)
                {
                    root->maxWeight = root->sons[j]->maxWeight;
                }
                if (root->sons[j]->minWeight < root->minWeight)
                {
                    root->minWeight = root->sons[j]->minWeight;
                }
            }
        }
        else if (root->and_or_root == 'a')
        {
            int s_min = 0;
            int s_max = 0;

            for (int k = 0; k < root->sons.size();k++)
            {
                s_min += root->sons[k]->minWeight;
                s_max += root->sons[k]->maxWeight;
            }
            root->minWeight = s_min;
            root->maxWeight = s_max;
        }
    }
}

void count_weighted_tree(tree* root, int weight)
{
    int temp_weight;
    for (int i = 0; i < root->sons.size();i++)
    {
        if (root->sons[i]->and_or_root == 'o' && root->sons[i]->minWeight > weight)
        {
            root->sons[i]->isChosen = false;
        }
        else if (root->sons[i]->and_or_root == 'o')
        {
            if (root->and_or_root == 'a')
            {
                temp_weight = weight - root->minWeight + root->sons[i]->minWeight;
            }
            else if (root->and_or_root == 'o')
            {
                temp_weight = weight;
            }
            count_weighted_tree(root->sons[i], temp_weight);
        }
        else if (root->sons[i]->and_or_root == ' ' && root->sons[i]->maxWeight > weight)
        {
            root->sons[i]->isChosen = false;
        }
        else if (root->sons[i]->and_or_root == 'a' && root->sons[i]->minWeight > weight)
        {
            root->sons[i]->isChosen = false;
        }
    }
}

void print_tree(tree* root)
{
    string grade = "";
    if (root && root->isChosen)
    {
        for (int i = 0; i < root->level; i++)
        {
            grade = grade + ".";
        }
        grade = grade + root->name;
        if (root->and_or_root == ' ')
        {
            grade = grade + " " + to_string(root->minWeight);
        }
        else if (root->and_or_root == 'a' || root->and_or_root == 'o')
        {
            grade = grade + " " + root->and_or_root;
        }
        cout << grade << endl;
        for (int i = 0; i < root->sons.size(); i++)
        {
            print_tree(root->sons[i]);
        }
    }
}

void delete_tree(tree*& root)
{
    for (int i = 0; i < root->sons.size(); i++)
    {
        delete_tree(root->sons[i]);
    }
    delete root;
    root = nullptr;
}

int main()
{
    string filename;
    ifstream fIn;
    tree* root = nullptr;

    cout << "Enter a filename: ";
    cin >> filename;
    fIn.open(filename);

    if (!fIn.is_open())
    {
        cout << "File doesn't exists" << endl;
    }
    else
    {
        int weight;
        fIn.close();

        init_tree(filename, root);
        calculate_weight(root);
        cout << "Your tree: " << endl;
        print_tree(root);

        cout << "Range of input number should be from " << root->minWeight << " to " << root->maxWeight << endl;
        cout << "Enter a number to cut the tree: ";
        cin >> weight;
        count_weighted_tree(root, weight);
        cout << "Your new tree:" << endl;
        print_tree(root);
        delete_tree(root);
    }
    return 0;
}

