# Reinforcement Learning Visualization with C++

This project is a collection of very simple reinforcement learning algorithms implemented in C++ with a basic visualization component. The main purpose of this project is to demonstrate various reinforcement learning algorithms and their behaviors in a grid world environment. The visualization provides an intuitive way to observe how the agents learn and navigate through the environment. The algorithms may not be 100% correct.

## Dependencies
To compile and run this project, you will need the following dependencies:

SFML library (Simple and Fast Multimedia Library): The visualization component relies on SFML to create the graphical interface.
Installation
Make sure you have the SFML library installed on your system. For Windows users, the project should work out of the box. However, for Linux users, you may need to recompile the SFML library to make it compatible.

## Usage
The project allows you to visualize different reinforcement learning algorithms in action. Here's how to use it:

The program starts with a grid world environment.

Several agents are available, each implementing a different reinforcement learning algorithm.

### Code

main.cpp

```c++
#include "SARSA.h"
#include "DoubleQAgent.h"
#include "NStepQAgent.h" //Needs optimisation
#include "DQNAgent.h" //Not yet Working
#include "GUI.h"

int main()
{
    Environment gridWorld;
    std::vector<std::shared_ptr<Agent>> agents;

    //Basic Q-Learning 
    agents.emplace_back(new QAgent(0.4, 0.95, 0.2, 0.90));

    //SARSA (similar to Q-Learning but tries to avoid going near bad squares)
    agents.emplace_back(new SARSA(0.4, 0.95, 0.2, 0.90));
   
    //agents.emplace_back(new DoubleQAgent(0.4, 0.95, 0.2, 0.90)); //does not really work

    //Q_Learning but able to specifing depth of Forward looking(4th paramter)
    agents.emplace_back(new NStepQAgent(0.4, 0.95, 0.2, 2, 0.90));
    
    //Q_Learning with NeuralNetwork
    agents.emplace_back(new DQNAgent(0.4, 0.95,{GRID_SIZE*2,50,20,4},{ReLU::get(),ReLU::get(),RTanH::get()})); //Not yet Working

    //Adjust cellSize for samller Monitors
    RLGUI rlgui(sf::VideoMode(cellSize * GRID_SIZE + 350, cellSize * GRID_SIZE), "Reinforcment LEarning Test", 7);
    rlgui.mainLoop(gridWorld, agents);

    return 0;
}
```

### Controls

The visualization provides control over the number of iterations per visualization update. Use the following controls to adjust the visualization speed:

SpaceBar: 1 iteration per visualization update
NUM1: 5 iterations per visualization update
NUM2: 20 iterations per visualization update
NUM3: 50 iterations per visualization update
NUM4: 100 iterations per visualization update
NUM5: 200 iterations per visualization update
NUM6: 1000 iterations per visualization update
To finish the iteration, press ENTER.

To close the program at any time, press ESCAPE.

## Agents
QAgent: Basic Q-Learning agent with configurable learning rate and discount factor.

SARSA: Similar to Q-Learning but tries to avoid going near bad squares.

NStepQAgent: Q-Learning agent with the ability to specify the depth of forward-looking.

DQNAgent: Q-Learning agent with a neural network for learning.

## Notes
The project is currently developed and tested on Windows, but with slight adjustments, it can be made to work on Linux as well (recompile SFML for Linux).

The DQNAgent is a work in progress and may not be fully functional yet.

Feel free to experiment with different parameters and agents to observe their effects on the agents' learning behavior.

