#pragma once
#include<functional>

class Timer {
public:
	Timer() = default;
	~Timer() = default;

	void restart() {	//重新开始计时
		passed_time = 0;
		shotted = false;
	};  

	void set_wait_time(int time) {	//设置等待时间
		wait_time = time;
	};

	void set_one_shot(bool one_shot) {	//设置是否重置
		this->one_shot = one_shot;
	};

	void set_callback(std::function<void()> callback) {	//设置回调函数
		this->callback = callback;
	};

	void pause() {	//暂停
		paused = true;
	};

	void resume() {	//继续
		paused = false;
	};

	void on_update(int delta) {	//更新
		if (paused) return;
		passed_time += delta;
		if (passed_time >= wait_time) {
			if(!one_shot || (one_shot&& !shotted) && callback) {
				callback();
				shotted = true;
				passed_time = 0;
			}
		}
	};

private:
	int passed_time = 0;     //已经过去的时间
	int wait_time = 0;     //等待的时间
	bool paused = false;   //是否暂停
	bool shotted = false;  //是否触发
	bool one_shot = false;  //触发后重置
	std:: function<void()> callback = nullptr;  //回调函数
};