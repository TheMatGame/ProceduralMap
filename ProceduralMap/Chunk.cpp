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

std::pair<int, int> Chunk::toIso(float x, float y, float width, float height) {
	return { -(x * 0.5 * width + y * -0.5 * width),(x * 0.25 * height + y * 0.25 * height) };
}

std::pair<float, float> Chunk::toCart(float x, float y, float width, float height) {
	float inv_det = 1.0 / (0.25 * height * 0.5 * width);
	float x2d = (0.25 * height * x + 0.5 * width * y) * inv_det;
	float y2d = (-0.25 * height * x + 0.5 * width * y) * inv_det;
	return { x2d, y2d };
}

void Chunk::printChunk(float offsetX, float offsetY, sf::RenderWindow* window) {
	for (int i = 1; i <= 5; ++i) {
		for (int j = 1; j <= 5; ++j) {
			std::pair<int, int> p = toIso(origin.first+i, origin.second+j, fr.width, fr.height);
			block.setPosition(p.first + offsetX, p.second + offsetY);
			window->draw(block);
		}
	}
}

void Chunk::printChunkWave(float offsetX, float offsetY, float time, sf::RenderWindow* window) {
	for (int i = 1; i <= 5; ++i) {
		for (int j = 1; j <= 5; ++j) {
			std::pair<int, int> p = toIso(i, j, fr.width, fr.height);
			float waveMovement = 20 * sin(0.001 * time + i + origin.first) + 20 * sin(0.001 * time + j + origin.second);
			block.setPosition(p.first + offsetX, p.second + offsetY + waveMovement);
			window->draw(block);
		}
	}
}

void Chunk::setScale(float scale) {
	block.setScale(scale, scale);
	fr = block.getGlobalBounds();
}

void Chunk::setColor(sf::Color color) {
	block.setColor(color);
}