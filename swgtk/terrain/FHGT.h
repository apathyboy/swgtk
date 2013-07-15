#include "Filter.h"

class FHGT : public TRNLib::Filter
{
public:
	FHGT(unsigned char* data, unsigned int dataSize);

	float minHeight;
	float maxHeight;

	float process(float px, float pz, float transform_value, float& base_value, TRNLib::TerrainManager* terrain);
};