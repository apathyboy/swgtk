#include "Layer.h"

class AFSC : public TRNLib::LAYER
{
public:
	AFSC(unsigned char* data, unsigned int dataSize);

	unsigned char* data;
	unsigned int size;
};