//
// Created by Valeria Ryvak on 24.09.2024.
//

#ifndef PROJECT_TRANSPORT_H
#define PROJECT_TRANSPORT_H

#include <iostream>
#include <string>
#include <vector>

class Transport {
protected:
    double price;
    double duration;
    double distance;

public:

    Transport(double price, double duration, double distance)
            : price(price), duration(duration), distance(distance) {}
    virtual std::string toString() const = 0;
    virtual ~Transport() {}
};


class LandTransport : public Transport {
public:
    LandTransport(double price, double duration, double distance)
            : Transport(price, duration, distance) {}

    std::string toString() const override {
        return "Land Transport: "+ std::to_string(3) ;
    }
    virtual ~LandTransport() {}
};


class WaterTransport : public Transport {
public:
    WaterTransport(double price, double duration, double distance)
            : Transport(price, duration, distance) {}
    std::string toString() const override {
        return "WaterTransport: "+ std::to_string(1) ;
    }
    ~WaterTransport(){};
};



class AirTransport : public Transport {
public:
    AirTransport(double price, double duration, double distance)
            : Transport(price, duration, distance) {}
    std::string toString() const override {
        return "AirTransport: "+ std::to_string(1) ;
    }
    ~AirTransport(){};
};


class Car : public LandTransport {

public:
    Car(double price, double duration, double distance)
            : LandTransport(price, duration, distance) {}


    std::string toString() const override {
        return "Car: Price = " + std::to_string(price) +
               ", Duration = " + std::to_string(duration) +
               ", Distance = " + std::to_string(distance);
    }
    ~Car() {}
};


class Bus : public LandTransport {

public:
    Bus(double price, double duration, double distance)
            : LandTransport(price, duration, distance) {}


    std::string toString() const override {
        return "Bus: Price = " + std::to_string(price) +
               ", Duration = " + std::to_string(duration) +
               ", Distance = " + std::to_string(distance);
    }
    ~Bus() {}


};


class Train : public LandTransport {

public:
    Train(double price, double duration, double distance)
            :   LandTransport(price, duration, distance) {
    }

    std::string toString() const override {
        return "Train: Price = " + std::to_string(price) +
               ", Duration = " + std::to_string(duration) +
               ", Distance = " + std::to_string(distance);
    }
    ~Train() {}
};


class Airplane : public AirTransport {
public:
    Airplane(double price, double duration, double distance)
            : AirTransport(price, duration, distance) {}

    std::string toString() const override {
        return "Airplane: Price = " + std::to_string(price) +
               ", Duration = " + std::to_string(duration) +
               " hours, Distance = " + std::to_string(distance) + " km";
    }
    ~Airplane(){}
};


class Ship : public WaterTransport {
public:
    Ship(double price, double duration, double distance)
            : WaterTransport(price, duration, distance) {}

    std::string toString() const override {
        return "Ship: Price = " + std::to_string(price) +
               ", Duration = " + std::to_string(duration) +
               " hours, Distance = " + std::to_string(distance) + " km";
    }
    ~Ship(){}
};

#endif //PROJECT_TRANSPORT_H