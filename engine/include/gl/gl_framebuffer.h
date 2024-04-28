#pragma once
#include "common.h"
#include "gl_texture.h"

#define GL_FRAMEBUFFER_MAX_ATTACHMENTS 5

typedef struct
{
  gl_texture texture;
  u32 internal_format;
  u32 data_format;
  u32 type;
  u32 attachment;
} gl_framebuffer_attachment;

typedef struct
{
  u32 id;
  u32 width;
  u32 height;
  u32 attachment_count;
  gl_framebuffer_attachment attachments[GL_FRAMEBUFFER_MAX_ATTACHMENTS];
  bool has_depth;
  gl_framebuffer_attachment depth_attachment;
} gl_framebuffer;

gl_framebuffer gl_framebuffer_create(u32 w, u32 h);

bool gl_framebuffer_texture_attachment_add(gl_framebuffer *fb, u32 color_slot);

bool gl_framebuffer_depth_attachment_add(gl_framebuffer *fb);

bool gl_framebuffer_invalidate(gl_framebuffer *fb);

void gl_framebuffer_resize(gl_framebuffer* fb, u32 w, u32 h);

void gl_framebuffer_clear(gl_framebuffer *fb);

void gl_framebuffer_bind(gl_framebuffer *fb);

void gl_framebuffer_unbind();

void gl_framebuffer_free(gl_framebuffer *fb);

u32 gl_get_active_framebuffers();