#include "PPU466.hpp"
#include "Mode.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----
	glm::vec2 position = glm::vec2(24, 24);
	glm::vec2 goal_position = glm::vec2(224, 208);
	float goal_timer = 5.0f;
	float fail_timer = 30.0f;
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
	bool victory = false;
	bool failure = false;
	float speed = 0.0f;
	uint32_t level = 3;

	//----- drawing handled by PPU466 -----
	PPU466 ppu;
};
