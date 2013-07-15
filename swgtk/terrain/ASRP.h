#include "Layer.h"

class ASRP : public TRNLib::LAYER
{
public:
	ASRP(unsigned char* data, unsigned int dataSize);

	unsigned char* data;
	unsigned int size;
};

