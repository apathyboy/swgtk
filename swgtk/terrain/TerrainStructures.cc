#include "TerrainStructures.h"
#include <string>
#include <string.h>

using namespace TRNLib;

SFAM::SFAM(unsigned char* data)
{
	unsigned int i = 0;
	
	//Read family_id
	memcpy(&family_id, &data[i], 4); i += 4;

	//Read family_name

	unsigned int strLen = strlen(const_cast<const char*>((char*)&data[i])) + 1;
	family_name = new unsigned char[strLen];
	memcpy(family_name, &data[i], strLen); i+= strLen;

	//Read file_name
	strLen = strlen(const_cast<const char*>((char*)&data[i])) + 1;
	file_name = new unsigned char[strLen];
	memcpy(file_name, &data[i], strLen); i+= strLen;

	//Read unk1
	memcpy(&unk1, &data[i], 1); ++i;
	//Read unk2
	memcpy(&unk2, &data[i], 1); ++i;
	//Read unk3
	memcpy(&unk3, &data[i], 1); ++i;
	//Read unk4
	memcpy(&unk4, &data[i], 4); i+=4;
	//Read unk5
	memcpy(&unk5, &data[i], 4); i+=4;

	unsigned int child_count;
	memcpy(&child_count, &data[i], 4); i+=4;

	for(unsigned int j=0; j < child_count; ++j)
	{
		SFAM_CHILD* pChild = new SFAM_CHILD();

		//Read shader_name
		unsigned int strLen = strlen(const_cast<const char*>((char*)&data[i])) + 1;
		pChild->shader_name = new unsigned char[strLen];
		memcpy(pChild->shader_name, &data[i], strLen); i+= strLen;

		//Read unk1
		memcpy(&pChild->unk1, &data[i], 4); i+=4;

		children.push_back(pChild);
	}
}

FFAM::FFAM(unsigned char* data)
{
	unsigned int i=0;

	//Read family_id
	memcpy(&family_id, &data[i], 4); i += 4;

	//Read family_name
	unsigned int strLen = strlen(const_cast<const char*>((char*)&data[i])) + 1;
	family_name = new unsigned char[strLen];
	memcpy(family_name, &data[i], strLen); i+= strLen;

	//Read unk1
	memcpy(&unk1, &data[i], 1); ++i;
	//Read unk2
	memcpy(&unk2, &data[i], 1); ++i;
	//Read unk3
	memcpy(&unk3, &data[i], 1); ++i;
	//Read unk4
	memcpy(&unk4, &data[i], 4); i+=4;
	//Read unk5
	memcpy(&unk5, &data[i], 4); i+=4;

	unsigned int child_count;
	memcpy(&child_count, &data[i], 4); i+=4;

	for(unsigned int j=0; j < child_count; ++j)
	{
		FFAM_CHILD* pChild = new FFAM_CHILD();

		unsigned int strLen = strlen(const_cast<const char*>((char*)&data[i])) + 1;
		pChild->appearance_name = new unsigned char[strLen];
		memcpy(pChild->appearance_name, &data[i], strLen); i+= strLen;

		//Read unk1;
		memcpy(&pChild->unk1, &data[i], 4); i+=4;
		//Read unk2; //Type unknown!
		memcpy(&pChild->unk2, &data[i], 4); i+=4;
		//Read unk3;
		memcpy(&pChild->unk3, &data[i], 4); i+=4;
		//Read unk4;
		memcpy(&pChild->unk4, &data[i], 4); i+=4;
		//Read unk5;
		memcpy(&pChild->unk5, &data[i], 4); i+=4;
		//Read unk6;
		memcpy(&pChild->unk6, &data[i], 4); i+=4;
		//Read unk7;
		memcpy(&pChild->unk7, &data[i], 4); i+=4;
		//Read unk8;
		memcpy(&pChild->unk8, &data[i], 4); i+=4;

		children.push_back(pChild);
	}
}

RFAM::RFAM(unsigned char* data)
{
	unsigned int i=0;

	//Read family_id
	memcpy(&family_id, &data[i], 4); i += 4;

	//Read family_name
	unsigned int strLen = strlen(const_cast<const char*>((char*)&data[i])) + 1;
	family_name = new unsigned char[strLen];
	memcpy(family_name, &data[i], strLen); i+= strLen;

	//Read unk1
	memcpy(&unk1, &data[i], 1); ++i;
	//Read unk2
	memcpy(&unk2, &data[i], 1); ++i;
	//Read unk3
	memcpy(&unk3, &data[i], 1); ++i;
	//Read unk4
	memcpy(&unk4, &data[i], 4); i+=4;

	unsigned int child_count;
	memcpy(&child_count, &data[i], 4); i+=4;

	for(unsigned int j=0; j < child_count; ++j)
	{
		RFAM_CHILD* pChild = new RFAM_CHILD();

		unsigned int strLen = strlen(const_cast<const char*>((char*)&data[i])) + 1;
		pChild->shader_name = new unsigned char[strLen];
		memcpy(pChild->shader_name, &data[i], strLen); i+= strLen;

		//Read unk1;
		memcpy(&pChild->unk1, &data[i], 4); i+=4;
		//Read unk2; //Type unknown!
		memcpy(&pChild->unk2, &data[i], 4); i+=4;
		//Read unk3;
		memcpy(&pChild->unk3, &data[i], 4); i+=4;
		//Read unk4;
		memcpy(&pChild->unk4, &data[i], 4); i+=4;
		//Read unk5;
		memcpy(&pChild->unk5, &data[i], 4); i+=4;
		//Read unk6;
		memcpy(&pChild->unk6, &data[i], 4); i+=4;
		//Read unk7;
		memcpy(&pChild->unk7, &data[i], 4); i+=4;
		//Read unk8;
		memcpy(&pChild->unk8, &data[i], 4); i+=4;
		//Read unk9;
		memcpy(&pChild->unk9, &data[i], 4); i+=4;

		children.push_back(pChild);
	}
}

EFAM::EFAM(unsigned char* data)
{
	unsigned int i=0;

	//Read family_id;
	memcpy(&family_id, &data[i], 4); i+=4;
	//Read family_name;
	unsigned int strLen = strlen(const_cast<const char*>((char*)&data[i])) + 1;
	family_name = new unsigned char[strLen];
	memcpy(family_name, &data[i], strLen); i+= strLen;

	//Read unk1;
	memcpy(&unk1, &data[i], 1); ++i;
	//Read unk2;
	memcpy(&unk1, &data[i], 1); ++i;
	//Read unk3;
	memcpy(&unk1, &data[i], 1); ++i;
	//Read unk4;
	memcpy(&unk1, &data[i], 4); i+=4;
}

MFAM::MFAM(unsigned char* data, unsigned char* data2)
{
	unsigned int i = 0;

	//First Part
	//Read fractal_id;
	memcpy(&fractal_id, &data[i], 4); i+=4;
	//Read fractal_label;
	unsigned int strLen = strlen(const_cast<const char*>((char*)&data[i])) + 1;
	fractal_label = new unsigned char[strLen];
	memcpy(fractal_label, &data[i], strLen); i+= strLen;

	//Reset the counter
	i=0;

	//Second Part
	//Read seed_arg;
	memcpy(&seed , &data2[i], 4); i+=4;
	setSeed(seed);
	//Read use_bias;
	memcpy(&use_bias , &data2[i], 4); i+=4;
	//Read bias;
	memcpy(&bias , &data2[i], 4); i+=4;
	//Read use_gain;
	memcpy(&use_gain , &data2[i], 4); i+=4;
	//Read gain;
	memcpy(&gain , &data2[i], 4); i+=4;
	//Read octaves;
	memcpy(&octaves , &data2[i], 8); i+=4;
	//Read octaves_arg;
	memcpy(&octaves_arg , &data2[i], 4); i+=4;
	//Read amplitude_arg;
	memcpy(&amplitude , &data2[i], 4); i+=4;
	setAmplitude();
	//Read freq_x;
	memcpy(&freq_x, &data2[i], 4); i+=4;
	//Read freq_z;
	memcpy(&freq_z, &data2[i], 4); i+=4;
	//Read offset_x; //type unknown
	memcpy(&offset_x, &data2[i], 4); i+=4;
	//Read offset_y;
	memcpy(&offset_z, &data2[i], 4); i+=4;
	//Read combination
	memcpy(&combination_type, &data2[i], 4); i+=4;
}

void MFAM::setSeed(int seed)
{
	rand = new Random();
	rand->setSeed(seed);

	noise = new PerlinNoise(rand);
	double coord[2];
	coord[0] = 0, coord[1] = 0;

	noise->noise2(coord);
}

void MFAM::setAmplitude()
{
	offset = 0.0f;

	float curr_amplitude = 0.0f;
	float next_amplitude = 1.0f;

	for (unsigned int i = 0; i < octaves; i++)
	{
		curr_amplitude = curr_amplitude + next_amplitude;
		next_amplitude = next_amplitude * amplitude;
	}

	offset = curr_amplitude;

	if (offset != 0)
		offset = (float)(1.0/offset);
}

float MFAM::getNoise(float x, float z) 
{
	float xFrequency = x * freq_x;
	float zFrequency = z * freq_z;

	double result = 0;

	switch (combination_type) 
	{
	case 0:
	case 1:
		result = calculateCombination1(xFrequency, zFrequency);
		break;
	case 2:
		result = calculateCombination2(xFrequency, zFrequency);
		break;
	case 3:
		result = calculateCombination3(xFrequency, zFrequency);
		break;
	case 4:
		result = calculateCombination4(xFrequency, zFrequency);
		break;
	case 5:
		result = calculateCombination5(xFrequency, zFrequency);
		break;
	}

	if (use_bias) 
	{
		result = pow(result, log(bias) / log(0.5));
	}

	if (use_gain) 
	{
		if (result < 0.001) 
		{
			result = 0;

			return (float)result;
		}

		if (result > 0.999) 
		{
			result = 1.0;

			return (float)result;
		}

		double log_gain = log(1.0 - gain) / log(0.5);

		if (result < 0.5) 
		{
			result = pow(result * 2, log_gain) * 0.5;

			return (float)result;
		}

		result = 1.0 - pow((1.0 - result) * 2, log_gain) * 0.5;
	}

	return (float)result;
}

double MFAM::calculateCombination1(float x, float z) 
{
	float curr_offset = 1, curr_ampl = 1;
	double result = 0;

	float zOffset = z + offset_z; 
	float xOffset = x + offset_x; 
	float zNoise, xNoise, noise_gen = 0;
	double coord[2];

	for (unsigned int i = 0; i < octaves; ++i) 
	{
		zNoise = zOffset * curr_offset;
		xNoise = xOffset * curr_offset;

		coord[0] = xNoise;
		coord[1] = zNoise;

		noise_gen = noise->noise2(coord) * curr_ampl + noise_gen;
		curr_offset = curr_offset * octaves_arg; 
		curr_ampl = curr_ampl * amplitude; 
	}

	result = (noise_gen * offset + 1.0) * 0.5;

	return result;
}

double MFAM::calculateCombination2(float x, float z) 
{
	float noise_gen = 0;
	float zOffset = z + offset_z;
	float zNoise, xOffset, xNoise;
	float curr_offset = 1.0;
	float curr_ampl = 1.0;
	double result = 0;
	double coord[2];

	for (unsigned int i = 0; i < octaves; ++i) 
	{
		zNoise = zOffset * curr_offset;
		xOffset = x + offset_x;
		xNoise = xOffset * curr_offset;

		coord[0] = xNoise;
		coord[1] = zNoise;

		noise_gen = (float)((1.0 - fabs(noise->noise2(coord))) * curr_ampl + noise_gen);
		curr_offset = curr_offset * octaves_arg; 
		curr_ampl = curr_ampl * amplitude;
	}

	result = noise_gen * offset;

	return result;
}

double MFAM::calculateCombination3(float x, float z) 
{
	float curr_offset = 1.0;
	float curr_ampl = 1.0;
	double result = 0;
	float noise_gen = 0;
	float zOffset = z + offset_z; 
	float zNoise, xOffset, xNoise;

	double coord[2];

	for (unsigned int i = 0; i < octaves; ++i) 
	{
		zNoise = zOffset * curr_offset;
		xOffset = x + offset_x; 
		xNoise = xOffset * curr_offset;

		coord[0] = xNoise;
		coord[1] = zNoise;

		noise_gen = fabs(noise->noise2(coord)) * curr_ampl + noise_gen;
		curr_offset = curr_offset * octaves_arg;
		curr_ampl = curr_ampl * amplitude; 
	}

	result = noise_gen * offset; 

	return result;
}

double MFAM::calculateCombination4(float x, float z) 
{
	float noise_gen = 0;
	float zNoise, xNoise, noise_gain;
	float zOffset = z + offset_z; 
	float xOffset = x + offset_x; 
	double coord[2];

	float curr_offset = 1.0;
	float curr_ampl = 1.0;
	double result = 0;

	for (unsigned int i = 0; i < octaves; ++i) 
	{
		zNoise = zOffset * curr_offset;
		xNoise = xOffset * curr_offset;

		coord[0] = xNoise;
		coord[1] = zNoise;

		noise_gain = noise->noise2(coord);
		if ( noise_gain >= 0.0f ) 
			if ( noise_gain > 1.0f )
				noise_gain = 1.0f;
		else 
			noise_gain = 0.0f;

		noise_gen = (1.0f - noise_gain) * curr_ampl + noise_gen;
		curr_offset = curr_offset * octaves_arg; 
		curr_ampl = curr_ampl * amplitude;
	}

	result = noise_gen * offset;

	return result;
}

double MFAM::calculateCombination5(float x, float z) 
{
	float noise_gen = 0;
	float zNoise, xNoise, noise_gain;
	float zOffset = z + offset_z; 
	float xOffset = x + offset_x;
	double coord[2];
	double result = 0;

	float curr_offset = 1.0;
	float curr_ampl = 1.0;

	for (unsigned int i = 0; i < octaves; ++i) 
	{
		zNoise = zOffset * curr_offset;
		xNoise = xOffset * curr_offset;

		coord[0] = xNoise;
		coord[1] = zNoise;

		noise_gain = noise->noise2(coord);

		if ( noise_gain >= 0.0 ) 
			if (noise_gain > 1.0)
				noise_gain = 1.0;
		else 
			noise_gain = 0.0;

		noise_gen = noise_gain * curr_ampl + noise_gen;
		curr_offset = curr_offset * octaves_arg;
		curr_ampl = curr_ampl * amplitude;
	}

	result = noise_gen * offset;

	return result;
}