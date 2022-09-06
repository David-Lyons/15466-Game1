#include "PlayMode.hpp"

//for the GL_ERRORS() macro:
#include "gl_errors.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>

#include <random>
#include <time.h>
#include <fstream>

#include "data_path.hpp"
#include "load_save_png.hpp"
#include "read_write_chunk.hpp"

struct surface {
	glm::vec2 bottomleft;
	uint32_t width;
	uint32_t height;
};

std::vector<surface> blocks;

PlayMode::PlayMode() {
	// Set up random seed based on current time so that rand() returns different value every game
	// Based on https://stackoverflow.com/questions/28656004/c-random-doesnt-workreturns-same-value-always
	srand((unsigned int)time(NULL));

	// Background sky blue
	ppu.background_color = glm::u8vec3(150, 200, 250);

	// Blank out all palettes, tiles, background tiles, and sprites
	for (uint32_t i = 0; i < 8; i++) {
		ppu.palette_table[i] = { glm::u8vec4(0,0,0,0), glm::u8vec4(0,0,0,0),
			glm::u8vec4(0,0,0,0), glm::u8vec4(0,0,0,0) };
	}
	for (uint32_t i = 0; i < 256; i++) {
		ppu.tile_table[i] = { { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 } };
	}
	for (uint32_t i = 0; i < PPU466::BackgroundWidth * PPU466::BackgroundHeight; i++) {
		ppu.background[i] = 0;
	}
	for (uint32_t i = 0; i < 64; i++) {
		ppu.sprites[i] = { 0, 250, 0, 0 };
	}

	// Load runtime assets into PPU
	std::ifstream datafile(data_path("RuntimeAssets.bin"), std::ios::in);
	std::vector< PPU466::Palette > palettes;
	std::vector< PPU466::Tile > tiles;
	std::vector< PPU466::Sprite > sprites;
	read_chunk(datafile, "PALT", &palettes);
	read_chunk(datafile, "TILE", &tiles);
	read_chunk(datafile, "SPRT", &sprites);
	for (uint32_t i = 0; i < palettes.size(); i++) {
		ppu.palette_table[i] = palettes[i];
	}
	for (uint32_t i = 0; i < tiles.size(); i++) {
		ppu.tile_table[i] = tiles[i];
	}
	for (uint32_t i = 0; i < sprites.size(); i++) {
		ppu.sprites[i] = sprites[i];
	}

	// Create maze
	for (uint32_t i = 0; i < 32; i++) {
		ppu.background[i] = ((2 + i % 2) << 8) + 3;
	}
	for (uint32_t i = 0; i < 32; i++) {
		ppu.background[PPU466::BackgroundWidth + i] = ((3 - i % 2) << 8) + 3;
	}
	for (uint32_t i = 0; i < 30; i++) {
		ppu.background[PPU466::BackgroundWidth * i] = ((2 + i % 2) << 8) + 3;
	}
	for (uint32_t i = 0; i < 30; i++) {
		ppu.background[PPU466::BackgroundWidth * i + 1] = ((3 - i % 2) << 8) + 3;
	}
	for (uint32_t i = 0; i < 32; i++) {
		ppu.background[PPU466::BackgroundWidth * 28 + i] = ((2 + i % 2) << 8) + 3;
	}
	for (uint32_t i = 0; i < 32; i++) {
		ppu.background[PPU466::BackgroundWidth * 29 + i] = ((3 - i % 2) << 8) + 3;
	}
	for (uint32_t i = 0; i < 30; i++) {
		ppu.background[PPU466::BackgroundWidth * i + 30] = ((2 + i % 2) << 8) + 3;
	}
	for (uint32_t i = 0; i < 30; i++) {
		ppu.background[PPU466::BackgroundWidth * i + 31] = ((3 - i % 2) << 8) + 3;
	}
	ppu.background[5 * PPU466::BackgroundWidth + 2] = (3 << 8) + 3;
	ppu.background[5 * PPU466::BackgroundWidth + 3] = (2 << 8) + 3;
	ppu.background[5 * PPU466::BackgroundWidth + 4] = (3 << 8) + 3;
	ppu.background[5 * PPU466::BackgroundWidth + 5] = (2 << 8) + 3;
	ppu.background[5 * PPU466::BackgroundWidth + 9] = (3 << 8) + 3;
	ppu.background[6 * PPU466::BackgroundWidth + 9] = (2 << 8) + 3;
	ppu.background[7 * PPU466::BackgroundWidth + 9] = (3 << 8) + 3;
	ppu.background[8 * PPU466::BackgroundWidth + 9] = (2 << 8) + 3;
	ppu.background[9 * PPU466::BackgroundWidth + 9] = (3 << 8) + 3;
	ppu.background[9 * PPU466::BackgroundWidth + 8] = (2 << 8) + 3;
	ppu.background[9 * PPU466::BackgroundWidth + 7] = (3 << 8) + 3;
	ppu.background[9 * PPU466::BackgroundWidth + 6] = (2 << 8) + 3;
	ppu.background[9 * PPU466::BackgroundWidth + 5] = (3 << 8) + 3;
	ppu.background[10 * PPU466::BackgroundWidth + 9] = (2 << 8) + 3;
	ppu.background[11 * PPU466::BackgroundWidth + 9] = (3 << 8) + 3;
	ppu.background[12 * PPU466::BackgroundWidth + 9] = (2 << 8) + 3;
	ppu.background[13 * PPU466::BackgroundWidth + 9] = (3 << 8) + 3;
	ppu.background[13 * PPU466::BackgroundWidth + 10] = (2 << 8) + 3;
	ppu.background[13 * PPU466::BackgroundWidth + 11] = (3 << 8) + 3;
	ppu.background[13 * PPU466::BackgroundWidth + 12] = (2 << 8) + 3;
	ppu.background[13 * PPU466::BackgroundWidth + 13] = (3 << 8) + 3;
	ppu.background[13 * PPU466::BackgroundWidth + 14] = (2 << 8) + 3;
	ppu.background[13 * PPU466::BackgroundWidth + 15] = (3 << 8) + 3;
	ppu.background[13 * PPU466::BackgroundWidth + 16] = (2 << 8) + 3;
	ppu.background[13 * PPU466::BackgroundWidth + 17] = (3 << 8) + 3;
	ppu.background[2 * PPU466::BackgroundWidth + 13] = (3 << 8) + 3;
	ppu.background[3 * PPU466::BackgroundWidth + 13] = (2 << 8) + 3;
	ppu.background[4 * PPU466::BackgroundWidth + 13] = (3 << 8) + 3;
	ppu.background[5 * PPU466::BackgroundWidth + 13] = (2 << 8) + 3;
	ppu.background[6 * PPU466::BackgroundWidth + 13] = (3 << 8) + 3;
	ppu.background[7 * PPU466::BackgroundWidth + 13] = (2 << 8) + 3;
	ppu.background[8 * PPU466::BackgroundWidth + 13] = (3 << 8) + 3;
	ppu.background[9 * PPU466::BackgroundWidth + 13] = (2 << 8) + 3;
	ppu.background[13 * PPU466::BackgroundWidth + 2] = (3 << 8) + 3;
	ppu.background[13 * PPU466::BackgroundWidth + 3] = (2 << 8) + 3;
	ppu.background[13 * PPU466::BackgroundWidth + 4] = (3 << 8) + 3;
	ppu.background[13 * PPU466::BackgroundWidth + 5] = (2 << 8) + 3;
	ppu.background[12 * PPU466::BackgroundWidth + 17] = (2 << 8) + 3;
	ppu.background[11 * PPU466::BackgroundWidth + 17] = (3 << 8) + 3;
	ppu.background[10 * PPU466::BackgroundWidth + 17] = (2 << 8) + 3;
	ppu.background[9 * PPU466::BackgroundWidth + 17] = (3 << 8) + 3;
	ppu.background[5 * PPU466::BackgroundWidth + 17] = (3 << 8) + 3;
	ppu.background[5 * PPU466::BackgroundWidth + 18] = (2 << 8) + 3;
	ppu.background[5 * PPU466::BackgroundWidth + 19] = (3 << 8) + 3;
	ppu.background[5 * PPU466::BackgroundWidth + 20] = (2 << 8) + 3;
	ppu.background[5 * PPU466::BackgroundWidth + 21] = (3 << 8) + 3;
	ppu.background[6 * PPU466::BackgroundWidth + 21] = (2 << 8) + 3;
	ppu.background[7 * PPU466::BackgroundWidth + 21] = (3 << 8) + 3;
	ppu.background[8 * PPU466::BackgroundWidth + 21] = (2 << 8) + 3;
	ppu.background[9 * PPU466::BackgroundWidth + 21] = (3 << 8) + 3;
	ppu.background[10 * PPU466::BackgroundWidth + 21] = (2 << 8) + 3;
	ppu.background[11 * PPU466::BackgroundWidth + 21] = (3 << 8) + 3;
	ppu.background[12 * PPU466::BackgroundWidth + 21] = (2 << 8) + 3;
	ppu.background[13 * PPU466::BackgroundWidth + 21] = (3 << 8) + 3;
	ppu.background[9 * PPU466::BackgroundWidth + 22] = (2 << 8) + 3;
	ppu.background[9 * PPU466::BackgroundWidth + 23] = (3 << 8) + 3;
	ppu.background[9 * PPU466::BackgroundWidth + 24] = (2 << 8) + 3;
	ppu.background[9 * PPU466::BackgroundWidth + 25] = (3 << 8) + 3;
	ppu.background[8 * PPU466::BackgroundWidth + 25] = (2 << 8) + 3;
	ppu.background[7 * PPU466::BackgroundWidth + 25] = (3 << 8) + 3;
	ppu.background[6 * PPU466::BackgroundWidth + 25] = (2 << 8) + 3;
	ppu.background[5 * PPU466::BackgroundWidth + 25] = (3 << 8) + 3;
	ppu.background[14 * PPU466::BackgroundWidth + 5] = (3 << 8) + 3;
	ppu.background[15 * PPU466::BackgroundWidth + 5] = (2 << 8) + 3;
	ppu.background[16 * PPU466::BackgroundWidth + 5] = (3 << 8) + 3;
	ppu.background[17 * PPU466::BackgroundWidth + 5] = (2 << 8) + 3;
	ppu.background[21 * PPU466::BackgroundWidth + 5] = (2 << 8) + 3;
	ppu.background[22 * PPU466::BackgroundWidth + 5] = (3 << 8) + 3;
	ppu.background[23 * PPU466::BackgroundWidth + 5] = (2 << 8) + 3;
	ppu.background[24 * PPU466::BackgroundWidth + 5] = (3 << 8) + 3;
	ppu.background[24 * PPU466::BackgroundWidth + 6] = (2 << 8) + 3;
	ppu.background[24 * PPU466::BackgroundWidth + 7] = (3 << 8) + 3;
	ppu.background[24 * PPU466::BackgroundWidth + 8] = (2 << 8) + 3;
	ppu.background[24 * PPU466::BackgroundWidth + 9] = (3 << 8) + 3;
	ppu.background[25 * PPU466::BackgroundWidth + 9] = (2 << 8) + 3;
	ppu.background[26 * PPU466::BackgroundWidth + 9] = (3 << 8) + 3;
	ppu.background[27 * PPU466::BackgroundWidth + 9] = (2 << 8) + 3;
	ppu.background[17 * PPU466::BackgroundWidth + 6] = (3 << 8) + 3;
	ppu.background[17 * PPU466::BackgroundWidth + 7] = (2 << 8) + 3;
	ppu.background[17 * PPU466::BackgroundWidth + 8] = (3 << 8) + 3;
	ppu.background[17 * PPU466::BackgroundWidth + 9] = (2 << 8) + 3;
	ppu.background[17 * PPU466::BackgroundWidth + 10] = (3 << 8) + 3;
	ppu.background[17 * PPU466::BackgroundWidth + 11] = (2 << 8) + 3;
	ppu.background[17 * PPU466::BackgroundWidth + 12] = (3 << 8) + 3;
	ppu.background[17 * PPU466::BackgroundWidth + 13] = (2 << 8) + 3;
	ppu.background[18 * PPU466::BackgroundWidth + 9] = (3 << 8) + 3;
	ppu.background[19 * PPU466::BackgroundWidth + 9] = (2 << 8) + 3;
	ppu.background[20 * PPU466::BackgroundWidth + 9] = (3 << 8) + 3;
	ppu.background[21 * PPU466::BackgroundWidth + 13] = (2 << 8) + 3;
	ppu.background[22 * PPU466::BackgroundWidth + 13] = (3 << 8) + 3;
	ppu.background[23 * PPU466::BackgroundWidth + 13] = (2 << 8) + 3;
	ppu.background[24 * PPU466::BackgroundWidth + 13] = (3 << 8) + 3;
	ppu.background[14 * PPU466::BackgroundWidth + 17] = (2 << 8) + 3;
	ppu.background[15 * PPU466::BackgroundWidth + 17] = (3 << 8) + 3;
	ppu.background[16 * PPU466::BackgroundWidth + 17] = (2 << 8) + 3;
	ppu.background[17 * PPU466::BackgroundWidth + 17] = (3 << 8) + 3;
	ppu.background[18 * PPU466::BackgroundWidth + 17] = (2 << 8) + 3;
	ppu.background[19 * PPU466::BackgroundWidth + 17] = (3 << 8) + 3;
	ppu.background[20 * PPU466::BackgroundWidth + 17] = (2 << 8) + 3;
	ppu.background[17 * PPU466::BackgroundWidth + 18] = (2 << 8) + 3;
	ppu.background[17 * PPU466::BackgroundWidth + 19] = (3 << 8) + 3;
	ppu.background[17 * PPU466::BackgroundWidth + 20] = (2 << 8) + 3;
	ppu.background[17 * PPU466::BackgroundWidth + 21] = (3 << 8) + 3;
	ppu.background[24 * PPU466::BackgroundWidth + 14] = (2 << 8) + 3;
	ppu.background[24 * PPU466::BackgroundWidth + 15] = (3 << 8) + 3;
	ppu.background[24 * PPU466::BackgroundWidth + 16] = (2 << 8) + 3;
	ppu.background[24 * PPU466::BackgroundWidth + 17] = (3 << 8) + 3;
	ppu.background[24 * PPU466::BackgroundWidth + 18] = (2 << 8) + 3;
	ppu.background[24 * PPU466::BackgroundWidth + 19] = (3 << 8) + 3;
	ppu.background[24 * PPU466::BackgroundWidth + 20] = (2 << 8) + 3;
	ppu.background[24 * PPU466::BackgroundWidth + 21] = (3 << 8) + 3;
	ppu.background[27 * PPU466::BackgroundWidth + 25] = (2 << 8) + 3;
	ppu.background[26 * PPU466::BackgroundWidth + 25] = (3 << 8) + 3;
	ppu.background[25 * PPU466::BackgroundWidth + 25] = (2 << 8) + 3;
	ppu.background[24 * PPU466::BackgroundWidth + 25] = (3 << 8) + 3;
	ppu.background[23 * PPU466::BackgroundWidth + 25] = (2 << 8) + 3;
	ppu.background[22 * PPU466::BackgroundWidth + 25] = (3 << 8) + 3;
	ppu.background[21 * PPU466::BackgroundWidth + 25] = (2 << 8) + 3;
	ppu.background[20 * PPU466::BackgroundWidth + 25] = (3 << 8) + 3;
	ppu.background[20 * PPU466::BackgroundWidth + 24] = (2 << 8) + 3;
	ppu.background[20 * PPU466::BackgroundWidth + 23] = (3 << 8) + 3;
	ppu.background[13 * PPU466::BackgroundWidth + 29] = (2 << 8) + 3;
	ppu.background[13 * PPU466::BackgroundWidth + 28] = (3 << 8) + 3;
	ppu.background[13 * PPU466::BackgroundWidth + 27] = (2 << 8) + 3;
	ppu.background[13 * PPU466::BackgroundWidth + 26] = (3 << 8) + 3;
	ppu.background[13 * PPU466::BackgroundWidth + 25] = (2 << 8) + 3;
	ppu.background[14 * PPU466::BackgroundWidth + 25] = (3 << 8) + 3;
	ppu.background[15 * PPU466::BackgroundWidth + 25] = (2 << 8) + 3;

	// Collision Detection
	blocks.push_back({ glm::vec2(0, 0), 256, 16 });
	blocks.push_back({ glm::vec2(0, 224), 256, 16 });
	blocks.push_back({ glm::vec2(0, 0), 16, 240 });
	blocks.push_back({ glm::vec2(240, 0), 16, 240 });

	blocks.push_back({ glm::vec2(72, 40), 8, 72 });
	blocks.push_back({ glm::vec2(40, 72), 32, 8 });
	blocks.push_back({ glm::vec2(80, 104), 64, 8 });
	blocks.push_back({ glm::vec2(136, 72), 8, 96 });
	blocks.push_back({ glm::vec2(144, 136), 32, 8 });

	blocks.push_back({ glm::vec2(104, 16), 8, 64 });
	blocks.push_back({ glm::vec2(136, 40), 40, 8 });
	blocks.push_back({ glm::vec2(168, 48), 8, 64 });
	blocks.push_back({ glm::vec2(176, 72), 32, 8 });
	blocks.push_back({ glm::vec2(200, 40), 8, 32 });

	blocks.push_back({ glm::vec2(200, 104), 40, 8 });
	blocks.push_back({ glm::vec2(200, 112), 8, 16 });
	blocks.push_back({ glm::vec2(200, 160), 8, 64 });
	blocks.push_back({ glm::vec2(184, 160), 16, 8 });

	blocks.push_back({ glm::vec2(40, 168), 8, 32 });
	blocks.push_back({ glm::vec2(48, 192), 32, 8 });
	blocks.push_back({ glm::vec2(72, 200), 8, 24 });
	blocks.push_back({ glm::vec2(104, 168), 8, 32 });
	blocks.push_back({ glm::vec2(112, 192), 64, 8 });

	blocks.push_back({ glm::vec2(16, 40), 32, 8 });
	blocks.push_back({ glm::vec2(16, 104), 32, 8 });
	blocks.push_back({ glm::vec2(40, 112), 8, 32 });
	blocks.push_back({ glm::vec2(48, 136), 64, 8 });
	blocks.push_back({ glm::vec2(72, 144), 8, 24 });
}

PlayMode::~PlayMode() {
}

float tryMoveLeft(float goal, glm::vec2 position) {
	for (struct surface block : blocks) {
		if (position.y > block.bottomleft.y - 8.0f && position.y < block.bottomleft.y + block.height) {
			float dist = position.x - (block.bottomleft.x + block.width);
			if (dist >= 0 && dist <= goal) {
				return dist;
			}
		}
	}
	return goal;
}

float tryMoveRight(float goal, glm::vec2 position) {
	for (struct surface block : blocks) {
		if (position.y > block.bottomleft.y - 8.0f && position.y < block.bottomleft.y + block.height) {
			float dist = (block.bottomleft.x - 8.0f) - position.x;
			if (dist >= 0 && dist <= goal) {
				return dist;
			}
		}
	}
	return goal;
}

float tryMoveDown(float goal, glm::vec2 position) {
	for (struct surface block : blocks) {
		if (position.x > block.bottomleft.x - 8.0f && position.x < block.bottomleft.x + block.width) {
			float dist = position.y - (block.bottomleft.y + block.height);
			if (dist >= 0 && dist <= goal) {
				return dist;
			}
		}
	}
	return goal;
}

float tryMoveUp(float goal, glm::vec2 position) {
	for (struct surface block : blocks) {
		if (position.x > block.bottomleft.x - 8.0f && position.x < block.bottomleft.x + block.width) {
			float dist = (block.bottomleft.y - 8.0f) - position.y;
			if (dist >= 0 && dist <= goal) {
				return dist;
			}
		}
	}
	return goal;
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			if (!right) {
				left = true;
			}
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			if (!left) {
				right = true;
			}
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			if (!down) {
				up = true;
			}
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			if (!up) {
				down = true;
			}
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down = false;
			return true;
		}
	}

	return false;
}

bool detectVictory(glm::vec2 goal, glm::vec2 position) {
	return (position.x - goal.x <= 8 && position.x - goal.x >= -8)
		&& (position.y - goal.y <= 8 && position.y - goal.y >= -8);
}

void PlayMode::update(float elapsed) {
	if (!failure && detectVictory(goal_position, position)) {
		victory = true;
		return;
	}
	fail_timer -= elapsed;
	if (!victory && fail_timer <= 0) {
		failure = true;
	}
	goal_timer -= elapsed;
	if (goal_timer <= 0) {
		goal_timer = 3.0f;
		goal_position.x = 24.0f + (float)(rand() % 7) * 32.0f;
		goal_position.y = 24.0f + (float)(rand() % 7) * 32.0f;
	}
	if (left) {
		position.x -= tryMoveLeft(60.0f * elapsed, position);
		ppu.sprites[0].index = 6;
	}
	if (right) {
		position.x += tryMoveRight(60.0f * elapsed, position);
		ppu.sprites[0].index = 7;
	}
	if (up) {
		position.y += tryMoveUp(60.0f * elapsed, position);
		ppu.sprites[0].index = 1;
	}
	if (down) {
		position.y -= tryMoveDown(60.0f * elapsed, position);
		ppu.sprites[0].index = 5;
	}
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//--- set ppu state based on game state ---
	if (victory) {
		// We won the game. Print "Win!"
		ppu.background_color = { 0, 0, 0 };
		for (uint32_t i = 0; i < PPU466::BackgroundWidth * PPU466::BackgroundHeight; i++) {
			ppu.background[i] = 0;
		}
		ppu.background[17 * 64 + 3] = (4 << 8) + 4;
		ppu.background[16 * 64 + 3] = (4 << 8) + 4;
		ppu.background[15 * 64 + 3] = (4 << 8) + 4;
		ppu.background[14 * 64 + 4] = (4 << 8) + 4;
		ppu.background[13 * 64 + 5] = (4 << 8) + 4;
		ppu.background[14 * 64 + 6] = (4 << 8) + 4;
		ppu.background[15 * 64 + 7] = (4 << 8) + 4;
		ppu.background[16 * 64 + 7] = (4 << 8) + 4;
		ppu.background[17 * 64 + 7] = (4 << 8) + 4;
		ppu.background[14 * 64 + 8] = (4 << 8) + 4;
		ppu.background[13 * 64 + 9] = (4 << 8) + 4;
		ppu.background[14 * 64 + 10] = (4 << 8) + 4;
		ppu.background[15 * 64 + 11] = (4 << 8) + 4;
		ppu.background[16 * 64 + 11] = (4 << 8) + 4;
		ppu.background[17 * 64 + 11] = (4 << 8) + 4;

		ppu.background[17 * 64 + 13] = (4 << 8) + 4;
		ppu.background[17 * 64 + 14] = (4 << 8) + 4;
		ppu.background[17 * 64 + 15] = (4 << 8) + 4;
		ppu.background[17 * 64 + 16] = (4 << 8) + 4;
		ppu.background[17 * 64 + 17] = (4 << 8) + 4;
		ppu.background[13 * 64 + 13] = (4 << 8) + 4;
		ppu.background[13 * 64 + 14] = (4 << 8) + 4;
		ppu.background[13 * 64 + 15] = (4 << 8) + 4;
		ppu.background[13 * 64 + 16] = (4 << 8) + 4;
		ppu.background[13 * 64 + 17] = (4 << 8) + 4;
		ppu.background[16 * 64 + 15] = (4 << 8) + 4;
		ppu.background[15 * 64 + 15] = (4 << 8) + 4;
		ppu.background[14 * 64 + 15] = (4 << 8) + 4;


		ppu.background[13 * 64 + 19] = (4 << 8) + 4;
		ppu.background[14 * 64 + 19] = (4 << 8) + 4;
		ppu.background[15 * 64 + 19] = (4 << 8) + 4;
		ppu.background[16 * 64 + 19] = (4 << 8) + 4;
		ppu.background[17 * 64 + 19] = (4 << 8) + 4;
		ppu.background[17 * 64 + 20] = (4 << 8) + 4;
		ppu.background[16 * 64 + 21] = (4 << 8) + 4;
		ppu.background[15 * 64 + 22] = (4 << 8) + 4;
		ppu.background[14 * 64 + 23] = (4 << 8) + 4;
		ppu.background[13 * 64 + 24] = (4 << 8) + 4;
		ppu.background[13 * 64 + 25] = (4 << 8) + 4;
		ppu.background[14 * 64 + 25] = (4 << 8) + 4;
		ppu.background[15 * 64 + 25] = (4 << 8) + 4;
		ppu.background[16 * 64 + 25] = (4 << 8) + 4;
		ppu.background[16 * 64 + 25] = (4 << 8) + 4;
		ppu.background[17 * 64 + 25] = (4 << 8) + 4;

		ppu.background[13 * 64 + 27] = (4 << 8) + 4;
		ppu.background[15 * 64 + 27] = (4 << 8) + 4;
		ppu.background[16 * 64 + 27] = (4 << 8) + 4;
		ppu.background[16 * 64 + 27] = (4 << 8) + 4;
		ppu.background[17 * 64 + 27] = (4 << 8) + 4;

		ppu.sprites[0] = { 0, 250, 0, 0 };
		ppu.sprites[1] = { 0, 250, 0, 0 };
		ppu.draw(drawable_size);
		return;
	} else if (failure) {
		// We lost the game. Print "Lose..."
		ppu.background_color = { 0, 0, 0 };
		for (uint32_t i = 0; i < PPU466::BackgroundWidth * PPU466::BackgroundHeight; i++) {
			ppu.background[i] = 0;
		}
		ppu.background[17 * 64 + 3] = (4 << 8) + 4;
		ppu.background[16 * 64 + 3] = (4 << 8) + 4;
		ppu.background[15 * 64 + 3] = (4 << 8) + 4;
		ppu.background[14 * 64 + 3] = (4 << 8) + 4;
		ppu.background[13 * 64 + 3] = (4 << 8) + 4;
		ppu.background[13 * 64 + 4] = (4 << 8) + 4;
		ppu.background[13 * 64 + 5] = (4 << 8) + 4;
		ppu.background[13 * 64 + 6] = (4 << 8) + 4;

		ppu.background[13 * 64 + 8] = (4 << 8) + 4;
		ppu.background[14 * 64 + 8] = (4 << 8) + 4;
		ppu.background[15 * 64 + 8] = (4 << 8) + 4;
		ppu.background[16 * 64 + 8] = (4 << 8) + 4;
		ppu.background[17 * 64 + 8] = (4 << 8) + 4;
		ppu.background[13 * 64 + 9] = (4 << 8) + 4;
		ppu.background[17 * 64 + 9] = (4 << 8) + 4;
		ppu.background[13 * 64 + 10] = (4 << 8) + 4;
		ppu.background[17 * 64 + 10] = (4 << 8) + 4;
		ppu.background[13 * 64 + 11] = (4 << 8) + 4;
		ppu.background[14 * 64 + 11] = (4 << 8) + 4;
		ppu.background[15 * 64 + 11] = (4 << 8) + 4;
		ppu.background[16 * 64 + 11] = (4 << 8) + 4;
		ppu.background[17 * 64 + 11] = (4 << 8) + 4;

		ppu.background[17 * 64 + 13] = (4 << 8) + 4;
		ppu.background[17 * 64 + 14] = (4 << 8) + 4;
		ppu.background[17 * 64 + 15] = (4 << 8) + 4;
		ppu.background[17 * 64 + 16] = (4 << 8) + 4;
		ppu.background[16 * 64 + 13] = (4 << 8) + 4;
		ppu.background[15 * 64 + 13] = (4 << 8) + 4;
		ppu.background[15 * 64 + 14] = (4 << 8) + 4;
		ppu.background[15 * 64 + 15] = (4 << 8) + 4;
		ppu.background[15 * 64 + 16] = (4 << 8) + 4;
		ppu.background[14 * 64 + 16] = (4 << 8) + 4;
		ppu.background[13 * 64 + 16] = (4 << 8) + 4;
		ppu.background[13 * 64 + 15] = (4 << 8) + 4;
		ppu.background[13 * 64 + 14] = (4 << 8) + 4;
		ppu.background[13 * 64 + 13] = (4 << 8) + 4;

		ppu.background[13 * 64 + 18] = (4 << 8) + 4;
		ppu.background[13 * 64 + 19] = (4 << 8) + 4;
		ppu.background[13 * 64 + 20] = (4 << 8) + 4;
		ppu.background[13 * 64 + 21] = (4 << 8) + 4;
		ppu.background[14 * 64 + 18] = (4 << 8) + 4;
		ppu.background[15 * 64 + 18] = (4 << 8) + 4;
		ppu.background[16 * 64 + 18] = (4 << 8) + 4;
		ppu.background[17 * 64 + 18] = (4 << 8) + 4;
		ppu.background[15 * 64 + 19] = (4 << 8) + 4;
		ppu.background[15 * 64 + 20] = (4 << 8) + 4;
		ppu.background[17 * 64 + 19] = (4 << 8) + 4;
		ppu.background[17 * 64 + 20] = (4 << 8) + 4;
		ppu.background[17 * 64 + 21] = (4 << 8) + 4;
		ppu.background[13 * 64 + 23] = (4 << 8) + 4;
		ppu.background[13 * 64 + 25] = (4 << 8) + 4;
		ppu.background[13 * 64 + 27] = (4 << 8) + 4;

		ppu.sprites[0].y = 250;
		ppu.sprites[1].y = 250;
		ppu.draw(drawable_size);
		return;
	}

	//player sprite:
	ppu.sprites[0].x = int8_t(position.x);
	ppu.sprites[0].y = int8_t(position.y);

	//goal sprite:
	ppu.sprites[1].x = int8_t(goal_position.x);
	ppu.sprites[1].y = int8_t(goal_position.y);

	//--- actually draw ---
	ppu.draw(drawable_size);
}
