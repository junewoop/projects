0. Copy constructors
 My copy constructor for Scene copies light data and primitive object data from scene.
 My copy constructor for RayScene calls the copy constructor for Scene on scene and
 computes several more matrices such as world-to-object change of coordinate matrices and
 transformation matrices that convert object coordinate normal vectors to world coordinate 
 normal vectors. These will be used to increase efficiency.

1. Pipeline
When Mainwindow::renderImage() calls ui->canvas2D->renderImage(), Canvas2D::renderImage()
resizes and calls RayScene::draw(). RayScene::draw() confiugres canvas and camera information
by calling setCanvas() and setCamer(). For each pixel on canvas, createRay() is called to
create a ray that passes that pixel and intersect() is called on that ray.

intersect() computes t of closest intersection by calling intersectAt() for each primitive
object. intersectAt() computes t for given primitive object by converting the given ray
to a ray in object coordinate and calling intersect[Cone, Cube, Cylinder, Sphere]() depending 
on the object type. intersect[Cone, Cube, Cylinder, Sphere]() computes the intersection
of given ray and its corresponding standard object.

After looping through all primitive objects, intersect() knows which object given ray
is intersecting and the precise intersecting parameter. If there is an intersection,
then lightingAt() is called on the ray vector and the primitive object index.
lightingAt() first computes the world coordinate normal vector on the point of intersection
by calling normalAt(). Just like what intersectAt() does, normalAt() computes the 
(word-coordinate) normal vector on the intersection point by calling one of Normal[Cone, 
Cube, Cylinder, Sphere](). 

Back in LightingAt(), my implementation loops through the light data and add diffuse
componenets.

2. Bugs (Details to improve)
Loading scenefiles more than once requires resizing to be updated properly.
There are some pixelations, which are somewhat expected in regular implementation.
Speckles occur in complex scenefiles such as chess.xml, but this only occurs
within small microscopic region. I suspect this is caused by floating point compu-
tations.
Also, I could see some minor differences between rendered image and scene (cf. Piazza post @773)