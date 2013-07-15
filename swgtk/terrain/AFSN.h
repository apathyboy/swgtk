#include "Layer.h"

class AFSN : public TRNLib::LAYER
{
public:
	AFSN(unsigned char* data, unsigned int dataSize);

	unsigned char* data;
	unsigned int size;
};