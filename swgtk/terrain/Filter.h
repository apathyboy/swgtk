#ifndef ANH_TERRAIN_FILTER_H_
#define ANH_TERRAIN_FILTER_H_

#include "layer.h"

namespace TRNLib
{
    class Filter : public TRNLib::LAYER
    {
    public:
        int   feather_type;
        float feather_amount;

		virtual float process(float x, float z, float transform_value, float& base_value, TerrainManager* terrain) = 0;
    };
};
#endif