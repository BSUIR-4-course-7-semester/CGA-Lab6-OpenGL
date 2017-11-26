#pragma once

struct Coordinates {
	float* values;
	int count;
};

class LadderGenerator {
	public:
		LadderGenerator(float step_height, float step_length, float ladder_width, float step_count);

		Coordinates* generate();

	private:
		float step_height;
		float step_length;
		float ladder_width;
		float step_count;
		Coordinates* coordinates;

		float center_x = .0f;
		float center_y = .0f;
		float center_z = .0f;

		void generate_base();
		void generate_back();
		void generate_steps();
		void add_coordinate(float c);
		void add_point(float x, float y, float z);
};
