#include "Layer.h"
#include "TerrainManager.h"

//Boundries
#include "BREC.h"
#include "BCIR.h"
#include "BPOL.h"
#include "BPLN.h"

//Filters
#include "FDIR.h"
#include "FFRA.h"
#include "FHGT.h"
#include "FSLP.h"

//Shader
#include "ASCN.h"
#include "ACCN.h"
#include "FSHD.h"

//Height
#include "AHCN.h"
#include "AHFR.h"
#include "AHTR.h"

//Flora
#include "AFSC.h"
#include "AFSN.h"
#include "AFDN.h"
#include "AFDF.h"

//Other
#include "AENV.h"
#include "AEXC.h"
#include "AROA.h"
#include "ARIV.h"
#include "ACRF.h"
#include "ACRH.h"
#include "ASRP.h"

// Math
#include <cmath>
#include <limits>

#define M_PI 3.14159265358979323846 /* pi */

using namespace TRNLib;
using namespace IFFLib;

LAYER* LAYER::LOAD(IFF::NODE* node)
{
	LAYER* layer = NULL;

	//See if the name is prefixed funkily or something
	char* nameStart = node->name;

	//Do it
	if(nameStart[0] == 'A')
	{
		if(nameStart[1] == 'C')
		{
			if(nameStart[2] == 'C')
			{
				if(nameStart[3] == 'N')
				{
					layer = new ACCN(node->children[1]->data, node->children[1]->size);
				}
			}
			else if(nameStart[2] == 'R')
			{
				if(nameStart[3] == 'F')
				{
					layer = new ACRF(node->children[1]->children[0]->data, node->children[1]->children[0]->size);
				}
				else if(nameStart[3] == 'H')
				{
					layer = new ACRH(node->children[1]->data, node->children[1]->size);
				}
			}
		}
		else if(nameStart[1] == 'E')
		{
			if(nameStart[2] == 'N')
			{
				if(nameStart[3] == 'V')
				{
					layer = new AENV(node->children[1]->data, node->children[1]->size);
				}
			}
			else if(nameStart[2] == 'X')
			{
				if(nameStart[3] == 'C')
				{
					layer = new AEXC();
				}
			}
		}
		else if(nameStart[1] == 'F')
		{
			if(nameStart[2] == 'D')
			{
				if(nameStart[3] == 'F')
				{
					layer = new AFDF(node->children[1]->data, node->children[1]->size);
				}
				else if(nameStart[3] == 'N')
				{
					layer = new AFDN(node->children[1]->data, node->children[1]->size);
				}
			}
			else if(nameStart[2] == 'S')
			{
				if(nameStart[3] == 'C')
				{
					layer = new AFSC(node->children[1]->data, node->children[1]->size);
				}
				else if(nameStart[3] == 'N')
				{
					layer = new AFSN(node->children[1]->data, node->children[1]->size);
				}
			}
		}
		else if(nameStart[1] == 'H')
		{
			if(nameStart[2] == 'C')
			{
				if(nameStart[3] == 'N')
				{
					layer = new AHCN(node->children[1]->data, node->children[1]->size);
				}
			}
			else if(nameStart[2] == 'F')
			{
				if(nameStart[3] == 'R')
				{
					layer = new AHFR(node->children[1]->children[0]->data, node->children[1]->children[0]->size);
				}
			}
			else if(nameStart[2] == 'T')
			{
				if(nameStart[3] == 'R')
				{
					layer = new AHTR(node->children[1]->data, node->children[1]->size);
				}
			}
		}
		else if(nameStart[1] == 'R')
		{
			if(nameStart[2] == 'I')
			{
				if(nameStart[3] == 'V')
				{
					layer = new ARIV(	node->children[1]->children[0]->children[0]->children[0]->data, 
										node->children[1]->children[0]->children[0]->children[0]->size, 
										node->children[1]->children[1]->data, 
										node->children[1]->children[1]->size);
				}
			}
			else if(nameStart[2] == 'O')
			{
				if(nameStart[3] == 'A')
				{
					layer = new AROA(	node->children[1]->children[0]->children[0]->children[0]->data, 
										node->children[1]->children[0]->children[0]->children[0]->size, 
										node->children[1]->children[1]->data, 
										node->children[1]->children[1]->size);
				}
			}
		}
		else if(nameStart[1] == 'S')
		{
			if(nameStart[2] == 'C')
			{
				if(nameStart[3] == 'N')
				{
					layer = new ASCN(node->children[1]->data, node->children[1]->size);
				}
			}
			else if(nameStart[2] == 'R')
			{
				if(nameStart[3] == 'P')
				{
					layer = new ASRP(node->children[1]->data, node->children[1]->size);
				}
			}
		}
	}
	else if(nameStart[0] == 'B')
	{
		if(nameStart[1] == 'C')
		{
			if(nameStart[2] == 'I')
			{
				if(nameStart[3] == 'R')
				{
					layer = new BCIR(node->children[1]->data, node->children[1]->size);
				}
			}
		}
		else if(nameStart[1] == 'P')
		{
			if(nameStart[2] == 'L')
			{
				if(nameStart[3] == 'N')
				{
					layer = new BPLN(node->children[1]->data, node->children[1]->size);
				}
			}
			else if(nameStart[2] == 'O')
			{
				if(nameStart[3] == 'L')
				{
					layer = new BPOL(node->children[1]->data, node->children[1]->size);
				}
			}
		}
		else if(nameStart[1] == 'R')
		{
			if(nameStart[2] == 'E')
			{
				if(nameStart[3] == 'C')
				{
					layer = new BREC(node->children[1]->data, node->children[1]->size);
				}
			}
		}
	}
	else if(nameStart[0] == 'F')
	{
		if(nameStart[1] == 'D')
		{
			if(nameStart[2] == 'I')
			{
				if(nameStart[3] == 'R')
				{
					layer = new FDIR(node->children[1]->data, node->children[1]->size);
				}
			}
		}
		else if(nameStart[1] == 'F')
		{
			if(nameStart[2] == 'R')
			{
				if(nameStart[3] == 'A')
				{
					layer = new FFRA(node->children[1]->children[0]->data, node->children[1]->children[0]->size);
				}
			}
		}
		else if(nameStart[1] == 'H')
		{
			if(nameStart[2] == 'G')
			{
				if(nameStart[3] == 'T')
				{
					layer = new FHGT(node->children[1]->data, node->children[1]->size);
				}
			}
		}
		else if(nameStart[1] == 'S')
		{
			if(nameStart[2] == 'H')
			{
				if(nameStart[3] == 'D')
				{
					layer = new FSHD(node->children[1]->data, node->children[1]->size);
				}
			}
			else if(nameStart[2] == 'L')
			{
				if(nameStart[3] == 'P')
				{
					layer = new FSLP(node->children[1]->data, node->children[1]->size);
				}
			}
		}
	}

	if(layer)
	{
		//Find it's data.
		unsigned char* data = node->children[0]->children[0]->children[0]->data;

		layer->enabled    = (data[0] == 1) ? true : false;
		
		unsigned int strLen = strlen(const_cast<const char*>((char*)&data[4])) + 1;
		layer->customName = new unsigned char[strLen];
		memcpy(layer->customName, &data[4], strLen);
	}

	return layer;
}

ACCN::ACCN(unsigned char* data, unsigned int dataSize)
{
	type = LAYER_ACCN;

	this->data = new unsigned char[dataSize];
	memcpy(this->data, data, dataSize);
	this->size = dataSize;
}

ASCN::ASCN(unsigned char* data, unsigned int dataSize)
{
	type = LAYER_ASCN;

	this->data = new unsigned char[dataSize];
	memcpy(this->data, data, dataSize);
	this->size = dataSize;
}

FSHD::FSHD(unsigned char* data, unsigned int dataSize)
{
	type = LAYER_ACCN;

	this->data = new unsigned char[dataSize];
	memcpy(this->data, data, dataSize);
	this->size = dataSize;
}

ACRF::ACRF(unsigned char* data, unsigned int dataSize)
{
	type = LAYER_ACRF;

	this->data = new unsigned char[dataSize];
	memcpy(this->data, data, dataSize);
	this->size = dataSize;
}

ACRH::ACRH(unsigned char* data, unsigned int dataSize)
{
	type = LAYER_ACRH;

	this->data = new unsigned char[dataSize];
	memcpy(this->data, data, dataSize);
	this->size = dataSize;
}

AENV::AENV(unsigned char* data, unsigned int dataSize)
{
	type = LAYER_AENV;

	this->data = new unsigned char[dataSize];
	memcpy(this->data, data, dataSize);
	this->size = dataSize;
}

AEXC::AEXC()
{
	type = LAYER_AEXC;
}

AROA::AROA(unsigned char* data, unsigned int dataSize, unsigned char* data2, unsigned int dataSize2)
{
	type = LAYER_AROA;

	this->data = new unsigned char[dataSize];
	memcpy(this->data, data, dataSize);
	this->size = dataSize;

	this->data2 = new unsigned char[dataSize2];
	memcpy(this->data2, data2, dataSize2);
	this->size2 = dataSize2;
}

AHCN::AHCN(unsigned char* data, unsigned int dataSize)
{
	type = LAYER_AHCN;

	memcpy(&transform_type, &data[0], 4);
	memcpy(&height_val, &data[4], 4);

	//printf("C: %d %f\n", unk1, unk2);
}

void AHCN::getBaseHeight(float x, float z, float transform_value, float& base_value, TRNLib::TerrainManager* terrain)
{
	if (transform_value == 0)
		return;

	float result;

	switch (transform_type) 
	{
	case 1:
		result = transform_value * height_val + base_value;
		break;
	case 2:
		result = base_value - transform_value * height_val;
		break;
	case 3:
		result = base_value + (base_value * height_val - base_value) * transform_value;
		break;
	case 4:
		result = 0;
		break;
	default:
		result = (1.0f - transform_value) * base_value + transform_value * height_val;
	}

	base_value = result;
}

AHFR::AHFR(unsigned char* data, unsigned int dataSize)
{
	type = LAYER_AHFR;

	memcpy(&fractal_id, &data[0], 4);
	memcpy(&transform_type, &data[4], 4);
	memcpy(&height_val, &data[8], 4);

	//printf("F: %d %d %f\n", unk1, unk2, unk3);
}

void AHFR::getBaseHeight(float x, float z, float transform_value, float& base_value, TRNLib::TerrainManager* terrain)
{
	TRNLib::MFAM* fractal = terrain->getFractal(fractal_id);
	
	float noise_result = fractal->getNoise(x, z) * height_val;

	float result;

	switch (transform_type)
	{
	case 1:
		result = base_value + noise_result * transform_value;
		break;
	case 2:
		result = base_value - noise_result * transform_value;
		break;
	case 3:
		result = base_value + (noise_result * base_value - base_value) * transform_value;
		break;
	case 4:
		result = base_value;
		break;
	default:
		result = base_value + (noise_result - base_value) * transform_value;
		break;
	}

	base_value = result;
}

FFRA::FFRA(unsigned char* data, unsigned int dataSize)
{
	type = LAYER_FFRA;

	unsigned int i = 0;

	memcpy(&fractal_id, &data[i], 4); i += 4;
	memcpy(&feather_type, &data[i], 4); i += 4;
	memcpy(&feather_amount, &data[i], 4); i += 4;
	memcpy(&min, &data[i], 4); i += 4;
	memcpy(&max, &data[i], 4); i += 4;
	memcpy(&step, &data[i], 4); i += 4;
}

float FFRA::process(float x, float z, float transform_value, float& base_value, TRNLib::TerrainManager* terrain)
{
	TRNLib::MFAM* fractal = terrain->getFractal(fractal_id);

	float noise_result = fractal->getNoise(x, z) * step;
	float result = 0;

	if (noise_result > min && noise_result < max) {
		float feather_result = (max - min) * feather_amount * 0.5f;

		if (min + feather_result <= noise_result) {
			if (max - feather_result >= noise_result)
				result = 1.0;
			else
				result = (max - noise_result) / feather_result;
		} else
			result = (noise_result - min) / feather_result;
	} else
		result = 0;

	return result;
}

FHGT::FHGT(unsigned char* data, unsigned int dataSize)
{
	type = LAYER_FHGT;

	unsigned int i=0;

	memcpy(&minHeight, &data[i], 4); i+=4;

	memcpy(&maxHeight, &data[i], 4); i+=4;

	memcpy(&feather_type, &data[i], 4); i+=4;
	
	memcpy(&feather_amount, &data[i], 4); i+=4;
}

float FHGT::process(float px, float pz, float transform_value, float& base_value, TRNLib::TerrainManager* terrain)
{
	float result;

	if ((base_value > minHeight) && (base_value < maxHeight)) {
		float feather_result = (float)((maxHeight - minHeight) * feather_amount * 0.5);

		if (minHeight + feather_result <= base_value) {
			if (maxHeight - feather_result >= base_value) {
				result = 1.0;
			} else
				result = (maxHeight - base_value) / feather_result;

		} else {
			result = (base_value - minHeight) / feather_result;
		}
	} else
		result = 0;

	return result;
}

FSLP::FSLP(unsigned char* data, unsigned int dataSize)
{
	type = LAYER_FSLP;
	default_value = 1.5707964f;

	unsigned int i = 0;

	memcpy(&min_angle, &data[i], 4); i+=4;
	setMinAngle((float)(M_PI * min_angle * 0.005555555690079927));
	memcpy(&max_angle, &data[i], 4); i+=4;
	setMaxAngle((float)(M_PI * max_angle * 0.005555555690079927));

	memcpy(&feather_type, &data[i], 4); i+=4;
	memcpy(&feather_amount, &data[i], 4); i+=4;

	if (feather_amount <= 0)
		feather_amount = 0.0f;
	else if (feather_amount >= 1)
		feather_amount = 1.0f;
}

void FSLP::setMinAngle(float new_angle) {
	if (new_angle >= 0) {
		if (new_angle <= default_value) {
			min_angle = new_angle;
			max = sin(default_value - new_angle);
		} else {
			min_angle = default_value;
			max = 0;
		}
	} else {
		min_angle = 0;
		max = sin(default_value);
	}
}

void FSLP::setMaxAngle(float new_angle) {
	if (new_angle >= 0) {
		if (new_angle <= default_value) {
			max_angle = new_angle;
			min = sin(default_value - new_angle);
		} else {
			max_angle = default_value;
			min = 0;
		}
	} else {
		max_angle = 0;
		min = sin(default_value);
	}
}

float FSLP::process(float px, float pz, float transform_value, float& base_value, TRNLib::TerrainManager* terrain) {
	float result;

	if (base_value > min && base_value < max) {
		float feather_result = (float)(max - min * feather_amount * 0.5);

		if (min + feather_result <= base_value) {
			if (max - feather_result >= base_value) {
				result = 1.0;
			} else {
				result = (max - base_value) / feather_result;
			}
		} else
			result = (base_value - min) / feather_result;
	} else
		result = 0;

	return result;
}

BREC::BREC(unsigned char* data, unsigned int dataSize)
{
	type = LAYER_BREC;

	memcpy(&x1, &data[0], 4);
	memcpy(&z1, &data[4], 4);
	
	memcpy(&x2, &data[8], 4);
	memcpy(&z2, &data[12], 4);

	memcpy(&feather_type, &data[16], 4);
	memcpy(&feather_amount, &data[20], 4);

	//printf("==BREC==\nPOS1: (%f,%f)\nPOS2: (%f,%f)\nFeather Type: (%d)\nFeather Amount: (%f)\n", x1, y1, x2, y2, feather_type, feather_amount);
	// x1 always lowest, z1 always lowest
	float temp;

	if (x1 > x2)
	{
		temp = x1;
		x1 = x2;
		x2 = temp;
	}

	if (z1 > z2)
	{
		temp = z1;
		z1 = z2;
		z2 = temp;
	}
}

bool BREC::isContained(float px, float pz)
{
	if (x2 >= px && x1 <= px && z2 >= pz && z1 <= pz)
		return true;

	return false;
}

float BREC::process(float px, float pz)
{
	float result;
	
	if (!isContained(px, pz))
		result = 0.0f;
	else
	{
		float min_distx = px - x1;
		float max_distx = x2 - px;
		float min_distz = pz - z1;
		float max_distz = z2 - pz;
		float x_length = x2 - x1;
		float length = z2 - z1;

		if (x_length < length)
			length = x_length;

		float feather_length = (float)(feather_amount * length * 0.5);
		float feather_result = feather_length;

		float newX0 = x1 + feather_length;
		float newX1 = x2 - feather_length;
		float newZ0 = z1 + feather_length;
		float newZ1 = z2 - feather_length;

		if (px < newX1 || px > newX0 || pz < newZ1 || pz > newZ0)
			return 1.0f;

		if (min_distx < feather_length)
			feather_result = min_distx;
		if (max_distx < feather_result)
			feather_result = max_distx;
		if (min_distz < feather_result)
			feather_result = min_distz;
		if (max_distz < feather_result)
			feather_result = max_distz;

		result = feather_result / feather_length;
	}

	return result;
}



BPOL::BPOL(unsigned char* data, unsigned int dataSize)
{
	type = LAYER_BPOL;

	unsigned int sizeTemp;
	unsigned int i=0;
	memcpy(&sizeTemp, &data[0], 4); i+=4;

	// Initialize min and max values
	min_x = FLT_MAX;
	max_x = FLT_MIN;
	min_z = FLT_MAX;
	max_z = FLT_MIN;

	for(unsigned int j = 0; j < sizeTemp; j++)
	{
		float tempX;
		float tempZ;
		memcpy(&tempX, &data[i], 4); i+=4;
		memcpy(&tempZ, &data[i], 4); i+=4;

		verts.push_back(new VERTEX(tempX, tempZ));

		// Track max and min values
		if (tempX > max_x)
			max_x = tempX;
		else if (tempX < min_x)
			min_x = tempX;

		if (tempZ > max_z)
			max_z = tempZ;
		else if (tempZ < min_z)
			min_z = tempZ;
	}

	memcpy(&feather_type, &data[i], 4); i+=4;
	memcpy(&feather_amount, &data[i], 4); i+=4;
	memcpy(&use_water_height, &data[i], 4); i+=4;
	memcpy(&water_height, &data[i], 4); i+=4;
	memcpy(&water_shader_size, &data[i], 4); i+=4;

	unsigned int strLen = strlen(const_cast<const char*>((char*)&data[i])) + 1;
	water_shader = new unsigned char[strLen];
	memcpy(water_shader, &data[i], strLen); i+= strLen;
}

bool BPOL::isContained(float x, float z)
{
	int j;
	bool odd_nodes = false;
	float x1, x2;

	for ( unsigned int i = 0; i < verts.size(); ++i )
	{
		j = (i+1) % verts.size();

		if ( verts.at(i)->x < verts.at(j)->x)
		{
			x1 = verts.at(i)->x;
			x2 = verts.at(j)->x;
		} 
		else 
		{
			x1 = verts.at(j)->x;
			x2 = verts.at(i)->x;
		}

			/* First check if the ray is possible to cross the line */
			if ( x > x1 && x <= x2 && ( z < verts.at(i)->z || z <= verts.at(j)->z ) ) {
				static const float eps = 0.000001f;

				/* Calculate the equation of the line */
				float dx = verts.at(j)->x - verts.at(i)->x;
				float dz = verts.at(j)->z - verts.at(i)->z;
				float k;

				if ( fabs(dx) < eps ){
					k = std::numeric_limits<float>::infinity();
				} else {
					k = dz/dx;
				}

				float m = verts.at(i)->z - k * verts.at(i)->x;

				/* Find if the ray crosses the line */
				float z2 = k * x + m;
				if ( z <= z2 )
				{
					odd_nodes=!odd_nodes;
				}
			}
		}


  return odd_nodes;
}

float BPOL::process(float px, float pz)
{
	float result;
	VERTEX* last = verts.at(verts.size() - 1);
	bool odd_nodes = false;

	if (px < min_x || px > max_x || pz < min_z || pz > max_z)
		return 0.0f;

	if (verts.size() <= 0)
		return 0.0f;

	for (unsigned int i = 0; i < verts.size(); i++)
	{
		VERTEX* point = verts.at(i);

		if ((point->z <= pz && pz < last->z) || (last->z <= pz && pz < point->z))
			if ((pz - point->z) * (last->x - point->x) / (last->z - point->z) + point->x > (double)px) 
				odd_nodes = !odd_nodes;

		last = point;
	}

	double feather2, new_feather;

	if (odd_nodes)
	{
		if (feather_amount == 0)
			return 1.0f;

		feather2 = pow(feather_amount,2);
		new_feather = feather2;
		double diffz, diffx, dist;

		for (unsigned int i = 0; i < verts.size(); ++i) 
		{
			VERTEX* point = verts.at(i);

			diffz = pz - point->z;
			diffx = px - point->x;
			dist = pow(diffz,2) + pow(diffx,2);

			if ( dist < feather2 ) 
				feather2 = dist;
		}

		double ltp_x, ltp_z, ptl_z, ptl_x, diff, new_dist, newX, newZ;

		last = verts.at(verts.size() - 1);

		for (unsigned int i = 0; i < verts.size(); ++i) 
		{
			VERTEX* point = verts.at(i);

			ltp_x = last->x - point->x;
			ltp_z = last->z - point->z;
			ptl_z = point->z - last->z;
			ptl_x = point->x - last->x;
			diff = ((px - last->x) * ptl_x + (pz - last->z) * ptl_z) / (ltp_z * ptl_z + ltp_x * ltp_x);
			if ( diff >= 0.0 ) 
			{
				if ( diff <= 1.0 ) 
				{
					newX = px - (ptl_x * diff + last->x);
					newZ = pz - (ptl_z * diff + last->z);
					new_dist = newZ * newZ + newX * newX;
					if ( new_dist < feather2 ) 
					{
						feather2 = new_dist;
					}
				}
			}

			last = point;
		}

		if ( feather2 >= new_feather - 0.00009999999747378752 && feather2 <= new_feather + 0.00009999999747378752 )
			result = 1.0;
		else
			result = (float)(sqrt(feather2) / feather_amount);
	} 
	else 
	{
		result = 0.0;
	}

	return result;
}

BPLN::BPLN(unsigned char* data, unsigned int dataSize)
{
	type = LAYER_BPLN;
	unsigned int sizeTemp;
	unsigned int i=0;
	memcpy(&sizeTemp, &data[0], 4); i+=4;

	// Initialize min and max values
	min_x = FLT_MAX;
	max_x = FLT_MIN;
	min_z = FLT_MAX;
	max_z = FLT_MIN;

	for(unsigned int j = 0; j < sizeTemp; j++)
	{
		float tempX;
		float tempZ;
		memcpy(&tempX, &data[i], 4); i+=4;
		memcpy(&tempZ, &data[i], 4); i+=4;

		verts.push_back(new VERTEX(tempX, tempZ));

		// Track max values
		if (tempX > max_x)
			max_x = tempX;
		else if (tempX < min_x)
			min_x = tempX;

		if (tempZ > max_z)
			max_z = tempZ;
		else if (tempZ < min_z)
			min_z = tempZ;
	}

	memcpy(&feather_type, &data[i], 4); i+=4;

	memcpy(&feather_amount, &data[i], 4); i+=4;
	memcpy(&line_width, &data[i], 4); i+=4;

	// Account for line width
	min_x = min_x - line_width;
	max_x = max_x + line_width;
	min_z = min_z - line_width;
	max_z = max_z + line_width;
}

bool BPLN::isContained(float x, float z)
{
	return false;
}

float BPLN::process(float px, float pz)
{
	if (px < min_x)
			return 0.0;

		if (px > max_x || pz < (double)min_z )
			return 0.0;

		if ( pz > max_z )
			return 0.0;

		double line2 = line_width * line_width;

		double new_line = line2;
		double distz, distx, dist;

		double result = 0;

		for (unsigned int i = 0; i < verts.size(); ++i) {
			VERTEX* point = verts.at(i);

			distz = pz - point->z;
			distx = px - point->x;
			dist = pow(distx,1) + pow(distz,2);
			if ( dist < line2 ) 
				line2 = dist;
		}

		double x_dist, diff, new_x, new_z, new_dist, z_dist;

		for (unsigned int i = 0; i < verts.size() - 1; ++i) 
		{
			const VERTEX* point = verts.at(i);
			const VERTEX* point2 = verts.at(i + 1);

			x_dist = point2->x - point->x;
			z_dist = point2->z - point->z;
			diff = ((pz -  point->z) * z_dist + (px - point->x) * x_dist) / (z_dist * z_dist + x_dist * x_dist);

			if ( diff >= 0.0 ) {
				if ( diff <= 1.0 ) {
					new_x = px - (x_dist * diff + point->x);
					new_z = pz - (z_dist * diff +  point->z);
					new_dist = pow(new_z,2) + pow(new_x,2);

					if ( new_dist < line2 ) {
						line2 = new_dist;
					}
				}
			}

		}

		if ( line2 >= new_line )
			return 0.0;

		double new_feather = (1.0 - feather_amount) * line_width;

		if ( line2 >= pow(new_feather,2) )
			result = 1.0 - (sqrt(line2) - new_feather) / (line_width - new_feather);
		else
			result = 1.0;

		return (float)result;
}

BCIR::BCIR(unsigned char* data, unsigned int dataSize)
{
	type = LAYER_BCIR;
	memcpy(&x, &data[0], 4);
	memcpy(&z, &data[4], 4);
	
	memcpy(&rad, &data[8], 4);

	memcpy(&feather_type, &data[12], 4);
	memcpy(&feather_amount, &data[16], 4);
	//printf("==BCIR==\nPOS: (%f,%f)\nRAD: %f\nFeather Type: (%d)\nFeather Amount: (%f)\n", x, y, rad, feather_type, feather_amount);
}

bool BCIR::isContained(float px, float pz)
{
	float dist = pow(px-x,2) + pow(pz-z,2);
	float r2 = pow(rad,2);

	if ( dist < r2)
		return true;

	return false;
}

float BCIR::process(float px, float pz)
{
	float result;
	float dist = pow(px-x,2) + pow(pz-z,2);
	float r2 = pow(rad,2);

	if (dist <= r2)
	{
		float fCircle = (float)pow((1.0 - feather_amount) * rad,2);

		if (dist > fCircle)
			result = 1.0f - (dist - fCircle) / (r2 - fCircle);
		else
			result = 1.0f;
	}
	else
		result = 0.0f;

	return result;
}

AFSC::AFSC(unsigned char* data, unsigned int dataSize)
{
	type = LAYER_AFSC;

	this->data = new unsigned char[dataSize];
	memcpy(this->data, data, dataSize);
	this->size = dataSize;
}

AFSN::AFSN(unsigned char* data, unsigned int dataSize)
{
	type = LAYER_AFSN;

	this->data = new unsigned char[dataSize];
	memcpy(this->data, data, dataSize);
	this->size = dataSize;
}

AFDF::AFDF(unsigned char* data, unsigned int dataSize)
{
	type = LAYER_AFDF;

	this->data = new unsigned char[dataSize];
	memcpy(this->data, data, dataSize);
	this->size = dataSize;
}

AFDN::AFDN(unsigned char* data, unsigned int dataSize)
{
	type = LAYER_AFDN;

	this->data = new unsigned char[dataSize];
	memcpy(this->data, data, dataSize);
	this->size = dataSize;
}

ARIV::ARIV(unsigned char* data, unsigned int dataSize, unsigned char* data2, unsigned int dataSize2)
{
	type = LAYER_ARIV;

	//These are generally unused and can be ignored.
	this->data = new unsigned char[dataSize];
	memcpy(this->data, data, dataSize);
	this->size = dataSize;

	this->data2 = new unsigned char[dataSize2];
	memcpy(this->data2, data2, dataSize2);
	this->size2 = dataSize2;
}

AHTR::AHTR(unsigned char* data, unsigned int dataSize)
{
	type = LAYER_AHTR;
	
	memcpy(&flat_ratio, &data[0], 4);
	memcpy(&height_delta, &data[4], 4);
}

ASRP::ASRP(unsigned char* data, unsigned int dataSize)
{
	type = LAYER_ASRP;
	
	this->data = new unsigned char[dataSize];
	memcpy(this->data, data, dataSize);
	this->size = dataSize;
}

FDIR::FDIR(unsigned char* data, unsigned int dataSize)
{
	type = LAYER_FDIR;

	this->data = new unsigned char[dataSize];
	memcpy(this->data, data, dataSize);
	this->size = dataSize;
}