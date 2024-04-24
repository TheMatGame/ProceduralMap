class PerlinNoise {
public:

	struct vector2 {
		float x, y;
	};

	static float perlin(float x, float y);

private:

	static vector2 randomGradient(int ix, int iy);

	static float dotGridGradient(int ix, int iy, float x, float y);

	static float interpolate(float a0, float a1, float w);
};