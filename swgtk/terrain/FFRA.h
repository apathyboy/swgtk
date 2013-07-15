#include "Filter.h"

class FFRA : public TRNLib::Filter
{
public:
	FFRA(unsigned char* data, unsigned int dataSize);

	int fractal_id;
	float min;
	float max;
	float step;

	float process(float x, float z, float transform_value, float& base_value, TRNLib::TerrainManager* terrain);
};