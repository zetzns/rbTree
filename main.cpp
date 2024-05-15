#include <iostream>
#include <bitset>
#include <string>
#include <queue>

enum Color { RED, BLACK };

// Определение узла для красно-черного дерева
struct Node {
    std::string data;
    Color color;
    Node *left, *right, *parent;

    // Конструктор инициализирует узел с предоставленными данными и устанавливает начальные значения.
    explicit Node(std::string data) : data(std::move(data)), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

// Класс для реализации красно-черного дерева
class RBTree {
private:
    Node *root;

    // Левое вращение вокруг данного узла
    void leftRotate(Node *x) {
        //f = 1+1+1+1+1+1+1 = 7 => O(f) = O(1)
        Node *y = x->right;
        x->right = y->left;
        if (y->left != nullptr) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    // Правое вращение вокруг данного узла
    void rightRotate(Node *x) {
        // f = 1+1+1+1+1+1+1 = 7 => O(f) = O(1)
        Node *y = x->left;
        x->left = y->right;
        if (y->right != nullptr) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            root = y;
        } else if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    // Восстановление свойств красно-черного дерева после вставки
    void insertFixUp(Node *z) {
        // f = 5log(N) + 3 + 1 => O(f) = O(logN)
        while (z->parent != nullptr && z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node *y = z->parent->parent->right;
                if (y != nullptr && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                Node *y = z->parent->parent->left;
                if (y != nullptr && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    // Вспомогательный метод для рекурсивного обхода в глубину
    void inorderHelper(Node *node) const {
        // f = n + 1 + n = 2n+1 => O(f) = O(n)
        if (node != nullptr) {
            inorderHelper(node->left);
            std::cout << node->data << " ";
            inorderHelper(node->right);
        }
    }

public:
    RBTree() : root(nullptr) {}

    void insert(const std::string &data) {
        //В среднем f = 1 + logN + logN => O(f) = O(logN)
        //В худшем f = 1 + N + logN => O(f) = O(N)
        Node *node = new Node(data);
        Node *y = nullptr;
        Node *x = this->root;

        while (x != nullptr) {
            y = x;
            if (node->data < x->data) {
                x = x->left;
            } else {
                x = x->right;
            }
        }

        node->parent = y;
        if (y == nullptr) {
            root = node;
        } else if (node->data < y->data) {
            y->left = node;
        } else {
            y->right = node;
        }

        if (node->parent == nullptr) {
            node->color = BLACK;
            return;
        }

        if (node->parent->parent == nullptr) {
            return;
        }

        insertFixUp(node);
    }

    void inorderTraversal() const {
        //f = N + 1 => O(f) = O(N)
        inorderHelper(root);
        std::cout << std::endl;
    }
};


int main() {

    // f = 1+1+1+1+1+1+n+2*m*logN+1+3*m = 7 + N + 2MlogN + 3M => O(f) = O(n+ mlogN + m)

    // Выводим сообщение пользователю о том, как использовать программу
    std::cout << "Number of elements: ";

    size_t numberOfElements;
    std::cin >> numberOfElements;

    // Проверяем, что ввод корректен
    if (std::cin.fail()) [[unlikely]] {
        std::cerr << "Wrong data on input" << std::endl;
        return 1; // Завершаем программу с кодом ошибки
    }

    int numbers[numberOfElements];
    std::cout << "Enter " << numberOfElements << " positive integers:" << std::endl;

    for (size_t i = 0; i < numberOfElements; ++i) {
        std::cin >> numbers[i];

        // Проверяем каждый ввод на корректность
        if (std::cin.fail()) [[unlikely]] {
            std::cerr << "Wrong data on input" << std::endl;
            return 1; // Завершаем программу с кодом ошибки
        }
    }

    // Создаем экземпляр красно-черного дерева
    RBTree tree;

    // Вставляем числа в дерево в двоичном формате
    for (int num : numbers) {
        if (num < 0) [[unlikely]] {
            std::cerr << "Wrong data on input" << std::endl;
            return 1; // Завершаем программу с кодом ошибки
        }
        std::string binary = std::bitset<16>(num).to_string(); // Преобразуем в 8-битное двоичное число
        tree.insert(binary);
    }

    // Выводим отсортированное дерево
    std::cout << "Sorted:" << std::endl;
    tree.inorderTraversal();

    return 0;
}
