#include <SFML/Graphics.hpp>

class Chunk {
public:

	Chunk(int x, int y);

	sf::Texture blockTexture;
	sf::Sprite block;
	sf::FloatRect fr;

	std::pair<int, int> origin;

	void printChunk(float offsetX, float offsetY, sf::RenderWindow* window);
	void printChunkWave(float offsetX, float offsetY, float time, sf::RenderWindow* window);

	void setScale(float scale);
	void setColor(sf::Color color);

private:
	std::pair<int, int> toIso(float x, float y, float width, float height);
	std::pair<float, float> toCart(float x, float y, float width, float height);
};