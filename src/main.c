#include "basic_operators.h"

static void sharpen_rgba() {
  // TODO: do
}
int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: <input_image_path> <output_image.png>\n");
    return 2;
  }
  const char *in_path = argv[1];
  const char *out_path = argv[2];

  int w = 0, h = 0, channels_in_file = 0;

  uint8_t *img = stbi_load(in_path, &w, &h, &channels_in_file, 4);
  if (!img) {
    fprintf(stderr, "stbi_load failed: %s\n", stbi_failure_reason());
    return 1;
  }

  fprintf(stderr, "Loaded %s: %dx%d (file channels=%d, using RGBA)\n", in_path,
          w, h, channels_in_file);
  invert_rgba(img, w, h);

  int stride_in_bytes = w * 4;
  if (!stbi_write_png(out_path, w, h, 4, img, stride_in_bytes)) {
    fprintf(stderr, "stbi_write_png failed: %s\n", out_path);
    stbi_image_free(img);
    return 1;
  }

  fprintf(stderr, "Wrote %s\n", out_path);

  stbi_image_free(img);
  return 0;
}
