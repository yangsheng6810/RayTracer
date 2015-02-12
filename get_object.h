#ifndef GET_OBJECT
#define GET_OBJECT

#define GET_SCENE_PTR(x) (*static_cast<boost::shared_ptr<Scene>*>(x))
#define GET_MATERIAL(x) (*static_cast<Material*>(x))
#define GET_MATERIAL_PTR(x) (boost::shared_ptr<Material>(static_cast<Material*>(x)))
#define GET_POINT3(x) (*static_cast<Point3*>(x))
#define GET_VECTOR3(x) (*static_cast<Vector3*>(x))
#define GET_COLOR(x) (*static_cast<Color*>(x))
#define WRAP(x) ((void *) (x))

#endif // GET_OBJECT

