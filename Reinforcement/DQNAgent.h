#pragma once
#include "pch.h"
#include "Enviroment.h"
#include "Agent.h"
#include "NeuralNetwork.h"

//Not working yet
class DQNAgent : public Agent
{
    static std::string NAME;
     // The neural network for DQN

public:
    NeuralNetwork neuralNetwork;

    DQNAgent(double discountFactor, double explorationRate, const std::vector<uint32_t>& topology, const std::vector<std::shared_ptr<ActivationFunction>>& activations, double explorationRateDecay = 0.9)
        : Agent(1,discountFactor, explorationRate, explorationRateDecay), neuralNetwork(topology, activations)
    {
        name = NAME;
    }

    std::vector<std::vector<std::pair<int, int>>> train(Environment& env, int iterations, bool verbose = true) override
    {
        std::vector<std::vector<std::pair<int, int>>> hHistory;

        int x, y;
        int state, nextState;
        int action, nextAction;
        double reward;

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

                updateQValue(state, action, reward, nextState);

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
            
            if (env.isTerminalState(x, y)) {
                env.printPath(historyMoves);
                break; // Reached the goal
            }
            nextState = env.getState(x, y);
        }
    }
private:
    // Returns best Action using the neural network for DQN
    int getBestAction(int state)
    {
        int x, y;
        Environment::getPositionFromIndex(state, x, y);

        std::vector<double> qValues = neuralNetwork.forwardPropagation(Environment::oneHotEncoded(x,y));
        return std::distance(qValues.begin(), std::max_element(qValues.begin(), qValues.end()));
    }

    int getBestAction(const std::vector<double>& qValues)
    {
        return std::distance(qValues.begin(), std::max_element(qValues.begin(), qValues.end()));
    }

    // Choose action using the neural network for DQN
    int chooseAction(int state) {
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        if (dist(rng) < explorationRate) {
            return dist(rng) * NUM_ACTIONS; // Random action
        }
        else {
            return getBestAction(state);
        }
    }

    // Update Q-value using the neural network for DQN (Not needed in DQN since we use experience replay)
    void updateQValue(int state, int action, double reward, int nextState)
    {
        int x, y;
        
        Environment::getPositionFromIndex(nextState, x, y);
        std::vector<double> nextQValues = neuralNetwork.forwardPropagation(Environment::oneHotEncoded(x,y));
        int nextBestAction = getBestAction(nextQValues);
        double target = reward + discountFactor * nextQValues[nextBestAction];

        
        Environment::getPositionFromIndex(state, x, y);
        if (x == 6 && y == 8)
        {
            std::cout << "WPWPWPWP\n";
        }
        std::vector<double> QValues = neuralNetwork.forwardPropagation(Environment::oneHotEncoded(x, y));
        QValues[action] = target;
        neuralNetwork.backPropagation(QValues);
        UP, DOWN, LEFT, RIGHT;
    }

};

