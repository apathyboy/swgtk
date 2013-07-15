#include "Layer.h"

class AROA : public TRNLib::LAYER
{
public:
	AROA(unsigned char* data, unsigned int dataSize, unsigned char* data2, unsigned int dataSize2);

	unsigned char* data;
	unsigned char* data2;
	unsigned int size;
	unsigned int size2;
};