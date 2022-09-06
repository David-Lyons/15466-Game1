#include "asset_script.hpp"

void create_assets() {
	// Set up vectors to hold load_png results
	glm::uvec2 palette_size = glm::uvec2(0, 0);
	std::vector< glm::u8vec4 > palette_data;

	// Set up vectors for storage
	std::vector< PPU466::Palette > palettes;
	std::vector< PPU466::Tile > tiles;
	std::vector< PPU466::Sprite > sprites;

	/*
	* -------------------------------------
	* -------------Palettes----------------
	* -------------------------------------
	*/

	// Standard RGB (the goal)
	try {
		load_png(data_path("../assets/RGB.png"), &palette_size, &palette_data, OriginLocation::UpperLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(palette_size.x == 4 && palette_size.y == 1);

	palettes.push_back({
		glm::u8vec4(0,0,0,0),
		glm::u8vec4(palette_data[1][0],palette_data[1][1],palette_data[1][2],255),
		glm::u8vec4(palette_data[2][0],palette_data[2][1],palette_data[2][2],255),
		glm::u8vec4(palette_data[3][0],palette_data[3][1],palette_data[3][2],255),
	});

	// The player - gray, yellow, blue
	try {
		load_png(data_path("../assets/PlayerPalette.png"), &palette_size, &palette_data, OriginLocation::UpperLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(palette_size.x == 4 && palette_size.y == 1);

	palettes.push_back({
		glm::u8vec4(0,0,0,0),
		glm::u8vec4(palette_data[1][0],palette_data[1][1],palette_data[1][2],255),
		glm::u8vec4(palette_data[2][0],palette_data[2][1],palette_data[2][2],255),
		glm::u8vec4(palette_data[3][0],palette_data[3][1],palette_data[3][2],255),
	});

	// Brick 1 - brown & gray
	try {
		load_png(data_path("../assets/Brick1.png"), &palette_size, &palette_data, OriginLocation::UpperLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(palette_size.x == 4 && palette_size.y == 1);

	palettes.push_back({
		glm::u8vec4(0,0,0,0),
		glm::u8vec4(palette_data[1][0],palette_data[1][1],palette_data[1][2],255),
		glm::u8vec4(palette_data[2][0],palette_data[2][1],palette_data[2][2],255),
		glm::u8vec4(palette_data[3][0],palette_data[3][1],palette_data[3][2],255),
	});

	// Brick 2 - gray & green
	try {
		load_png(data_path("../assets/Brick2.png"), &palette_size, &palette_data, OriginLocation::UpperLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(palette_size.x == 4 && palette_size.y == 1);

	palettes.push_back({
		glm::u8vec4(0,0,0,0),
		glm::u8vec4(palette_data[1][0],palette_data[1][1],palette_data[1][2],255),
		glm::u8vec4(palette_data[2][0],palette_data[2][1],palette_data[2][2],255),
		glm::u8vec4(palette_data[3][0],palette_data[3][1],palette_data[3][2],255),
	});

	// Black and white palette
	palettes.push_back({
		glm::u8vec4(0,0,0,0),
		glm::u8vec4(0,0,0,255),
		glm::u8vec4(255,255,255,255),
		glm::u8vec4(255,255,255,0),
	});

	/*
	* -------------------------------------
	* ---------------Tiles-----------------
	* -------------------------------------
	*/
	std::array<uint8_t, 8> bit0 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	std::array<uint8_t, 8> bit1 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	
	// Empty first tile
	tiles.push_back({ bit0, bit1 });

	// Player tile
	try {
		load_png(data_path("../assets/Player.png"), &palette_size, &palette_data, OriginLocation::LowerLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(palette_size.x == 8 && palette_size.y == 8);

	for (uint32_t row = 0; row < palette_size.y; row++) {
		for (uint32_t col = 0; col < palette_size.x; col++) {
			glm::u8vec4 color = palette_data[row * palette_size.x + col];
			if (color[0]) {
				bit0[row] |= (1 << col);
			}
			else if (color[1]) {
				bit1[row] |= (1 << col);
			}
			else if (color[2]) {
				bit0[row] |= (1 << col);
				bit1[row] |= (1 << col);
			}
		}
	}
	tiles.push_back({ bit0, bit1 });

	// Goal tile
	bit0 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	bit1 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	try {
		load_png(data_path("../assets/Goal.png"), &palette_size, &palette_data, OriginLocation::LowerLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(palette_size.x == 8 && palette_size.y == 8);

	for (uint32_t row = 0; row < palette_size.y; row++) {
		for (uint32_t col = 0; col < palette_size.x; col++) {
			glm::u8vec4 color = palette_data[row * palette_size.x + col];
			if (color[0]) {
				bit0[row] |= (1 << col);
			}
			else if (color[1]) {
				bit1[row] |= (1 << col);
			}
			else if (color[2]) {
				bit0[row] |= (1 << col);
				bit1[row] |= (1 << col);
			}
		}
	}
	tiles.push_back({ bit0, bit1 });

	// Brick tile
	bit0 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	bit1 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	try {
		load_png(data_path("../assets/Brick.png"), &palette_size, &palette_data, OriginLocation::LowerLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(palette_size.x == 8 && palette_size.y == 8);
	for (uint32_t row = 0; row < palette_size.y; row++) {
		for (uint32_t col = 0; col < palette_size.x; col++) {
			glm::u8vec4 color = palette_data[row * palette_size.x + col];
			if (color[0]) {
				bit0[row] |= (1 << col);
			}
			else if (color[1]) {
				bit1[row] |= (1 << col);
			}
			else if (color[2]) {
				bit0[row] |= (1 << col);
				bit1[row] |= (1 << col);
			}
		}
	}
	tiles.push_back({ bit0, bit1 });

	// Black and white tiles
	bit0 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	bit1 = { 255, 255, 255, 255, 255, 255, 255, 255 };
	tiles.push_back({ bit0, bit1 });

	// Other player directions
	bit0 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	bit1 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	try {
		load_png(data_path("../assets/PlayerDown.png"), &palette_size, &palette_data, OriginLocation::LowerLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(palette_size.x == 8 && palette_size.y == 8);

	for (uint32_t row = 0; row < palette_size.y; row++) {
		for (uint32_t col = 0; col < palette_size.x; col++) {
			glm::u8vec4 color = palette_data[row * palette_size.x + col];
			if (color[0]) {
				bit0[row] |= (1 << col);
			}
			else if (color[1]) {
				bit1[row] |= (1 << col);
			}
			else if (color[2]) {
				bit0[row] |= (1 << col);
				bit1[row] |= (1 << col);
			}
		}
	}
	tiles.push_back({ bit0, bit1 });

	bit0 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	bit1 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	try {
		load_png(data_path("../assets/PlayerLeft.png"), &palette_size, &palette_data, OriginLocation::LowerLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(palette_size.x == 8 && palette_size.y == 8);

	for (uint32_t row = 0; row < palette_size.y; row++) {
		for (uint32_t col = 0; col < palette_size.x; col++) {
			glm::u8vec4 color = palette_data[row * palette_size.x + col];
			if (color[0]) {
				bit0[row] |= (1 << col);
			}
			else if (color[1]) {
				bit1[row] |= (1 << col);
			}
			else if (color[2]) {
				bit0[row] |= (1 << col);
				bit1[row] |= (1 << col);
			}
		}
	}
	tiles.push_back({ bit0, bit1 });

	bit0 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	bit1 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	try {
		load_png(data_path("../assets/PlayerRight.png"), &palette_size, &palette_data, OriginLocation::LowerLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(palette_size.x == 8 && palette_size.y == 8);

	for (uint32_t row = 0; row < palette_size.y; row++) {
		for (uint32_t col = 0; col < palette_size.x; col++) {
			glm::u8vec4 color = palette_data[row * palette_size.x + col];
			if (color[0]) {
				bit0[row] |= (1 << col);
			}
			else if (color[1]) {
				bit1[row] |= (1 << col);
			}
			else if (color[2]) {
				bit0[row] |= (1 << col);
				bit1[row] |= (1 << col);
			}
		}
	}
	tiles.push_back({ bit0, bit1 });

	/*
	* -------------------------------------
	* --------------Sprites----------------
	* -------------------------------------
	*/

	// Player & Goal
	sprites.push_back({ 0, 0, 1, 0b1000001 });
	sprites.push_back({ 0, 0, 2, 0b1000000 });

	/*
	* -------------------------------------
	* ---------------Saving----------------
	* -------------------------------------
	*/

	assert(palettes.size() == 5);
	assert(tiles.size() == 8);
	assert(sprites.size() == 2);

	// Learned how to create an ofstream from https://stackoverflow.com/questions/52803734/c-ostream-not-creating-a-new-file
	std::ofstream datafile(data_path("RuntimeAssets.bin"), std::ios::out);

	write_chunk("PALT", palettes, &datafile);
	write_chunk("TILE", tiles, &datafile);
	write_chunk("SPRT", sprites, &datafile);
}