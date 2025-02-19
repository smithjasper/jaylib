#include "raylib.h"
static Janet cfun_LoadImage(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    const char *fileName = janet_getcstring(argv, 0);
    Image *image = janet_abstract(&AT_Image, sizeof(Image));
    *image = LoadImage(fileName);
    return janet_wrap_abstract(image);
}

static Janet cfun_LoadImageFromBuffer(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    const char *fileType = janet_getcstring(argv, 0);
    JanetBuffer *data = janet_getbuffer(argv, 1);
    Image *image = janet_abstract(&AT_Image, sizeof(Image));
    *image = LoadImageFromMemory(fileType, data->data, data->count);
    return janet_wrap_abstract(image);
}

static Janet cfun_GetBitmapHeaderBuffer(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    int16_t sizeX = (int16_t)janet_getnumber(argv, 0);
    int16_t sizeY = (int16_t)janet_getnumber(argv, 1);
    const int32_t headerlength = 26;
    const int32_t magicheadersize = 12;
    const int16_t planes = 1;
    const int16_t bpp = 24;
    const int32_t filesize = headerlength + (sizeX * sizeY);

    JanetBuffer *data = janet_buffer(filesize);
    janet_buffer_push_cstring(data, "BM");
    janet_buffer_push_u32(data, filesize);
    janet_buffer_push_u32(data, 0); // should be two int16, reserved.
    janet_buffer_push_u32(data, headerlength);
    janet_buffer_push_u32(data, magicheadersize); // if 12 we can skip alot of complicated(?) stuff in the header
    janet_buffer_push_u16(data, sizeX);
    janet_buffer_push_u16(data, sizeY);
    janet_buffer_push_u16(data, planes);
    janet_buffer_push_u16(data, bpp);

    return janet_wrap_buffer(data);
}

static Janet cfun_IsImageValid(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Image image = *jaylib_getimage(argv, 0);
    if (IsImageValid(image)) {
        return janet_wrap_true();
    } else {
        return janet_wrap_false();
    }
}

static Janet cfun_ImageDimensions(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Image image = *jaylib_getimage(argv, 0);
    Janet dim[2] = { janet_wrap_integer(image.width), janet_wrap_integer(image.height) };
    return janet_wrap_tuple(janet_tuple_n(dim, 2));
}

static Janet cfun_ExportImage(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Image image = *jaylib_getimage(argv, 0);
    const char *fileName = janet_getcstring(argv, 1);
    ExportImage(image, fileName);
    return janet_wrap_nil();
}

static Janet cfun_ExportImageAsCode(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Image image = *jaylib_getimage(argv, 0);
    const char *fileName = janet_getcstring(argv, 1);
    ExportImageAsCode(image, fileName);
    return janet_wrap_nil();
}

static Janet cfun_LoadTexture(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    const char *fileName = janet_getcstring(argv, 0);
    Texture2D *texture = janet_abstract(&AT_Texture2D, sizeof(Texture2D));
    *texture = LoadTexture(fileName);
    return janet_wrap_abstract(texture);
}

static Janet cfun_IsTextureValid(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Texture2D texture = *jaylib_gettexture2d(argv, 0);
    if (IsTextureValid(texture)) {
        return janet_wrap_true();
    } else {
        return janet_wrap_false();
    }
}

static Janet cfun_LoadTextureFromImage(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Image image = *jaylib_getimage(argv, 0);
    Texture2D *texture = janet_abstract(&AT_Texture2D, sizeof(Texture2D));
    *texture = LoadTextureFromImage(image);
    return janet_wrap_abstract(texture);
}

static Janet cfun_LoadTextureCubemap(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Image image = *jaylib_getimage(argv, 0);
    const uint8_t *kw = janet_getkeyword(argv, 1);
    int layoutType = 0;
    if (!janet_cstrcmp(kw, "auto-detect")) {
        layoutType = CUBEMAP_LAYOUT_AUTO_DETECT;
    } else if (!janet_cstrcmp(kw, "line-vertical")) {
        layoutType = CUBEMAP_LAYOUT_LINE_VERTICAL;
    } else if (!janet_cstrcmp(kw, "line-horizontal")) {
        layoutType = CUBEMAP_LAYOUT_LINE_HORIZONTAL;
    } else if (!janet_cstrcmp(kw, "3x4")) {
        layoutType = CUBEMAP_LAYOUT_CROSS_THREE_BY_FOUR;
    } else if (!janet_cstrcmp(kw,  "4x3")) {
        layoutType = CUBEMAP_LAYOUT_CROSS_FOUR_BY_THREE;
    } else {
        janet_panicf("unknown layout type %v", argv[1]);
    }
    TextureCubemap *cubemap = janet_abstract(&AT_TextureCubemap, sizeof(TextureCubemap));
    *cubemap = LoadTextureCubemap(image, layoutType);
    return janet_wrap_abstract(cubemap);
}

static Janet cfun_LoadRenderTexture(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    int width = janet_getinteger(argv, 0);
    int height = janet_getinteger(argv, 1);
    RenderTexture *texture = janet_abstract(&AT_RenderTexture, sizeof(RenderTexture));
    *texture = LoadRenderTexture(width, height);
    return janet_wrap_abstract(texture);
}

static Janet cfun_IsRenderTextureReady(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    RenderTexture texture = *jaylib_getrendertexture(argv, 0);
    if (IsRenderTextureValid(texture)) {
        return janet_wrap_true();
    } else {
        return janet_wrap_false();
    }
}

static Janet cfun_UnloadImage(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Image image = *jaylib_getimage(argv, 0);
    UnloadImage(image);
    return janet_wrap_nil();
}

static Janet cfun_UnloadTexture(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Texture2D texture = *jaylib_gettexture2d(argv, 0);
    UnloadTexture(texture);
    return janet_wrap_nil();
}

static Janet cfun_UnloadRenderTexture(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    RenderTexture texture = *jaylib_getrendertexture(argv, 0);
    UnloadRenderTexture(texture);
    return janet_wrap_nil();
}

static Janet cfun_LoadImageFromTexture(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Texture2D texture = *jaylib_gettexture2d(argv, 0);
    Image *image = janet_abstract(&AT_Image, sizeof(Image));
    *image = LoadImageFromTexture(texture);
    return janet_wrap_abstract(image);
}

static Janet cfun_LoadImageFromScreen(int32_t argc, Janet *argv) {
    (void) argv;
    janet_fixarity(argc, 0);
    Image *image = janet_abstract(&AT_Image, sizeof(Image));
    *image = LoadImageFromScreen();
    return janet_wrap_abstract(image);
}

static Janet cfun_GetRenderTextureTexture2d(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    RenderTexture renderTexture = *jaylib_getrendertexture(argv, 0);
    Texture2D *texture = janet_abstract(&AT_Texture2D, sizeof(Texture2D));
    *texture = renderTexture.texture;
    return janet_wrap_abstract(texture);
}

static Janet cfun_ImageCopy(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Image image = *jaylib_getimage(argv, 0);
    Image *newImage = janet_abstract(&AT_Image, sizeof(Image));
    *newImage = ImageCopy(image);
    return janet_wrap_abstract(newImage);
}

static Janet cfun_ImageFromImage(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Image image = *jaylib_getimage(argv, 0);
    Rectangle rect = jaylib_getrect(argv, 1);
    Image *newImage = janet_abstract(&AT_Image, sizeof(Image));
    *newImage = ImageFromImage(image, rect);
    return janet_wrap_abstract(newImage);
}

static Janet cfun_ImageToPOT(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Image *image = jaylib_getimage(argv, 0);
    Color color = jaylib_getcolor(argv, 1);
    ImageToPOT(image, color);
    return argv[0];
}

static Janet cfun_ImageFormat(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Image *image = jaylib_getimage(argv, 0);
    int format = jaylib_getpixelformat(argv, 1);
    ImageFormat(image, format);
    return argv[0];
}

static Janet cfun_ImageAlphaMask(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Image *image = jaylib_getimage(argv, 0);
    Image *mask = jaylib_getimage(argv, 1);
    ImageAlphaMask(image, *mask);
    return argv[0];
}

static Janet cfun_ImageAlphaClear(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 3);
    Image *image = jaylib_getimage(argv, 0);
    Color color = jaylib_getcolor(argv, 1);
    float threshold = (float) janet_getnumber(argv, 2);
    ImageAlphaClear(image, color, threshold);
    return argv[0];
}

static Janet cfun_ImageAlphaCrop(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Image *image = jaylib_getimage(argv, 0);
    float threshold = (float) janet_getnumber(argv, 1);
    ImageAlphaCrop(image, threshold);
    return argv[0];
}

static Janet cfun_ImageAlphaPremultiply(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Image *image = jaylib_getimage(argv, 0);
    ImageAlphaPremultiply(image);
    return argv[0];
}

static Janet cfun_ImageCrop(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Image *image = jaylib_getimage(argv, 0);
    Rectangle rect = jaylib_getrect(argv, 1);
    ImageCrop(image, rect);
    return argv[0];
}

static Janet cfun_ImageResize(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 3);
    Image *image = jaylib_getimage(argv, 0);
    int newWidth = janet_getinteger(argv, 1);
    int newHeight = janet_getinteger(argv, 2);
    ImageResize(image, newWidth, newHeight);
    return argv[0];
}

static Janet cfun_ImageResizeNN(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 3);
    Image *image = jaylib_getimage(argv, 0);
    int newWidth = janet_getinteger(argv, 1);
    int newHeight = janet_getinteger(argv, 2);
    ImageResizeNN(image, newWidth, newHeight);
    return argv[0];
}

static Janet cfun_ImageResizeCanvas(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 6);
    Image *image = jaylib_getimage(argv, 0);
    int newWidth = janet_getinteger(argv, 1);
    int newHeight = janet_getinteger(argv, 2);
    int offsetX = janet_getinteger(argv, 3);
    int offsetY = janet_getinteger(argv, 4);
    Color color = jaylib_getcolor(argv, 5);
    ImageResizeCanvas(image, newWidth, newHeight, offsetX, offsetY, color);
    return argv[0];
}

static Janet cfun_ImageMipmaps(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Image *image = jaylib_getimage(argv, 0);
    ImageMipmaps(image);
    return argv[0];
}

static Janet cfun_ImageDither(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 5);
    Image *image = jaylib_getimage(argv, 0);
    int rBpp = janet_getinteger(argv, 1);
    int gBpp = janet_getinteger(argv, 2);
    int bBpp = janet_getinteger(argv, 3);
    int aBpp = janet_getinteger(argv, 4);
    ImageDither(image, rBpp, gBpp, bBpp, aBpp);
    return argv[0];
}

static Janet cfun_LoadImagePalette(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Image *image = jaylib_getimage(argv, 0);
    int maxPaletteSize = janet_getinteger(argv, 1);
    int extractCount = 0;
    Color *colors = LoadImagePalette(*image, maxPaletteSize, &extractCount);
    JanetArray *acolors = janet_array(extractCount);
    for (int i = 0; i < extractCount; i++) {
        Color c = colors[i];
        Janet *t = janet_tuple_begin(4);
        t[0] = janet_wrap_integer(c.r);
        t[1] = janet_wrap_integer(c.g);
        t[2] = janet_wrap_integer(c.b);
        t[3] = janet_wrap_integer(c.a);
        janet_array_push(acolors, janet_wrap_tuple(janet_tuple_end(t)));
    }
    return janet_wrap_array(acolors);
}

static Janet cfun_ImageText(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 3);
    const char *text = janet_getcstring(argv, 0);
    int fontSize = janet_getinteger(argv, 1);
    Color color = jaylib_getcolor(argv, 2);
    Image *image = janet_abstract(&AT_Image, sizeof(Image));
    *image = ImageText(text, fontSize, color);
    return janet_wrap_abstract(image);
}

static Janet cfun_ImageTextEx(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 5);
    Font *font  = jaylib_getfont(argv, 0);
    const char *text = janet_getcstring(argv, 1);
    float fontSize = (float) janet_getnumber(argv, 2);
    float spacing = (float) janet_getnumber(argv, 3);
    Color color = jaylib_getcolor(argv, 4);
    Image *image = janet_abstract(&AT_Image, sizeof(Image));
    *image = ImageTextEx(*font, text, fontSize, spacing, color);
    return janet_wrap_abstract(image);
}

static Janet cfun_ImageDraw(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 5);
    Image *dst = jaylib_getimage(argv, 0);
    Image *src = jaylib_getimage(argv, 1);
    Rectangle srcRect = jaylib_getrect(argv, 2);
    Rectangle destRect = jaylib_getrect(argv, 3);
    Color tint = jaylib_getcolor(argv, 4);
    ImageDraw(dst, *src, srcRect, destRect, tint);
    return argv[0];
}

static Janet cfun_ImageDrawRectangleRec(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 3);
    Image *dst = jaylib_getimage(argv, 0);
    Rectangle rec = jaylib_getrect(argv, 1);
    Color color = jaylib_getcolor(argv, 2);
    ImageDrawRectangleRec(dst, rec, color);
    return argv[0];
}

static Janet cfun_ImageDrawRectangleLines(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 4);
    Image *dst = jaylib_getimage(argv, 0);
    Rectangle rec = jaylib_getrect(argv, 1);
    int thick = janet_getinteger(argv, 2);
    Color color = jaylib_getcolor(argv, 3);
    ImageDrawRectangleLines(dst, rec, thick, color);
    return argv[0];
}

static Janet cfun_ImageDrawText(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 6);
    Image *dst = jaylib_getimage(argv, 0);
    const char *text = janet_getcstring(argv, 1);
    int posX = janet_getinteger(argv, 2);
    int posY = janet_getinteger(argv, 3);
    int fontSize = janet_getinteger(argv, 4);
    Color color = jaylib_getcolor(argv, 5);
    ImageDrawText(dst, text, posX, posY, fontSize, color);
    return argv[0];
}

static Janet cfun_ImageDrawTextEx(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 7);
    Image *dst = jaylib_getimage(argv, 0);
    Font *font = jaylib_getfont(argv, 1);
    const char *text = janet_getcstring(argv, 2);
    Vector2 position = jaylib_getvec2(argv, 3);
    float fontSize = (float) janet_getnumber(argv, 4);
    float spacing = (float) janet_getnumber(argv, 5);
    Color color = jaylib_getcolor(argv, 6);
    ImageDrawTextEx(dst, *font, text, position, fontSize, spacing, color);
    return argv[0];
}

static Janet cfun_ImageFlipVertical(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Image *image = jaylib_getimage(argv, 0);
    ImageFlipVertical(image);
    return argv[0];
}

static Janet cfun_ImageFlipHorizontal(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Image *image = jaylib_getimage(argv, 0);
    ImageFlipHorizontal(image);
    return argv[0];
}

static Janet cfun_ImageRotateCW(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Image *image = jaylib_getimage(argv, 0);
    ImageRotateCCW(image);
    return argv[0];
}

static Janet cfun_ImageRotateCCW(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Image *image = jaylib_getimage(argv, 0);
    ImageRotateCCW(image);
    return argv[0];
}

static Janet cfun_ImageColorTint(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Image *image = jaylib_getimage(argv, 0);
    Color tint = jaylib_getcolor(argv, 1);
    ImageColorTint(image, tint);
    return argv[0];
}

static Janet cfun_ImageColorInvert(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Image *image = jaylib_getimage(argv, 0);
    ImageColorInvert(image);
    return argv[0];
}

static Janet cfun_ImageColorGrayscale(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Image *image = jaylib_getimage(argv, 0);
    ImageColorGrayscale(image);
    return argv[0];
}

static Janet cfun_ImageColorContrast(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Image *image = jaylib_getimage(argv, 0);
    float contrast = (float) janet_getnumber(argv, 1);
    ImageColorContrast(image, contrast);
    return argv[0];
}

static Janet cfun_ImageColorBrightness(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Image *image = jaylib_getimage(argv, 0);
    int brightness = janet_getinteger(argv, 1);
    ImageColorBrightness(image, brightness);
    return argv[0];
}

static Janet cfun_ImageColorReplace(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 3);
    Image *image = jaylib_getimage(argv, 0);
    Color color = jaylib_getcolor(argv, 1);
    Color replace = jaylib_getcolor(argv, 2);
    ImageColorReplace(image, color, replace);
    return argv[0];
}

static Janet cfun_DrawTexture(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 4);
    Texture2D texture = *jaylib_gettexture2d(argv, 0);
    int x = janet_getinteger(argv, 1);
    int y = janet_getinteger(argv, 2);
    Color color = jaylib_getcolor(argv, 3);
    DrawTexture(texture, x, y, color);
    return janet_wrap_nil();
}

static Janet cfun_DrawTextureV(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 3);
    Texture2D texture = *jaylib_gettexture2d(argv, 0);
    Vector2 position = jaylib_getvec2(argv, 1);
    Color color = jaylib_getcolor(argv, 2);
    DrawTextureV(texture, position, color);
    return janet_wrap_nil();
}

static Janet cfun_DrawTextureEx(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 5);
    Texture2D texture = *jaylib_gettexture2d(argv, 0);
    Vector2 position = jaylib_getvec2(argv, 1);
    float rotation = (float) janet_getnumber(argv, 2);
    float scale = (float) janet_getnumber(argv, 3);
    Color color = jaylib_getcolor(argv, 4);
    DrawTextureEx(texture, position, rotation, scale, color);
    return janet_wrap_nil();
}

static Janet cfun_DrawTextureRec(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 4);
    Texture2D texture = *jaylib_gettexture2d(argv, 0);
    Rectangle rect = jaylib_getrect(argv, 1);
    Vector2 position = jaylib_getvec2(argv, 2);
    Color color = jaylib_getcolor(argv, 3);
    DrawTextureRec(texture, rect, position, color);
    return janet_wrap_nil();
}

static Janet cfun_DrawTextureNPatch(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 6);
    Texture2D texture = *jaylib_gettexture2d(argv, 0);
    NPatchInfo nPatchInfo = jaylib_getnpatchinfo(argv, 1);
    Rectangle rect = jaylib_getrect(argv, 2);
    Vector2 origin = jaylib_getvec2(argv, 3);
    float rotation = (float)janet_getnumber(argv, 4);
    Color color = jaylib_getcolor(argv, 5);
    DrawTextureNPatch(texture, nPatchInfo, rect, origin, rotation, color);
    return janet_wrap_nil();
}

static Janet cfun_DrawTexturePro(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 6);
    Texture2D texture = *jaylib_gettexture2d(argv, 0);
    Rectangle rect = jaylib_getrect(argv, 1);
    Rectangle dest = jaylib_getrect(argv, 2);
    Vector2 position = jaylib_getvec2(argv, 3);
    float rotation = janet_getnumber(argv, 4);
    Color color = jaylib_getcolor(argv, 5);
    DrawTexturePro(texture, rect, dest, position, rotation, color);
    return janet_wrap_nil();
}

static Janet cfun_GenImageColor(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 3);
    int width = janet_getinteger(argv, 0);
    int height = janet_getinteger(argv, 1);
    Color color = jaylib_getcolor(argv, 2);
    Image *image = janet_abstract(&AT_Image, sizeof(Image));
    *image = GenImageColor(width, height, color);
    return janet_wrap_abstract(image);
}

static Janet cfun_GenImageGradientLinear(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 5);
    int width = janet_getinteger(argv, 0);
    int height = janet_getinteger(argv, 1);
    int direction = janet_getinteger(argv, 2);
    Color start = jaylib_getcolor(argv, 3);
    Color end = jaylib_getcolor(argv, 4);
    Image *image = janet_abstract(&AT_Image, sizeof(Image));
    *image = GenImageGradientLinear(width, height, direction, start, end);
    return janet_wrap_abstract(image);
}

static Janet cfun_GenImageGradientRadial(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 5);
    int width = janet_getinteger(argv, 0);
    int height = janet_getinteger(argv, 1);
    float density = (float) janet_getnumber(argv, 2);
    Color inner = jaylib_getcolor(argv, 3);
    Color outer = jaylib_getcolor(argv, 4);
    Image *image = janet_abstract(&AT_Image, sizeof(Image));
    *image = GenImageGradientRadial(width, height, density, inner, outer);
    return janet_wrap_abstract(image);
}

static Janet cfun_GenImageGradientSquare(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 5);
    int width = janet_getinteger(argv, 0);
    int height = janet_getinteger(argv, 1);
    float density = (float) janet_getnumber(argv, 2);
    Color inner = jaylib_getcolor(argv, 3);
    Color outer = jaylib_getcolor(argv, 4);
    Image *image = janet_abstract(&AT_Image, sizeof(Image));
    *image = GenImageGradientSquare(width, height, density, inner, outer);
    return janet_wrap_abstract(image);
}

static Janet cfun_GenImageChecked(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 6);
    int width = janet_getinteger(argv, 0);
    int height = janet_getinteger(argv, 1);
    int checksX = janet_getinteger(argv, 2);
    int checksY = janet_getinteger(argv, 3);
    Color col1 = jaylib_getcolor(argv, 4);
    Color col2 = jaylib_getcolor(argv, 5);
    Image *image = janet_abstract(&AT_Image, sizeof(Image));
    *image = GenImageChecked(width, height, checksX, checksY, col1, col2);
    return janet_wrap_abstract(image);
}

static Janet cfun_GenImageWhiteNoise(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 3);
    int width = janet_getinteger(argv, 0);
    int height = janet_getinteger(argv, 1);
    float factor = (float) janet_getnumber(argv, 2);
    Image *image = janet_abstract(&AT_Image, sizeof(Image));
    *image = GenImageWhiteNoise(width, height, factor);
    return janet_wrap_abstract(image);
}

static Janet cfun_GenImageCellular(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 3);
    int width = janet_getinteger(argv, 0);
    int height = janet_getinteger(argv, 1);
    int tileSize = janet_getinteger(argv, 2);
    Image *image = janet_abstract(&AT_Image, sizeof(Image));
    *image = GenImageCellular(width, height, tileSize);
    return janet_wrap_abstract(image);
}

static Janet cfun_GenTextureMipmaps(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Texture2D *texture = jaylib_gettexture2d(argv, 0);
    GenTextureMipmaps(texture);
    return argv[0];
}

static Janet cfun_SetTextureFilter(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Texture2D *texture = jaylib_gettexture2d(argv, 0);
    const uint8_t *kw = janet_getkeyword(argv, 1);
    int filter = 0;
    if (!janet_cstrcmp(kw, "point")) {
        filter = TEXTURE_FILTER_POINT;
    } else if (!janet_cstrcmp(kw, "bilinear")) {
        filter = TEXTURE_FILTER_BILINEAR;
    } else if (!janet_cstrcmp(kw, "trilinear")) {
        filter = TEXTURE_FILTER_TRILINEAR;
    } else if (!janet_cstrcmp(kw, "anisotropic-4x")) {
        filter = TEXTURE_FILTER_ANISOTROPIC_4X;
    } else if (!janet_cstrcmp(kw, "anisotropic-8x")) {
        filter = TEXTURE_FILTER_ANISOTROPIC_8X;
    } else if (!janet_cstrcmp(kw, "anisotropic-16x")) {
        filter = TEXTURE_FILTER_ANISOTROPIC_16X;
    } else {
        janet_panicf("unknown filter %v", argv[1]);
    }
    SetTextureFilter(*texture, filter);
    return janet_wrap_nil();
}

static Janet cfun_SetTextureWrap(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Texture2D *texture = jaylib_gettexture2d(argv, 0);
    const uint8_t *kw = janet_getkeyword(argv, 1);
    int wrap = 0;
    if (!janet_cstrcmp(kw, "repeat")) {
        wrap = TEXTURE_WRAP_REPEAT;
    } else if (!janet_cstrcmp(kw, "clamp")) {
        wrap = TEXTURE_WRAP_CLAMP;
    } else if (!janet_cstrcmp(kw, "mirror-repeat")) {
        wrap = TEXTURE_WRAP_MIRROR_REPEAT;
    } else if (!janet_cstrcmp(kw, "mirror-clamp")) {
        wrap = TEXTURE_WRAP_MIRROR_CLAMP;
    } else {
        janet_panicf("unknown wrap-mode %v", argv[1]);
    }
    SetTextureWrap(*texture, wrap);
    return janet_wrap_nil();
}

static Janet cfun_GetImageDimensions(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Image *src = jaylib_getimage(argv, 0);
    Vector2 dim = {
        src->width,
        src->height
    };
    return jaylib_wrap_vec2(dim);
}

static Janet cfun_LoadImageFromMemory(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 3);
    const char *fileType = janet_getcstring(argv, 0);
    const unsigned char *fileData = janet_getbytes(argv, 1).bytes;
    int dataSize = janet_getinteger(argv, 2);

    Image *image = janet_abstract(&AT_Image, sizeof(Image));
    *image = LoadImageFromMemory(fileType, fileData, dataSize);
    return janet_wrap_abstract(image);
}

/*
// Image/Texture2D data loading/unloading/saving functions
RLAPI Image LoadImagePro(void *data, int width, int height, int format);                                 // Load image from raw data with parameters
RLAPI Image LoadImageRaw(const char *fileName, int width, int height, int format, int headerSize);       // Load image from RAW file data
RLAPI Color *GetImageData(Image image);                                                                  // Get pixel data from image as a Color struct array
RLAPI Vector4 *GetImageDataNormalized(Image image);                                                      // Get pixel data from image as Vector4 array (float normalized)
RLAPI Rectangle GetImageAlphaBorder(Image image, float threshold);                                       // Get image alpha border rectangle
RLAPI int GetPixelDataSize(int width, int height, int format);                                           // Get pixel data size in bytes (image or texture)
RLAPI void UpdateTexture(Texture2D texture, const void *pixels);                                         // Update GPU texture with new data

// Texture2D drawing functions
RLAPI void DrawTextureNPatch(Texture2D texture, NPatchInfo nPatchInfo, Rectangle destRec, Vector2 origin, float rotation, Color tint);  // Draws a texture (or part of it) that stretches or shrinks nicely
*/

static const JanetReg image_cfuns[] = {
    {
        "load-image-1", cfun_LoadImage,
        "(load-image-1 file-name)\n\n"
        "Load image from file into CPU memory (RAM)"
    }, // load-image is janet core function, don't want to overwrite if we use (use jaylib)
    {
        "load-image-from-buffer", cfun_LoadImageFromBuffer,
        "(load-image-from-buffer file-type buffer)\n\n"
        "Load buffer as image object"
    },
    {
        "get-bitmap-header-buffer", cfun_GetBitmapHeaderBuffer,
        "(get-bitmap-header-buffer dimx dimy)\n\n"
        "returns a header for a 24bpp bitmap with dimensions dimx*dimy"
    },
    {
        "image-valid?", cfun_IsImageValid,
        "(image-valid? image)\n\n"
        "Check if an image is valid"
    },
    {
        "export-image", cfun_ExportImage,
        "(export-image image file-name)\n\n"
        "Export image data to file, returns true on success"
    },
    {
        "export-image-as-code", cfun_ExportImageAsCode,
        "(export-image-as-code image file-name)\n\n"
        "Export image as code file defining an array of bytes, returns true on success"
    },
    {
        "load-texture", cfun_LoadTexture,
        "(load-texture file-name)\n\n"
        "Load texture from file into GPU memory (VRAM)"
    },
    {
        "texture-valid?", cfun_IsTextureValid,
        "(texture-valid? texture)\n\n"
        "Check if a texture is valid"
    },
    {
        "load-texture-from-image", cfun_LoadTextureFromImage,
        "(load-texture-from-image image)\n\n"
        "Load texture from image data"
    },
    {
        "load-texture-cubemap", cfun_LoadTextureCubemap,
        "(load-texture-cubemap image layout)\n\n"
        "Load cubemap from image, multiple image cubemap layouts supported"
    },
    {
        "load-render-texture", cfun_LoadRenderTexture,
        "(load-render-texture width height)\n\n"
        "Load texture for rendering (framebuffer)"
    },
    {
        "render-texture-ready?", cfun_IsRenderTextureReady,
        "(render-texture-ready? texture)\n\n"
        "Check if a render texture is ready"
    },
    {
        "unload-image", cfun_UnloadImage,
        "(unload-image image)\n\n"
        "Unload image from CPU memory (RAM)"
    },
    {
        "unload-texture", cfun_UnloadTexture,
        "(unload-texture texture)\n\n"
        "Unload texture from GPU memory (VRAM)"
    },
    {
        "unload-render-texture", cfun_UnloadRenderTexture,
        "(unload-render-texture target)\n\n"
        "Unload render texture from GPU memory (VRAM)"
    },
    {
        "get-image-dimensions", cfun_GetImageDimensions,
        "(get-image-dimensions image)\n\n"
        "Get image dimensions."
    },
    {
        "load-image-from-texture", cfun_LoadImageFromTexture,
        "(load-image-from-texture texture)\n\n"
        "Load image from GPU texture data"
    },
    {
        "load-image-from-screen", cfun_LoadImageFromScreen,
        "(load-image-from-screen)\n\n"
        "Load image from screen buffer and (screenshot)"
    },
    {
        "get-render-texture-texture2d", cfun_GetRenderTextureTexture2d,
        "(get-render-texture-texture2d texture)\n\n"
        "Load texture for rendering (framebuffer)"
    },
    {
        "image-copy", cfun_ImageCopy,
        "(image-copy image)\n\n"
        "Create an image duplicate (useful for transformations)"
    },
    {
        "image-from-image", cfun_ImageFromImage,
        "(image-from-image image rec)\n\n"
        "Create an image from another image piece"
    },
    {
        "image-to-pot", cfun_ImageToPOT,
        "(image-to-pot image fill)\n\n"
        "Convert image to POT (power-of-two)"
    },
    {
        "image-format", cfun_ImageFormat,
        "(image-format image new-format)\n\n"
        "Convert image data to desired format"
    },
    {
        "image-alpha-mask", cfun_ImageAlphaMask,
        "(image-alpha-mask image alpha-mask)\n\n"
        "Apply alpha mask to image"
    },
    {
        "image-alpha-clear", cfun_ImageAlphaClear,
        "(image-alpha-clear image clear threshold)\n\n"
        "Clear alpha channel to desired color"
    },
    {
        "image-alpha-crop", cfun_ImageAlphaCrop,
        "(image-alpha-crop image threshold)\n\n"
        "Crop image depending on alpha value"
    },
    {
        "image-alpha-premultiply", cfun_ImageAlphaPremultiply,
        "(image-alpha-premultiply image)\n\n"
        "Premultiply alpha channel"
    },
    {
        "image-crop", cfun_ImageCrop,
        "(image-crop image crop)\n\n"
        "Crop an image to a defined rectangle"
    },
    {
        "image-resize", cfun_ImageResize,
        "(image-resize image new-width new-height)\n\n"
        "Resize image (Bicubic scaling algorithm)"
    },
    {
        "image-resize-nn", cfun_ImageResizeNN,
        "(image-resize-nn image new-width new-height)\n\n"
        "Resize image (Nearest-Neighbor scaling algorithm)"
    },
    {
        "image-resize-canvas", cfun_ImageResizeCanvas,
        "(image-resize-canvas image new-width new-height offset-x offset-y fill)\n\n"
        "Resize canvas and fill with color"
    },
    {
        "image-mipmaps", cfun_ImageMipmaps,
        "(image-mipmaps image)\n\n"
        "Generate all mipmap levels for a provided image"
    },
    {
        "image-dimensions", cfun_ImageDimensions,
        "(image-dimensions image)\n\n"
        "Get image dimensions"
    },
    {
        "image-dither", cfun_ImageDither,
        "(image-dither image r-bpp g-bpp b-bpp a-bpp)\n\n"
        "Dither image data to 16bpp or lower (Floyd-Steinberg dithering)"
    },
    {
        "load-image-pallete", cfun_LoadImagePalette,
        "(load-image-pallete image max-palette-size colors-count)\n\n"
        "Load colors palette from image as a Color array (RGBA - 32bit)"
    },
    {
        "image-text", cfun_ImageText,
        "(image-text text font-size color)\n\n"
        "Create an image from text (default font)"
    },
    {
        "image-text-ex", cfun_ImageTextEx,
        "(image-text-ex font text font-size spacing tint)\n\n"
        "Create an image from text (custom sprite font)"
    },
    {
        "image-draw", cfun_ImageDraw,
        "(image-draw dst src src-rec dst-rec tint)\n\n"
        "Draw a source image within a destination image (tint applied to source)"
    },
    {
        "image-draw-rectangle-rec", cfun_ImageDrawRectangleRec,
        "(image-draw-rectangle-rec dst rec color)\n\n"
        "Draw rectangle within an image"
    },
    {
        "image-draw-rectangle-lines", cfun_ImageDrawRectangleLines,
        "(image-draw-rectangle-lines dst rec thick color)\n\n"
        "Draw rectangle lines within an image"
    },
    {
        "image-draw-text", cfun_ImageDrawText,
        "(image-draw-text dst text pos-x pos-y font-size color)\n\n"
        "Draw text (using default font) within an image (destination)"
    },
    {
        "image-draw-text-ex", cfun_ImageDrawTextEx,
        "(image-draw-text-ex dst font text [pos-x pos-y] font-size spacing tint)\n\n"
        "Draw text (custom sprite font) within an image (destination)"
    },
    {
        "image-flip-vertical", cfun_ImageFlipVertical,
        "(image-flip-vertical image)\n\n"
        "Flip image vertically"
    },
    {
        "image-flip-horizontal", cfun_ImageFlipHorizontal,
        "(image-flip-horizontal image)\n\n"
        "Flip image horizontally"
    },
    {
        "image-rotate-cw", cfun_ImageRotateCW,
        "(image-rotate-cw image)\n\n"
        "Rotate image clockwise 90deg"
    },
    {
        "image-rotate-ccw", cfun_ImageRotateCCW,
        "(image-rotate-ccw image)\n\n"
        "Rotate image counter-clockwise 90deg"
    },
    {
        "image-color-tint", cfun_ImageColorTint,
        "(image-color-tint image color)\n\n"
        "Modify image color: tint"
    },
    {
        "image-color-invert", cfun_ImageColorInvert,
        "(image-color-invert image)\n\n"
        "Modify image color: invert"
    },
    {
        "image-color-grayscale", cfun_ImageColorGrayscale,
        "(image-color-grayscale image)\n\n"
        "Modify image color: grayscale"
    },
    {
        "image-color-brightness", cfun_ImageColorBrightness,
        "(image-color-brightness image brightness)\n\n"
        "Modify image color: brightness (-255 to 255)"
    },
    {
        "image-color-contrast", cfun_ImageColorContrast,
        "(image-color-contrast image contrast)\n\n"
        "Modify image color: contrast (-100 to 100)"
    },
    {
        "image-color-replace", cfun_ImageColorReplace,
        "(image-color-replace image color replace)\n\n"
        "Modify image color: replace color"
    },
    {
        "draw-texture", cfun_DrawTexture,
        "(draw-texture texture pos-x pos-y tint)\n\n"
        "Draw a Texture2D"
    },
    {
        "draw-texture-v", cfun_DrawTextureV,
        "(draw-texture-v texture [pos-x pos-y] tint)\n\n"
        "Draw a Texture2D with position defined as Vector2"
    },
    {
        "draw-texture-ex", cfun_DrawTextureEx,
        "(draw-texture-ex texture [pos-x pos-y] rotation scale tint)\n\n"
        "Draw a Texture2D with extended parameters"
    },
    {
        "draw-texture-pro", cfun_DrawTexturePro,
        "(draw-texture-pro texture source dest origin rotation tint)\n\n"
        "Draw a part of a texture defined by a rectangle with 'pro' parameters"
    },
    {
        "draw-texture-rec", cfun_DrawTextureRec,
        "(draw-texture-rec texture source position tint)\n\n"
        "Draw a part of a texture defined by a rectangle"
    },
    {
        "draw-texture-n-patch", cfun_DrawTextureNPatch,
        "(draw-texture-n-patch texture n-patch-info dest origin rotation tint)\n\n"
        "Draw a texture (or part of it) that stretches or shrinks nicely"
    },
    {
        "gen-image-color", cfun_GenImageColor,
        "(gen-image-color width height color)\n\n"
        "Generate image: plain color"
    },
    {
        "gen-image-gradient-linear", cfun_GenImageGradientLinear,
        "(gen-image-gradient-linear width height direction start end)\n\n"
        "Generate image: linear gradient, direction in degrees [0..360], 0=Vertical gradient"
    },
    {
        "gen-image-gradient-radial", cfun_GenImageGradientRadial,
        "(gen-image-gradient-radial width height density inner outer)\n\n"
        "Generate image: radial gradient"
    },
    {
        "gen-image-gradient-square", cfun_GenImageGradientSquare,
        "(gen-image-gradient-square width height density inner outer)\n\n"
        "Generate image: square gradient"
    },
    {
        "gen-image-checked", cfun_GenImageChecked,
        "(gen-image-checked width height checks-x checks-y color1 color2)\n\n"
        "Generate image: checked"
    },
    {
        "gen-image-white-noise", cfun_GenImageWhiteNoise,
        "(gen-image-white-noise width height factor)\n\n"
        "Generate image: white noise"
    },
    {
        "gen-image-cellular", cfun_GenImageCellular,
        "(gen-image-cellular width height tile-size)\n\n"
        "Generate image: cellular algorithm. Bigger tileSize means bigger cells"
    },
    {
        "gen-texture-mipmaps", cfun_GenTextureMipmaps,
        "(gen-texture-mipmaps texture)\n\n"
        "Generate GPU mipmaps for a texture"
    },
    {
        "set-texture-filter", cfun_SetTextureFilter,
        "(set-texture-filter texture filter)\n\n"
        "Set texture scaling filter mode"
    },
    {
        "set-texture-wrap", cfun_SetTextureWrap,
        "(set-texture-wrap texture wrap)\n\n"
        "Set texture wrapping mode"
    },
    {
        "load-image-from-memory", cfun_LoadImageFromMemory,
        "(load-image-from-memory filetype data size)\n\n"
        "Load image from memory buffer, fileType refers to extension: i.e. '.png'"
    },
    {NULL, NULL, NULL}
};
