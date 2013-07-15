/*
This file is part of MMOServer. For more information, visit http://swganh.com
Copyright (c) 2006 - 2010 The SWG:ANH Team

MMOServer is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

MMOServer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with MMOServer. If not, see <http://www.gnu.org/licenses/>.
*/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Terrain

#include <boost/test/unit_test.hpp>

#include <vector>
#include <glm/glm.hpp>
#include "TerrainManager.h"

/// By default a new instance of the dispatcher should not have any listeners.
BOOST_AUTO_TEST_CASE(getWaterHeight) {
	// Construct a terrain manager for Tattooine.
	TRNLib::TerrainManager tm_test("terrain/tatooine.trn");

	// Obtain headers from the terrain manager.
	TRNLib::HEADER* header = tm_test.getHeaders();

	// Test if we are getting values we expect for global water height from the header
	BOOST_REQUIRE_MESSAGE((0 == header->use_global_water_height), "Expected use_global_water_height of 0");
	BOOST_REQUIRE_MESSAGE((0, header->global_water_height), "Expected global_water_height of 0");
}

BOOST_AUTO_TEST_CASE(getHeight) {
	// Construct a terrain manager for Tatooine
	TRNLib::TerrainManager tm_test("terrain/tatooine.trn");

	std::vector<glm::vec3> coords;
	glm::vec3 pos;

	pos.x = 3472.22f; pos.y = 5.f; pos.z = -4918.59f;
	coords.push_back(pos);
	pos.x = -1278.5f; pos.y = 12.53f; pos.z = -3643.1f;
	coords.push_back(pos);
	pos.x = 3888.2f; pos.y = 33.f; pos.z = 2350.f;
	coords.push_back(pos);
	pos.x = 3951.4f; pos.y = 53.5f; pos.z = 2265.1f;
	coords.push_back(pos);
	pos.x = 3951.11f; pos.y = 54.45f; pos.z = 2282.93f;
	coords.push_back(pos);

	for (unsigned int i = 0; i < coords.size(); i++)
	{
		pos = coords.at(i);
		float height_test = tm_test.getHeight(pos.x, pos.z);
		BOOST_REQUIRE_MESSAGE((fabs(height_test - pos.y) < 1), "Expected height of " << pos.y << " and got " << height_test);
	}
}
