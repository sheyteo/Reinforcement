#include "pch.h"
#include "Enviroment.h"
#include "QAgent.h"
#include "SARSA.h"
#include "DoubleQAgent.h"
#include "NStepQAgent.h"
#include "DQNAgent.h"
#include "names.h"
#include "GUI.h"

/// CONTROLS    Iterations per Visualsisation
/// SpaceBar -> 1
/// NUM1     -> 5
/// NUM2     -> 20
/// NUM3     -> 50
/// NUM4     -> 100
/// NUM5     -> 200
/// NUM6     -> 500

/// ENTER -> Finish this Visualsisation
/// ESCAPE -> close Program

/// Playing around with the parameters changes the behaviour of the Agents so feel free to expiriment
 
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