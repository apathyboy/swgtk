
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "swgtk/terrain/TerrainManager.h"

int main()
{
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

        if (fabs(height_test - pos.y) < 1) {
            std::cout << "Success!" << std::endl;
        } else {
            std::cout <<"Expected height of " << pos.y << " and got " << height_test << std::endl;
        }
	}
}