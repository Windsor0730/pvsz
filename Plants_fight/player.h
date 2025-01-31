#pragma once

#include"camera.h"
#include<graphics.h>

#include"vector2.h"

#include"animation.h"
#include"player_id.h"

#include"platform.h"
#include"bullet.h"

#include"particle.h"

extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;

extern Atlas atlas_run_effect;
extern Atlas atlas_jump_effect;
extern Atlas atlas_land_effect;

extern IMAGE img_1P_cursor;
extern IMAGE img_2P_cursor;


class Player {
public:
	Player(bool facing_right = true) : is_facing_right(facing_right){
		current_animation = is_facing_right? &animation_idle_right : &animation_idle_left;

		timer_attack_cd.set_wait_time(attack_cd);
		timer_attack_cd.set_one_shot(true);
		timer_attack_cd.set_callback([&]() {
			can_attack = true;
			}
		);

		timer_invulnerable.set_wait_time(750);
		timer_invulnerable.set_one_shot(true);
		timer_invulnerable.set_callback(
			[&]() {
				is_invulnerable = false;
			}
		);

		timer_invulnerable_blink.set_wait_time(75);
		timer_invulnerable_blink.set_callback(
			[&]() {
				is_showing_sketch_frame = !is_showing_sketch_frame;
			}
		);

		timer_run_effect_generation.set_wait_time(75);
		timer_run_effect_generation.set_callback(
			[&]() {
				Vector2 particle_position;
				IMAGE* frame = atlas_run_effect.get_image(0);
				particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
				particle_position.y = position.y + size.y - frame->getheight();
				particle_list.emplace_back(particle_position, &atlas_run_effect, 45);
			}
		);

		timer_die_effect_generation.set_wait_time(35);
		timer_die_effect_generation.set_callback(
			[&]() {
				Vector2 particle_position;
				IMAGE* frame = atlas_run_effect.get_image(0);
				particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
				particle_position.y = position.y + size.y - frame->getheight();
				particle_list.emplace_back(particle_position, &atlas_run_effect, 150);
			}
		);

		animation_jump_effect.set_atlas(&atlas_jump_effect);
		animation_jump_effect.set_interval(25);
		animation_jump_effect.set_loop(false);
		animation_jump_effect.set_callback(
			[&]() {
				is_jump_effect_visible = false;
			}
		);

		animation_land_effect.set_atlas(&atlas_jump_effect);
		animation_land_effect.set_interval(25);
		animation_land_effect.set_loop(false);
		animation_land_effect.set_callback(
			[&]() {
				is_land_effect_visible = false;
			}
		);

		timer_cursor_visibility.set_wait_time(2500);
		timer_cursor_visibility.set_one_shot(true);
		timer_cursor_visibility.set_callback(
			[&]() {
				is_cursor_visible = false;
			}
		);
	}

	~Player() = default;

	virtual void on_input(const ExMessage& msg) {
		switch (msg.message) {
			case WM_KEYDOWN:
				switch (id) {
					case PlayerId::P1:
						switch (msg.vkcode) {
							case 0x41:
								is_left_key_down = true;
								break;
							case 0x44:
								is_right_key_down = true;
								break;
							case 0x57:
								on_jump();
								break;
							case 0x46:
								if (can_attack) {
									on_attack();
									can_attack = false;
									timer_attack_cd.restart();
								}
								break;
							case 0x47:
								if (mp >= 100) {
									on_attack_ex();
									mp = 0;
								}
								break;
							default:
								break;
						}
						break;
					case PlayerId::P2:
						switch (msg.vkcode) {
							case VK_LEFT:
								is_left_key_down = true;
								break;
							case VK_RIGHT:
								is_right_key_down = true;
								break;
							case VK_UP:
								on_jump();
								break;
							case VK_OEM_PERIOD:
								if (can_attack) {
									on_attack();
									can_attack = false;
									timer_attack_cd.restart();
								}
								break;
							case VK_OEM_2:
								if (mp >= 100) {
									on_attack_ex();
									mp = 0;
								}
								break;
							default:
								break;
						}
						break;
					default:
						break;
				}
				break;
			case WM_KEYUP:
				switch (id) {
					case PlayerId::P1:
						switch (msg.vkcode) {
							case 0x41:
								is_left_key_down = false;
								break;
							case 0x44:
								is_right_key_down = false;
								break;
							default:
								break;
						}
						break;
					case PlayerId::P2:
						switch (msg.vkcode) {
							case VK_LEFT:
								is_left_key_down = false;
								break;
							case VK_RIGHT:
								is_right_key_down = false;
								break;
							default:
								break;
						}
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}

	virtual void on_update(int delta) {
		int direction = is_right_key_down - is_left_key_down;

		if (direction != 0) {
			if (!is_attacking_ex) is_facing_right = direction > 0;
			current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
			float distance = run_velocity * delta * direction; //玩家在这一帧中移动的距离
			on_run(distance);
		}
		else {
			current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;
			timer_run_effect_generation.pause();
		}

		if (is_attacking_ex) current_animation = is_facing_right ? &animation_attack_ex_right : &animation_attack_ex_left;
		if (hp <= 0) current_animation = last_hurt_direction.x < 0 ? &animation_die_left : &animation_die_right;

		current_animation->on_update(delta);
		animation_jump_effect.on_update(delta);
		animation_land_effect.on_update(delta);

		timer_attack_cd.on_update(delta);
		timer_invulnerable.on_update(delta);
		timer_invulnerable_blink.on_update(delta);
		timer_run_effect_generation.on_update(delta);
		timer_cursor_visibility.on_update(delta);

		if (hp <= 0) timer_die_effect_generation.on_update(delta);

		particle_list.erase(std::remove_if(
			particle_list.begin(), particle_list.end(),
			[](const Particle& particle) {
				return !particle.check_valid();
			}),
			particle_list.end()
		);

		for (Particle& particle : particle_list) particle.on_update(delta);


		if (is_showing_sketch_frame) sketch_image(current_animation->get_current_frame(), &img_sketch);

		move_and_collide(delta);
	}

	virtual void on_draw(const Camera& camera) {
		if (is_jump_effect_visible) animation_jump_effect.on_draw(camera, (int)position_jump_effect.x, (int)position_jump_effect.y);
		if (is_land_effect_visible) animation_land_effect.on_draw(camera, (int)position_land_effect.x, (int)position_land_effect.y);

		for (Particle& particle : particle_list) particle.on_draw(camera);

		if (hp > 0 && is_invulnerable && is_showing_sketch_frame) {
			putimage_alpha(camera, (int)position.x, (int)position.y, &img_sketch);
		}
		else current_animation->on_draw(camera, (int)position.x, (int)position.y);

		if (is_cursor_visible) {
			switch (id) {
			case PlayerId::P1:
				putimage_alpha(camera, (int)(position.x + (size.x - img_1P_cursor.getwidth()) / 2),
					(int)(position.y - img_2P_cursor.getheight()), &img_1P_cursor);
				break;
			case PlayerId::P2:
				putimage_alpha(camera, (int)(position.x + (size.x - img_2P_cursor.getwidth()) / 2),
					(int)(position.y - img_2P_cursor.getheight()), &img_2P_cursor);
				break;
			default:
				break;
			}
		}

	}

	void set_id(PlayerId id) {
		this -> id = id;
	}

	void set_position(float x, float y) {
		position.x = x, position.y = y;
	}

	virtual void on_run(float distance) {
		if(is_attacking_ex)
			return;

		position.x += distance;
		timer_run_effect_generation.resume();
	}

	virtual void on_jump() {
		if (velocity.y != 0 || is_attacking_ex)
			return;
		velocity.y += jump_velocity;
		is_jump_effect_visible = true;
		animation_jump_effect.reset();

		IMAGE* effect_frame = animation_jump_effect.get_current_frame();
		position_jump_effect.x = position.x + (size.x - effect_frame->getwidth()) / 2;
		position_jump_effect.y = position.y + size.y - effect_frame->getheight();
	}

	virtual void on_land() {
		is_land_effect_visible = true;
		animation_land_effect.reset();

		IMAGE* effect_frame = animation_land_effect.get_current_frame();
		position_land_effect.x = position.x + (size.x - effect_frame->getwidth()) / 2;
		position_land_effect.y = position.y + size.y - effect_frame->getheight();
	}

	virtual void on_attack() {}

	virtual void on_attack_ex() {}

	const Vector2& get_position() const {
		return position;
	}

	const Vector2& get_size() const {
		return size;
	}

	int get_hp() const {
		return hp;
	}

	int get_mp() const {
		return mp;
	}

	void set_hp(int val) {
		hp = val;
	}




protected:
	void make_invulnerable() {
		is_invulnerable = true;
		timer_invulnerable.restart();
	}

	void move_and_collide(int delta) {
		float last_velocity_y = velocity.y;

		velocity.y += gravity * delta;
		position += velocity * (float)delta;

		if (hp <= 0) return;

		if(position.y > 0) {
			for(const Platform& platform : platform_list) {
				const Platform::CollisionShape& shape= platform.shape;
				bool is_collide_x = (max(position.x + size.x, shape.right) - min(position.x, shape.left)
					<= size.x + (shape.right - shape.left));
				bool is_collide_y = (shape.y >= position.y && shape.y <= position.y +size.y);

				if (is_collide_x && is_collide_y) {
					float delta_pos_y = velocity.y * delta;
					float last_tick_foot_pos_y = position.y + size.y - delta_pos_y;
					if (last_tick_foot_pos_y <= shape.y) {
						position.y = shape.y - size.y;
						velocity.y = 0;

						if(last_velocity_y != 0) on_land();

						break;
					}
				}
			}
		}

		if (!is_invulnerable) {
			for (Bullet* bullet : bullet_list) {
				if (!bullet->get_valid() || bullet->get_collide_target() != id) continue;

				if (bullet->check_colllision(position, size)) {
					make_invulnerable();
					bullet->on_collide();
					bullet->set_valid(false);
					hp -= bullet->get_damage();
					last_hurt_direction = bullet->get_position() - position;
					if (hp <= 0) {
						velocity.x = last_hurt_direction.x < 0 ? 0.35f : -0.35f;
						velocity.y = -1.0f;
					}
				}
			}
		}

	}
protected:
	Animation animation_idle_left;
	Animation animation_idle_right;
	Animation animation_run_left;
	Animation animation_run_right;
	Animation animation_attack_ex_left;
	Animation animation_attack_ex_right;
	Animation animation_jump_effect;
	Animation animation_land_effect;
	Animation animation_die_left;
	Animation animation_die_right;

	bool is_jump_effect_visible = false;
	bool is_land_effect_visible = false;
	Vector2 position_jump_effect;
	Vector2 position_land_effect;


	Animation* current_animation = nullptr; //当前正在播放的动画对象
	
	PlayerId id = PlayerId::P1;

	bool is_left_key_down = false;
	bool is_right_key_down = false;

	bool is_facing_right = true;

	bool can_attack = true;
	int attack_cd = 500;
	Timer timer_attack_cd;



protected:
	const float run_velocity = 0.55f; //奔跑速度
	const float gravity = 1.6e-3f; //重力加速度
	const float jump_velocity = -0.85f; //跳跃速度

protected:
	Vector2 size;                  //玩家的尺寸
	Vector2 position;
	Vector2 velocity;              //速度向量

	int mp = 0;
	int hp = 100;

	bool is_attacking_ex = false;

	IMAGE img_sketch;
	bool is_invulnerable = false;
	bool is_showing_sketch_frame = false; //当玩家处于无敌帧的时候
	Timer timer_invulnerable; 
	Timer timer_invulnerable_blink;

	Timer timer_run_effect_generation;
	Timer timer_die_effect_generation;
	std::vector<Particle> particle_list;

	bool is_cursor_visible = true;
	Timer timer_cursor_visibility;

	Vector2 last_hurt_direction;

};

