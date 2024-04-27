#include "SFML/Graphics.hpp"
#include <vector>
#include <random>
#include <iostream>


struct Sand {    
    sf::Color color;
    bool exists = false;
    sf::VertexArray shape;
};

const int SIZE_OF_MATRIX = 64;
const unsigned int SCREEN_SIZE_X = 800;
const unsigned int SCREEN_SIZE_Y = 800;

void drawSandMatrix(std::vector<std::vector<Sand>>& sandMatrix, sf::RenderWindow& window);
bool leftOrRight();
void updateSandMatrix(std::vector<std::vector<Sand>>& sandMatrix);


int main()
{
    sf::RenderWindow window(
        sf::VideoMode(800, 800),
        "Glowing sand");

    sf::Event e;
    
    
    /*
        Sand matrix that is used to do all of the logic
        Holds all of the blocks and everything within the matrix
    */
    std::vector<std::vector<Sand>> sandMatrix(SIZE_OF_MATRIX, std::vector<Sand>(SIZE_OF_MATRIX));

    sf::Vector2f sizeOfSand(
        (SCREEN_SIZE_X / 2) / SIZE_OF_MATRIX,
        (SCREEN_SIZE_X / 2) / SIZE_OF_MATRIX
    );

    // Main loop
    while (window.isOpen())
    {
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }

            if (e.type == sf::Event::KeyReleased)
            {
                if (e.key.code == sf::Keyboard::LShift)
                {
                    // cleanSegmentEditStuff()
                }
            }
            

            // Clear the window
            window.clear(sf::Color::Black);

            // draw
            drawSandMatrix(sandMatrix, window);
            

            // display
            window.display();
        }

    }
}

void initializeMatrix(std::vector<std::vector<Sand>>& sandMatrix, const sf::Vector2f sandDimensions)
{
    for (int i = 0; i < sandMatrix.size(); i++)
    {
        for (int k = 0; k < sandMatrix[0].size(); k++)
        {
            sf::VertexArray shape = sf::VertexArray(sf::Quads, 4);

            sandMatrix[i][k].shape = shape;

            shape[0].position = sf::Vector2f(i * sandDimensions.x, k * sandDimensions.y);
        }
    }
    
}

void addSand(std::vector<std::vector<Sand>>& sandMatrix, sf::Vector2f positionClicked)
{
    
}

void updateSandMatrix(std::vector<std::vector<Sand>>& sandMatrix)
{
    for (int i = sandMatrix.size() - 1; i >= 0; i--)
    {
        for (int k = 0; k < sandMatrix[i].size(); k++)
        {
            if (sandMatrix[i][k].exists)
            {
                // there exists an active sand block on this position

                if (i == sandMatrix.size() - 1)
                {
                    // if we are at the most bottom layer, don't update
                    continue;
                }

                if (sandMatrix[i - 1][k].exists)
                {
                    // there is a sand block underneath already
                    continue;
                }

                // Otherwise, we have to go down

                bool leftAvailable = false;
                bool rightAvailable = false;

                if (k - 1 >= 0)
                {
                    if (!sandMatrix[i][k].exists)
                    {
                        leftAvailable = true;
                    }
                }

                if (k <= sandMatrix[i].size() - 2)
                {
                    if (!sandMatrix[i][k].exists)
                    {
                        rightAvailable = true;
                    }
                }

                // Can't go left or right
                if (!leftAvailable && !rightAvailable)
                {
                    continue;
                }

                // false for left, true for right
                bool leftRight = leftOrRight();
                Sand sand = sandMatrix[i][k];

                if (leftRight)
                {
                    // check for right availability first
                    if (rightAvailable)
                    {
                        sandMatrix[i][k].exists = false;
                        sandMatrix[i][k + 1].shape = sand.shape;
                        sandMatrix[i][k + 1].exists = true;
                        continue;
                    }

                    if (leftAvailable)
                    {
                        sandMatrix[i][k].exists = false;
                        sandMatrix[i][k - 1].shape = sand.shape;
                        sandMatrix[i][k - 1].exists = true;
                        continue;
                    }  
                }
                else
                {

                    if (leftAvailable)
                    {
                        sandMatrix[i][k].exists = false;
                        sandMatrix[i][k - 1].shape = sand.shape;
                        sandMatrix[i][k - 1].exists = true;
                        continue;
                    }

                    if (rightAvailable)
                    {
                        sandMatrix[i][k].exists = false;
                        sandMatrix[i][k + 1].shape = sand.shape;
                        sandMatrix[i][k + 1].exists = true;
                        continue;
                    }
                }


            }
             
        }
    }
}

void drawSandMatrix(std::vector<std::vector<Sand>>& sandMatrix, sf::RenderWindow& window)
{
    for (int i = 0; i < sandMatrix.size(); i++)
    {
        for (int k = 0; k < sandMatrix[i].size(); k++)
        {
            if (sandMatrix[i][k].exists)
            {
                window.draw(sandMatrix[i][k].shape);
            }
        }
    }
}

bool leftOrRight() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 1); // distribution in range [1, 6]

    if (dist6(rng) == 1)
    {
        return true;
    }

    return false;
}