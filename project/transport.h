//
// Created by Valeria Ryvak on 24.09.2024.
//

#ifndef PROJECT_TRANSPORT_H
#define PROJECT_TRANSPORT_H

#include <iostream>
#include <string>
#include <vector>

// Базовий клас для всіх транспортних засобів
class Transport {
public:
    virtual void move() = 0; // Метод для руху
    virtual std::string toString() const = 0; // Метод для отримання рядкового представлення
    virtual ~Transport() = default; // Віртуальний деструктор
};


class LandTransport : public Transport {
protected:
    std::string origin;
    std::string destination;

public:
    LandTransport(std::string origin, std::string destination)
            : origin(origin), destination(destination) {}

    std::string getOrigin() const { return origin; }
    std::string getDestination() const { return destination; }
};


class Car : public LandTransport {
public:
    Car(std::string origin, std::string destination)
            : LandTransport(origin, destination) {}

    void move() override {
        std::cout << "Car is driving from " << origin << " to " << destination << std::endl;
    }

    std::string toString() const override {
        return "Car from " + origin + " to " + destination;
    }
};


class Bus : public LandTransport {
public:
    Bus(std::string origin, std::string destination)
            : LandTransport(origin, destination) {}

    void move() override {
        std::cout << "Bus is driving from " << origin << " to " << destination << std::endl;
    }

    std::string toString() const override {
        return "Bus from " + origin + " to " + destination;
    }
};


class Train : public LandTransport {
public:
    Train(std::string origin, std::string destination)
            : LandTransport(origin, destination) {}

    void move() override {
        std::cout << "Train is moving from " << origin << " to " << destination << std::endl;
    }

    std::string toString() const override {
        return "Train from " + origin + " to " + destination;
    }
};


class WaterTransport : public Transport {
public:
    void move() override {
        std::cout << "Water vehicle is moving through water." << std::endl;
    }

    std::string toString() const override {
        return "Water Vehicle";
    }
};


class Ship : public WaterTransport {
public:
    void move() override {
        std::cout << "Ship is sailing." << std::endl;
    }

    std::string toString() const override {
        return "Ship";
    }
};


class AirTransport : public Transport {
public:
    void move() override {
        std::cout << "Air vehicle is flying." << std::endl;
    }

    std::string toString() const override {
        return "Air Vehicle";
    }
};


class Plane : public AirTransport {
public:
    void move() override {
        std::cout << "Plane is flying." << std::endl;
    }

    std::string toString() const override {
        return "Plane";
    }
};

#endif //PROJECT_TRANSPORT_H