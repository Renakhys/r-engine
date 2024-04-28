#pragma once
#include <app/application.h>
#include <app/application_layer.h>

typedef struct
{
  const char *name;
} main_menu_layer_data;

application_layer* create_main_menu_layer(application *app, const char *name);
