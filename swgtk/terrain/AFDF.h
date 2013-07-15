#include "Layer.h"

class AFDF : public TRNLib::LAYER
{
public:
	AFDF(unsigned char* data, unsigned int dataSize);

	unsigned char* data;
	unsigned int size;
};