#include "gl/gl_framebuffer.h"
#include "gl/gl_common.h"
#include "core/log.h"

u32 created_framebuffers = 0;
u32 deleted_framebuffers = 0;

gl_framebuffer gl_framebuffer_create(u32 w, u32 h)
{
  gl_framebuffer fb = {0};

  fb.width = w;
  fb.height = h;
  fb.attachment_count = 0;
  fb.has_depth = false;

  created_framebuffers++;

  glGenFramebuffers(1, &fb.id);
  return fb;
}

static bool gl_framebuffer_attachment_add(gl_framebuffer *fb, u32 internal_format, u32 data_format, u32 type, u32 attachment)
{
  if (fb->attachment_count + 1 > GL_FRAMEBUFFER_MAX_ATTACHMENTS)
  {
    log_error("max number of attachments on framebuffer");
    return false;
  }
  u32 a = fb->attachment_count++;

  fb->attachments[a].internal_format = internal_format;
  fb->attachments[a].data_format = data_format;
  fb->attachments[a].type = type;
  fb->attachments[a].attachment = attachment;
  return true;
}

bool gl_framebuffer_texture_attachment_add(gl_framebuffer *fb, u32 color_slot)
{
  return gl_framebuffer_attachment_add(fb, GL_RGBA8, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0 + color_slot);
}

bool gl_framebuffer_depth_attachment_add(gl_framebuffer *fb)
{
  fb->has_depth = true;
  fb->depth_attachment.internal_format = GL_DEPTH24_STENCIL8;
  fb->depth_attachment.data_format = GL_DEPTH_STENCIL;
  fb->depth_attachment.type = GL_UNSIGNED_INT_24_8;
  fb->depth_attachment.attachment = GL_DEPTH_STENCIL_ATTACHMENT;
  return true;
}

bool gl_framebuffer_invalidate(gl_framebuffer *fb)
{
  gl_framebuffer_bind(fb);

  // free all textures
  for (i32 i = 0; i < fb->attachment_count; i++)
  {
    gl_texture_free(&fb->attachments[i].texture);
  }

  // regenerate all textures
  for (i32 i = 0; i < fb->attachment_count; i++)
  {
    fb->attachments[i].texture = gl_texture_create_fb(fb->width, fb->height, fb->attachments[i].internal_format, fb->attachments[i].data_format, fb->attachments[i].type);
    glFramebufferTexture2D(GL_FRAMEBUFFER, fb->attachments[i].attachment, GL_TEXTURE_2D, fb->attachments[i].texture.id, 0);
  }

  // depth texture
  if (fb->has_depth)
  {
    fb->depth_attachment.texture = gl_texture_create_fb(fb->width, fb->height, fb->depth_attachment.internal_format, fb->depth_attachment.data_format, fb->depth_attachment.type);
    glFramebufferTexture2D(GL_FRAMEBUFFER, fb->depth_attachment.attachment, GL_TEXTURE_2D, fb->depth_attachment.texture.id, 0);
  }

  u32 attachments[GL_FRAMEBUFFER_MAX_ATTACHMENTS];
  for (i32 i = 0; i < GL_FRAMEBUFFER_MAX_ATTACHMENTS; i++)
    attachments[i] = GL_COLOR_ATTACHMENT0 + i;
  glDrawBuffers(fb->attachment_count, attachments);

  return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void gl_framebuffer_resize(gl_framebuffer *fb, u32 w, u32 h)
{
  bool resize = (fb->width != w) || (fb->height != h);

  if (resize)
  {
    fb->width = w;
    fb->height = h;
    gl_framebuffer_invalidate(fb);
  }
}

void gl_framebuffer_clear(gl_framebuffer *fb)
{
  gl_framebuffer_bind(fb);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

u32 bound_fb = 0;
void gl_framebuffer_bind(gl_framebuffer *fb)
{
  if (fb->id == bound_fb)
    return;

  glBindFramebuffer(GL_FRAMEBUFFER, fb->id);
  bound_fb = fb->id;
}

void gl_framebuffer_unbind()
{
  if (0 == bound_fb)
    return;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  bound_fb = 0;
}

void gl_framebuffer_free(gl_framebuffer *fb)
{
  // free all textures
  for (i32  i = 0; i < fb->attachment_count; i++)
  {
    gl_texture_free(&fb->attachments[i].texture);
  }

  if (fb->has_depth)
    gl_texture_free(&fb->depth_attachment.texture);

  glDeleteFramebuffers(1, &fb->id);
  deleted_framebuffers++;
}

u32 gl_get_active_framebuffers()
{
  return created_framebuffers - deleted_framebuffers;
}