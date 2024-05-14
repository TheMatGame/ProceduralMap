#include<iostream>
#include "Chunk.hpp"

Chunk::Chunk(int x, int y) {
	if (!blockTexture.loadFromFile("textures/white.png")) {
		std::cout << "ERROR: No texture load in chunk blocks" << std::endl;
		return;
	}
	
	block.setTexture(blockTexture);
	block.setScale(0.25, 0.25);
	fr = block.getGlobalBounds();
	origin = { x,y };
}

std::pair<float, float> Chunk::toIso(float x, float y, float width, float height) {
	return { -(x * 0.5 * width + y * -0.5 * width),(x * 0.25 * height + y * 0.25 * height) };
}

std::pair<float, float> Chunk::toCart(float x, float y, float width, float height) {
	float inv_det = 1.0 / (0.25 * height * 0.5 * width);
	float x2d = (0.25 * height * x + 0.5 * width * y) * inv_det;
	float y2d = (-0.25 * height * x + 0.5 * width * y) * inv_det;
	return { x2d, y2d };
}

void Chunk::printChunk(float offsetX, float offsetY, sf::RenderWindow* window) {
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			std::pair<float, float> p = toIso(origin.first+i, origin.second+j, fr.width, fr.height);
			block.setPosition((-75 / 2.0) + p.first + offsetX, p.second + offsetY);
			//std::cout << p.first << " " << p.second << std::endl;
			window->draw(block);
		}
	}
}

void Chunk::printChunkWave(float offsetX, float offsetY, float time, sf::RenderWindow* window) {
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			std::pair<float, float> p = toIso(origin.first + i, origin.second + j, fr.width, fr.height);
			float waveMovement = 20 * sin(0.001 * time + i + origin.first) + 20 * sin(0.001 * time + j + origin.second);
			block.setPosition((-75 / 2.0) + p.first + offsetX, p.second + offsetY + waveMovement);
			window->draw(block);
		}
	}
}

void Chunk::printChunkNoise(float offsetX, float offsetY, unsigned char* pixels, sf::RenderWindow* window) {

	unsigned char chunkDistribution[5][5];
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			chunkDistribution[i][j] = pixels[(origin.first + i) * 960 + origin.second + j];
		}
	}

	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			std::pair<float, float> p = toIso(origin.first + i, origin.second + j, fr.width, fr.height);
			int height = chunkDistribution[i][j];
			//for (int k = 0; k <= heigh; ++k) {
				block.setPosition((-75 / 2.0) + p.first + offsetX, p.second + offsetY - height*(fr.height/2.f));
				setColor(height);
				window->draw(block);
			//}
			//std::cout << heigh << " ";
		}
		//std::cout << std::endl;
	}
}

void Chunk::setColor(sf::Color color) {
	block.setColor(color);
}

void Chunk::setColor(float heigh) {
	if (heigh < 6) block.setColor(sf::Color::Blue);
	else if (heigh < 9) block.setColor(sf::Color(237,241,110));
	else if (heigh < 12) block.setColor(sf::Color::Green);
	else if (heigh < 15) block.setColor(sf::Color(145,72,0));
	else if (heigh < 17) block.setColor(sf::Color(100,100,100));
	else if (heigh < 19) block.setColor(sf::Color(200,200,200));
	else block.setColor(sf::Color::White);
}

void Chunk::reScale(float zoom) {
	block.setScale(0.25*zoom, 0.25*zoom);
	fr = block.getGlobalBounds();
}