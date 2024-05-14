#include <SFML/Graphics.hpp>

class Chunk {
public:

	Chunk(int x, int y);

	sf::Texture blockTexture;
	sf::Sprite block;
	sf::FloatRect fr;

	std::pair<int, int> origin;
	std::pair<int, int> diagonal;

	void printChunk(float offsetX, float offsetY, sf::RenderWindow* window);
	void printChunkWave(float offsetX, float offsetY, float time, sf::RenderWindow* window);
	void printChunkNoise(float offsetX, float offsetY, unsigned char* pixels, sf::RenderWindow* window);

	void setColor(sf::Color color);
	void setColor(float heigh);

	void reScale(float zoom);

private:
	std::pair<float, float> toIso(float x, float y, float width, float height);
	std::pair<float, float> toCart(float x, float y, float width, float height);
};