#pragma once
#include "pch.h"
#include "Enviroment.h"

//TODO: ADD SINGLE STEP TRAIN FOR BETTER VISAULSIATION
//Base Class for Agents
class Agent
{
protected:
    double learningRate;
    double discountFactor;
    double explorationRate;
    double explorationRateDecay;
    std::random_device rd;
    std::mt19937 rng;

    std::string name;
    static uint32_t agentCounter;
    uint32_t id;
public:
    Agent(double learningRate, double discountFactor, double explorationRate, double explorationRateDecay = 0.9)
        : learningRate(learningRate), discountFactor(discountFactor), explorationRate(explorationRate), explorationRateDecay(explorationRateDecay), rng(rd()) {
        id = agentCounter++;
    };
    virtual std::vector<std::vector<std::pair<int, int>>> train(Environment& env, int iterations, bool verbose = true) { return {}; };
    virtual void test(Environment& env) {};
    const std::string& getName()
    {
        return name;
    }
    virtual std::string getInfoText()
    {
        return "";
    }
    const uint32_t& getId()
    {
        return id;
    }
};

