#pragma once
#include "Ray.hpp"

class Box
{
public:
	Box() {};
	Box(Eigen::Vector3f vmin, Eigen::Vector3f vmax);
	Box(Tucano::Mesh& mesh, Eigen::Vector3f vmin, Eigen::Vector3f vmax);
	//Box(float x_min, float x_max, float y_min, float y_max, float z_min, float z_max);
	static bool intersects(const Eigen::Vector3f& origin, const Eigen::Vector3f& direction, Eigen::Vector3f& vmin, Eigen::Vector3f& vmax, float& t);
	bool intersects(Tucano::Mesh& mesh, const Eigen::Vector3f& origin, const Eigen::Vector3f& direction, float& t, Tucano::Face& face);
	static void findMinMax(Tucano::Mesh& mesh, Eigen::Vector3f& vmin, Eigen::Vector3f& vmax);
	void splitBox(Tucano::Mesh& mesh);
	void splitboxLimit(Tucano::Mesh& mesh, int max);
	std::vector<Box> iboxes;
	std::vector<Tucano::Face> faces;
	Eigen::Vector3f vmin, vmax;
	std::vector<Box> getBoxesList();
	bool isEmpty();

private:
	void getBoxesListRec(std::vector<Box>& boxes);
	int largestSide();
};
