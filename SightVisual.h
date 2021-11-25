#pragma once

#include <Godot.hpp>
#include <Node2D.hpp>
#include <KinematicBody2D.hpp>

namespace godot {

	struct AngleRay
	{
		bool hit = false;
		float angle;
		Vector2 point;
	};

	class SightVisual : public Node2D
	{
		GODOT_CLASS(SightVisual, Node2D)

	public:
		static void _register_methods();

		SightVisual();
		~SightVisual();

		void _init(); // our initializer called by Godot
		void _process(float delta);
		void _physics_process(float delta);
		void _draw();
		void _ready();

	private:
		// methods
		PoolVector2Array* generate_points();
		void locate_corner(AngleRay start_ray, AngleRay end_ray, int solver_steps);
		AngleRay ray_cast_angle(float angle);
		Vector2& angle_to_point(float angle);

		//exported
		int fov = 0;
		int segments = 1;
		int radius = 0;
		Color color;
		int collision_layer = 1;

		// properties
		PoolVector2Array* draw_points;
		Node2D* parent;
	};

}



