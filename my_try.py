import bpy
from mathutils import Vector
import time
from imp import reload
from sys import getrefcount

class MyRender():
    def __init__(self, librender):
        # print("ref count of library is ", getrefcount(librender))
        self.L = librender
        # print("ref count of library is ", getrefcount(self.L))
        self.C = bpy.context
        self.D = bpy.data

    def __del__(self):
        print("in MyRender del");
        self.L.clear_scene()
        self.L = None

    def callback(self):
        print("success!")

    def addCamera(self):
        camera = self.D.cameras[self.C.scene.camera.name]
        origin = Vector().to_4d()
        v2, v4, v3, v1 = camera.view_frame(bpy.context.scene)
        m = self.C.scene.camera.matrix_world
        origin = (m * origin).to_3d()
        p1 = m * v1
        p2 = m * v2
        p3 = m * v3
        p4 = m * v4

        self.L.add_camera(origin, p1, p2, p3, p4)

    def addObject(self, scene, ob):
        mesh = ob.to_mesh(scene, True, 'RENDER')
        matrix = ob.matrix_world
        self.L.add_object()
        for material in mesh.materials:
            self.L.add_material(material)
        self.L.add_material(None)
        for v in mesh.vertices:
            self.L.add_vertice(matrix * v.co, matrix * v.normal)
        for p in mesh.polygons:
            p_vertices = p.vertices
            size = len(p_vertices)
            for j in range(1, size - 1):
                self.L.add_face(p_vertices[0], p_vertices[j+1], p_vertices[j], p.material_index)
        self.L.finish_object()

    def addLamp(self, scene, lamp):
        location = lamp.location
        data = lamp.data
        matrix = lamp.matrix_world
        v = Vector()
        v.z = 1.0
        direction = -(matrix * v)
        if data.type == 'SPOT':
            self.L.add_lamp(location, direction, data.color, data.energy, data.spot_size)

    def setParameters(self, addon):
        pass
        self.L.set_resolution(addon.size_x, addon.size_y)

    def is_renderable(self, scene, ob):
        return (ob.is_visible(scene) and not ob.hide_render \
                and ob.type not in {'LAMP', 'CAMERA', 'EMPTY', 'META', 'ARMATURE', 'LATTICE'})

    def renderable_objects(self, scene):
        return [ob for ob in scene.objects if self.is_renderable(scene, ob)]


    def addObjects(self, addon, context, data):
        objs = self.renderable_objects(context.scene)
        for obj in objs:
            self.addObject(context.scene, obj)

    def is_light(self, scene, ob):
        return (ob.is_visible(scene) and not ob.hide_render and ob.type in {'LAMP'})

    def addLamps(self, addon, context, data):
        lamps = [l for l in context.scene.objects if self.is_light(context.scene, l)]
        for l in lamps:
            self.addLamp(context.scene, l)

    def render(self, addon, context, data, sample_number):
        print("here!")
        self.C = context
        self.D = data
        self.L.new_scene(addon)
        self.addCamera()
        self.setParameters(addon)
        self.L.set_sample(sample_number)
        addon.thread_num = self.L.get_thread_num()
        self.addObjects(addon, context, data)
        self.addLamps(addon, context, data)
        print("before calling rendering!")
        self.L.render_scene()
        print("after calling rendering!")

    def stopRender(self):
        self.L.stop_render()
