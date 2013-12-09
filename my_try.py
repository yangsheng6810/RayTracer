import bpy
from mathutils import Vector
import time
import librender as L
C = bpy.context
D = bpy.data


def callback():
    print("success!")

def addCamera():
    camera = D.cameras[C.scene.camera.name]
    origin = Vector().to_4d()
    v2, v4, v3, v1 = camera.view_frame(bpy.context.scene)
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

    L.add_camera(origin, p1, p2, p3, p4)

def addObject(scene, ob):
    mesh = ob.to_mesh(scene, True, 'RENDER')
    matrix = ob.matrix_world
    L.add_object()
    for v in mesh.vertices:
        L.add_vertice(matrix * v.co, matrix * v.normal)
    for p in mesh.polygons:
        p_vertices = p.vertices
        size = len(p_vertices)
        for j in range(1, size - 1):
            L.add_face(p_vertices[0], p_vertices[j+1], p_vertices[j])
    L.finish_object()

def setParameters(addon):
    L.set_resolution(addon.size_x, addon.size_y)

def is_renderable(scene, ob):
    return (ob.is_visible(scene) and not ob.hide_render \
            and ob.type not in {'LAMP', 'CAMERA', 'EMPTY', 'META', 'ARMATURE', 'LATTICE'})

def renderable_objects(scene):
    return [ob for ob in scene.objects if is_renderable(scene, ob)]


def addObjects(addon, context, data):
    objs = renderable_objects(context.scene)
    for obj in objs:
        addObject(context.scene, obj)

def render(addon, context, data):
    print("here!")
    C = context
    D = data
    L.new_scene(addon)
    #L.new_scene(addon.update_tile)
    addCamera()
    #librender.set_python_callback(callback)
    setParameters(addon)
    L.set_sample(10)
    addon.thread_num = L.get_thread_num()
    addObjects(addon, context, data)
    L.render_scene()
    #time.sleep(30)
    #L.stop_render()

def	main():
	render(bpy.context, bpy.data)

def stopRender():
    L.stop_render()

if __name__ == "__main__":
    main()
