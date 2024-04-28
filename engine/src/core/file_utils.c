#include "core/file_utils.h"
#include <stdlib.h>
#include <stdio.h>

static Errno file_size(FILE *file, size_t *size)
{
  long saved = ftell(file);
  if (saved < 0)
    return errno;
  if (fseek(file, 0, SEEK_END) < 0)
    return errno;
  long result = ftell(file);
  if (result < 0)
    return errno;
  if (fseek(file, saved, SEEK_SET) < 0)
    return errno;
  *size = (size_t)result;
  return 0;
}

Errno write_file(const char *file_path, const char *buf, size_t buf_size)
{
  FILE *f = NULL;

  f = fopen(file_path, "wb");
  if (f == NULL)
    return errno;

  fwrite(buf, 1, buf_size, f);
  if (ferror(f))
  {
    fclose(f);
    return errno;
  }

  fclose(f);
  return 0;
}

Errno read_file(const char *file_path, cstring *content)
{
  Errno result = 0;
  FILE *f = NULL;

  f = fopen(file_path, "r");
  if (f == NULL)
    return errno;

  size_t size;
  Errno err = file_size(f, &size);
  if (err != 0)
  {
    fclose(f);
    return errno;
  }

  cstring_resize(content, size + 1);
  fread(content->data, size, 1, f);

  if (ferror(f))
  {
    fclose(f);
    return errno;
  }
  content->lenght = size;
  content->data[content->lenght] = '\0';

  if (f)
    fclose(f);
  return result;
}
