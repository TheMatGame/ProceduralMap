#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "PerlinNoise.hpp"
#include "Chunk.hpp"

#define Step 10
#define MapSize 30

const int windowWidth = 1920 / 2;
const int windowHeight = 1080 / 2;

std::pair<int, int> playerPos = { 0,0 };

// La posicion inicial del objeto y el tamaño que este ocupa (sirve para calcular la separacion)
std::pair<int, int> toIso(float x, float y, float width, float height) {
    return {-(x*0.5*width+y*-0.5*width),(x*0.25*height+y*0.25*height)};
}

std::pair<int, int> toCart(float x, float y, float w, float h) {
    float det = 0.5 * w * 0.25 * h - (-0.5 * w * 0.25 * h);
    float invDet = 1.f / det;

    // Inversa de la matriz
    float a = 0.25 * h * invDet;
    float b = 0.5 * w * invDet;
    float c = -0.25 * h * invDet;
    float d = 0.5 * w * invDet;

    float newX = c * x + d * y;
    float newY = a * x + b * y;
    return { newX, newY };
}

std::pair<float, float> cartToScreen(float cartX, float cartY, float screenWidth, float screenHeight, float zoomFactor, float offsetX, float offsetY) {
    float screenX = zoomFactor * cartX + screenWidth / 2 + offsetX;
    float screenY = screenHeight / 2 - zoomFactor * cartY - offsetY;
    return { screenX,screenY };
}

bool insideOf(Chunk* chunk, float offsetX, float offsetY) {
    int ox = chunk->origin.first;
    int oy = chunk->origin.second;
    int width = chunk->fr.width;
    int height = chunk->fr.height;

    std::pair<int, int> isoPlayerPos = { playerPos.first / width, playerPos.second / height };
    isoPlayerPos = toIso(isoPlayerPos.first, isoPlayerPos.second, width, height);
    //isoPlayerPos = { (-75 / 2.0) + isoPlayerPos.first + offsetX, isoPlayerPos.second + offsetY };
    
    std::cout << isoPlayerPos.first << " " << isoPlayerPos.second << std::endl;
    /*
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

    return  0 <= calc && calc <= 0.5;*/
    return false;
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

    sf::Uint8* pixels = new sf::Uint8[windowWidth * windowHeight * 4];
    unsigned char* noisemap = new unsigned char[windowWidth * windowHeight];


    const int GRID_SIZE = 70;


    for (int x = 0; x < windowWidth; x++)
    {
        for (int y = 0; y < windowHeight; y++)
        {
            int index = (y * windowWidth + x) * 4;


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

            int r, g, b, h = (val + 1.f) / 0.1;
            noisemap[y * windowWidth + x] = h;
            if (h < 6) { r = 0, g = 0, b = 255; noisemap[y * windowWidth + x] = 5; }    // water
            else if (h < 9) { r = 237, g = 241, b = 110;  }                             // sand
            else if (h < 12) { r = 0, g = 255, b = 0; }                                 // grass
            else if (h < 15) { r = 145, g = 72, b = 0; }                                // dirt
            else if (h < 17) { r = 100, g = 100, b = 100; }                             // rocks
            else if (h < 19) { r = 200, g = 200, b = 200; }                             // mountain rocks
            else { r = 255, g = 255, b = 255; }                                         // snow

            // Convert 1 to -1 into 255 to 0
            /*int color = (int)(((val + 1.0f) * 0.5f) * 10);
            noisemap[y * windowWidth + x] = color;*/

            // Set pixel color
            pixels[index] = r;
            pixels[index + 1] = g;
            pixels[index + 2] = b;
            pixels[index + 3] = 255;
        }
    }

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            int aux = noisemap[i * windowWidth + j];
            std::cout << aux << " ";
        }
        std::cout << std::endl;
    }

    sf::Texture texture;
    sf::Sprite sprite;
 
    texture.create(windowWidth, windowHeight);
 
    texture.update(pixels);
 
    sprite.setTexture(texture);
    
    bool showPerlin = false;

    float zoom = 1;

    sf::CircleShape point(5);
    point.setOrigin(5, 5);

    sf::CircleShape pmove(5);
    pmove.setFillColor(sf::Color::Red);
    pmove.setOrigin(5, 5);
    pmove.setPosition(windowWidth / 2, windowHeight / 2);


    sf::Clock timer;
    timer.restart();

    std::pair<float, float> offset = { 0,0 };

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
                //std::cout << playerPos.first << " " << playerPos.second << std::endl;
                //insideOf(terrain[0][0], offset.first, offset.second);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                playerPos = { playerPos.first - Step,playerPos.second };
                //std::cout << playerPos.first << " " << playerPos.second << std::endl;
                //insideOf(terrain[0][0], offset.first, offset.second);

            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                playerPos = { playerPos.first,playerPos.second - Step };
                //std::cout << playerPos.first << " " << playerPos.second << std::endl;
                //insideOf(terrain[0][0], offset.first, offset.second);

            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                playerPos = { playerPos.first + Step,playerPos.second };
                //std::cout << playerPos.first << " " << playerPos.second << std::endl;
                //insideOf(terrain[0][0], offset.first, offset.second);

                /*
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
                */
            }
            // Zoom in - Zoom out
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                showPerlin = true;
                std::cout << showPerlin << std::endl;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
                showPerlin = false;
                std::cout << showPerlin << std::endl;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
                zoom += 0.1; 
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
                zoom -= 0.1;
            }
        }


        window.clear();

        int TIME = timer.getElapsedTime().asMilliseconds();
        offset = cartToScreen(0, 0, windowWidth, windowHeight, 1, -playerPos.first, -playerPos.second);

        for (int i = 0; i < MapSize; ++i) {
            for (int j = 0; j < MapSize; ++j) {
                terrain[i][j]->reScale(zoom);

                //terrain[i][j]->printChunkWave(offset.first, offset.second, TIME, &window);
                //terrain[i][j]->printChunk(offset.first, offset.second, &window);
                terrain[i][j]->printChunkNoise(offset.first, offset.second, noisemap, &window);
            }
        }


        //window.draw(sprite
        point.setPosition(offset.first, offset.second);
        window.draw(point);
        window.draw(pmove);
        if (showPerlin) {
            window.clear();
            window.draw(sprite);
        }
        window.display();
    }

    return 0;
}