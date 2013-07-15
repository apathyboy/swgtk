#include "Layer.h"

class ACRH : public TRNLib::LAYER
{
public:
	ACRH(unsigned char* data, unsigned int dataSize);
	
	unsigned char* data;
	unsigned int size;
};


