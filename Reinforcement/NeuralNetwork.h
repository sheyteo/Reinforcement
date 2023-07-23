#pragma once
#include "pch.h"

constexpr double sinR = 0.03141075907;

double sechSquared(double x) {
	double coshX = std::cosh(x);
	return 1.0 / (coshX * coshX);
}

class ActivationFunction
{
public:
	virtual double apply(const double& val) = 0;
	virtual double d_apply(const double& val) = 0;
};

class Sigmoid : public ActivationFunction
{
public:
	double apply(const double& val) override
	{
		return 1.0 / (1.0 + exp(-val));
	}
	double d_apply(const double& val) override
	{
		double sigmoid_val = apply(val);
		return sigmoid_val * (1.0 - sigmoid_val);
	}
	static std::shared_ptr<Sigmoid> get()
	{
		return std::make_shared<Sigmoid>();
	}
};

class Tanh : public ActivationFunction
{
public:
	double apply(const double& val) override
	{
		return std::tanh(val);
	}

	double d_apply(const double& val) override
	{
		double tanh_val = apply(val);
		return 1.0 - (tanh_val * tanh_val);
	}
	static std::shared_ptr<Tanh> get()
	{
		return std::make_shared<Tanh>();
	}
};

class ReLU : public ActivationFunction
{
public:
	double apply(const double& val) override
	{
		return std::max(0.0, val);
	}

	double d_apply(const double& val) override
	{
		return (val > 0.0) ? 1.0 : 0.0;
	}

	static std::shared_ptr<ReLU> get()
	{
		return std::make_shared<ReLU>();
	}
};

class RTanH : public ActivationFunction
{
	
public:
	double apply(const double& val) override
	{
		return val * sinR + tanh(val);
	}

	double d_apply(const double& val) override
	{
		return sechSquared(val) + sinR;
	}

	static std::shared_ptr<RTanH> get()
	{
		return std::make_shared<RTanH>();
	}
};

double derr(double a, double b)
{
	return 2 * (b - a); //derivative of (a-b)^2
}

class NeuralNetwork
{
public:
	std::vector<uint32_t> topology;
	std::vector<std::vector<double>> values;
	std::vector<std::vector<double>> biasWeights;
	std::vector<std::vector<std::vector<double>>> weights;

	std::random_device rd;
	std::mt19937 gen;

	std::vector<std::shared_ptr<ActivationFunction>> activations;

	double learningRate = 0.2;

	NeuralNetwork(const std::vector<uint32_t>& topology, const std::vector<std::shared_ptr<ActivationFunction>>& activations)
		:topology(topology), activations(activations), gen(rd())
	{
		std::uniform_real_distribution<> distribution(-1, +1);

		//values
		values.resize(topology.size());
		for (size_t i = 0; i < topology.size(); i++)
		{
			values[i] = std::vector<double>(topology[i],0);
		}

		//weights
		weights.resize(topology.size() - 1);
		for (size_t i = 0; i < topology.size() - 1; i++)
		{
			weights[i] = std::vector<std::vector<double>>(topology[i]);
			for (size_t j = 0; j < topology[i]; j++)
			{
				weights[i][j] = std::vector<double>(topology[i + 1]);
				for (size_t k = 0; k < topology[i + 1]; k++)
				{
					weights[i][j][k] = distribution(gen);
				}
			}
		}

		//biasWeights
		biasWeights.resize(topology.size() - 1);
		for (size_t i = 0; i < topology.size() - 1; i++)
		{
			biasWeights[i] = std::vector<double>(topology[i + 1]);
			for (size_t j = 0; j < topology[i + 1]; j++)
			{
				biasWeights[i][j] = distribution(gen);
			}
		}
	}

	std::vector<double> forwardPropagation(const std::vector<double>& input)
	{
		if (input.size() != values.front().size())
		{
			std::cout << "ERROR\n";
			return {};
		}

		values.front() = input;

		for (size_t i = 0; i < topology.size()-1; i++)
		{
			for (size_t k = 0; k < topology[i+1]; k++)
			{
				double sum = biasWeights[i][k];
				for (size_t j = 0; j < topology[i]; j++)
				{
					sum += values[i][j] * weights[i][j][k];
				}
				values[i+1][k] = activations[i]->apply(sum);
			}
		}
		return values.back();
	}

	void backPropagation(const std::vector<double>& expected)
	{
		if (expected.size() != topology.back())
		{
			std::cout << "ERROR WRONG EXPECTED INPUTR SIZE";
			return;
		}

		//calculate errors
		std::vector<std::vector<double>> errors(topology.size() - 1);
		for (size_t i = 0; i < topology.size()-1; i++)
		{
			errors[i] = std::vector<double>(topology[i+1]);
		}

		for (size_t i = 0; i < topology.back(); i++)
		{
			errors.back()[i] = derr(expected[i], values.back()[i]);
		}

		for (size_t i = topology.size()-2; i > 0; i--)
		{
			for (size_t j = 0; j < topology[i]; j++)
			{
				double sum = 0;
				for (size_t k = 0; k < topology[i+1]; k++)
				{
					sum += weights[i][j][k] * errors[i][k];
				}
				errors[i-1][j] = sum * activations[i]->d_apply(values[i][j]);
			}
		}

		//update weights
		for (size_t i = 0; i < topology.size()-1; i++)
		{
			for (size_t k = 0; k < topology[i+1]; k++)
			{
				biasWeights[i][k] -= learningRate * errors[i][k];
				for (size_t j = 0; j < topology[i]; j++)
				{
					weights[i][j][k] -= learningRate * errors[i][k] * values[i][j];
				}
			}
		}
	}
};