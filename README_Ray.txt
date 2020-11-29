0. Design decisions
I created a separate header Raybox.h with data definitions needed for raytracing and kd-tree acceleration.
struct ray and intsct include information needed for raytracing. struct AABB and KDNode include information
needed for kd-tree acceleration. AABB has a method that computes intersection points with a given input ray.
KDNode has a method that splits itself into two children nodes according to the construction algorithm of
kd-trees.

In RayScene class, recursiveLight() computes illumination recursively while considering the number of additional 
reflections to add. lightingAt() computes all lighting components and adds them according to the components
needed specified by settings. Specifically, lightingAt() handles whether to use shadow and texture or not.
Whether to use kd-trees or reflections are handled in draw(). kd-tree is created by constructKDTree() and
deleted by deleteNode(). intsersectKDTree() traverse the kd-tree until it finds the closest intersection, if
it exists.

1. Reflection Depth
In Ray tab, reflection depth slider and textbox are added. These are enabled if and only if reflection box
is checked, and a user can set reflection depth from 1 to 200, inclusive, when reflection they are enabled.
Also, reflection depth can be hardcoded in line 550 of RayScene.cpp. Just uncomment the line and enter reflection
depth to <ENTER VALUE HERE>.

2. Extra credit - kd-tree
I implemented a kd-tree with O(nlog^2n) construction time. However, the traversal time was the bottle neck 
to the raytracing running time. It took about 1 minute to render recursiveSpheres6.xml with shadows, reflections,
and textures while construction took only 32 milliseconds.