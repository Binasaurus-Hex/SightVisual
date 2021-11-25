#include "SightVisual.h"
#include <World2D.hpp>
#include <Physics2DDirectSpaceState.hpp>
#include <Texture.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void SightVisual::_register_methods() {
	register_method("_process", &SightVisual::_process);
	register_method("_physics_process", &SightVisual::_physics_process);
	register_method("_draw", &SightVisual::_draw);
	register_method("_init", &SightVisual::_init);
	register_method("_ready", &SightVisual::_ready);
	register_property<SightVisual, int>("segments", &SightVisual::segments, 1);
	register_property<SightVisual, int>("radius", &SightVisual::radius, 1);
	register_property<SightVisual, int>("fov", &SightVisual::fov, 0);
	register_property<SightVisual, Color>("color", &SightVisual::color, Color(1, 0, 0, 1));
	register_property<SightVisual, int>("collision_layer", &SightVisual::collision_layer, 1);
}

SightVisual::SightVisual() {
	draw_points = new PoolVector2Array();
}

SightVisual::~SightVisual() {
	delete draw_points;
}

void SightVisual::_init() {
}

void SightVisual::_ready() {
	parent = Object::cast_to<Node2D>(get_parent());
}

void SightVisual::_process(float delta) {
	set_global_rotation(0);
}

void SightVisual::_physics_process(float delta) {
	draw_points = generate_points();
	update();
}

PoolVector2Array* SightVisual::generate_points() {
	PoolVector2Array* points = new PoolVector2Array();
	Vector2 global_position = get_global_position();
	points->push_back(Vector2(0, 0));

	float rotation = 0;
	if (parent) {
		rotation = parent->get_global_rotation_degrees();
	}
	float start_angle = rotation - fov / 2;
	float segment_angle = (float)fov / (float)segments;

	AngleRay old_raycast;

	for (int i = 0; i < segments; i++) {
		float angle = start_angle + i * segment_angle;
		AngleRay raycast = ray_cast_angle(angle);
		points->push_back(raycast.point - global_position);
	}

	return points;
}

AngleRay SightVisual::ray_cast_angle(float angle)
{
	AngleRay angle_ray;
	angle_ray.angle = angle;
	Vector2 point = angle_to_point(angle);
	Physics2DDirectSpaceState* space_state = get_world_2d()->get_direct_space_state();
	Dictionary collision = space_state->intersect_ray(get_global_position(), point, Array(), collision_layer);
	if (!collision.empty()) {
		angle_ray.hit = true;
		point = collision["position"];
	}
	angle_ray.point = point;
	return angle_ray;
}

Vector2& SightVisual::angle_to_point(float angle)
{
	float angle_rads = (Math_PI / 180) * angle;
	Vector2 point = Vector2(cos(angle_rads), sin(angle_rads));
	point *= radius;
	point += get_global_position();
	return point;
}

void SightVisual::_draw() {
	PoolColorArray colors = PoolColorArray();
	colors.push_back(color);
	if (draw_points != nullptr) {
		draw_polygon(*draw_points, colors);
	}
}
