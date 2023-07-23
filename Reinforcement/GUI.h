#pragma once
#include "pch.h"
#include "Enviroment.h"
#include "Agent.h"

constexpr int cellSize = 35;

//return shortest vector from vector
template<typename T>
std::vector<T> sV(const std::vector<std::vector<T>>& vectors) {
    if (vectors.empty()) {
        // Return an empty vector if the input vector is empty
        return std::vector<T>();
    }

    size_t shortestLength = vectors[0].size();
    size_t shortestIndex = 0;

    // Find the shortest vector
    for (size_t i = 1; i < vectors.size(); ++i) {
        if (vectors[i].size() < shortestLength) {
            shortestLength = vectors[i].size();
            shortestIndex = i;
        }
    }

    // Return the shortest vector
    return vectors[shortestIndex];
}

//creates an arrow curently unused
sf::VertexArray createArrow(int n, const sf::Vector2f& position, Actions direction) {
    sf::VertexArray arrow(sf::Triangles);

    // Length and width of the arrow
    float length = static_cast<float>(n);
    float width = static_cast<float>(n) / 2.0f;

    // Points for the arrow head
    sf::Vector2f top(0.0f, -length);
    sf::Vector2f left(-width, 0.0f);
    sf::Vector2f right(width, 0.0f);

    // Points for the arrow body
    sf::Vector2f bodyTop(0.0f, 0.0f);
    sf::Vector2f bodyBottom(0.0f, n - length);

    // Transform arrow direction
    switch (direction) {
    case UP:
        break;
    case DOWN:
        top = -top;
        left = -left;
        right = -right;
        bodyTop = -bodyTop;
        bodyBottom = -bodyBottom;
        break;
    case RIGHT:
        std::swap(left, right);
        std::swap(bodyTop, bodyBottom);
        break;
    case LEFT:
        std::swap(left, right);
        top = -top;
        bodyTop = -bodyTop;
        bodyBottom = -bodyBottom;
        break;
    }

    sf::Color color = sf::Color::Blue;
    color.a /= 2;

    // Adding points to the vertex array
    sf::Vector2f add(n / 2, 0);
    if (direction == UP)
    {
        add.y += 1*n;
    }
    arrow.append(sf::Vertex(top + position+add, color));
    arrow.append(sf::Vertex(left + position + add, color));
    arrow.append(sf::Vertex(right + position + add, color));
    arrow.append(sf::Vertex(bodyTop + position + add, color));
    arrow.append(sf::Vertex(bodyBottom + position + add, color));

    return arrow;
}

class RLGUI
{
	sf::RenderWindow window;
	sf::Font font;

    int progression = 0;
    int stopAt = 0;

    std::vector<std::vector<std::pair<int,int>>> history;

    std::chrono::steady_clock::time_point lastUpdate = std::chrono::steady_clock::now();

    bool change = true;
    int iteration = 0;
    /*bool drawA = false;*/

    std::vector<std::shared_ptr<Agent>> agents;

    std::map<uint32_t,std::vector<uint32_t>> stepHistory;

public:
	RLGUI(const sf::VideoMode& videomode, const std::string& windowName, sf::Uint32 style)
		:window(videomode,windowName,style)
	{
		font.loadFromFile("KOMIKAX_.ttf");
	}

	void mainLoop(Environment& env, const std::vector<std::shared_ptr<Agent>>& _agents)
	{
        agents = _agents;
        std::map<uint32_t, sf::Color> cMap;
        {
            sf::Color color(255, 215, 0);

            for (const auto& agent : agents)
            {
                cMap[agent->getId()] = color;
                color.b += 256 / 2;
                color.g += 256 / 3;
                color.r += 256 / 5;
            }
        }

        //main loop
        while (window.isOpen())
        {
            // Event handling
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                    window.close();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                {
                    auto temp_time = std::chrono::steady_clock::now();
                    if ((temp_time - lastUpdate) >= 10ms)
                    {

                        lastUpdate = temp_time;
                        startTrain(env, 1);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1))
                {
                    startTrain(env, 5);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
                {
                    startTrain(env, 20);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3))
                {
                    startTrain(env, 50);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
                {
                    startTrain(env, 100);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5))
                {
                    startTrain(env, 250);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6))
                {
                    startTrain(env, 500);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                {
                    progression = MAX_STEPS;
                }
                /*if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                {
                    change = true;
                    drawA = true;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                {
                    change = true;
                    drawA = false;
                }*/
            }

            
            if (change) //if something on the screen changed then
            {
                change = false;
                // Clear the window
                window.clear(sf::Color::White);

                // Draw the Map
                for (int i = 0; i < GRID_SIZE; ++i)
                {
                    for (int j = 0; j < GRID_SIZE; ++j)
                    {
                        sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
                        cell.setPosition(i * cellSize, j * cellSize);

                        if (env.at(i, j) == 0) // Empty (white)
                            cell.setFillColor(sf::Color::White);
                        else if (env.at(i, j) == 1) // Brown
                            cell.setFillColor(sf::Color(128, 128, 128)); // Brown color
                        else if (env.at(i, j) == 2) // Green
                            cell.setFillColor(sf::Color::Green);

                        window.draw(cell);
                    }
                }

                int idx = 0;
                bool needMore = false;

                //draw current history
                for (const auto& agentHistory : history)
                {
                    if (agentHistory.size() > progression)
                    {
                        sf::RectangleShape cell(sf::Vector2f(cellSize - 4, cellSize - 4));
                        cell.setFillColor(cMap[agents[idx]->getId()]);
                        cell.setPosition(agentHistory[progression].first * cellSize + 2, agentHistory[progression].second * cellSize + 2);
                        cell.setOutlineThickness(2);
                        cell.setOutlineColor(sf::Color::Black);
                        window.draw(cell);
                        needMore = true;
                    }
                    idx++;
                }

                idx = 0;

                double factor = cellSize / 50.f;

                int bxSize = window.getSize().x - cellSize*GRID_SIZE;
                int bx1Size = window.getSize().y / 4;

                //write info to the right magin
                for (const auto& agent: agents)
                {
                    sf::RectangleShape textBox(sf::Vector2f(bxSize, bx1Size));
                    textBox.setFillColor(cMap[agent->getId()]);
                    textBox.setPosition(window.getSize().x - bxSize, idx * bx1Size);
                    window.draw(textBox);

                    sf::Text numberText;
                    numberText.setFont(font);

                    int sPath = 0;
                    if (history.size() > idx)
                    {
                        sPath = history[idx].size();
                    }
                    numberText.setString(std::string("Algorithm: ") + agent->getName() + "\nShortest Path : " + std::to_string(sPath) + "\nIteration : " + std::to_string(iteration)+"\n"+agent->getInfoText()); // Replace "42" with your desired 
                    numberText.setCharacterSize(25*factor);
                    numberText.setFillColor(sf::Color::Black);
                    numberText.setPosition(cellSize*GRID_SIZE+10, 10 + idx * bx1Size);
                    window.draw(numberText);
                    idx++;
                }

                //draw line between text and Grid
                sf::RectangleShape thickLine;
                thickLine.setPosition(sf::Vector2f(cellSize* GRID_SIZE, 0.0f));
                thickLine.setSize(sf::Vector2f(3.0f, cellSize* GRID_SIZE));
                thickLine.setFillColor(sf::Color::Black);
                window.draw(thickLine);
                if (!needMore)
                {
                    progression = MAX_STEPS;
                }
                /*if (drawA)
                {
                    DQNAgent* dqn = (DQNAgent*)agents[0].get();
                    for (size_t x = 0; x < GRID_SIZE; x++)
                    {
                        for (size_t y = 0; y < GRID_SIZE; y++)
                        {
                            if (env.at(x,y) != 1 && env.at(x, y) != 2)
                            {
                                auto a = dqn->neuralNetwork.forwardPropagation(Environment::oneHotEncoded(x,y));
                                int idx = std::distance(a.begin(), std::max_element(a.begin(), a.end()));
                                std::cout << a[0] <<" " << a[1] << " " << a[2] << " " << a[3] << "\n";
                                window.draw(createArrow(cellSize, sf::Vector2f(x*cellSize, y*cellSize),(Actions)idx));
                            }
                        }
                    }
                }*/
            }
            //display
            window.display();
        }
	}
private:
    //Start training iterations and saves history
    void startTrain(Environment& env, uint32_t iterations)
    {
        change = true;

        progression++;
        if (progression >= stopAt)
        {

            history.clear();
            for (const auto& agent: agents)
            {
                auto temp = agent->train(env, iteration, false); // train
                auto temp1 = sV(temp);
                stopAt = std::max(stopAt, (int)temp1.size());
                history.push_back(std::move(temp1));

                for (const auto& ap: temp)
                {
                    if (!stepHistory.contains(agent->getId()))
                    {
                        stepHistory[agent->getId()] = {};
                    }
                    stepHistory[agent->getId()].push_back(ap.size());
                }
            }

            iteration += iterations;
            progression = 0;
        }
    }
};