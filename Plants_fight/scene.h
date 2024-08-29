#pragma once
#include<graphics.h>
#include"camera.h"

class Scene {
public:
	Scene() = default;
	~Scene() = default;
	

	virtual void on_input(const ExMessage& msg) { //�����������ʱ����
		
	}

	virtual void on_update(int delta) { //������Ϸ�߼�ʱ����

	}

	virtual void on_draw(const Camera& camera) { //������Ⱦʱ����

	}
	
	virtual void on_enter() { //���볡��ʱ����

	}
	
	virtual void on_exit() { //�˳�����ʱ����

	}

private:


};