#include "color.h"
#include "point3.h"
#include "vector3.h"
#include "material.h"
#include "scene.h"
#include "constants.h"
#include <stdio.h>

// NOTE!!! this scene_ptr need to modify to adapt to boost::shared_ptr
void *scene_ptr;

void print_color(){
	printf("come in print_color\n");
    color_print(color_new());
	void *p = color_new_rgb(0.4, 0.5, 0.8);
	color_print(p);
    color_print(color_new_pointer(p));
    color_print(color_new_double(0.4));
}

void add_object(){
	scene_add_object(scene_ptr, TRUE);
}

