import bpy
from mathutils import Vector
import time
import librender
C = bpy.context
D = bpy.data


def callback():
    print("success!")

def addCamera():
    camera = D.cameras[C.scene.camera.name]
    origin = Vector().to_4d()
    v2, v4, v3, v1 = camera.view_frame(C.scene)
    m = C.scene.camera.matrix_world
    origin = (m * origin).to_3d()
    p1 = m * v1
    p2 = m * v2
    p3 = m * v3
    p4 = m * v4
    #print(origin)
    #print(p1)
    #print(p2)
    #print(p3)
    #print(p4)

    librender.add_camera(origin, p1, p2, p3, p4)

def setResolution():
    scene = D.scenes[C.scene.name]
    librender.set_resolution(scene.render.resolution_x, scene.render.resolution_y)

def	main():
    librender.new_scene()
    addCamera()
    #librender.set_python_callback(callback)
    setResolution()
    librender.render_scene()
    #time.sleep(30)
    #librender.stop_render()

def stopRender():
    librender.stop_render()

main()
