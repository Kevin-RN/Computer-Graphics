#pragma once
#include "Entity.hpp"
//#ifndef __BOUNDBOX__
//#define __BOUNDBOX__

// Must be included before glfw.


class BoundingBox:
	public Entity
{
public:
	std::vector<Entity> entities;

	// virtual method inherited from Entity class
	//std::pair<Entity, Eigen::Vector3f> intersects(Eigen::Vector3f &origin, Eigen::Vector3f& ray);

	BoundingBox(std::vector < Eigen::Vector3f> position);
	//BoundingBox(std::vector<Eigen::Vector3f> vertices);
	int size(); // returns number of elements in the bounding box 
	void addEntity(Entity e); // adds an entity to the bounding box
	//static BoundingBox generateFromModel(Tucano::Mesh mesh);

	//std::vector<Entity> split();

	Eigen::Vector3f min;
	Eigen::Vector3f max;

	//int getLongestAxis();
};
//#endif 
