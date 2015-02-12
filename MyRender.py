# -*- coding: utf-8 -*-
import bpy
from mathutils import Vector
import time
from imp import reload
from sys import getrefcount
from ctypes import *

def get_point(point):
    return self.L.point3_new_xyz(point.x, point.y, point.z)

def get_vect(vect):
    return self.L.vector3_new_xyz(vect.x, vect.y, vect.z)

def get_color(color):
    return self.L.color_new_rgb(color.r, color.g, color.b)

def get_material(material):
    if materian == None:
        return self.L.material_new_parameters(
            self.L.color_new_shade(0.8),
            0.05,
            self.L.color_new_shade(0.9),
            0.1,
            100,
            False,
            False)
    else:
        return self.L.material_new_parameters(
            get_color(material.diffuse_color),
            material.diffuse_intensity,
            get_color(material.specular_color),
            material.specular_intensity,
            material.specular_hardness,
            material.raytrace_mirror.use,
            material.use_transparency,
            get_color(material.emit))

class MyRender():
    def __init__(self):
        self.L = cdll.LoadLibrary('./librender.so')
        self.scene = None
        self.C = bpy.context
        self.D = bpy.data

    def __del__(self):
        print("in MyRender del");
        # the following line need to change!
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

        self.L.scene_add_camera(
            self.scene,
            get_point(origin),
            get_point(p1 - origin),
            get_point(p2 - origin),
            get_point(p3 - origin),
            get_point(p4 - origin))

    def addObject(self, scene, ob):
        mesh = ob.to_mesh(scene, True, 'RENDER')
        matrix = ob.matrix_world
        self.L.scene_add_object(self.scene)
        for material in mesh.materials:
            self.L.scene_add_material(
                self.scene,
                get_material(material))
        self.L.scene_add_material(
            self.scene,
            get_material(None))
        for v in mesh.vertices:
            self.L.scene_add_vertice(
                self.scene,
                get_point(matrix * v.co),
                get_vect(matrix * v.normal));
        for p in mesh.polygons:
            p_vertices = p.vertices
            size = len(p_vertices)
            for j in range(1, size - 1):
                self.L.scene_add_face(
                    self.scene,
                    p_vertices[0], p_vertices[j+1], p_vertices[j], p.material_index)
        self.L.scene_finish_object(self.scene)

    def addLamp(self, scene, lamp):
        location = lamp.location
        data = lamp.data
        matrix = lamp.matrix_world.to_quaternion()
        v = Vector()
        v.z = -1.0
        # print(location)
        direction = matrix * v
        if data.type == 'SPOT':
            self.L.scene_add_lamp(
                self.scene,
                get_point(location),
                get_vect(direction),
                data.distance / 10,
                get_color(data.color),
                data.energy,
                data.spot_size)

    def setParameters(self, addon):
        pass
        self.L.secne_set_resolution(
            self.scene, addon.size_x, addon.size_y)

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
        # the following line may need change, according to render1.c
        self.scene = self.L.new_scene(addon)
        self.addCamera()
        self.setParameters(addon)
        self.L.scene_set_sample(
            self.scene,
            sample_number)
        addon.thread_num = self.L.scene_get_thread_num(self.scene)
        self.addObjects(addon, context, data)
        self.addLamps(addon, context, data)
        print("before calling rendering!")
        self.L.render_scene(self.scene)
        print("after calling rendering!")

    def stopRender(self):
        # may need to check whether valid!
        self.L.scene_stop_all_threads(self.scene)

