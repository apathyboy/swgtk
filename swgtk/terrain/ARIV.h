#include "Layer.h"

class ARIV : public TRNLib::LAYER
{
public:
	ARIV(unsigned char* data, unsigned int dataSize, unsigned char* data2, unsigned int dataSize2);

	unsigned char* data;
	unsigned char* data2;
	unsigned int size;
	unsigned int size2;
};