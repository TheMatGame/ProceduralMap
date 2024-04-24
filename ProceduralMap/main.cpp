#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "PerlinNoise.hpp"
#include "Chunk.hpp"

#define Step 10
#define MapSize 1

std::pair<int, int> toIso(float x, float y, float width, float height) {
    return {-(x*0.5*width+y*-0.5*width),(x*0.25*height+y*0.25*height)};
}

int main()
{
    const int windowWidth = 1920/2;
    const int windowHeight = 1080/2;

    Chunk* terrain [MapSize][MapSize];
    //Inicialize terrain
    for (int i = 0; i < MapSize; ++i)
        for (int j = 0; j < MapSize; ++j) {
            terrain[i][j] = new Chunk(i*5,j*5);
        }

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight, 32), "Procedural Terrain");

    sf::Uint8* pixels = new sf::Uint8[windowWidth * windowHeight];

    sf::CircleShape point(5);
    point.setOrigin(5, 5);
    point.setPosition(windowWidth/2,windowHeight/2);


    const int GRID_SIZE = 300;

    /*
    for (int x = 0; x < windowWidth; x++)
    {
        for (int y = 0; y < windowHeight; y++)
        {
            int index = y * windowWidth + x;


            float val = 0;

            float freq = 1;
            float amp = 1;

            for (int i = 0; i < 12; i++)
            {
                val += PerlinNoise::perlin(x * freq / GRID_SIZE, y * freq / GRID_SIZE) * amp;

                freq *= 2;
                amp /= 2;

            }

            // Contrast
            val *= 1.2;

            // Clipping
            if (val > 1.0f)
                val = 1.0f;
            else if (val < -1.0f)
                val = -1.0f;

            // Convert 1 to -1 into 255 to 0
            int color = (int)(((val + 1.0f) * 0.5f) * 255);

            // Set pixel color
            pixels[index] = color;
        }
    }
    */


    std::pair<int, int> playerPos = { 0,0 };
    float zoom = 0.25;

    float blockDimension = 300 * zoom;

    float offsetX = -blockDimension / 2 + windowWidth/2;
    float offsetY = windowHeight/4;


    sf::Clock timer;
    timer.restart();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                // Por si acaso //
                for (int i = 0; i < MapSize; ++i) {
                    for (int j = 0; j < MapSize; ++j) {
                        delete terrain[i][j];
                    }
                }
                window.close();
            }
            // Movement WASD
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                playerPos = { playerPos.first+1,playerPos.second };
                offsetY += Step;
                std::cout << playerPos.first << " " << playerPos.second << std::endl;
                for (int i = 0; i < MapSize; ++i) {
                    for (int j = 0; j < MapSize; ++j) {
                        delete terrain[i][j];
                        int x = playerPos.first / 5;
                        int y = playerPos.second / 5;
                        terrain[x][y] = new Chunk(x, y);
                    }
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                playerPos = { playerPos.first,playerPos.second-1 };
                offsetX += Step;
                std::cout << playerPos.first << " " << playerPos.second << std::endl;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                playerPos = { playerPos.first-1,playerPos.second };
                offsetY -= Step;
                std::cout << playerPos.first << " " << playerPos.second << std::endl;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                playerPos = { playerPos.first,playerPos.second+1 };
                offsetX -= Step;
                std::cout << playerPos.first << " " << playerPos.second << std::endl;
            }
            // Zoom in - Zoom out
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            }
        }


        window.clear();

        int TIME = timer.getElapsedTime().asMilliseconds();
        //float waveMovement = 5 * sin(0.005 * TIME + i) + 5 * sin(0.005 * TIME + j);

        for (int i = 0; i < MapSize *5; i+=5) {
            for (int j = 0; j < MapSize *5; j+=5) {
                //std::pair<int, int> p = toIso(i, j, blockDimension, blockDimension);

                //terrain[i / 5][j / 5]->setScale(zoom);
                //terrain[i/5][j/5]->printChunkWave(p.first + offsetX, p.second + offsetY, TIME, &window);
                terrain[i / 5][j / 5]->printChunk(offsetX, offsetY, &window);
            }
        }


        //window.draw(sprite);
        window.draw(point);
        window.display();
    }

    return 0;
}