#ifndef PROJECT_TRANSPORT_H
#define PROJECT_TRANSPORT_H

#include "includes.h"
//10класів

class Environment {
private:
    std::string startPoint;
    std::string endPoint;
    std::vector<std::string> roads;
    std::vector<std::string> obstacles;

public:
    Environment(const std::string& start, const std::string& end)
            : startPoint(start), endPoint(end) {}

    void addRoad(const std::string& road) {
        roads.push_back(road);
    }

    void addObstacle(const std::string& obstacle) {
        obstacles.push_back(obstacle);
    }

    std::string getStartPoint() const { return startPoint; }
    std::string getEndPoint() const { return endPoint; }

    std::string toString() const {
        std::ostringstream oss;
        oss << "Environment: Start = " << startPoint
            << ", End = " << endPoint
            << ", Roads = [";
        for (const auto& road : roads) oss << road << ", ";
        oss << "], Obstacles = [";
        for (const auto& obstacle : obstacles) oss << obstacle << ", ";
        oss << "]";
        return oss.str();
    }
};

class Transport {
protected:
    double price;
    double duration;
    double distance;
    Environment* env; // Нове поле для середовища

public:
    Transport(double price, double duration, double distance, Environment* env)
            : price(price), duration(duration), distance(distance), env(env) {}

    virtual std::string toString() const = 0;

    std::string getEnvironmentInfo() const {
        return env ? env->toString() : "No environment set.";
    }

    virtual ~Transport() {}
};

class LandTransport : public Transport {
public:
    LandTransport(double price, double duration, double distance, Environment* env)
            : Transport(price, duration, distance, env) {}

    std::string toString() const override {
        return "Land Transport: " + std::to_string(3) ;
    }
};

class WaterTransport : public Transport {
public:
    WaterTransport(double price, double duration, double distance, Environment* env)
            : Transport(price, duration, distance, env) {}

    std::string toString() const override {
        return "Water Transport:" + std::to_string(1);
    }
};

class AirTransport : public Transport {
public:
    AirTransport(double price, double duration, double distance, Environment* env)
            : Transport(price, duration, distance, env) {}

    std::string toString() const override {
        return "Air Transport: " + std::to_string(1) ;
    }
};

class Car : public LandTransport {
public:
    Car(double price, double duration, double distance, Environment* env)
            : LandTransport(price, duration, distance, env) {}

    std::string toString() const override {
        return "Car: Price = " + std::to_string(price) +
               ", Duration = " + std::to_string(duration) +
               ", Distance = " + std::to_string(distance);
    }
};

class Bus : public LandTransport {
public:
    Bus(double price, double duration, double distance, Environment* env)
            : LandTransport(price, duration, distance, env) {}

    std::string toString() const override {
        return "Bus: Price = " + std::to_string(price) +
               ", Duration = " + std::to_string(duration) +
               ", Distance = " + std::to_string(distance);
    }
};

class Train : public LandTransport {
public:
    Train(double price, double duration, double distance, Environment* env)
            : LandTransport(price, duration, distance, env) {}

    std::string toString() const override {
        return "Train: Price = " + std::to_string(price) +
               ", Duration = " + std::to_string(duration) +
               ", Distance = " + std::to_string(distance);
    }
};

class Airplane : public AirTransport {
public:
    Airplane(double price, double duration, double distance, Environment* env)
            : AirTransport(price, duration, distance, env) {}

    std::string toString() const override {
        return "Airplane: Price = " + std::to_string(price) +
               ", Duration = " + std::to_string(duration) +
               " hours, Distance = " + std::to_string(distance) + " km";
    }
};

class Ship : public WaterTransport {
public:
    Ship(double price, double duration, double distance, Environment* env)
            : WaterTransport(price, duration, distance, env) {}

    std::string toString() const override {
        return "Ship: Price = " + std::to_string(price) +
               ", Duration = " + std::to_string(duration) +
               " hours, Distance = " + std::to_string(distance) + " km";
    }
};


#endif // PROJECT_TRANSPORT_H
