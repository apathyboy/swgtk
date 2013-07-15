#include "Layer.h"

class AENV : public TRNLib::LAYER
{
public:
	AENV(unsigned char* data, unsigned int dataSize);

	unsigned char* data;
	unsigned int size;
};

