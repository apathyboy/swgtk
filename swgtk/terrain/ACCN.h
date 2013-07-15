#include "Layer.h"

class ACCN : public TRNLib::LAYER
{
public:
	ACCN(unsigned char* data, unsigned int dataSize);

	unsigned char* data;
	unsigned int size;
};