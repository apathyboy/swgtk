#ifndef ANH_TERRAIN_HEIGHT_H_
#define ANH_TERRAIN_HEIGHT_H_

#include "layer.h"

namespace TRNLib
{
	class Height : public TRNLib::LAYER
	{
	public:
		virtual void getBaseHeight(float x, float z, float transform_value, float& base_value, TerrainManager* terrain) = 0;
	};
};
#endif
