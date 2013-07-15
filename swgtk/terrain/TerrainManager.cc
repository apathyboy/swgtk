/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team
---------------------------------------------------------------------------------------
Use of this source code is governed by the GPL v3 license that can be found
in the COPYING file or at http://www.gnu.org/licenses/gpl-3.0.html

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
---------------------------------------------------------------------------------------
*/

#include "TerrainManager.h"
#include "BPOL.h"
#include "Height.h"
#include "Filter.h"

#include <iostream>

//#include <glog\logging.h>

using namespace TRNLib;

TerrainManager::TerrainManager(std::string terrain_filename)
	: terrain_file(terrain_filename)
{
	if (terrain_file.getError() == 0)
	{
		//LOG(INFO) << "Successfully loaded the terrain file.";
	}
	else
	{
		//LOG(ERROR) << "Unable to load the terrain file.";
		abort();
	}
}


TerrainManager::~TerrainManager(void)
{
}

HEADER* TerrainManager::getHeaders(void)
{
	return terrain_file.getHeaderData();
}

float TerrainManager::getWaterHeight(float x, float z, float& water_height)
{
	HEADER* header = getHeaders();
	std::vector<Boundary*>* boundaries = terrain_file.getWaterBoundaries();
	Boundary* boundary;

	for (unsigned int i = 0; i < boundaries->size(); i++)
	{
		boundary = boundaries->at(i);
		if (boundary->isContained(x, z))
		{
			water_height = ((BPOL*)(boundary))->water_height;
			return true;
		}
	}

	if (header->use_global_water_height != 0)
	{
		water_height = header->global_water_height;
		return true;
	}

	return false;
}

bool TerrainManager::isWater(float x, float z)
{
	float water_height;
	float height;

	if (getWaterHeight(x, z, water_height))
	{
		height = getHeight(x, z);
		if (height <= water_height)
			return true;
	}

	return false;
}

LAYER* TerrainManager::findLayer(float x, float z)
{
	std::vector<CONTAINER_LAYER*>* layers = terrain_file.getLayers();
	TRNLib::CONTAINER_LAYER* layer;
	std::vector<LAYER*> boundaries;
	TRNLib::Boundary* boundary;

	for (unsigned int i = 0; i < layers->size(); i++)
	{
		layer = (CONTAINER_LAYER*)layers->at(i);
		boundaries = layer->boundaries;

		for (unsigned int j = 0; j < boundaries.size(); j++)
		{
			boundary = (Boundary*)boundaries.at(j);
			if (boundary->isContained(x, z))
				return findLayerRecursive(x, z, layer);
		}
	}

	return layers->at(0);
}

TRNLib::LAYER* TerrainManager::findLayerRecursive(float x, float z, LAYER* rootLayer)
{
	std::vector<CONTAINER_LAYER*> layers = ((CONTAINER_LAYER*)rootLayer)->children;
	CONTAINER_LAYER* layer;
	std::vector<LAYER*> boundaries;
	Boundary* boundary;

	for (unsigned int i = 0; i < layers.size(); i++)
	{
		layer = (CONTAINER_LAYER*)layers.at(i);
		boundaries = layer->boundaries;

		for (unsigned int j = 0; j < boundaries.size(); j++)
		{
			boundary = (Boundary*)boundaries.at(j);
			if (boundary->isContained(x, z))
				return findLayerRecursive(x, z, layer);
		}
	}

	return rootLayer;
}

TRNLib::MFAM* TerrainManager::getFractal(int fractal_id)
{
	MFAM* fractal;
	std::vector<MFAM*>* fractals = terrain_file.getFractalFamilies();

	for (unsigned int i = 0; i < fractals->size(); i++)
	{
		fractal = fractals->at(i);
		if (fractal->fractal_id == fractal_id)
			return fractal;
	}

	return NULL;
}

float TerrainManager::getHeight(float x, float z)
{
	std::vector<CONTAINER_LAYER*>* layers = terrain_file.getLayers();

	float affector_transform = 1.0f;
	float transform_value = 0.0f;
	float height_result = 0.0f;

	for (unsigned int i = 0; i < layers->size(); i++)
	{
		CONTAINER_LAYER* layer = layers->at(i);

		if (layer->enabled)
			transform_value = processLayerHeight(layer, x, z, height_result, affector_transform);
	}

	return height_result;
}

float TerrainManager::processLayerHeight(TRNLib::CONTAINER_LAYER* layer, float x, float z, float& base_value, float affector_transform)
{
	std::vector<TRNLib::LAYER*> boundaries = layer->boundaries;
	std::vector<TRNLib::LAYER*> heights = layer->heights;
	std::vector<TRNLib::LAYER*> filters = layer->filters;

	float transform_value = 0.0f;
	bool has_boundaries = false;
	float result = 0.0f;

	for (unsigned int i = 0; i < boundaries.size(); i++)
	{
		TRNLib::Boundary* boundary = (Boundary*)boundaries.at(i);

		if (!boundary->enabled)
			continue;
		else
			has_boundaries = true;

		float result = boundary->process(x, z);

		result = calculateFeathering(result, boundary->feather_type);

		if (result > transform_value)
			transform_value = result;

		if (transform_value >= 1)
			break;
	}

	if (has_boundaries == false)
		transform_value = 1.0f;

	if (layer->invert_boundaries)
		transform_value = 1.0f - transform_value;

	if (transform_value != 0) 
	{
		for (unsigned int i = 0; i < filters.size(); ++i) 
		{
			TRNLib::Filter* filter = (Filter*)filters.at(i);

			if (!filter->enabled)
				continue;

			float result = filter->process(x, z, transform_value, base_value, this);

			result = calculateFeathering(result, filter->feather_type);

			if (transform_value > result)
				transform_value = result;

			if (transform_value == 0)
				break;
		}

		if (layer->invert_filters)
			transform_value = 1.0f - transform_value;

		if (transform_value != 0)
		{
			for (unsigned int i = 0; i < heights.size(); i++)
			{
				TRNLib::Height* affector = (Height*)heights.at(i);
			
				if (affector->enabled)
				{
					affector->getBaseHeight(x, z, transform_value, base_value, this);
				}
			}

			std::vector<CONTAINER_LAYER*> children = layer->children;

			for (unsigned int i = 0; i < children.size(); i++)
			{
				CONTAINER_LAYER* child = children.at(i);

				if (child->enabled)
					processLayerHeight(child, x, z, base_value, affector_transform * transform_value);
			}
		}
	}

	return transform_value;
}

float TerrainManager::calculateFeathering(float value, int featheringType) {
	/* 1: x^2
	 * 2: sqrt(x)
	 * 3: x^2 * (3 - 2x)
	 */


	float result = value;

	switch (featheringType) {
	case 1:
		result = result * result;
		break;
	case 2:
		result = sqrt(result);
		break;
	case 3:
		result = result * result * (3 - 2 * result);
		break;
	case 0:
		result = result;
		break;
	default:
		result = 0;
		break;
	}

	return result;
}