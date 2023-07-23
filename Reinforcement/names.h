#pragma once
#include "QAgent.h"
#include "SARSA.h"
#include "DoubleQAgent.h"
#include "NStepQAgent.h"
#include "DQNAgent.h"

uint32_t Agent::agentCounter = 0;
std::string NStepQAgent::NAME = "NStepQAgent";
std::string QAgent::NAME = "QAgent";
std::string SARSA::NAME = "SARSA";
std::string DQNAgent::NAME = "DQNAgent";
std::string DoubleQAgent::NAME = "DQAgent";