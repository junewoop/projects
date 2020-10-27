0. Design choices
  In the Scene class, parse() is used to extract information about the scenegraph from its input parser. 
It stores extracted information in member variables m_numLights, m_numPrims, m_global, m_lightData,
m_materials, m_types, and m_transformations. 

  I added member variables m_cube, m_cone, m_cylinder, and m_sphere to the class SceneviewScene. Corresponding
shapes are initialized and assigned to these variables when the constructor method calls settingschanged().
Shape parameters from settings are used to initialize these shapes, so users can adjust shape parameters
to see how scene varies. I also decided a lower limit of these parameters to be 10, so that the resulting
scene is not too coarse.

  Lastly, but most importantly, the pre-initialized shapes are reused in renderGeometry(), so that the function
does not have to initialize each shape instance of the primitive objects in the scenegraph. This is allowed since
positions, shapes, and orientiations of pre-transformed primitive objects only depend on their shape types. 
This will significantly optimize computation timewise and memory-wise, especially when there are hundreds of
primitive objects in a scenefile.

1. addPrimitiveDFS()
  addPrimitiveDFS() is a function of Scene that can be used to extract information about primitive objects in 
the scenegraph from the root of the scenegraph. It simulates depth first search on the scenegraph, so takes 
the current node and transformation matrix for the parent node as its input. It first computes a transformation
that should be applied to primitive objects attached to the current node. Then, it stores all such primitive
objects and the computed transformation by calling addPrimitive(). Lastly, it calls recursively on its child
node, so that the search is continued. Since every node is visited exactly once, addPrimitiveDFS() enables
to store the scenegraph in linear time with respect to the number of nodes.