#include "Boundary.h"

class BPOL : public TRNLib::Boundary
{
public:
	BPOL(unsigned char* data, unsigned int dataSize);
	
	std::vector<TRNLib::VERTEX*> verts;

	int use_water_height;
	float water_height;
	float water_shader_size;
	unsigned char* water_shader;

	float max_x, max_z, min_x, min_z;

	bool isContained(float px, float pz);
	float process(float px, float pz);
};