import bpy
import bpy.context as C
import bpy.data as D
import librender


def callback():
    print("success!")


def addCamera():
    camera = D.cameras[C.scene.camera.name]
    origin = C.scene.camera.location
    v1, v2, v3, v4 = camera.view_frame(C.scene)
    p1 = camera.matrix_world * v1
    p2 = camera.matrix_world * v2
    p3 = camera.matrix_world * v3
    p4 = camera.matrix_world * v4

    librender.add_camera(origin, p1, p2, p3, p4)


def main():
    librender.new_scene()
    addCamera()
    # librender.set_python_callback(callback)
    librender.render_scene()


main()
