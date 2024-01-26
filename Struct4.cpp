#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <assert.h>
#include <Windows.h>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;


struct Node { // ����������� ��������� ����� � �������� �����
    int num; // ����� �����
    Node* left; // ��� ��������
    Node* right; // ����� ��������
};


void push(Node*& tree, int n) {
    if (tree == nullptr) { // ���� ������ �����
        tree = new Node{ n, nullptr, nullptr }; // �������� ����� �����, �� ���������� ���� �� tree
        return;
    }
    if (n < tree->num) { // ���� ����� ����� ����� �� ��������
        push(tree->left, n); // �� ������ � ��� ��������
        return;
    }
    push(tree->right, n); // ���� �����, �� ������ � ����� ��������
}

void delNode(Node*& tree, int n) {
    if (tree == nullptr) { // ���� ������ �����
        return;
    }
    if (n < tree->num) { //  ���� ����� ����� ����� �� ��������
        delNode(tree->left, n); // ��������� � ����� �������� n �������
        return;
    }
    if (n > tree->num) { // ���� ����� ����� ����� �� ��������
        delNode(tree->right, n); // ��������� � ������� �������� n �������
        return;
    }
    
    if (n == tree->num) {  // ���� ����� ����� ������� ����������
        if (tree->left == nullptr && tree->right == nullptr) { // ���� ����� �� �� ������� �����, ��������� ����, ����������� ���� �������� �� ����
            tree = nullptr;
            return;
        }
        
        if (tree->left != nullptr && tree->right == nullptr) { // ���� ����� �� ���� ���� ������� �������, �������� ���� ���� ������� ������
            tree = tree->left;
            return;
        }

        if (tree->left == nullptr && tree->right != nullptr) { // ���� ����� �� ���� ������ ������� �������, �������� ���� ������ ������� ������
            tree = tree->right;
            return;
        }        
       
        if (tree->left != nullptr && tree->right != nullptr) { // ���� ����� �� ���� ����, ��������� ���������� ����� (�������� �������� � ���� ������� �������)
            
            Node* s = tree->right;
            while (s->left != nullptr) {
                s = s->left; // � �������� ����� ���� �����������
            }
            tree->num = s->num; // �������� ����� ���� �����������
            delNode(tree->right, s->num); // ��������� ��������� �����
            return;
        }
    }
}

bool delLine(string filename, size_t index) // �������, �� ������� ����� �� ���� ��������
{
    vector<string> vec; // ����������� �������
    ifstream file(filename); // ³������� ����� �� �������
    if (file.is_open()) // ���� ���� �������� 
    {
        string str; // ��������� ����� ��� ������ �����
        while (getline(file, str)) // ������� ��������� � �����
            vec.push_back(str); // ����� � ������
        file.close(); // �������� �����
        if (vec.size() < index) // ���� ����� ������� ������ �� ��������� ������ �����
            return false; // ��������� false
        vec.erase(vec.begin() + index); // ���� ��� ����� �� �������� � ������� ��� �����
        ofstream outfile(filename); // ����� � ����
        if (outfile.is_open()) // ���� ���� ��������
        {
            copy(vec.begin(), vec.end(), // �������
                ostream_iterator<string>(outfile, "\n")); // ��������� � ����� �����
            outfile.close(); // ��������� ����
            return true; // ��������� true
        }
        return false; // ��������� false ���� ���� �� ��������
    }
    return false; // ��������� false ���� ���� �� ��������
}

int nodesSum(Node* tree) { // �������, �� �������� ���� ��� �����
    if (tree == nullptr) { // ���� ������ �����
        return 0; // �� ��������� 0
    }
    int sum = tree->num + nodesSum(tree->left) + nodesSum(tree->right); // ���� �������� ����� �� ����� ���� ����� �� ������� �������
    return sum; // ������� �������� ����
}
int sumNodesOnLevel(Node* tree, int level) { // �������, �� �������� ���� ����� �� ������� ���
    if (tree == nullptr || level < 1) { // ���� ������ �����, ��� ���� ����� �� 1
        return 0; // ��������� 0
    }
    if (level == 1) { // ���� ���� 1
        return tree->num; // ��������� ����� ������� �����
    }
    int sum = sumNodesOnLevel(tree->left, level - 1) + sumNodesOnLevel(tree->right, level - 1); // ���� ������� ����� � ����� �� ������� ��������� �� �������� ���

    return sum; // ��������� �������� ����
}

struct treePrint { // ��������� ����������� ������
    treePrint* child;
    bool printing_last_child;
};
treePrint* TP = NULL; // ��������� ����� ��������

void printSubtree(const Node* tree) { // ������� ������ ��������
    treePrint* parent; // ��������� ����������� ������
    if (TP != NULL) { // �������� �� ����
        printf(" ");
        treePrint* s = TP;
        while (s->child != NULL) { // ���� ������� ����� �� ����
            printf(s->printing_last_child ? " " : "| "); // �������� ������� ������� �������
            s = s->child; // ������ �������� ������ ���������� ������
        }
        parent = s; // ������ ������������ ������ �������� ������ S
        printf(parent->printing_last_child ? "-" : "+"); // �������� ������� ������� ������� ������������ ������
    }
    else { // ������
        parent = NULL; // ������ �������� ������������ ������ ����
    }
    printf(">%i\n", tree->num); // �������� �������� ����� ������
    if ((tree->left != NULL) || (tree->right != NULL)) // ���� �������� �� ����, ����� ���� ���� �������(���� � ����)
    {
        treePrint tp; // ��������� ��'��� ��������� treePrint
        if (parent != NULL) { // ���� ����������� ������� �� ����
            parent->child = &tp; // ������ ���������� �������� �������� ����� ���������� ��'���� ��������� treePrint
        }
        else { // ������
            TP = &tp; // ������ TP �������� tp
        }
        tp.child = NULL; // ���������� �������� ������ �������� NULL
        if (tree->left != NULL) { // ���� ��� �������� ����
            tp.printing_last_child = (tree->right == NULL); // ������ ������ �������� ���� ��������� �� �����, �� ����� ������ �� ����
            printSubtree(tree->left); // ���� ����� ��������
        }
        if (tree->right != NULL) {// ���� ����� �������� ����
            tp.printing_last_child = true; // ������ ������ �������� ���� ���������
            printSubtree(tree->right);// ���� ������� ��������
        }
        if (parent != NULL) { // ���� ����������� ����� �� NULL
            parent->child = NULL; // ���������� �������� ������ �������� NULL
        }
        else { // ������
            TP = NULL; // ������ �������� TP NULL
        }
    }
}
void printTree(const Node* tree) {
    assert(TP == NULL);// �������� ����� ��������
    try {
        if (tree != NULL) {
            printSubtree(tree); // ���� ��������
        }
    }
    catch (...) { // �������� ����� � ����� � ��������
        TP = NULL;
        throw;
    }
}

void findMinMax(Node* tree, int& min, int& max) {
    
    if (tree == nullptr) { // ���� ������ �����
        return;
    }

    min = min < tree->num ? min : tree->num; // ����������� ���������� �������� �� ��������� ���������� ���������
    max = max > tree->num ? max : tree->num; // ����������� ������������� �������� �� ��������� ���������� ���������

    findMinMax(tree->left, min, max); // ����� � ����� ������� ���������� �� �������������, �� ��������� ������
    findMinMax(tree->right, min, max); // ����� � ������� ������� ���������� �� �������������, �� ��������� ������
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Node* tree = nullptr; // ��������� �������� ������
    string line; // ����� ��� ���������� � �����
    const string URL = "main.txt"; // ���� �� �����
    
    while (true) { // ���� True
        int choice; // ����� ��� ������ �����������
        cout << "������� �������: \n" << "1. ������ �����\n" << "2. �������� �����\n" << "3. ������� �� ����� ���� ������\n" 
            << "4. ������� ���� ��� �����\n" << "5. ������� ���� ����� �� ������� ���\n" << "6. ������� �������� �� ����������� ��������" << endl;
        cout << "��� ����: ";
        cin >> choice;
        switch (choice) { // Switch �����������
            case 1: {
                ofstream fout(URL, ios::app); // ³������� ����� �� ������� � ����� �����
                string newNode; // ����� ��� ������ ������ �����
                cout << "������ ����� �����: ";
                cin >> newNode;
                fout << '\n' << newNode; // ����� ������ ����� � ����� �����
                cout << endl << "---------------------------------------------------------" << endl << endl;
                fout.close(); // �������� �����
                tree = nullptr; // ��������� ������
                break;
            }

            case 2: {
                ifstream fin(URL); // �������� �� ������� �����
                string deleteNode; // ����� � ��� ���� ���������� �����, ���� ���������� ������ ��������
                if (!fin.is_open()) { // ���� ���� �� ��������
                    return 0; // ��������� 0
                }
                cout << "������ �����, ���� ������ ��������: ";
                cin >> deleteNode; // ������� �����, ���� ������ ��������
                
                int i = 0; // ����� ��� ��������� ���������� ����� �����
                while (getline(fin, line)) { // ���������� � �����
                    
                    if (line == deleteNode) { // ���� ����� ������� � ��� ���� ���������� ���� ��������
                        delLine(URL, i); // �������� ����� � �����
                    }
                    push(tree, stoi(line)); // ������ � ������ �����
                    i++; // ������ 1 �� ����� �������
                }
                printTree(tree); // ������� ������ ���������
                cout << endl << "---------------------------------------------------------" << endl << endl;
                delNode(tree, stoi(deleteNode)); // ��������� � ��������� �����
                printTree(tree); // ������� ������� ������
                tree = nullptr; // ��������� ������
                fin.close();
                break;
            }

            case 3: {
                ifstream fin(URL); // �������� �� ������� �����
                if (!fin.is_open()) { // �������� �� ��������
                    return 0; // ���� �� �������� �� ��������� 0
                }
                while (getline(fin, line)) { // ���� ���� �� ������ � �����
                    push(tree, stoi(line)); // ���� ����� � �����
                }
                printTree(tree); // ³�������� ������
                cout << endl << "---------------------------------------------------------" << endl << endl;
                tree = nullptr; // ��������� ������
                fin.close(); // �������� �����
                break;
            }

            case 4: {
                ifstream fin(URL); // �������� �� ������� �����
                if (!fin.is_open()) {// �������� �� ��������
                    return 0;// ���� �� �������� �� ��������� 0
                }
                while (getline(fin, line)) {// ���� ���� �� ������ � �����
                    push(tree, stoi(line));// ���� ����� � �����
                }
                cout << "���� ��� �����: " << nodesSum(tree) << endl; // ϳ���������� �� ���� ���� ��� �����
                cout << endl << "---------------------------------------------------------" << endl << endl;
                tree = nullptr; // ��������� ������
                fin.close();
                break;
            }
            

            case 5: {
                int level; // ����� ��� ������ ���� ������
                cout << "������ ����� ������, �� ����� ������ ���������� ���� �����: ";
                cin >> level;
                ifstream fin(URL); // �������� �� ������� ����� 
                if (!fin.is_open()) { // �������� �� ��������
                    return 0;
                }
                while (getline(fin, line)) {
                    push(tree, stoi(line));
                }
                cout << "���� ����� �� " << level << " ���: " << sumNodesOnLevel(tree, level) << endl; // ϳ���������� �� ���� ���� ����� �� ������� ��� �������� ������
                cout << endl << "---------------------------------------------------------" << endl << endl;
                tree = nullptr; // ��������� ������
                fin.close();
                break;
            }

            case 6: {
                ifstream fin(URL); // ³������� �� ������� �����
                if (!fin.is_open()) { // �������� �� ��������
                    return 0; 
                }
                while (getline(fin, line)) {
                    push(tree, stoi(line));
                }
                int min = INT_MAX; // ������� �������� ������������� Integer � ����� ��� ���������� �������� ��� ����������� ���������
                int max = INT_MIN; // ������� �������� ���������� Integer � ����� ��� ������������� �������� ��� ����������� ���������
                findMinMax(tree, min, max); // ������������ ������� ������ ������������� ����� �� ����������
                cout << "̳������� ��������: " << min << endl; // ���� ����������
                cout << "M���������� ��������: " << max << endl; // ���� �������������
                cout << endl << "---------------------------------------------------------" << endl << endl;
                tree = nullptr; // ��������� ������
                fin.close();
                break;
            }
            default: {
                cout << "�� ����� ����������� ��������! ��������� �� ���!" << endl;
                break;
            }
                
        }
    }
}
?
