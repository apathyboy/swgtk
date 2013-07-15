#include "Layer.h"

class ACRF : public TRNLib::LAYER
{
public:
	ACRF(unsigned char* data, unsigned int dataSize);

	unsigned char* data;
	unsigned int size;
};

