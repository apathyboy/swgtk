#include "Layer.h"

class AFDN : public TRNLib::LAYER
{
public:
	AFDN(unsigned char* data, unsigned int dataSize);

	unsigned char* data;
	unsigned int size;
};