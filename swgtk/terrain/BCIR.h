#include "Boundary.h"

class BCIR : public TRNLib::Boundary
{
public:
	BCIR(unsigned char* data, unsigned int dataSize);

	float x;
	float z;
	float rad;

	bool isContained(float px, float pz);
	float process(float px, float pz);
};