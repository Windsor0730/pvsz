#pragma once
#include<graphics.h>
#include"camera.h"

class Scene {
public:
	Scene() = default;
	~Scene() = default;
	

	virtual void on_input(const ExMessage& msg) { //处理玩家输入时调用
		
	}

	virtual void on_update(int delta) { //处理游戏逻辑时调用

	}

	virtual void on_draw(const Camera& camera) { //处理渲染时调用

	}
	
	virtual void on_enter() { //进入场景时调用

	}
	
	virtual void on_exit() { //退出场景时调用

	}

private:


};