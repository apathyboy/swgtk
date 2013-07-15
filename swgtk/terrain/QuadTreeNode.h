/*
---------------------------------------------------------------------------------------
This source file is part of swgANH (Star Wars Galaxies - A New Hope - Server Emulator)
For more information, see http://www.swganh.org


Copyright (c) 2006 - 2010 The swgANH Team

---------------------------------------------------------------------------------------
*/

#ifndef	ANH_ZONESERVER_QUADTREE_H
#define	ANH_ZONESERVER_QUADTREE_H

#include "Rectangle.h"
#include <map>
#include <set>
#include "glm/glm.hpp"

class Object;

namespace Anh_Math
{
    class Space;
}

typedef std::map<unsigned long long,Object*> StdObjectMap;
typedef std::set<Object*> ObjectSet;

//======================================================================================================================

class QuadTreeNode : public Anh_Math::Rectangle
{
	public:

		QuadTreeNode(float lowX,float lowZ,float width,float height);
		virtual ~QuadTreeNode();

		int		addObject(Object* object);
		int		removeObject(Object* object);
		int		updateObject(Object* object, const glm::vec3& newPosition);

		bool	checkBounds(Object* object);
		bool	intersects(Anh_Math::Shape* shape);
		bool	ObjectContained(Anh_Math::Shape* shape, Object* object);
		void	getObjectsInRange(Object* object,ObjectSet* resultSet,unsigned int typeMask,Anh_Math::Shape* shape);
		void	getObjectsInRangeContains(Object* object,ObjectSet* resultSet,unsigned int typeMask,Anh_Math::Shape* shape);
		
		void	subDivide();

	protected:

		QuadTreeNode**	mSubNodes;
		StdObjectMap	mObjects;
};

//======================================================================================================================

#endif


