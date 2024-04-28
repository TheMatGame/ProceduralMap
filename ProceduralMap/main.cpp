#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "PerlinNoise.hpp"
#include "Chunk.hpp"

#define Step 10
#define MapSize 2

const int windowWidth = 1920 / 2;
const int windowHeight = 1080 / 2;

std::pair<int, int> playerPos = { 0,0 };


std::pair<int, int> toIso(float x, float y, float width, float height) {
    return {-(x*0.5*width+y*-0.5*width),(x*0.25*height+y*0.25*height)};
}

std::pair<float, float> cartToScreen(float cartX, float cartY, float screenWidth, float screenHeight, float zoomFactor, float offsetX, float offsetY) {
    float screenX = zoomFactor * cartX + screenWidth / 2 + offsetX;
    float screenY = screenHeight / 2 - zoomFactor * cartY - offsetY;
    return { screenX,screenY };
}

bool insideOf(/*int x, int y,*/ Chunk* chunk) {
    int ox = chunk->origin.first;
    int oy = chunk->origin.second;
    int width = chunk->fr.width;
    int height = chunk->fr.height;
    //Window coordinates
    //std::pair<float, float> offsetPlayer = cartToScreen(0, 0, windowWidth, windowHeight, 1, -playerPos.first, -playerPos.second);
    std::pair<float, float> offsetChunk = cartToScreen(ox, oy, windowWidth, windowHeight, 1, -playerPos.first, -playerPos.second);

    int minX = toIso(ox + 5, oy    , width, height).first + offsetChunk.first;
    int maxX = toIso(ox    , oy + 5, width, height).first + offsetChunk.first;
    int minY = toIso(ox    , oy    , width, height).second + offsetChunk.second;
    int maxY = toIso(ox + 5, oy + 5, width, height).second + offsetChunk.second;

    int ancho = std::abs(minX - maxX);
    int alto = std::abs(minY - maxY);
    float dx = std::abs(windowWidth / 2 - (minX+maxX)/2);
    float dy = std::abs(windowHeight / 2 - (minY+maxY)/2);

    float calc = (dx / ancho) + (dy / alto);

    return  0 <= calc && calc <= 0.5;
}

int main()
{
   
    Chunk* terrain [MapSize][MapSize];
    //Inicialize terrain
    for (int i = 0; i < MapSize; ++i)
        for (int j = 0; j < MapSize; ++j) {
            terrain[i][j] = new Chunk(i*5,j*5);
        }

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight, 32), "Procedural Terrain");

    //sf::Uint8* pixels = new sf::Uint8[windowWidth * windowHeight];
    
    //const int GRID_SIZE = 300;

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


    float zoom = 0.25;

    sf::CircleShape point(5);
    point.setOrigin(5, 5);

    sf::CircleShape pmove(5);
    pmove.setFillColor(sf::Color::Red);
    pmove.setOrigin(5, 5);
    pmove.setPosition(windowWidth / 2, windowHeight / 2);


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
            // Movement WASD -- esta mal -> cambiar los ejes
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                playerPos = { playerPos.first,playerPos.second + Step };
                std::cout << playerPos.first << " " << playerPos.second << std::endl;

                for (int i = 0; i < MapSize; ++i) {
                    for (int j = 0; j < MapSize; ++j) {
                        if (insideOf(terrain[i][j])) {
                            std::cout << "Dentro de " << i << " " << j << std::endl;
                            terrain[i][j]->setColor(sf::Color::Red);
                        }
                        else {
                            terrain[i][j]->setColor(sf::Color::White);
                        }
                    }
                }
                
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                playerPos = { playerPos.first - Step,playerPos.second };
                std::cout << playerPos.first << " " << playerPos.second << std::endl;

                for (int i = 0; i < MapSize; ++i) {
                    for (int j = 0; j < MapSize; ++j) {
                        if (insideOf(terrain[i][j])) {
                            std::cout << "Dentro de " << i << " " << j << std::endl;
                            terrain[i][j]->setColor(sf::Color::Red);
                        }
                        else {
                            terrain[i][j]->setColor(sf::Color::White);
                        }
                    }
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                playerPos = { playerPos.first,playerPos.second - Step };
                std::cout << playerPos.first << " " << playerPos.second << std::endl;

                for (int i = 0; i < MapSize; ++i) {
                    for (int j = 0; j < MapSize; ++j) {
                        if (insideOf(terrain[i][j])) {
                            std::cout << "Dentro de " << i << " " << j << std::endl;
                            terrain[i][j]->setColor(sf::Color::Red);
                        }
                        else {
                            terrain[i][j]->setColor(sf::Color::White);
                        }
                    }
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                playerPos = { playerPos.first + Step,playerPos.second };
                std::cout << playerPos.first << " " << playerPos.second << std::endl;

                for (int i = 0; i < MapSize; ++i) {
                    for (int j = 0; j < MapSize; ++j) {
                        if (insideOf(terrain[i][j])) {
                            std::cout << "Dentro de " << i << " " << j << std::endl;
                            terrain[i][j]->setColor(sf::Color::Red);
                        }
                        else {
                            terrain[i][j]->setColor(sf::Color::White);
                        }
                    }
                }
            }
            // Zoom in - Zoom out
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {}
        }


        window.clear();

        int TIME = timer.getElapsedTime().asMilliseconds();
        std::pair<float, float> offset = cartToScreen(0, 0, windowWidth, windowHeight, 1, -playerPos.first, -playerPos.second);

        for (int i = 0; i < MapSize; ++i) {
            for (int j = 0; j < MapSize; ++j) {
                //terrain[i][j]->printChunkWave(offset.first, offset.second, TIME, &window);
                terrain[i][j]->printChunk(offset.first, offset.second, &window);
            }
        }


        //window.draw(sprite
        point.setPosition(offset.first, offset.second);
        window.draw(point);
        window.draw(pmove);
        window.display();
    }

    return 0;
}