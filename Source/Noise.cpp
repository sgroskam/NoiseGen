#include <math.h>
#include <vector>
#include <time.h>
#include "Noise.h"
#include <iostream>

using namespace std;

PerlinNoise::PerlinNoise(int seed, int wavelength, int octaves, int maxHeight, float persistence)
{
	itsSeed = seed;
	itsWavelength = wavelength;
	itsOctaves = octaves;
	itsMaxHeight = maxHeight;
	itsPersistence = persistence;
	itsNormalizationFactor = 0.0f;
	
	for(int i = 0; i < octaves; i++)
	{
		itsNormalizationFactor += pow(persistence, i);
	}
}

float PerlinNoise::GetHeight()
{
	height = PerlinNoise_2D();
	return itsMaxHeight*height;
}
float PerlinNoise::Noise(int x, int y)
{	
	
	int n = x + y * 57;
	float randHeight = 0.0f;
	n = (n<<13 * itsSeed<<10) ^ n;
    randHeight = ((( (n * (n * n * 15731) + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0)/2;

	return (randHeight);

}

float PerlinNoise::SmoothNoise_2D(int x, int y)
{
    
    float corners = ( Noise(x-1, y-1) +Noise(x+1, y-1) +Noise(x-1, y+1) +Noise(x+1, y+1) ) / 16;
    float sides   = ( Noise(x-1, y)  +Noise(x+1, y)  +Noise(x, y-1)  +Noise(x, y+1) ) /  8;
    float center  =  Noise(x, y) / 4;

    return (corners + sides + center);
}

float PerlinNoise::Cosine_Interpolate(float a, float b, int delta)
{
	float ft = (delta * 3.1415927) / itsWavelength;  
	float f = (1 - cos(ft)) * .5;

	return  (a*(1-f) + b*f);
}

float PerlinNoise::InterpolatedNoise(int x, int y)
{
	if( (x % itsWavelength) != 0 || (y % itsWavelength) != 0)      
	{
        int deltaX = x % itsWavelength;
		int integer_X = x - deltaX;

        int deltaY = y % itsWavelength;
		int integer_Y = y - deltaY;

        float v1 = SmoothNoise_2D(integer_X,					integer_Y);
        float v2 = SmoothNoise_2D(integer_X + itsWavelength,	integer_Y);
        float v3 = SmoothNoise_2D(integer_X,					integer_Y + itsWavelength);
        float v4 = SmoothNoise_2D(integer_X + itsWavelength,	integer_Y + itsWavelength);

        float i1 = Cosine_Interpolate(v1, v2,  deltaX);
        float i2 = Cosine_Interpolate(v3, v4,  deltaX);

        return (Cosine_Interpolate(i1, i2, deltaY));
	}
	else
	{
		return SmoothNoise_2D(x, y);
	}
}


float PerlinNoise::PerlinNoise_2D()
{
	float total = 0.0f;
	float wavelength = itsWavelength;    
    for(int i = 0; i < itsOctaves; i++)
	{


		itsAmplitude = pow(itsPersistence, i);
		wavelength = wavelength/ pow(2.0f, i);
		float frequency = 1/wavelength;

		total = (total + InterpolatedNoise((float)x*frequency, (float)y*frequency) * itsAmplitude);
	}

	total /= itsNormalizationFactor;
	return total;
}

float PerlinNoise::FindMaxValue(int leftX, int rightX, int bottomY, int topY)
{
	float maxValue = 0.0f;
	for(int i = leftX; i <= rightX; i++)
	{
		for(int j = bottomY; j <= topY; j++)
		{
			x = i;
			y = j;

			float currentValue = PerlinNoise_2D();
			if(currentValue > maxValue)
			{
				maxValue = currentValue;
			}
		}
	}
	return maxValue * itsMaxHeight;
}

float PerlinNoise::FindMinValue(int leftX, int rightX, int bottomY, int topY)
{
	float minValue = 1.0f;
	for(int i = leftX; i <= rightX; i++)
	{
		for(int j = bottomY; j <= topY; j++)
		{
			x = i;
			y = j;

			float currentValue = PerlinNoise_2D();
			if(currentValue < minValue)
			{
				minValue = currentValue;
			}
		}
	}
	return minValue * itsMaxHeight;
}



/******************************************************************************************************
/Primes
//input:  int lowerBound
//		  int upperBound
//output: int prime	(randomly chosen from multiple primes)
*******************************************************************************************************/
int PerlinNoise::RandomPrime(int lowerBound, int upperBound)
{
	vector <int> primesVector;
	bool isPrime = true;
	for ( int i = lowerBound; i <= upperBound; i++)
	{
		for ( int j = 2; j <= upperBound; j++)
			{
				if ( (i != j) && (i % j == 0) )
				{
					isPrime=false;
					break;
				}
			}
			if (isPrime == true)
			{
				primesVector.push_back(i);
				break;
			}

			isPrime=true;
	}

	int randomPrime = rand() % primesVector.size();

	return (primesVector.at(randomPrime));
}
