#pragma once

#include "Scene.h"
#include"scene_manager.h"

#include<iostream>

#include"atlas.h"
#include"animation.h"
#include"scene_manager.h"
#include"camera.h"
#include"Timer.h"

extern IMAGE img_menu_background;

extern SceneManager scene_manager;

class MenuScene : public Scene {
public:
	MenuScene() = default;
	~MenuScene() = default;

	void on_enter() override {
		mciSendString(_T("play bgm_menu repeat from 0"), nullptr, 0, nullptr);
	}

	void on_input(const ExMessage& msg) override {
		if (msg.message == WM_KEYUP) {
			scene_manager.switch_to(SceneManager::SceneType::Selector);
		}
	
	}

	void on_update(int delta) override {
		
	}

	void on_draw(const Camera& camera) override {
		putimage(0, 0, &img_menu_background);
	}

	void on_exit() override {
		
	}
private:
	Camera camera;
	Animation animation_peashooter_run_right;
	Timer timer;
};