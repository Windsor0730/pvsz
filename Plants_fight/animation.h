#pragma once
#include<graphics.h>

#include"atlas.h"
#include"util.h"
#include<functional>
#include"camera.h"

class Animation {
public:
	Animation() = default;
	~Animation() = default;

	void reset() {
		timer = 0;
		idx_frame = 0;
	}

	void set_atlas(Atlas* new_atlas) {
		reset();
		this->atlas = new_atlas;
	}

	void set_loop(bool flag) {
		this->is_loop = flag;
	}

	void set_interval(int ms) {
		this->interval = ms;
	}

	int get_idx_frame() {
		return idx_frame;
	}

	IMAGE* get_current_frame() {
		if (atlas == nullptr)
			return nullptr;
		return atlas->get_image(idx_frame);
	}

	bool check_finished() {
		if (is_loop) 
			return false;
		if (atlas == nullptr)
			return true;
		return idx_frame == atlas->get_size() - 1;
	}

	void on_update(int delta) {
		timer += delta;
		if (timer >= interval) {
			timer = 0;
			idx_frame++;
			if (idx_frame >= atlas->get_size()) {
				if (is_loop) {
					idx_frame = 0;
				}
				else {
					idx_frame = atlas->get_size() - 1; 
					if(callback) callback();
				}
			}
		}
	}

	void on_draw(const Camera& camera, int x, int y) const{
		putimage_alpha(camera, x, y, atlas->get_image(idx_frame));
	}

	void set_callback(std::function<void()> callback) {
		this->callback = callback;
	}
private:
	int timer = 0;               //计时器
	int interval = 0;            //帧间隔
	int idx_frame = 0;           //当前帧索引
	bool is_loop = true;        //是否循环播放

	Atlas* atlas = nullptr;

	std::function<void()> callback;


};