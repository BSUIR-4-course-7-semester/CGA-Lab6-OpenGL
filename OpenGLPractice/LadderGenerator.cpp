#include <stddef.h>
#include <stdlib.h>

#include "LadderGenerator.h"

LadderGenerator::LadderGenerator(float step_height, float step_length, float ladder_width, float step_count)
{
	this->step_height = step_height;
	this->step_length = step_length;
	this->ladder_width = ladder_width;
	this->step_count = step_count;
}

Coordinates* LadderGenerator::generate()
{
	this->coordinates = (Coordinates*)malloc(sizeof(Coordinates));
	this->coordinates->count = 0;
	this->coordinates->values = NULL;

	this->generate_base();
	this->generate_back();
	this->generate_steps();

	return this->coordinates;
}

void LadderGenerator::generate_base()
{
	float ladder_length = this->step_count * this->step_length;
	float ladder_height = this->step_count * this->step_height;

	float left_x = this->center_x - this->ladder_width / 2;
	float right_x = this->center_x + this->ladder_width / 2;
	float top_y = this->center_y - ladder_length / 2;
	float bottom_y = this->center_y + ladder_length / 2;
	float z = this->center_z - ladder_height / 2;

	this->add_point(left_x, top_y, z);
	this->add_point(0.0f, 0.0f, -1.0f);
	this->add_point(right_x, top_y, z);
	this->add_point(0.0f, 0.0f, -1.0f);
	this->add_point(right_x, bottom_y, z);
	this->add_point(0.0f, 0.0f, -1.0f);

	this->add_point(right_x, bottom_y, z);
	this->add_point(0.0f, 0.0f, -1.0f);
	this->add_point(left_x, bottom_y, z);
	this->add_point(0.0f, 0.0f, -1.0f);
	this->add_point(left_x, top_y, z);
	this->add_point(0.0f, 0.0f, -1.0f);
}

void LadderGenerator::generate_back()
{
	float ladder_length = this->step_count * this->step_length;
	float ladder_height = this->step_count * this->step_height;

	float left_x = this->center_x - this->ladder_width / 2;
	float right_x = this->center_x + this->ladder_width / 2;
	float top_z = this->center_z + ladder_height / 2;
	float bottom_z = this->center_z - ladder_height / 2;
	float y = this->center_y + ladder_length / 2;

	this->add_point(left_x, y, top_z);
	this->add_point(0.0f, -1.0f, 0.0f);
	this->add_point(right_x, y, top_z);
	this->add_point(0.0f, -1.0f, 0.0f);
	this->add_point(right_x, y, bottom_z);
	this->add_point(0.0f, -1.0f, 0.0f);

	this->add_point(right_x, y, bottom_z);
	this->add_point(0.0f, -1.0f, 0.0f);
	this->add_point(left_x, y, bottom_z);
	this->add_point(0.0f, -1.0f, 0.0f);
	this->add_point(left_x, y, top_z);
	this->add_point(0.0f, -1.0f, 0.0f);
}

void LadderGenerator::generate_steps()
{
	float ladder_length = this->step_count * this->step_length;
	float ladder_height = this->step_count * this->step_height;

	float left_x = this->center_x - this->ladder_width / 2;
	float right_x = this->center_x + this->ladder_width / 2;
	float init_y = this->center_y - ladder_length / 2;
	float init_z = this->center_z - ladder_height / 2;

	for (int step_number = 0; step_number < this->step_count; step_number++) {
		float front_y = init_y + step_number * this->step_length;
		float back_y = front_y + this->step_length;
		float bottom_z = init_z + (step_number + 1) * this->step_height;
		float top_z = init_z;

		add_point(right_x, back_y, bottom_z);
		add_point(0.0f, 0.0f, -1.0f);
		add_point(left_x, back_y, bottom_z);
		add_point(0.0f, 0.0f, -1.0f);
		add_point(left_x, front_y, bottom_z);
		add_point(0.0f, 0.0f, -1.0f);

		add_point(left_x, front_y, bottom_z);
		add_point(0.0f, 0.0f, -1.0f);
		add_point(right_x, back_y, bottom_z);
		add_point(0.0f, 0.0f, -1.0f);
		add_point(right_x, front_y, bottom_z);
		add_point(0.0f, 0.0f, -1.0f);

		add_point(left_x, front_y, top_z);
		add_point(0.0f, -1.0f, 0.0f);
		add_point(right_x, front_y, top_z);
		add_point(0.0f, -1.0f, 0.0f);
		add_point(right_x, front_y, bottom_z);
		add_point(0.0f, -1.0f, 0.0f);

		add_point(left_x, front_y, top_z);
		add_point(0.0f, -1.0f, 0.0f);
		add_point(left_x, front_y, bottom_z);
		add_point(0.0f, -1.0f, 0.0f);
		add_point(right_x, front_y, bottom_z);
		add_point(0.0f, -1.0f, 0.0f);

		add_point(left_x, front_y, top_z);
		add_point(-1.0f, 0.0f, 0.0f);
		add_point(left_x, back_y, top_z);
		add_point(-1.0f, 0.0f, 0.0f);
		add_point(left_x, back_y, bottom_z);
		add_point(-1.0f, 0.0f, 0.0f);

		add_point(left_x, front_y, top_z);
		add_point(-1.0f, 0.0f, 0.0f);
		add_point(left_x, front_y, bottom_z);
		add_point(-1.0f, 0.0f, 0.0f);
		add_point(left_x, back_y, bottom_z);
		add_point(-1.0f, 0.0f, 0.0f);

		add_point(right_x, front_y, top_z);
		add_point(-1.0f, 0.0f, 0.0f);
		add_point(right_x, back_y, top_z);
		add_point(-1.0f, 0.0f, 0.0f);
		add_point(right_x, back_y, bottom_z);
		add_point(-1.0f, 0.0f, 0.0f);

		add_point(right_x, front_y, top_z);
		add_point(-1.0f, 0.0f, 0.0f);
		add_point(right_x, front_y, bottom_z);
		add_point(-1.0f, 0.0f, 0.0f);
		add_point(right_x, back_y, bottom_z);
		add_point(-1.0f, 0.0f, 0.0f);
	}
}

void LadderGenerator::add_coordinate(float c)
{
	this->coordinates->count++;
	this->coordinates->values = (float*)realloc(this->coordinates->values, this->coordinates->count * sizeof(float));
	this->coordinates->values[this->coordinates->count - 1] = c;
}

void LadderGenerator::add_point(float x, float y, float z)
{
	this->add_coordinate(x);
	this->add_coordinate(y);
	this->add_coordinate(z);
}
