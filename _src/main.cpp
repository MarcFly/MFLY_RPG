#include "matoya.h"

struct context {
	MTY_App *app;
	void *image;
	uint32_t image_w;
	uint32_t image_h;
	bool quit = false;
};

static void event_func(const MTY_Event *evt, void *opaque)
{
	context *ctx = (context*)opaque;

	MTY_PrintEvent(evt);

	if (evt->type == MTY_EVENT_CLOSE)
		ctx->quit = true;
}

static bool app_func(void *opaque)
{
	context *ctx = (context*)opaque;

	// Set up a render description for the PNG
	MTY_RenderDesc desc;
    {
		desc.format = MTY_COLOR_FORMAT_BGRA;
		desc.effect = MTY_EFFECT_SCANLINES;
		desc.imageWidth = ctx->image_w;
		desc.imageHeight = ctx->image_h;
		desc.cropWidth = ctx->image_w;
		desc.cropHeight = ctx->image_h;
		desc.aspectRatio = (float) ctx->image_w / (float) ctx->image_h;
	}

	// Draw the quad
	MTY_WindowDrawQuad(ctx->app, 0, ctx->image, &desc);

	MTY_WindowPresent(ctx->app, 0, 1);

	return !ctx->quit;
}

int main(int argc, char **argv)
{
	struct context ctx = {0};
	ctx.app = MTY_AppCreate(app_func, event_func, &ctx);
	if (!ctx.app)
		return 1;

	MTY_WindowDesc desc;
    {
		desc.title = "My Window";
		desc.api = MTY_GFX_GL;
		desc.width = 800;
		desc.height = 600;
		desc.fullscreen = false;
		desc.hidden = false;
	}

	MTY_WindowCreate(ctx.app, &desc);
	MTY_WindowMakeCurrent(ctx.app, 0, true);

	// Fetch a PNG from the internet
	void *png = NULL;
	size_t png_size = 0;
	uint16_t code = 0;
	if (MTY_HttpRequest("user-images.githubusercontent.com", 0, true, "GET",
		"/328897/112402607-36d00780-8ce3-11eb-9707-d11bc6c73c59.png",
		NULL, NULL, 0, 5000, &png, &png_size, &code))
	{
		// On success, decompress it into RGBA
		if (code == 200)
			ctx.image = MTY_DecompressImage(png, png_size, &ctx.image_w, &ctx.image_h);

		MTY_Free(png);
	}

	MTY_AppRun(ctx.app);
	MTY_AppDestroy(&ctx.app);

	MTY_Free(ctx.image);

	return 0;
}