//
// Created by Valeria Ryvak on 24.09.2024.
//

#ifndef PROJECT_TREE_H
#define PROJECT_TREE_H


#include <iostream>
#include <vector>
#include <unordered_map>
#include "transport.h"


template <typename T>
class TreeNode {
public:
    T data;
    std::vector<TreeNode<T>*> children;

    TreeNode(T data) : data(data) {}
};

template <typename T>
class Tree {
private:
    TreeNode<T>* root;

    // Рекурсивна функція для очищення пам'яті
    void clear(TreeNode<T>* node) {
        if (node) {
            for (auto child : node->children) {
                clear(child);
            }
            delete node;
        }
    }

public:
    Tree() : root(nullptr) {}


    TreeNode<T>* getRoot() const {
        return root;
    }


    void addRoot(T data) {
        root = new TreeNode<T>(data);
    }


    void addChild(TreeNode<T>* parent, T childData) {
        if (parent) {
            TreeNode<T>* child = new TreeNode<T>(childData);
            parent->children.push_back(child);
        }
    }


    void traverse(TreeNode<T>* node) {
        if (node) {
            // Виводимо значення в залежності від типу
            if constexpr (std::is_pointer<T>::value) {
                std::cout << node->data->toString() << std::endl;
            } else {
                std::cout << node->data << std::endl;
            }
            for (auto child : node->children) {
                traverse(child);
            }
        }
    }


    void display() {
        traverse(root);
    }

    // Деструктор
    ~Tree() {
        clear(root);
    }
};


void demoTree() {

    Tree<int> intTree;
    intTree.addRoot(1);
    intTree.addChild(intTree.getRoot(), 2);
    intTree.addChild(intTree.getRoot(), 3);
    std::cout << "Integer Tree:" << std::endl;
    intTree.display();


    Tree<double> doubleTree;
    doubleTree.addRoot(1.1);
    doubleTree.addChild(doubleTree.getRoot(), 2.2);
    doubleTree.addChild(doubleTree.getRoot(), 3.3);
    std::cout << "\nDouble Tree:" << std::endl;
    doubleTree.display();


    Tree<std::string> stringTree;
    stringTree.addRoot("Root");
    stringTree.addChild(stringTree.getRoot(), "Child 1");
    stringTree.addChild(stringTree.getRoot(), "Child 2");
    std::cout << "\nString Tree:" << std::endl;
    stringTree.display();

    Tree<Transport*> transportTree;
    transportTree.addRoot(new Car("Kyiv", "Lviv"));
    transportTree.addChild(transportTree.getRoot(), new Car("Odesa", "Kharkiv"));
    transportTree.addChild(transportTree.getRoot(), new Car("Lviv", "Uzhhorod"));
    std::cout << "\nTransport Tree:" << std::endl;
    transportTree.display();


    delete transportTree.getRoot()->children[0]->data;
    delete transportTree.getRoot()->children[1]->data;
    delete transportTree.getRoot()->data;
}
#endif //PROJECT_TREE_H