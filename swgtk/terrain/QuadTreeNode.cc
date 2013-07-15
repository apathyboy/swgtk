/*
---------------------------------------------------------------------------------------
This source file is part of swgANH (Star Wars Galaxies - A New Hope - Server Emulator)
For more information, see http://www.swganh.org


Copyright (c) 2006 - 2010 The swgANH Team

---------------------------------------------------------------------------------------
*/

#include "QuadTreeNode.h"
#include "Rectangle.h"
#include "Circle.h"

#include <cassert>

//======================================================================================================================
//
// Constructor
//

QuadTreeNode::QuadTreeNode(float lowX,float lowZ,float width,float height) :
Rectangle(lowX,lowZ,width,height),mSubNodes(NULL)
{
}

//======================================================================================================================
//
// Deconstructor
//

QuadTreeNode::~QuadTreeNode()
{
	// if its a branch, free our children
	if(mSubNodes)
	{
		for(unsigned char i = 0;i < 4;i++)
		{
			delete(mSubNodes[i]);
		}

		free(mSubNodes);
	}
}

//======================================================================================================================
//
// grow the tree by one level
//

void QuadTreeNode::subDivide()
{
	// this is a leaf, so make it a branch and grow 4 leafs
	if(!mSubNodes)
	{
		// make them a quarter size of their parent
		float width		= mWidth * 0.5f;
		float height	= mHeight * 0.5f;

		// create them
		mSubNodes = (QuadTreeNode**)::malloc(4 * sizeof(QuadTreeNode*));

		mSubNodes[0] = new QuadTreeNode(mPosition.x,mPosition.z + height,width,height);
		mSubNodes[1] = new QuadTreeNode(mPosition.x + width,mPosition.z + height,width,height);
		mSubNodes[2] = new QuadTreeNode(mPosition.x + width,mPosition.z,width,height);
		mSubNodes[3] = new QuadTreeNode(mPosition.x,mPosition.z,width,height);
	}
	// its a branch, so traverse its children
	else
	{
		for(unsigned char i = 0;i < 4;i++)
		{
			mSubNodes[i]->subDivide();
		}
	}
}

//======================================================================================================================
//
// checks if a node intersects with a given region
//

bool QuadTreeNode::intersects(Anh_Math::Shape* shape)
{
	// rectangular
	if(Anh_Math::Rectangle* rectangle = dynamic_cast<Anh_Math::Rectangle*>(shape))
	{
		const glm::vec3& rectPos = rectangle->getPosition();

		// check intersection
		if(rectPos.x > mPosition.x + mWidth  || rectPos.x + rectangle->getWidth()  < mPosition.x
		|| rectPos.z > mPosition.z + mHeight || rectPos.z + rectangle->getHeight() < mPosition.z)
		{
			return(false);
		}

		return(true);
	}
	// circle
	else if(dynamic_cast<Anh_Math::Circle*>(shape))
	{
		// TODO
		return(false);
	}

	return(false);
}