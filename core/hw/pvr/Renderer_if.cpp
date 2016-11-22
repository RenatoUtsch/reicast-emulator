#include "Renderer_if.h"
#include "ta.h"
#include "hw/pvr/pvr_mem.h"
#include "rend/TexCache.h"
#include "rend/Renderer.h"
#include <thread>

Renderer* renderer;

int max_idx,max_mvo,max_op,max_pt,max_tr,max_vtx,max_modt, ovrn;

TA_context* _pvrrc;
void SetREP(TA_context* cntx);

bool rend_frame(TA_context* ctx) {
	bool proc = renderer->process(ctx);

	bool do_swp = proc && renderer->renderFrame();

	if (do_swp)
		renderer->renderOSD();

	return do_swp;
}

bool rend_single_frame() {
	//wait render start only if no frame pending
	do {
		_pvrrc = DequeueRender();
	} while (!_pvrrc);
	bool do_swp = rend_frame(_pvrrc);

	//clear up & free data ..
	FinishRender(_pvrrc);
	_pvrrc=0;

	return do_swp;
}

void rend_thread() {
	if (!renderer->init())
		die("rend->init() failed\n");

	for (;;) {
		if (rend_single_frame())
			renderer->show();
	}
}

void rend_start_render() {
	bool is_rtt=(FB_W_SOF1& 0x1000000)!=0;
	TA_context* ctx = tactx_Pop(CORE_CURRENT_CTX);

	SetREP(ctx);

	if (ctx) {
		if (!ctx->rend.Overrun) {
			FillBGP(ctx);
			
			ctx->rend.isRTT=is_rtt;
			ctx->rend.isAutoSort = UsingAutoSort();

			ctx->rend.fb_X_CLIP=FB_X_CLIP;
			ctx->rend.fb_Y_CLIP=FB_Y_CLIP;
			
			max_idx=max(max_idx,ctx->rend.idx.used());
			max_vtx=max(max_vtx,ctx->rend.verts.used());
			max_op=max(max_op,ctx->rend.global_param_op.used());
			max_pt=max(max_pt,ctx->rend.global_param_pt.used());
			max_tr=max(max_tr,ctx->rend.global_param_tr.used());
			
			max_mvo=max(max_mvo,ctx->rend.global_param_mvo.used());
			max_modt=max(max_modt,ctx->rend.modtrig.used());

			if (QueueRender(ctx))  {
				palette_update();
			}
		}
		else {
			ovrn++;
			printf("WARNING: Rendering context is overrun (%d), aborting frame\n",ovrn);
			tactx_Recycle(ctx);
		}
	}
}


void rend_end_render() {

}

bool rend_init() {
	renderer = new Renderer();

	std::thread t(rend_thread);
	t.detach();

	return true;
}

void rend_term() {

}

void rend_vblank() {
	os_DoEvents();
}

u32 getTexture(TSP tsp, TCW tcw) {
	return 0;
}

void rend_set_fb_scale(float x, float y) {

}

void rend_text_invl(vram_block* bl) {

}