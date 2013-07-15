#include "Boundary.h"

class BPLN : public TRNLib::Boundary
{
public:
	BPLN(unsigned char* data, unsigned int dataSize);
	
	std::vector<TRNLib::VERTEX*> verts;

	float line_width;

	float max_x, max_z, min_x, min_z;

	bool isContained(float x, float z);
	float process(float px, float pz);
};