#pragma once
#include "pch.h"
#include "Agent.h"

//Implementation of a simple SARSA Algorithm
class SARSA : public Agent
{
    static std::string NAME;
    std::vector<std::vector<double>> qTable;
public:
    SARSA(double learningRate, double discountFactor, double explorationRate, double explorationRateDecay = 0.9) : Agent(learningRate, discountFactor, explorationRate, explorationRateDecay = 0.9)
    {
        qTable.resize(GRID_SIZE * GRID_SIZE, std::vector<double>(NUM_ACTIONS, 0.0));
        name = NAME;
    }

    std::vector<std::vector<std::pair<int, int>>> train(Environment& env, int iterations, bool verbose = true) override
    {
        std::vector<std::vector<std::pair<int, int>>> hHistory;

        int x, y;
        int state, nextState;
        int action;
        double reward, nextAvgQVal;

        for (int iteration = 0; iteration < iterations; ++iteration) {
            x = 0; // Start position
            y = 0;

            int step = 0;
            std::vector<std::pair<int, int>> historyMoves;

            while (!env.isTerminalState(x, y) && step < MAX_STEPS)
            {
                historyMoves.emplace_back(x, y);
                state = env.getState(x, y);
                action = chooseAction(state);
                reward = env.performAction(x, y, action);
                nextState = env.getState(x, y);
                nextAvgQVal = getAverageNext(nextState);

                updateQValue(state, action, reward, nextAvgQVal);

                step++;
            }

            hHistory.push_back(historyMoves);

            if (verbose)
            {
                std::cout << "ITERATION:" << iteration << " STEPS: " << step << "\n";
            }

            // Reduce exploration rate after each episode
            explorationRate *= explorationRateDecay;
        }
        return hHistory;
    }

    void test(Environment& env) override
    {
        std::vector<std::pair<int, int>> historyMoves;

        int x = 0, y = 0;
        int state, action, nextState;
        double reward;

        for (int step = 0; step < MAX_STEPS; ++step) {
            historyMoves.emplace_back(x, y); //for printing finished picture

            state = env.getState(x, y);
            action = chooseAction(state);
            reward = env.performAction(x, y, action);
            nextState = env.getState(x, y);

            if (env.isTerminalState(x, y)) {
                env.printPath(historyMoves);
                break; // Reached the goal
            }
        }
    }

private:

    double getAverageNext(const int& nextState)
    {
        return std::reduce(qTable[nextState].begin(), qTable[nextState].end()) / qTable[nextState].size();
    }

    int getBestAction(const int& state)
    {
        return std::distance(qTable[state].begin(), std::max_element(qTable[state].begin(), qTable[state].end()));
    }

    int chooseAction(int state) {
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        if (dist(rng) < explorationRate) {
            return dist(rng) * NUM_ACTIONS; // Random action
        }
        else {
            return getBestAction(state);
        }
    }

    //updating the QValue table at the specied state
    void updateQValue(int state, int action, double reward, double nextAvgQVal) {

        qTable[state][action] += learningRate * (reward + discountFactor * nextAvgQVal - qTable[state][action]);
    }
};