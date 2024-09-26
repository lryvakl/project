//
// Created by Valeria Ryvak on 24.09.2024.
//


#ifndef PROJECT_GRAPH_H
#define PROJECT_GRAPH_H



#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include "transport.h"
#include <string>


namespace std {
    template <>
    struct hash<std::vector<int>> {
        std::size_t operator()(const std::vector<int>& vec) const {
            std::size_t seed = vec.size();
            for (const auto& i : vec) {
                // Об'єднуємо значення кожного елементу
                seed ^= std::hash<int>()(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };
}

template <typename T>
class Node {
public:
    T data;
    Node(T data) : data(data) {}

    // Методи для роботи з вершинами
    void setData(T newData) { data = newData; }
    T getData() const { return data; }
};

template <typename T>
class Edge {
public:
    Node<T>* from;
    Node<T>* to;
    double weight;

    Edge(Node<T>* from, Node<T>* to, double weight = 1.0)
            : from(from), to(to), weight(weight) {}

    // Методи для роботи з ребрами
    double getWeight() const { return weight; }
};

template <typename T>
class UnionFind {
private:
    std::unordered_map<T, T> parent;
    std::unordered_map<T, int> rank;

public:
    void makeSet(T element) {
        parent[element] = element;
        rank[element] = 0;
    }

    T find(T element) {
        if (parent[element] != element) {
            parent[element] = find(parent[element]); // Шляхова компресія
        }
        return parent[element];
    }

    void unionSets(T a, T b) {
        T rootA = find(a);
        T rootB = find(b);

        if (rootA != rootB) {
            // Об'єднання за рангом
            if (rank[rootA] < rank[rootB]) {
                parent[rootA] = rootB;
            } else if (rank[rootA] > rank[rootB]) {
                parent[rootB] = rootA;
            } else {
                parent[rootB] = rootA;
                rank[rootA]++;
            }
        }
    }
};

void printVector(const std::vector<int>& vec) {
    std::cout << "{ ";
    for (const auto& item : vec) {
        std::cout << item << " ";
    }
    std::cout << "}";
}
std::ostream& operator<<(std::ostream& os, const std::vector<int>& vec) {
    os << "{ ";
    for (const auto& item : vec) {
        os << item << " ";
    }
    os << "}";
    return os;
}
template <typename T>
class Graph {
private:
    std::vector<Node<T>*> nodes;
    std::vector<Edge<T>*> edges;

public:
    void addNode(T data) {
        Node<T>* newNode = new Node<T>(data);
        nodes.push_back(newNode);
    }

    void addEdge(Node<T>* from, Node<T>* to, double weight = 1.0) {
        Edge<T>* newEdge = new Edge<T>(from, to, weight);
        edges.push_back(newEdge);
    }

    void removeNode(Node<T>* node) {
        for (auto it = edges.begin(); it != edges.end(); ) {
            if ((*it)->from == node || (*it)->to == node) {
                it = edges.erase(it);
            } else {
                ++it;
            }
        }


        for (auto it = nodes.begin(); it != nodes.end(); ) {
            if (*it == node) {
                delete *it;
                it = nodes.erase(it);
            } else {
                ++it;
            }
        }
    }

    void removeEdge(Edge<T>* edge) {

        for (auto it = edges.begin(); it != edges.end(); ) {
            if (*it == edge) {
                delete *it;
                it = edges.erase(it);
                break;
            } else {
                ++it;
            }
        }
    }

    std::vector<Edge<T>*> kruskalMST() { //minimal spanning tree
        std::vector<Edge<T>*> mst;
        UnionFind<T> uf;

        // Створюємо множини для кожної вершини
        for (auto node : nodes) {
            uf.makeSet(node->getData());
        }

        // Сортуємо ребра за вагою
        std::sort(edges.begin(), edges.end(), [](Edge<T>* a, Edge<T>* b) {
            return a->getWeight() < b->getWeight();
        });

        // Проходимо через всі ребра та будуємо MST
        for (auto edge : edges) {
            T fromRoot = uf.find(edge->from->getData());
            T toRoot = uf.find(edge->to->getData());

            // Якщо корені не однакові, додаємо ребро до MST
            if (fromRoot != toRoot) {
                mst.push_back(edge);
                uf.unionSets(fromRoot, toRoot);
            }
        }

        return mst;
    }


    std::vector<Edge<T>*> findSpanningTree() {
        std::vector<Edge<T>*> spanningTree;
        std::unordered_map<Node<T>*, bool> visited;

        if (nodes.empty()) return spanningTree;

        // Використовуємо BFS для побудови кістякового дерева
        std::queue<Node<T>*> queue;
        queue.push(nodes[0]);
        visited[nodes[0]] = true;

        while (!queue.empty()) {
            Node<T>* currentNode = queue.front();
            queue.pop();

            for (auto edge : edges) {
                if (edge->from == currentNode && !visited[edge->to]) {
                    visited[edge->to] = true;
                    spanningTree.push_back(edge);
                    queue.push(edge->to);
                } else if (edge->to == currentNode && !visited[edge->from]) {
                    visited[edge->from] = true;
                    spanningTree.push_back(edge);
                    queue.push(edge->from);
                }
            }
        }

        return spanningTree;
    }


    void display() const {
        for (const auto& edge : edges) {
            std::cout << edge->from->getData() << " --(" << edge->getWeight() << ")--> ";

            // Перевірка типу даних
            if constexpr (std::is_same<T, std::vector<int>>::value) {
                printVector(static_cast<const std::vector<int>&>(edge->to->getData())); // Виводимо вектор
            } else {
                std::cout << edge->to->getData();
            }
            std::cout << std::endl;
        }
    }


    void displaySpanningTree(const std::vector<Edge<T>*>& spanningTree) const {
        std::cout << "Spanning Tree:" << std::endl;
        for (auto edge : spanningTree) {
            std::cout << edge->from->getData() << " -- " << edge->to->getData() << std::endl;
        }
    }


    void displayMST(const std::vector<Edge<T>*>& mst) const {
        std::cout << "Minimum Spanning Tree:" << std::endl;
        for (auto edge : mst) {
            std::cout << edge->from->getData() << " --(" << edge->getWeight() << ")--> " << edge->to->getData() << std::endl;
        }
    }

    std::vector<Node<T>*> getNodes() const {
        return nodes;
    }

    std::vector<Edge<T>*> getEdges() const {
        return edges;
    }
};

/*void demoGraph() {
    Graph<int> graph;

    graph.addNode(1);
    graph.addNode(2);
    graph.addNode(3);
    graph.addNode(4);

    auto nodes = graph.getNodes();
    graph.addEdge(nodes[0], nodes[1]);
    graph.addEdge(nodes[0], nodes[2]);
    graph.addEdge(nodes[1], nodes[2]);
    graph.addEdge(nodes[1], nodes[3]);
    graph.addEdge(nodes[2], nodes[3]);

    std::cout << "Graph before finding Spanning Tree:" << std::endl;
    graph.display();

    std::vector<Edge<int>*> spanningTree = graph.findSpanningTree();
    graph.displaySpanningTree(spanningTree);

    std::vector<Edge<int>*> mst = graph.kruskalMST();
    graph.displayMST(mst);
    // Очищення пам'яті
    for (auto edge : graph.getEdges()) {
        delete edge;
    }
    for (auto node : graph.getNodes()) {
        delete node;
    }
}
*/

void demoGraph() {

    // Граф для int
    Graph<int> graphInt;

    graphInt.addNode(1);
    graphInt.addNode(2);
    graphInt.addNode(3);
    graphInt.addNode(4);

    auto nodesInt = graphInt.getNodes();
    graphInt.addEdge(nodesInt[0], nodesInt[1]);
    graphInt.addEdge(nodesInt[0], nodesInt[2]);
    graphInt.addEdge(nodesInt[1], nodesInt[2]);
    graphInt.addEdge(nodesInt[1], nodesInt[3]);
    graphInt.addEdge(nodesInt[2], nodesInt[3]);

    std::cout << "Graph of integers:" << std::endl;
    graphInt.display();

    std::vector<Edge<int>*> spanningTree = graphInt.findSpanningTree();
    graphInt.displaySpanningTree(spanningTree);

    std::vector<Edge<int>*> mst = graphInt.kruskalMST();
    graphInt.displayMST(mst);

    graphInt.removeNode(nodesInt[1]);

    std::cout << "Graph of integers after removing node 2:" << std::endl;
    graphInt.display();

    // Граф для double

    Graph<double> graphDouble;

    graphDouble.addNode(1.1);
    graphDouble.addNode(2.2);
    graphDouble.addNode(3.3);
    graphDouble.addNode(4.4);

    auto nodesDouble = graphDouble.getNodes();
    graphDouble.addEdge(nodesDouble[0], nodesDouble[1]);
    graphDouble.addEdge(nodesDouble[0], nodesDouble[2]);
    graphDouble.addEdge(nodesDouble[1], nodesDouble[2]);
    graphDouble.addEdge(nodesDouble[1], nodesDouble[3]);
    graphDouble.addEdge(nodesDouble[2], nodesDouble[3]);

    std::cout << "\nGraph of doubles" << std::endl;
    graphDouble.display();

    std::vector<Edge<double>*> spanningTreeDouble = graphDouble.findSpanningTree();
    graphDouble.displaySpanningTree(spanningTreeDouble);

    std::vector<Edge<double>*> mstDouble = graphDouble.kruskalMST();
    graphDouble.displayMST(mstDouble);

    graphDouble.removeNode(nodesDouble[1]);

    std::cout << "Graph of doubles after removing node 2.2:" << std::endl;
    graphDouble.display();

    // Граф для рядків

    Graph<std::string> graphString;

    graphString.addNode("A");
    graphString.addNode("B");
    graphString.addNode("C");
    graphString.addNode("D");

    auto nodesString = graphString.getNodes();
    graphString.addEdge(nodesString[0], nodesString[1]);
    graphString.addEdge(nodesString[0], nodesString[2]);
    graphString.addEdge(nodesString[1], nodesString[2]);
    graphString.addEdge(nodesString[1], nodesString[3]);
    graphString.addEdge(nodesString[2], nodesString[3]);

    std::cout << "\nGraph of strings:" << std::endl;
    graphString.display();

    std::vector<Edge<std::string>*> spanningTreeString = graphString.findSpanningTree();
    graphString.displaySpanningTree(spanningTreeString);

    std::vector<Edge<std::string>*> mstString = graphString.kruskalMST();
    graphString.displayMST(mstString);

    graphString.removeNode(nodesString[1]);

    std::cout << "Graph of strings after removing node B:" << std::endl;
    graphString.display();

    // Граф для векторів

    Graph<std::vector<int>> graphVector;

    graphVector.addNode(std::vector<int>{1, 2});
    graphVector.addNode(std::vector<int>{3, 4});
    graphVector.addNode(std::vector<int>{5, 6});
    graphVector.addNode(std::vector<int>{7, 8});

    auto nodesVector = graphVector.getNodes();
    graphVector.addEdge(nodesVector[0], nodesVector[1]);
    graphVector.addEdge(nodesVector[0], nodesVector[2]);
    graphVector.addEdge(nodesVector[1], nodesVector[2]);
    graphVector.addEdge(nodesVector[1], nodesVector[3]);
    graphVector.addEdge(nodesVector[2], nodesVector[3]);

    std::cout << "\nGraph of vectors:" << std::endl;
    graphVector.display();

    std::vector<Edge<std::vector<int>>*> spanningTreeVector = graphVector.findSpanningTree();
    graphVector.displaySpanningTree(spanningTreeVector);

    std::vector<Edge<std::vector<int>>*> mstVector = graphVector.kruskalMST();
    graphVector.displayMST(mstVector);

    graphVector.removeNode(nodesVector[1]);

    std::cout << "Graph of vectors after removing vector {3, 4}:" << std::endl;
    graphVector.display();

    // Граф для транспортних засобів
    Graph<Transport*> transportGraph;

    // Створення транспортних засобів
    Transport* car = new Car("Kyiv", "Lviv");
    Transport* bus = new Bus("Kyiv", "Odesa");
    Transport* train = new Train("Lviv", "Kharkiv");
    Transport* waterVehicle = new WaterTransport();
    Transport* airVehicle = new AirTransport();

    // Додавання транспортних засобів у граф
    transportGraph.addNode(car);
    transportGraph.addNode(bus);
    transportGraph.addNode(train);
    transportGraph.addNode(waterVehicle);
    transportGraph.addNode(airVehicle);

    // Додавання ребер між транспортними засобами
    transportGraph.addEdge(transportGraph.getNodes()[0], transportGraph.getNodes()[1], 10.0); // Car to Bus
    transportGraph.addEdge(transportGraph.getNodes()[0], transportGraph.getNodes()[2], 15.0); // Car to Train
    transportGraph.addEdge(transportGraph.getNodes()[1], transportGraph.getNodes()[3], 20.0); // Bus to WaterVehicle
    transportGraph.addEdge(transportGraph.getNodes()[2], transportGraph.getNodes()[4], 25.0); // Train to AirVehicle

    std::cout << "\nTransport Graph before removing Bus:" << std::endl;
    transportGraph.display();

    // Видалення транспортного засобу (Bus)
    transportGraph.removeNode(transportGraph.getNodes()[1]); // Видаляємо Bus

    std::cout << "Transport Graph after removing Bus:" << std::endl;
    transportGraph.display();

    // Очищення пам'яті
    for (auto edge : graphInt.getEdges()) {
        delete edge;
    }
    for (auto node : graphInt.getNodes()) {
        delete node;
    }
    for (auto edge : graphDouble.getEdges()) {
        delete edge;
    }
    for (auto node : graphDouble.getNodes()) {
        delete node;
    }
    for (auto edge : graphString.getEdges()) {
        delete edge;
    }
    for (auto node : graphString.getNodes()) {
        delete node;
    }
    for (auto edge : graphVector.getEdges()) {
        delete edge;
    }
    for (auto node : graphVector.getNodes()) {
        delete node;
    }
    for (auto edge : transportGraph.getEdges()) {
        delete edge;
    }
    for (auto node : transportGraph.getNodes()) {
        delete node;
    }
}

#endif //PROJECT_GRAPH_H