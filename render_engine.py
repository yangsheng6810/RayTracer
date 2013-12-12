bl_info = {
    "name": "Yang",
    "category": "Render",
}

import bpy
import time
import os

class CustomRenderEngine(bpy.types.RenderEngine):
    # These three members are used by blender to set up the
    # RenderEngine; define its internal name, visible name and capabilities.
    bl_idname = 'custom_renderer'
    bl_label = 'Flat Color Renderer'
    bl_use_preview = True
    render_engine = ''
    def __init__(self):
        print("in init")
        self.thread_num = 0;
        self.thread_count = 0;
        import sys
        from imp import reload
        sys.path.append('/home/yangsheng/Documents/Homework/Graphics/3/Mine/RayTracer-build')
        import my_try
        self.render_engine = my_try
        self.finished = False

    def import_library(self):
        pass

    # This is the only method called by blender, in this example
    # we use it to detect preview rendering and call the implementation
    # in another method.
    def render(self, scene):
        self.import_library()
        scale = scene.render.resolution_percentage / 100.0
        self.size_x = int(scene.render.resolution_x * scale)
        self.size_y = int(scene.render.resolution_y * scale)

        if scene.name == 'preview':
            print("preview")
        #    self.render_preview(scene)
        else:
            print("render")
        #    self.render_scene(scene)
        #self.thread_num = self.render_engine.get_thread_num()
        self.render_engine.render(self, bpy.context, bpy.data)
        print("before sleep")
        while True:
            if self.test_break():
                print("before calling to sleep")
                self.report({'INFO'}, "Trying to stop")
                self.render_engine.stopRender()
                #result = self.begin_result(0, 0, self.size_x, self.size_y)
                #self.end_result(result)
                self.report({'INFO'}, "Stoped")
                break
            if self.finished:
                self.report({'INFO'}, "Succeed")
                break
            time.sleep(1)
        print("after sleep")


    # callback function
    def update_tile(self, x_start, y_start, width, height):
        #print("in callback function")
        #print("x_start is ", x_start)
        #print("y_start is ", y_start)
        #print("width is ", width)
        #print("height is ", height)
        result = self.begin_result(x_start, self.size_y - y_start - height, width, height)
        file_str = "/tmp/yang-blender/%d-%d.png" % (x_start, y_start)
        layer = result.layers[0]
        count = 0
        while True:
            if os.path.exists(file_str):
                try:
                    open(file_str)
                    break
                except IOError:
                    print("some error!")
                    pass
            count += 1
            if count > 10:
                break
            time.sleep(1)
        try:
            layer.load_from_file(file_str)
        except RuntimeError:
            print("runtime error in loading")
        self.end_result(result)
        self.thread_count += 1
        #print(self.thread_count / self.thread_num)
        self.update_progress(self.thread_count / self.thread_num)
        if self.thread_count == self.thread_num:
            self.finished = True

    # another callback
    def printing(self):
        print("Finally!")

    def update(self, data, scene):
        print("in update")

    # In this example, we fill the preview renders with a flat green color.
    def render_preview(self, scene):
        pixel_count = self.size_x * self.size_y

        # The framebuffer is defined as a list of pixels, each pixel
        # itself being a list of R,G,B,A values
        green_rect = [[0.0, 1.0, 0.0, 1.0]] * pixel_count

        # Here we write the pixel values to the RenderResult
        result = self.begin_result(0, 0, self.size_x, self.size_y)
        layer = result.layers[0]
        layer.rect = green_rect
        self.end_result(result)

    # In this example, we fill the full renders with a flat blue color.
    def render_scene(self, scene):
        pixel_count = self.size_x * self.size_y

        # The framebuffer is defined as a list of pixels, each pixel
        # itself being a list of R,G,B,A values
        blue_rect = [[0.0, 0.0, 1.0, 1.0]] * pixel_count

        # Here we write the pixel values to the RenderResult
        result = self.begin_result(0, 0, self.size_x, self.size_y)
        layer = result.layers[0]
        layer.rect = blue_rect
        self.end_result(result)

# Register the RenderEngine
def register():
    bpy.utils.register_class(CustomRenderEngine)
def unregister():
    bpy.utils.unregister_class(CustomRenderEngine)

# RenderEngines also need to tell UI Panels that they are compatible
# Otherwise most of the UI will be empty when the engine is selected.
# In this example, we need to see the main render image button and
# the material preview panel.
from bl_ui import properties_render
properties_render.RENDER_PT_render.COMPAT_ENGINES.add('custom_renderer')
properties_render.RENDER_PT_dimensions.COMPAT_ENGINES.add('custom_renderer')
properties_render.RENDER_PT_output.COMPAT_ENGINES.add('custom_renderer')
del properties_render

from bl_ui import properties_data_camera
properties_data_camera.DATA_PT_camera.COMPAT_ENGINES.add('custom_renderer')
properties_data_camera.DATA_PT_lens.COMPAT_ENGINES.add('custom_renderer')

from bl_ui import properties_material
properties_material.MATERIAL_PT_preview.COMPAT_ENGINES.add('custom_renderer')
properties_material.MATERIAL_PT_context_material.COMPAT_ENGINES.add('custom_renderer')
properties_material.MATERIAL_PT_diffuse.COMPAT_ENGINES.add('custom_renderer')
properties_material.MATERIAL_PT_specular.COMPAT_ENGINES.add('custom_renderer')
properties_material.MATERIAL_PT_transp.COMPAT_ENGINES.add('custom_renderer')
del properties_material
