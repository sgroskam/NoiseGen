#ifndef NOISE_H_
#define NOISE_H_

#include <time.h>

class PerlinNoise
{
public: 

	PerlinNoise(int seed, int wavelength, int octaves, int maxHeight, float persistence);
	~PerlinNoise(){};

	void SetX(int newX){x = newX;}
	void SetY(int newY){y = newY;}
	void SetAmplitude(int newAmplitude){itsAmplitude = newAmplitude;}
	void SetOctaves(int newOctaves){itsOctaves = newOctaves;}
	void SetWavelength(int newWavelength){itsWavelength = newWavelength;}

	float GetHeight();
	int GetX(){return x;}
	int GetY(){return y;}
	float PerlinNoise_2D();
	float FindMaxValue(int leftX, int rightX, int bottomY, int topY);
	float FindMinValue(int leftX, int rightX, int bottomY, int topY);

	

private:
	
	int x;
	int y;
	int itsMaxHeight;
	float height;
	
	int itsSeed;
	int itsWavelength;
	float itsAmplitude;
	int itsOctaves;
	float itsPersistence;
	float itsNormalizationFactor;

	int primeA;
	int primeB;
	int primeC;

	float Noise(int x, int y);
	float SmoothNoise_2D(int x, int y);
	float Cosine_Interpolate(float a, float b, int delta);
	float InterpolatedNoise(int x, int y);	
	int RandomPrime(int lowerBound, int upperBound);
};


#endif