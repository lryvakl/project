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
std::string generateRandomString(int length) {
    std::string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, characters.size() - 1);

    std::string randomString;
    for (int i = 0; i < length; ++i) {
        randomString += characters[dis(gen)];
    }
    return randomString;
}
std::vector<int> generateRandomIntVector(int size, int minValue, int maxValue) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minValue, maxValue);

    std::vector<int> randomVector(size);
    for (int& elem : randomVector) {
        elem = dis(gen);
    }
    return randomVector;
}
std::vector<std::string> generateRandomStringVector(int vectorSize, int stringLength) {
    std::vector<std::string> randomStringVector(vectorSize);
    for (auto& str : randomStringVector) {
        str = generateRandomString(stringLength);  // Генеруємо випадковий рядок
    }
    return randomStringVector;
}

template <typename T>
class Node {
public:
    T data;
    Node(T data) : data(data) {}

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
            parent[element] = find(parent[element]);
        }
        return parent[element];
    }

    void unionSets(T a, T b) {
        T rootA = find(a);
        T rootB = find(b);

        if (rootA != rootB) {

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
    std::vector<Node<T> *> nodes;
    std::vector<Edge<T> *> edges;

public:
    void addNode(T data) {
        Node<T> *newNode = new Node<T>(data);
        nodes.push_back(newNode);
    }

    void addEdge(Node<T> *from, Node<T> *to, double weight = 1.0) {
        Edge<T> *newEdge = new Edge<T>(from, to, weight);
        edges.push_back(newEdge);
    }

    void removeNode(Node<T> *node) {
        for (auto it = edges.begin(); it != edges.end();) {
            if ((*it)->from == node || (*it)->to == node) {
                it = edges.erase(it);
            } else {
                ++it;
            }
        }


        for (auto it = nodes.begin(); it != nodes.end();) {
            if (*it == node) {
                delete *it;
                it = nodes.erase(it);
            } else {
                ++it;
            }
        }
    }

    void removeEdge(Edge<T> *edge) {

        for (auto it = edges.begin(); it != edges.end();) {
            if (*it == edge) {
                delete *it;
                it = edges.erase(it);
                break;
            } else {
                ++it;
            }
        }
    }


    std::vector<Edge<T> *> findSpanningTree() {
        std::vector<Edge<T> *> spanningTree;
        std::unordered_map<Node<T> *, bool> visited;

        if (nodes.empty()) return spanningTree;

        // Використовуємо BFS(в ширину) для побудови кістякового дерева
        std::queue<Node<T> *> queue;
        queue.push(nodes[0]);
        visited[nodes[0]] = true;

        while (!queue.empty()) {
            Node<T> *currentNode = queue.front();
            queue.pop();

            for (auto edge: edges) {
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

    std::vector<Edge<T> *> kruskalMST() { //minimal spanning tree
        std::vector<Edge<T> *> mst;
        UnionFind<T> uf;

        // Створюємо множини для кожної вершини
        for (auto node: nodes) {
            uf.makeSet(node->getData());
        }

        // Сортуємо ребра за вагою
        std::sort(edges.begin(), edges.end(), [](Edge<T> *a, Edge<T> *b) {
            return a->getWeight() < b->getWeight();
        });

        // Проходимо через всі ребра та будуємо MST
        for (auto edge: edges) {
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


    std::vector<Edge<T>*> primMST() {
        std::vector<Edge<T>*> mst;
        std::unordered_set<Node<T>*> visited;
        auto compare = [](Edge<T>* a, Edge<T>* b) { return a->getWeight() > b->getWeight(); };
        std::priority_queue<Edge<T>*, std::vector<Edge<T>*>, decltype(compare)> minHeap(compare);

        if (nodes.empty()) return mst;

        visited.insert(nodes[0]);
        for (auto edge : edges) {
            if (edge->from == nodes[0] || edge->to == nodes[0]) {
                minHeap.push(edge);
            }
        }

        while (!minHeap.empty()) {
            Edge<T>* currentEdge = minHeap.top();
            minHeap.pop();

            Node<T>* nextNode = visited.count(currentEdge->from) ? currentEdge->to : currentEdge->from;

            if (visited.count(nextNode) == 0) {
                visited.insert(nextNode);
                mst.push_back(currentEdge);

                for (auto edge : edges) {
                    if (edge->from == nextNode || edge->to == nextNode) {
                        minHeap.push(edge);
                    }
                }
            }
        }

        return mst;
    }

    std::vector<Node<T> *> getNodes() const {
        return nodes;
    }

    std::vector<Edge<T> *> getEdges() const {
        return edges;
    }
    void display() const {
        for (const auto &edge: edges) {
            std::cout << edge->from->getData() << " --(" << edge->getWeight() << ")--> ";

            if constexpr (std::is_same<T, std::vector<int>>::value) {
                printVector(static_cast<const std::vector<int> &>(edge->to->getData()));
            } else {
                std::cout << edge->to->getData();
            }
            std::cout << std::endl;
        }
    }

    void displayTransport() const {
        for (const auto &node: nodes) {
            std::cout << node->getData()->toString() << std::endl;
        }
    }

    void displaySpanningTree(const std::vector<Edge<T> *> &spanningTree) const {
        std::cout << "Spanning Tree:" << std::endl;
        for (auto edge: spanningTree) {
            std::cout << edge->from->getData() << " -- " << edge->to->getData() << std::endl;
        }
    }

    void displaySTtrans(const std::vector<Edge<T> *> &spanningTree) const {
        std::cout << " Spanning Tree:" << std::endl;
        for (auto edge: spanningTree) {
            std::cout << edge->from->getData()->toString() << " --(" << edge->getWeight() << ")--> "
                      << edge->to->getData()->toString() << std::endl;
        }
    }

    void displayMST(const std::vector<Edge<T> *> &mst) const {
        std::cout << "Minimum Spanning Tree (Kruskal's algorithm): " << std::endl;
        for (auto edge: mst) {
            std::cout << edge->from->getData() << " --(" << edge->getWeight() << ")--> " << edge->to->getData()
                      << std::endl;
        }
    }

    void displayMSTtrans(const std::vector<Edge<T> *> &mst) const {
        std::cout << "Minimum Spanning Tree:" << std::endl;
        for (auto edge: mst) {
            std::cout << edge->from->getData()->toString() << " --(" << edge->getWeight() << ")--> "
                      << edge->to->getData()->toString() << std::endl;
        }
    }

    void displayMSTPrimsTrans(const std::vector<Edge<T> *> &mst) const {
        std::cout << "Minimum Spanning Tree (Prim's Algorithm):" << std::endl;
        for (auto edge : mst) {
            std::cout << edge->from->getData()->toString() << " --(" << edge->getWeight() << ")--> "
                      << edge->to->getData()->toString() << std::endl;
        }
    }

    void displayMSTPrims(const std::vector<Edge<T>*> &mst) const {
        std::cout << "Minimum Spanning Tree (Prim's Algorithm):" << std::endl;
        for (auto edge : mst) {
            std::cout << edge->from->getData() << " --(" << edge->getWeight() << ")--> "
                      << edge->to->getData() << std::endl;
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


    transportGraph.addEdge(transportGraph.getNodes()[0], transportGraph.getNodes()[1],1);
    transportGraph.addEdge(transportGraph.getNodes()[0], transportGraph.getNodes()[2],1);
    transportGraph.addEdge(transportGraph.getNodes()[0], transportGraph.getNodes()[3],1);
    transportGraph.addEdge(transportGraph.getNodes()[4], transportGraph.getNodes()[5],2);
    transportGraph.addEdge(transportGraph.getNodes()[6], transportGraph.getNodes()[7],3);

    for (const auto& node : transportGraph.getNodes()) {

        Transport* transport = node->getData();
        std::cout << transport->toString() << std::endl;
        std::cout << transport->getEnvironmentInfo() << std::endl;
    }

    std::vector<Edge<Transport*>*> spanningTreeTransport = transportGraph.findSpanningTree();
    transportGraph.displaySTtrans(spanningTreeTransport);

    std::vector<Edge<Transport*>*> mstTransport = transportGraph.kruskalMST();
    transportGraph.displayMSTtrans(mstTransport);

    std::vector<Edge<Transport*>*> mstTransportPrims = transportGraph.primMST();
    transportGraph.displayMSTPrimsTrans(mstTransportPrims);

    transportGraph.removeNode(transportGraph.getNodes()[5]);
    transportGraph.removeNode(transportGraph.getNodes()[4]);
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

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(1, 100);
std::random_device rdS;
std::mt19937 genS(rdS());
std::uniform_int_distribution<> weightDist(1, 10);

void demoIntGraph(){
    Graph<int> graphInt;

    for (int i = 0; i < 4; ++i) {
        graphInt.addNode(dis(gen));
    }

    auto nodesInt = graphInt.getNodes();

    graphInt.addEdge(nodesInt[0], nodesInt[1], dis(gen));
    graphInt.addEdge(nodesInt[0], nodesInt[2], dis(gen));
    graphInt.addEdge(nodesInt[1], nodesInt[2], dis(gen));
    graphInt.addEdge(nodesInt[1], nodesInt[3], dis(gen));
    graphInt.addEdge(nodesInt[2], nodesInt[3], dis(gen));

    std::cout << "Graph of integers:" << std::endl;
    graphInt.display();

    std::vector<Edge<int>*> spanningTree = graphInt.findSpanningTree();
    graphInt.displaySpanningTree(spanningTree);

    std::vector<Edge<int>*> mst = graphInt.kruskalMST();
    graphInt.displayMST(mst);

    std::vector<Edge<int>*> mstPrim = graphInt.primMST();
    graphInt.displayMSTPrims(mstPrim);

    graphInt.removeNode(nodesInt[1]);

    std::cout << "Graph of integers after removing node 2:" << std::endl;
    graphInt.display();
    for (auto edge : graphInt.getEdges()) {
        delete edge;
    }
    for (auto node : graphInt.getNodes()) {
        delete node;
    }

}
void demoDoubleGraph(){

    std::random_device rdouble;
    std::mt19937 genD(rdouble());
    std::uniform_real_distribution<> disD(1.0, 100.0);

    Graph<double> graphDouble;

    for (int i = 0; i < 4; ++i) {
        graphDouble.addNode(disD(genD));
    }

    auto nodesDouble = graphDouble.getNodes();

    graphDouble.addEdge(nodesDouble[0], nodesDouble[1], disD(genD));
    graphDouble.addEdge(nodesDouble[0], nodesDouble[2], disD(genD));
    graphDouble.addEdge(nodesDouble[1], nodesDouble[2], disD(genD));
    graphDouble.addEdge(nodesDouble[1], nodesDouble[3], disD(genD));
    graphDouble.addEdge(nodesDouble[2], nodesDouble[3], disD(genD));

    std::cout << "\nGraph of doubles:" << std::endl;
    graphDouble.display();

    std::vector<Edge<double>*> spanningTreeDouble = graphDouble.findSpanningTree();
    graphDouble.displaySpanningTree(spanningTreeDouble);

    std::vector<Edge<double>*> mstDouble = graphDouble.kruskalMST();
    graphDouble.displayMST(mstDouble);

    std::vector<Edge<double>*> mstPrimDouble = graphDouble.primMST();
    graphDouble.displayMSTPrims(mstPrimDouble);

    graphDouble.removeNode(nodesDouble[1]);

    std::cout << "Graph of doubles after removing node:" << std::endl;
    graphDouble.display();
    for (auto edge : graphDouble.getEdges()) {
        delete edge;
    }
    for (auto node : graphDouble.getNodes()) {
        delete node;
    }
}
void demoStringGraph(){
    Graph<std::string> graphString;

    for (int i = 0; i < 4; ++i) {
        graphString.addNode(generateRandomString(5));
    }

    auto nodesString = graphString.getNodes();

    graphString.addEdge(nodesString[0], nodesString[1], weightDist(gen));
    graphString.addEdge(nodesString[0], nodesString[2], weightDist(gen));
    graphString.addEdge(nodesString[1], nodesString[2], weightDist(gen));
    graphString.addEdge(nodesString[1], nodesString[3], weightDist(gen));
    graphString.addEdge(nodesString[2], nodesString[3], weightDist(gen));

    std::cout << "\nGraph of strings:" << std::endl;
    graphString.display();

    std::vector<Edge<std::string>*> spanningTreeString = graphString.findSpanningTree();
    graphString.displaySpanningTree(spanningTreeString);

    std::vector<Edge<std::string>*> mstString = graphString.kruskalMST();
    graphString.displayMST(mstString);

    std::vector<Edge<std::string>*> mstPrimString = graphString.primMST();
    graphString.displayMSTPrims(mstPrimString);

    graphString.removeNode(nodesString[1]);

    std::cout << "Graph of strings after removing node:" << std::endl;
    graphString.display();
    for (auto edge : graphString.getEdges()) {
        delete edge;
    }
    for (auto node : graphString.getNodes()) {
        delete node;
    }
}
void demoVectorIntGraph(){
    Graph<std::vector<int>> graphVector;

    for (int i = 0; i < 4; ++i) {
        graphVector.addNode(generateRandomIntVector(2, 1, 10));
    }

    auto nodesVector = graphVector.getNodes();

    graphVector.addEdge(nodesVector[0], nodesVector[1], weightDist(gen));
    graphVector.addEdge(nodesVector[0], nodesVector[2], weightDist(gen));
    graphVector.addEdge(nodesVector[1], nodesVector[2], weightDist(gen));
    graphVector.addEdge(nodesVector[1], nodesVector[3], weightDist(gen));
    graphVector.addEdge(nodesVector[2], nodesVector[3], weightDist(gen));

    std::cout << "\nGraph of vectors:" << std::endl;
    graphVector.display();

    std::vector<Edge<std::vector<int>>*> spanningTreeVector = graphVector.findSpanningTree();
    graphVector.displaySpanningTree(spanningTreeVector);

    std::vector<Edge<std::vector<int>>*> mstVector = graphVector.kruskalMST();
    graphVector.displayMST(mstVector);

    std::vector<Edge<std::vector<int>>*> mstPrimVector = graphVector.primMST();
    graphVector.displayMSTPrims(mstPrimVector);

    graphVector.removeNode(nodesVector[1]);

    std::cout << "Graph of vectors after removing vector:" << std::endl;
    graphVector.display();

    for (auto edge : graphVector.getEdges()) {
        delete edge;
    }
    for (auto node : graphVector.getNodes()) {
        delete node;
    }
}
void demoVectorStringGraph(){
    Graph<std::vector<std::string>> graphStringVector;


    for (int i = 0; i < 4; ++i) {
        graphStringVector.addNode(generateRandomStringVector(2, 5));
    }

    auto nodesStringVector = graphStringVector.getNodes();

    graphStringVector.addEdge(nodesStringVector[0], nodesStringVector[1], weightDist(gen));
    graphStringVector.addEdge(nodesStringVector[0], nodesStringVector[2], weightDist(gen));
    graphStringVector.addEdge(nodesStringVector[1], nodesStringVector[2], weightDist(gen));
    graphStringVector.addEdge(nodesStringVector[1], nodesStringVector[3], weightDist(gen));
    graphStringVector.addEdge(nodesStringVector[2], nodesStringVector[3], weightDist(gen));

    std::cout << "\nGraph of string vectors:" << std::endl;
    graphStringVector.display();

    std::vector<Edge<std::vector<std::string>>*> spanningTreeStringVector = graphStringVector.findSpanningTree();
    graphStringVector.displaySpanningTree(spanningTreeStringVector);

    std::vector<Edge<std::vector<std::string>>*> mstStringVector = graphStringVector.kruskalMST();
    graphStringVector.displayMST(mstStringVector);

    graphStringVector.removeNode(nodesStringVector[1]);

    std::cout << "Graph of string vectors after removing vector:" << std::endl;
    graphStringVector.display();

    for (auto edge : graphStringVector.getEdges()) {
        delete edge;
    }
    for (auto node : graphStringVector.getNodes()) {
        delete node;
    }
}

void demoGraph() {

    demoIntGraph();
    demoDoubleGraph();
    demoStringGraph();
    demoVectorIntGraph();
    demoVectorStringGraph();

}

#endif //PROJECT_GRAPH_H