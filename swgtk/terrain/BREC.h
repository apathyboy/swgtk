#include "Boundary.h"

class BREC : public TRNLib::Boundary
{
public:
	BREC(unsigned char* data, unsigned int dataSize);

	float x1;
	float z1;

	float x2;
	float z2;

	bool isContained(float px, float pz);
	float process(float px, float pz);
};