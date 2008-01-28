
#ifndef __HW_MAIN_H__
#define __HW_MAIN_H__

#include "Game.h"
#include "ExampleApplication.h"

class Main : public ExampleApplication {
public:
	Main() { }
	virtual ~Main() { }
protected:
	void createCamera();
	void chooseSceneManager();
	void createScene();	void createFrameListener();
};

#endif
