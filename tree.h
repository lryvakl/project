//
// Created by Valeria Ryvak on 24.09.2024.
//

#ifndef PROJECT_TREE_H
#define PROJECT_TREE_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "transport.h"

template <typename T>
void deleteData(T data) {
    if constexpr (std::is_pointer<T>::value) {
        delete data;
    }
}



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


    void clear(TreeNode<T>* node) {
        if (node) {
            for (auto child : node->children) {
                clear(child);
            }

            deleteData(node->data);
            delete node;
        }
    }


    bool removeNode(TreeNode<T>* node, TreeNode<T>* parent) {
        if (!node) return false;


        for (auto child : node->children) {
            clear(child);
        }


        if (parent) {
            auto& siblings = parent->children;
            siblings.erase(std::remove(siblings.begin(), siblings.end(), node), siblings.end());
        } else {

            root = nullptr;
        }

        deleteData(node->data);
        delete node;
        return true;
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


    bool remove(TreeNode<T>* node) {
        if (!node) return false;
        if (node == root) {
            return removeNode(root, nullptr);
        }

        // Знаходимо батька вузла для коректного видалення
        TreeNode<T>* parent = findParent(root, node);
        return removeNode(node, parent);
    }


    TreeNode<T>* findParent(TreeNode<T>* current, TreeNode<T>* target) {
        if (!current || !target) return nullptr;
        for (auto child : current->children) {
            if (child == target) return current;
            auto found = findParent(child, target);
            if (found) return found;
        }
        return nullptr;
    }

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

    intTree.remove(intTree.getRoot()->children[0]);
    std::cout << "\nAfter removing node with data 2:" << std::endl;
    intTree.display();

    Tree<double> doubleTree;
    doubleTree.addRoot(1.1);
    doubleTree.addChild(doubleTree.getRoot(), 2.2);
    doubleTree.addChild(doubleTree.getRoot(), 3.3);
    std::cout << "\nDouble Tree:" << std::endl;
    doubleTree.display();

    doubleTree.remove(doubleTree.getRoot()->children[1]);
    std::cout << "\nAfter removing node with data 3.3:" << std::endl;
    doubleTree.display();

    Tree<std::string> stringTree;
    stringTree.addRoot("Root");
    stringTree.addChild(stringTree.getRoot(), "Child 1");
    stringTree.addChild(stringTree.getRoot(), "Child 2");
    std::cout << "\nString Tree:" << std::endl;
    stringTree.display();

    stringTree.remove(stringTree.getRoot()->children[0]);
    std::cout << "\nAfter removing 'Child 1':" << std::endl;
    stringTree.display();

    Tree<std::vector<int>> vectorTree;
    std::vector<int> rootData = {1, 2, 3};
    vectorTree.addRoot(rootData);
    std::vector<int> childData1 = {4, 5};
    std::vector<int> childData2 = {6, 7};
    vectorTree.addChild(vectorTree.getRoot(), childData1);
    vectorTree.addChild(vectorTree.getRoot(), childData2);

    std::cout << "\nVector Tree:" << std::endl;
    vectorTree.display();

    vectorTree.remove(vectorTree.getRoot()->children[0]);
    std::cout << "\nAfter removing first child vector node:" << std::endl;
    vectorTree.display();

    Tree<std::vector<std::string>> vectorStringTree;

    std::vector<std::string> rootDataString = {"one", "two", "three"};
    vectorStringTree.addRoot(rootDataString);

    std::vector<std::string> childData3 = {"four", "five"};
    std::vector<std::string> childData4 = {"six", "seven"};

    vectorStringTree.addChild(vectorStringTree.getRoot(), childData3);
    vectorStringTree.addChild(vectorStringTree.getRoot(), childData4);

    std::cout << "\nVector Tree:" << std::endl;
    vectorStringTree.display();


    vectorStringTree.remove(vectorStringTree.getRoot()->children[0]);
    std::cout << "\nAfter removing first child vector node:" << std::endl;
    vectorStringTree.display();



}

void demoTransportTree() {

    Tree<Transport*> transportTree;


    Environment* envLand = new Environment("Київ", "Одеса");
    envLand->addRoad("М05");
    envLand->addObstacle("Construction of roads");

    Environment* envAir = new Environment("Київ", "Львів");
    envAir->addObstacle("Weather issues");

    Environment* envWater = new Environment("Київ", "Черкаси");
    envWater->addRoad("Dnieper River");

    Transport* car = new Car(1500, 6, 500,envLand);
    Transport* bus = new Bus(800, 9, 550,envLand);
    Transport* train = new Train(800, 10, 700,envLand);
    Transport* airplane = new Airplane(2500, 2, 650,envAir);
    Transport* ship = new Ship(2000, 2, 150,envWater);


    transportTree.addRoot(car);

    transportTree.addChild(transportTree.getRoot(), bus);
    transportTree.addChild(transportTree.getRoot(), train);

    TreeNode<Transport*>* busNode = transportTree.getRoot()->children[0];
    transportTree.addChild(busNode, airplane);
    transportTree.addChild(busNode, ship);

    std::cout << "Transport Tree:" << std::endl;
    transportTree.display();

    transportTree.remove(busNode->children[0]);
    std::cout << "\nAfter removing the Airplane:" << std::endl;
    transportTree.display();

}



#endif //PROJECT_TREE_H
