#ifndef CORE_RENDERER_H
#define CORE_RENDERER_H

#include "hw/pvr/ta_ctx.h"

class Renderer {

public:
	bool init();
	void term();
	void resize(int w, int h);
	bool process(TA_context *ctx);
	bool renderFrame();
	void renderOSD();
	void show();
};

#endif // !CORE_RENDERER_H
