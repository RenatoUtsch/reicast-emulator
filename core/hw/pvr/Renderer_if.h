#pragma once
#include "drkPvr.h"
#include "ta_ctx.h"

bool rend_init();
void rend_term();

void rend_vblank();
void rend_start_render();
void rend_end_render();

void rend_set_fb_scale(float x,float y);
void rend_text_invl(vram_block* bl);

u32 getTexture(TSP tsp, TCW tcw);

///////
extern TA_context* _pvrrc;

#define pvrrc (_pvrrc->rend)

struct text_info {
	u16* pdata;
	u32 width;
	u32 height;
	u32 textype; // 0 565, 1 1555, 2 4444
};
