#pragma once
#include "common.h"
#include "cstring.h"
#include <errno.h>

typedef i32 Errno;

Errno write_file(const char *file_path, const char *buf, size_t buf_size);

Errno read_file(const char *file_path, cstring *content);
