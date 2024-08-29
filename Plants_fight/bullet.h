#pragma once

#include"Vector2.h"
#include<functional>
#include"player_id.h"
#include"camera.h"
#include<graphics.h>


class Bullet {
public:
	Bullet() = default;
	~Bullet() = default;

	void set_damage(int val) {
		damage = val;
	}

	int get_damage() {
		return damage;
	}

	void set_position(float x, float y) {
		position.x = x, position.y = y;
	}

	const Vector2& get_position() const {
		return position;
	}

	const Vector2& get_size() const {
		return size;
	}

	void set_velocity(float x, float y) {
		velocity.x = x, velocity.y = y;
	}

	void set_collide_target(PlayerId target) {
		target_id = target;
	}

	PlayerId get_collide_target() const {
		return target_id;
	}

	void set_callback(std::function<void()> callback) {
		this->callback = callback;
	}

	void set_valid(bool flag) {
		valid = flag;
	}

	bool get_valid() const{
		return valid;
	}

	bool check_can_remove() const {
		return can_remove;
	}

	void on_collide() {
		if (callback) callback();
	}

	virtual bool check_colllision(const Vector2& position, const Vector2 size) {
		return this->position.x + this->size.x / 2 >= position.x
			&& this->position.x + this->size.x / 2 <= position.x + size.x
			&& this->position.y + this->size.y / 2 >= position.y
			&& this->position.y + this->size.y / 2 <= position.y + size.y;
	}


	virtual void on_update(int delta) {}

	virtual void on_draw(const Camera& camera) const {}

protected:
	Vector2 size;				//子弹尺寸
	Vector2 position;			//子弹位置
	Vector2 velocity;			//子弹速度
	int damage = 10;			//子弹伤害

	bool valid = true;			//子弹是否有效（可造成伤害）
	bool can_remove = false;	//子弹是否可以delete掉了

	std::function<void()> callback; //子弹碰撞后的回调函数

	PlayerId target_id = PlayerId::P1; //子弹的攻击目标

protected:
	bool check_if_exceeds_screen() {
		return (position.x + size.x <= 0 || position.x >= getwidth()
			|| position.y + size.y <= 0 || position.y >= getheight());
	}
};