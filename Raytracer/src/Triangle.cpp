#include "Triangle.hpp"

bool Triangle::intersects(Tucano::Mesh& mesh, const Tucano::Face& face, const Eigen::Vector3f& ray_direction, const Eigen::Vector3f& origin, float& thit) {

	// Get the vertices from the face
	const Eigen::Vector3f ver_0 = (mesh.getShapeModelMatrix() * mesh.getVertex(face.vertex_ids[0])).head<3>();
	const Eigen::Vector3f ver_1 = (mesh.getShapeModelMatrix() * mesh.getVertex(face.vertex_ids[1])).head<3>();
	const Eigen::Vector3f ver_2 = (mesh.getShapeModelMatrix() * mesh.getVertex(face.vertex_ids[2])).head<3>();

	Eigen::Vector3f normal_traingle = face.normal;

	float D = normal_traingle.dot(ver_0);
	float t_bottom = ray_direction.dot(normal_traingle);

	// Check if the ray is parallel to the triangle
	// Maybe change this to epsilon?
	if (t_bottom == 0.0f) {
		return false;
	}

	float t_top = (D - origin.dot(normal_traingle));
	float t = t_top / t_bottom;

	// Check if t is facing forward or backwards
	if (t <= exp2(-20)) {
		return false;
	}

	// Construct ray
	const Eigen::Vector3f p = origin + t * ray_direction;

	// Check for each edge of the traingle of the point is on the right of lef t(in- or outside the traingle)
	// Make use of Barycentric coordinates
	// Source: https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/barycentric-coordinates
	// Source: Slides Dhr. Eisenman Computer Graphics 
	const Eigen::Vector3f& edge0 = ver_1 - ver_0;
	const Eigen::Vector3f& vp0 = p - ver_0;
	Eigen::Vector3f C = edge0.cross(vp0);
	if (normal_traingle.dot(C) < 0) {
		return false;
	}

	const Eigen::Vector3f& edge1 = ver_2 - ver_1;
	const Eigen::Vector3f& vp1 = p - ver_1;
	C = edge1.cross(vp1);
	if (normal_traingle.dot(C) < 0) {
		return false;
	}

	const Eigen::Vector3f& edge2 = ver_0 - ver_2;
	const Eigen::Vector3f& vp2 = p - ver_2;
	C = edge2.cross(vp2);
	if (normal_traingle.dot(C) < 0) {
		return false;
	}

	// thit will be the new 'Closest' hit
	thit = t;

	return true;
}


























































/*myversion
bool Triangle::intersects(Tucano::Mesh mesh, int index, const Tucano::Face& face, const Eigen::Vector3f& ray_direction, const Eigen::Vector3f& origin) {
    const Eigen::Vector3f vertex_0 = mesh.getVertex(0);//(mesh.getShapeModelMatrix() * mesh.getVertex(face.vertex_ids[0])).head<3>();
    const Eigen::Vector3f vertex_1 = mesh.getVertex(1); //(mesh.getShapeModelMatrix() * mesh.getVertex(face.vertex_ids[1])).head<3>();
    const Eigen::Vector3f vertex_2 = mesh.getVertex(2);//(mesh.getShapeModelMatrix() * mesh.getVertex(face.vertex_ids[2])).head<3>();
    const Eigen::Vector3f normal = ((vertex_0-vertex_2).crossProduct((vertex_1 - vertex_2)))/fabs(((vertex_0-vertex_2).crossProduct((vertex_1 - vertex_2))))
    float dist = normal.dot(vertex_0);
    float t_numerator = dist - (origin.dot(normal));
    float t_denominator = ray_direction.dot(normal);
    if (t_numerator == 0.0f) {
        return false;
    }
    float t = t_numerator / t_denominator;
    if (t <= 0) {
        return false;
    }
    Eigen::Vector3f p = origin + t * ray_direction;
    Eigen::Vector3f edge0 = vertex_1 - vertex_0;
    Eigen::Vector3f edge1 = vertex_2 - vertex_1;
    Eigen::Vector3f edge2 = vertex_0 - vertex_2;
    Eigen::Vector3f vp0 = p - vertex_0;
    Eigen::Vector3f vp1 = p - vertex_1;
    Eigen::Vector3f vp2 = p - vertex_2;
    if (triangleNormal.dot(edge0.cross(vp0)) > 0 &&
        triangleNormal.dot(edge1.cross(vp1)) > 0 &&
        triangleNormal.dot(edge2.cross(vp2)) > 0) {

        return true;
    }
    return false;
}*/

