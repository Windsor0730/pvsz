#pragma once
#include<functional>

class Timer {
public:
	Timer() = default;
	~Timer() = default;

	void restart() {	//���¿�ʼ��ʱ
		passed_time = 0;
		shotted = false;
	};  

	void set_wait_time(int time) {	//���õȴ�ʱ��
		wait_time = time;
	};

	void set_one_shot(bool one_shot) {	//�����Ƿ�����
		this->one_shot = one_shot;
	};

	void set_callback(std::function<void()> callback) {	//���ûص�����
		this->callback = callback;
	};

	void pause() {	//��ͣ
		paused = true;
	};

	void resume() {	//����
		paused = false;
	};

	void on_update(int delta) {	//����
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
	int passed_time = 0;     //�Ѿ���ȥ��ʱ��
	int wait_time = 0;     //�ȴ���ʱ��
	bool paused = false;   //�Ƿ���ͣ
	bool shotted = false;  //�Ƿ񴥷�
	bool one_shot = false;  //����������
	std:: function<void()> callback = nullptr;  //�ص�����
};