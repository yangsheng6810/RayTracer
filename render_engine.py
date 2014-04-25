bl_info = {
    "name": "Yang",
    "category": "Render",
}

import bpy
import time
import os
import sys
from imp import reload
sys.path.append('/home/yangsheng/Documents/Homework/Graphics/3/Mine/RayTracer-try-build')
from my_try import MyRender
import librender

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
        self.render_engine = MyRender(librender)
        self.finished = False

    # This is the only method called by blender, in this example
    # we use it to detect preview rendering and call the implementation
    # in another method.
    def render(self, scene):
        scale = scene.render.resolution_percentage / 100.0
        self.size_x = int(scene.render.resolution_x * scale)
        self.size_y = int(scene.render.resolution_y * scale)

        if scene.name == 'preview':
            print("preview")
            return
        #    self.render_preview(scene)
        else:
            print("render")
        #    self.render_scene(scene)

        self.render_engine.render(self, bpy.context, bpy.data, 100)
        print("before sleep")
        while True:
            if self.test_break():
                print("before calling to sleep")
                self.report({'INFO'}, "Trying to stop")
                self.render_engine.stopRender()
                self.report({'INFO'}, "Stoped")
                break
            if self.finished:
                self.report({'INFO'}, "Succeed")
                break
            time.sleep(1)
        time.sleep(1)
        print("after sleep")
        del self.render_engine
        print("after deleting my_try")


    # callback function
    def update_tile(self, x_start, y_start, width, height):
        result = self.begin_result(x_start, self.size_y - y_start - height, width, height)
        file_str = "/tmp/yang-blender/%d-%d.png" % (x_start, y_start)
        #file_str = "/tmp/yang-blender/%d-%d.tiff" % (x_start, y_start)
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
        self.update_progress(self.thread_count / self.thread_num)
        #if self.thread_count >= self.thread_num:
        #    self.finished = True

    def success(self):
        self.finished = True

    def update(self, data, scene):
        print("in update")

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
from bl_ui import properties_material
#properties_material.MATERIAL_PT_preview.COMPAT_ENGINES.add('custom_renderer')
properties_material.MATERIAL_PT_context_material.COMPAT_ENGINES.add('custom_renderer')
properties_material.MATERIAL_PT_diffuse.COMPAT_ENGINES.add('custom_renderer')
properties_material.MATERIAL_PT_specular.COMPAT_ENGINES.add('custom_renderer')
properties_material.MATERIAL_PT_shading.COMPAT_ENGINES.add('custom_renderer')
properties_material.MATERIAL_PT_transp.COMPAT_ENGINES.add('custom_renderer')
del properties_material
