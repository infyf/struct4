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


struct Node { // ініціалізація структури вузла в бінарному дереві
    int num; // число вузла
    Node* left; // ліве піддерево
    Node* right; // праве піддерево
};


void push(Node*& tree, int n) {
    if (tree == nullptr) { // Якщо дерево пусте
        tree = new Node{ n, nullptr, nullptr }; // створити новий вузол, та встановити його як tree
        return;
    }
    if (n < tree->num) { // Якщо число вузла менше за передане
        push(tree->left, n); // то додати в ліве піддерево
        return;
    }
    push(tree->right, n); // Якщо більше, то додати в праве піддерево
}

void delNode(Node*& tree, int n) {
    if (tree == nullptr) { // Якщо дерево пусте
        return;
    }
    if (n < tree->num) { //  Якщо число вузла менше за передане
        delNode(tree->left, n); // Видаляємо з лівого піддерева n елемент
        return;
    }
    if (n > tree->num) { // Якщо число вузла менше за передане
        delNode(tree->right, n); // Видаляємо з правого піддерева n елемент
        return;
    }
    
    if (n == tree->num) {  // Якщо число вузла дорівнює переданому
        if (tree->left == nullptr && tree->right == nullptr) { // Якщо вузол не має дочірніх вузлів, видаляємо його, встановивши його вказівник на нуль
            tree = nullptr;
            return;
        }
        
        if (tree->left != nullptr && tree->right == nullptr) { // Якщо вузол має лише лівий дочірній елемент, заміняємо його лівим дочірнім вузлом
            tree = tree->left;
            return;
        }

        if (tree->left == nullptr && tree->right != nullptr) { // Якщо вузол має лише правий дочірній елемент, заміняємо його правим дочірнім вузлом
            tree = tree->right;
            return;
        }        
       
        if (tree->left != nullptr && tree->right != nullptr) { // Якщо вузол має двох дітей, знаходимо наступника вузла (мінімальне значення в його правому піддереві)
            
            Node* s = tree->right;
            while (s->left != nullptr) {
                s = s->left; // і заміняємо вузол його наступником
            }
            tree->num = s->num; // Заміняємо вузол його наступником
            delNode(tree->right, s->num); // Видаляємо наступний вузол
            return;
        }
    }
}

bool delLine(string filename, size_t index) // Функція, що видаляє рядок за його індексом
{
    vector<string> vec; // ініціалізація вектора
    ifstream file(filename); // Відкриття файлу на читання
    if (file.is_open()) // Якщо файл відкритий 
    {
        string str; // свторення змінної для запису рядка
        while (getline(file, str)) // читання порядково з файлу
            vec.push_back(str); // запис у вектор
        file.close(); // закриття файлу
        if (vec.size() < index) // якщо розмір вектору менший за переданий індекс рядка
            return false; // повертаємо false
        vec.erase(vec.begin() + index); // Якщо все добре то вилучаємо з вектору цей рядок
        ofstream outfile(filename); // запис у файл
        if (outfile.is_open()) // якщо файл відкритий
        {
            copy(vec.begin(), vec.end(), // копіюємо
                ostream_iterator<string>(outfile, "\n")); // видаляємо з файлу рядок
            outfile.close(); // закриваємо файл
            return true; // повертаємо true
        }
        return false; // повертаємо false якщо файл не відкритий
    }
    return false; // повертаємо false якщо файл не відкритий
}

int nodesSum(Node* tree) { // Функція, що підраховує суму всіх вузлів
    if (tree == nullptr) { // Якщо дерево пусте
        return 0; // то повернути 0
    }
    int sum = tree->num + nodesSum(tree->left) + nodesSum(tree->right); // Сумує поточний вузел та вузли його лівого та правого піддерев
    return sum; // повертає значення суми
}
int sumNodesOnLevel(Node* tree, int level) { // Функція, що підраховує суму вузлів на певному рівні
    if (tree == nullptr || level < 1) { // Якщо дерево пусте, або рівнів менше за 1
        return 0; // повернути 0
    }
    if (level == 1) { // Якщо рівнів 1
        return tree->num; // повернути число єдиного вузла
    }
    int sum = sumNodesOnLevel(tree->left, level - 1) + sumNodesOnLevel(tree->right, level - 1); // Сума значень вузлів у лівому та правому піддеревах на заданому рівні

    return sum; // Повертаємо значення суми
}

struct treePrint { // Структура відображення дерева
    treePrint* child;
    bool printing_last_child;
};
treePrint* TP = NULL; // Створюємо масив структур

void printSubtree(const Node* tree) { // Функція виводу піддерева
    treePrint* parent; // створюємо батьківський массив
    if (TP != NULL) { // Перевірка на нуль
        printf(" ");
        treePrint* s = TP;
        while (s->child != NULL) { // Поки дочірній масив не нуль
            printf(s->printing_last_child ? " " : "| "); // Виводимо останній дочірній елемент
            s = s->child; // Надаємо значення масиву дочірнього масиву
        }
        parent = s; // Надаємо батьківському масиву значення масиву S
        printf(parent->printing_last_child ? "-" : "+"); // Виводимо останній дочірній елемент батьківського масиву
    }
    else { // Інакше
        parent = NULL; // Надаємо значення батьківському масиву НУЛЬ
    }
    printf(">%i\n", tree->num); // Виводимо значення вузлів масиву
    if ((tree->left != NULL) || (tree->right != NULL)) // Якщо піддерева не НУЛЬ, тобто якщо вони існують(хоча б один)
    {
        treePrint tp; // створюємо об'єкт структури treePrint
        if (parent != NULL) { // Якщо батьківський елемент не НУЛЬ
            parent->child = &tp; // Надаємо дочірньому елементу значення раніше створеного об'єкту структури treePrint
        }
        else { // Інакше
            TP = &tp; // Надаємо TP значення tp
        }
        tp.child = NULL; // Дочірньому елементу надаємо значення NULL
        if (tree->left != NULL) { // Якщо ліве піддерево існує
            tp.printing_last_child = (tree->right == NULL); // Надаємо булеве значення полю структури за умови, що праве дерево не існує
            printSubtree(tree->left); // друг лівого піддерева
        }
        if (tree->right != NULL) {// Якщо праве піддерево існує
            tp.printing_last_child = true; // Надаємо булеве значення полю структури
            printSubtree(tree->right);// друг правого піддерева
        }
        if (parent != NULL) { // якщо батьківський масив не NULL
            parent->child = NULL; // Дочірьному елементу надаємо значення NULL
        }
        else { // Інакше
            TP = NULL; // надаємо значення TP NULL
        }
    }
}
void printTree(const Node* tree) {
    assert(TP == NULL);// Перевірка стану програми
    try {
        if (tree != NULL) {
            printSubtree(tree); // Вивід піддерева
        }
    }
    catch (...) { // перевірка стану і вихід з програми
        TP = NULL;
        throw;
    }
}

void findMinMax(Node* tree, int& min, int& max) {
    
    if (tree == nullptr) { // Якщо дерево пусте
        return;
    }

    min = min < tree->num ? min : tree->num; // Знаходження мінімального значення за допомогою тернарного оператора
    max = max > tree->num ? max : tree->num; // Знаходження максимального значення за допомогою тернарного оператора

    findMinMax(tree->left, min, max); // Пошук у лівому піддереві мінімального та максимального, за допомогою рекурсії
    findMinMax(tree->right, min, max); // Пошук у правому піддереві мінімального та максимального, за допомогою рекурсії
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Node* tree = nullptr; // Створення бінарного дерева
    string line; // Рядок для зчитування з файлу
    const string URL = "main.txt"; // Шлях до файлу
    
    while (true) { // Поки True
        int choice; // Змінна для вибору користувача
        cout << "Виберіть функцію: \n" << "1. Додати вузол\n" << "2. Видалити вузол\n" << "3. Вивести на екран вміст дерева\n" 
            << "4. Вивести суму всіх вузлів\n" << "5. Вивести суму вузлів на певному рівні\n" << "6. Вивести мінімальне та максимальне значення" << endl;
        cout << "Ваш вибір: ";
        cin >> choice;
        switch (choice) { // Switch конструкція
            case 1: {
                ofstream fout(URL, ios::app); // Відкриття файлу на дозапис у кінець файлу
                string newNode; // рядок для запису нового вузла
                cout << "Введіть новий вузол: ";
                cin >> newNode;
                fout << '\n' << newNode; // запис нового вузла у кінець файлу
                cout << endl << "---------------------------------------------------------" << endl << endl;
                fout.close(); // закриття файлу
                tree = nullptr; // обнулення дерева
                break;
            }

            case 2: {
                ifstream fin(URL); // відкриття на читання файлу
                string deleteNode; // змінна в якій буде зберігатися рядок, який користувач захоче видалити
                if (!fin.is_open()) { // якщо файл не відкритий
                    return 0; // повертаємо 0
                }
                cout << "Введіть вузол, який хочете видалити: ";
                cin >> deleteNode; // Вводимо вузол, який хочемо видалити
                
                int i = 0; // змінна для корректної індексації рядків файлу
                while (getline(fin, line)) { // зчитування з файлу
                    
                    if (line == deleteNode) { // якщо рядок співпадає з тим який користувач хоче видалити
                        delLine(URL, i); // видалити рядок з файлу
                    }
                    push(tree, stoi(line)); // додаємо у дерево вузол
                    i++; // додаємо 1 до змінної індексу
                }
                printTree(tree); // малюємо дерево початкове
                cout << endl << "---------------------------------------------------------" << endl << endl;
                delNode(tree, stoi(deleteNode)); // видаляємо зі структури вузол
                printTree(tree); // малюємо поточне дерево
                tree = nullptr; // обнуляємо дерево
                fin.close();
                break;
            }

            case 3: {
                ifstream fin(URL); // відкриття на читання файлу
                if (!fin.is_open()) { // Перевірка на відкриття
                    return 0; // якщо не відкритий то повернути 0
                }
                while (getline(fin, line)) { // поки читає та записує у рядок
                    push(tree, stoi(line)); // додає вузли з файлу
                }
                printTree(tree); // Відрисовка дерева
                cout << endl << "---------------------------------------------------------" << endl << endl;
                tree = nullptr; // обнулення дерева
                fin.close(); // закриття файлу
                break;
            }

            case 4: {
                ifstream fin(URL); // відкриття на читання файлу
                if (!fin.is_open()) {// Перевірка на відкриття
                    return 0;// якщо не відкритий то повернути 0
                }
                while (getline(fin, line)) {// поки читає та записує у рядок
                    push(tree, stoi(line));// додає вузли з файлу
                }
                cout << "Сума всіх вузлів: " << nodesSum(tree) << endl; // Підрахування та вивід суми всіх вузлів
                cout << endl << "---------------------------------------------------------" << endl << endl;
                tree = nullptr; // обнулення дерева
                fin.close();
                break;
            }
            

            case 5: {
                int level; // Змінна для запису рівня дерева
                cout << "Введіть рівень дерева, на якому хочете порахувати суму вузлів: ";
                cin >> level;
                ifstream fin(URL); // відкриття на читання файлу 
                if (!fin.is_open()) { // Перевірка на відкриття
                    return 0;
                }
                while (getline(fin, line)) {
                    push(tree, stoi(line));
                }
                cout << "Сума вузлів на " << level << " рівні: " << sumNodesOnLevel(tree, level) << endl; // Підрахування та вивід суми вузлів на певному рівні бінарного дерева
                cout << endl << "---------------------------------------------------------" << endl << endl;
                tree = nullptr; // обнулення дерева
                fin.close();
                break;
            }

            case 6: {
                ifstream fin(URL); // Відкриття на читання файлу
                if (!fin.is_open()) { // Перевірка на відкриття
                    return 0; 
                }
                while (getline(fin, line)) {
                    push(tree, stoi(line));
                }
                int min = INT_MAX; // Надання значення максимального Integer в змінну для мінімального значення для правильного підрахунку
                int max = INT_MIN; // Надання значення мінімального Integer в змінну для максимального значення для правильного підрахунку
                findMinMax(tree, min, max); // Застосування функції пошуку максимального вузла та мінімального
                cout << "Мінімальне значення: " << min << endl; // друк мінмального
                cout << "Mаксимальне значення: " << max << endl; // друк максимального
                cout << endl << "---------------------------------------------------------" << endl << endl;
                tree = nullptr; // обнулення дерева
                fin.close();
                break;
            }
            default: {
                cout << "Ви ввели неправильне значення! Спробуйте ще раз!" << endl;
                break;
            }
                
        }
    }
}
?
