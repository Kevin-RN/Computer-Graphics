#include "Box.hpp"
#include "Ray.hpp"
#include "Triangle.hpp"



// Source: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
// Source: Slides Dhr. Eisenman
bool Box::intersects(const Eigen::Vector3f& origin, const Eigen::Vector3f& direction, Eigen::Vector3f& vmin, Eigen::Vector3f& vmax, float& t) {

	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	Ray r = Ray(origin, direction);
	Eigen::Vector3f invdir = r.getRayInvDir();
	Eigen::Vector3f sign = r.getSigns();

	std::vector<Eigen::Vector3f> bounds;
	bounds.push_back(vmin);
	bounds.push_back(vmax);

	tmin = (bounds[sign[0]][0] - origin[0]) * invdir[0];
	tmax = (bounds[1 - sign[0]][0] - origin[0]) * invdir[0];
	tymin = (bounds[sign[1]][1] - origin[1]) * invdir[1];
	tymax = (bounds[1 - sign[1]][1] - origin[1]) * invdir[1];

	if ((tmin > tymax) || (tymin > tmax))
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	tzmin = (bounds[sign[2]][2] - origin[2]) * invdir[2];
	tzmax = (bounds[1 - sign[2]][2] - origin[2]) * invdir[2];

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	t = tmin;
	return true;
}

bool Box::intersects(Tucano::Mesh& mesh, const Eigen::Vector3f& origin, const Eigen::Vector3f& direction, float& t, Tucano::Face& face) {
	bool is = false;
	if (faces.size() != 0) {
		float thit = t;
		for (Tucano::Face f : faces) {
			if (Triangle::intersects(mesh, f, direction, origin, thit)) {
				is = true;
				if (thit < t) {
					t = thit;
					face = f;
				}
			}

		}
		return is;
	}
	if (iboxes.size() != 0) {
		if (iboxes[0].intersects(mesh, origin, direction, t, face))
			is = true;
		if (iboxes[1].intersects(mesh, origin, direction, t, face))
			is = true;
	}
	return is;
}

bool Box::isEmpty() {
	return (iboxes.size() == 0 && faces.size() == 0);
}

std::vector<Box> Box::getBoxesList() {
	std::vector<Box> boxes;
//	boxes.push_back(this);
	for (Box b : iboxes) {
		if (!b.isEmpty()) {
			boxes.push_back(b);
			b.getBoxesListRec(boxes);
		}

	}
	return boxes;
}


void Box::getBoxesListRec(std::vector<Box>& boxes) {
	for (Box b : iboxes) {
		if (!b.isEmpty()) {
			boxes.push_back(b);
			b.getBoxesListRec(boxes);
		}
	}
}

void Box::findMinMax(Tucano::Mesh& mesh, Eigen::Vector3f& vmin, Eigen::Vector3f& vmax) {
	vmin = Eigen::Vector3f(INFINITY, INFINITY, INFINITY);
	vmax = Eigen::Vector3f(-INFINITY, -INFINITY, -INFINITY);
	Eigen::Vector3f ver_0;

	for (int i = 0; i < mesh.getNumberOfVertices();i++) {
		ver_0 = (mesh.getShapeModelMatrix() * (mesh.getVertex(i))).head<3>();

		if (ver_0[0] < vmin[0])
			vmin[0] = ver_0[0];
		if (ver_0[1] < vmin[1])
			vmin[1] = ver_0[1];
		if (ver_0[2] < vmin[2])
			vmin[2] = ver_0[2];

		if (ver_0[0] > vmax[0])
			vmax[0] = ver_0[0];
		if (ver_0[1] > vmax[1])
			vmax[1] = ver_0[1];
		if (ver_0[2] > vmax[2])
			vmax[2] = ver_0[2];
	}
	return;
}

Box::Box(Eigen::Vector3f vmin, Eigen::Vector3f vmax) {
	this->vmin = vmin;
	this->vmax = vmax;
}
Box::Box(Tucano::Mesh& mesh, Eigen::Vector3f vmin, Eigen::Vector3f vmax) {
	this->vmin = vmin;
	this->vmax = vmax;

	for (int i = 0; i < mesh.getNumberOfFaces(); i++)
		this->faces.push_back(mesh.getFace(i));
}

void Box::splitBox(Tucano::Mesh& mesh) {
	int axis = largestSide();
	float splitval = vmin[axis] + ((vmax[axis] - vmin[axis]) / 2);
	Eigen::Vector3f ver_0;
	Eigen::Vector3f ver_1;
	Eigen::Vector3f ver_2;

	Box ibox1 = Box(vmin, vmax);
	ibox1.vmax[axis] = splitval;
	Box ibox2 = Box(vmin, vmax);
	ibox2.vmin[axis] = splitval;



	for (Tucano::Face f : faces) {
		ver_0 = (mesh.getShapeModelMatrix() * mesh.getVertex(f.vertex_ids[0])).head<3>();
		ver_1 = (mesh.getShapeModelMatrix() * mesh.getVertex(f.vertex_ids[1])).head<3>();
		ver_2 = (mesh.getShapeModelMatrix() * mesh.getVertex(f.vertex_ids[2])).head<3>();

		if (ver_0[axis] <= splitval || ver_1[axis] <= splitval || ver_2[axis] <= splitval) {
			ibox1.faces.push_back(f);
		}

		if (ver_0[axis] >= splitval || ver_1[axis] >= splitval || ver_2[axis] >= splitval) {
			ibox2.faces.push_back(f);
		}
	}

	iboxes.push_back(ibox1);
	iboxes.push_back(ibox2);
	faces.clear();
}

int Box::largestSide() {
	if (vmax[0] - vmin[0] > vmax[1] - vmin[1]) {
		if (vmax[0] - vmin[0] > vmax[2] - vmin[2]) {
			return 0;
		}
	}
	if (vmax[1] - vmin[1] > vmax[2] - vmin[2])
		return 1;
	else
		return 2;
}

void Box::splitboxLimit(Tucano::Mesh& mesh, int max) {
	if (faces.size() > max) {
		splitBox(mesh);
		iboxes[0].splitboxLimit(mesh, max);
		iboxes[1].splitboxLimit(mesh, max);
	}

}
