#include "Layer.h"

class AHTR : public TRNLib::LAYER
{
public:
	AHTR(unsigned char* data, unsigned int dataSize);

	float flat_ratio;
	float height_delta;
};

