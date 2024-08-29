#pragma once

#include"vector2.h"
#include"atlas.h"
#include"util.h"
#include"camera.h"

class Particle {
public:
	Particle() = default;
	Particle(const Vector2& position, Atlas* atlas, int lifespan) 
		:position(position), atlas(atlas), lifespan(lifespan) { }
	~Particle() = default;

	void set_atlas(Atlas* new_atlas) {
		atlas = new_atlas;
	}

	void set_position(const Vector2& pos) {
		position = pos;
	}

	void set_lifespan(int ms) {
		lifespan = ms;
	}

	bool check_valid() const{
		return valid;
	}

	void on_update(int delta) {
		timer += delta;
		if (timer >= lifespan) {
			timer = 0;
			idx_frame++;
			if (idx_frame >= atlas->get_size()) {
				idx_frame = atlas->get_size() - 1;
				valid = false;
			}
		}
	}

	void on_draw(const Camera& camera) {
		putimage_alpha(camera, (int)position.x, (int)position.y, atlas->get_image(idx_frame));
	}

private:
	int timer = 0;                //粒子播放定时器
	int lifespan = 0;			  //粒子生命周期
	int idx_frame = 0;			  //当前帧
	Vector2 position;			  //粒子世界坐标位置
	bool valid = true;			  //是否有效
	Atlas* atlas = nullptr;		  //粒子图集

};