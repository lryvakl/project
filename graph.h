//
// Created by Valeria Ryvak on 24.09.2024.
//


#ifndef PROJECT_GRAPH_H
#define PROJECT_GRAPH_H

#include "includes.h"

//4 класи

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
namespace std {
    template<>
    struct hash<std::vector<std::string>> {
        std::size_t operator()(const std::vector<std::string>& vec) const {
            std::size_t seed = 0;
            std::hash<std::string> hasher;
            for (const auto& str : vec) {
                seed ^= hasher(str) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };
}

std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& vec) {
    os << "{ ";
    for (const auto& str : vec) {
        os << "\"" << str << "\" ";
    }
    os << "}";
    return os;
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
    void displaySTtrans(const std::vector<Edge<T>*>& spanningTree) const {
        std::cout << " Spanning Tree:" << std::endl;
        for (auto edge : spanningTree) {
            std::cout << edge->from->getData()->toString() << " --(" << edge->getWeight() << ")--> " << edge->to->getData()->toString() << std::endl;
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

    void displayTransport() const {
        for (const auto& node : nodes) {
            std::cout << node->getData()->toString()<< std::endl;
        }
    }
    void displayMSTtrans(const std::vector<Edge<T>*>& mst) const {
        std::cout << "Minimum Spanning Tree:" << std::endl;
        for (auto edge : mst) {
            std::cout << edge->from->getData()->toString() << " --(" << edge->getWeight() << ")--> " << edge->to->getData()->toString() << std::endl;
        }
    }


};

void demoTransportGraph() {
    Graph<Transport*> transportGraph;


    Environment* envCar = new Environment("Київ", "Одеса");
    envCar->addRoad("М05");
    envCar->addObstacle("Traffic jam");

    Environment* envBus = new Environment("Львів", "Київ");
    envBus->addRoad("E05");
    envBus->addObstacle("Construction of roads");

    Environment* envTrain = new Environment("Одеса", "Львів");
    envTrain->addRoad("124К");
    envTrain->addObstacle("Delayed");

    Environment* envAir = new Environment("Київ", "Варшава");
    envAir->addObstacle("Weather issues");

    Environment* envWater = new Environment("Київ", "Львів");
    envWater->addRoad("Dnieper River");
    envAir->addObstacle("Storm");


    Transport* landTransport = new LandTransport(0, 0, 0, nullptr);
    Transport* car = new Car(1500, 6, 300, envCar);
    Transport* bus = new Bus(8000, 6, 400, envBus);
    Transport* train = new Train(20000, 8, 600, envTrain);
    Transport* airTransport = new AirTransport(0, 0, 0, nullptr);
    Transport* airplane = new Airplane(50000, 2, 1500, envAir);
    Transport* waterTransport = new WaterTransport(0, 0, 0, nullptr);
    Transport* ship = new Ship(25000, 12, 1000, envWater);

    std::cout<<"\nTransport Graph: "<<std::endl;

    transportGraph.addNode(landTransport);
    transportGraph.addNode(car);
    transportGraph.addNode(bus);
    transportGraph.addNode(train);
    transportGraph.addNode(airTransport);
    transportGraph.addNode(airplane);
    transportGraph.addNode(waterTransport);
    transportGraph.addNode(ship);


    transportGraph.addEdge(transportGraph.getNodes()[0], transportGraph.getNodes()[1]);
    transportGraph.addEdge(transportGraph.getNodes()[0], transportGraph.getNodes()[2]);
    transportGraph.addEdge(transportGraph.getNodes()[0], transportGraph.getNodes()[3]);
    transportGraph.addEdge(transportGraph.getNodes()[4], transportGraph.getNodes()[5]);
    transportGraph.addEdge(transportGraph.getNodes()[6], transportGraph.getNodes()[7]);


    for (const auto& node : transportGraph.getNodes()) {

        Transport* transport = node->getData();
        std::cout << transport->toString() << std::endl;
        std::cout << transport->getEnvironmentInfo() << std::endl;
    }

    std::vector<Edge<Transport*>*> spanningTreeTransport = transportGraph.findSpanningTree();
    transportGraph.displaySTtrans(spanningTreeTransport);

    std::vector<Edge<Transport*>*> mstTransport = transportGraph.kruskalMST();
    transportGraph.displayMSTtrans(mstTransport);


    transportGraph.removeNode(transportGraph.getNodes()[5]); // Наприклад, літак
    std::cout << "\nAfter removing the Airplane:" << std::endl;
    transportGraph.displayTransport();

    delete car;
    delete bus;
    delete train;
    delete airplane;
    delete ship;
    delete envCar;
    delete envTrain;
    delete envBus;
    delete envAir;
    delete envWater;

    for (auto edge : transportGraph.getEdges()) {
        delete edge;
    }
}

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

    // Граф для векторів<int>

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

// Граф для vector<string>

    Graph<std::vector<std::string>> graphStringVector;

    graphStringVector.addNode(std::vector<std::string>{"Hello", "World"});
    graphStringVector.addNode(std::vector<std::string>{"C++", "Programming"});
    graphStringVector.addNode(std::vector<std::string>{"Graph", "Data"});
    graphStringVector.addNode(std::vector<std::string>{"Algorithm", "Tree"});

    auto nodesStringVector = graphStringVector.getNodes();

    graphStringVector.addEdge(nodesStringVector[0], nodesStringVector[1]);
    graphStringVector.addEdge(nodesStringVector[0], nodesStringVector[2]);
    graphStringVector.addEdge(nodesStringVector[1], nodesStringVector[2]);
    graphStringVector.addEdge(nodesStringVector[1], nodesStringVector[3]);
    graphStringVector.addEdge(nodesStringVector[2], nodesStringVector[3]);

    std::cout << "\nGraph of string vectors:" << std::endl;
    graphStringVector.display();

    std::vector<Edge<std::vector<std::string>>*> spanningTreeStringVector = graphStringVector.findSpanningTree();
    graphStringVector.displaySpanningTree(spanningTreeStringVector);

    std::vector<Edge<std::vector<std::string>>*> mstStringVector = graphStringVector.kruskalMST();
    graphStringVector.displayMST(mstStringVector);

    graphStringVector.removeNode(nodesStringVector[1]);

    std::cout << "Graph of string vectors after removing vector {C++, Programming}:" << std::endl;
    graphStringVector.display();


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
}

#endif //PROJECT_GRAPH_H