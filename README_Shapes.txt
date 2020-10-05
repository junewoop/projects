0. Implementation details
The seven classes Cube, Cone, Sphere, Cylinder, Cap, Slant, and Barrel inherit the Shape class. 

Cap corresponds to the cap parts of Cone and Cylinder. Slant and Barrel correspond to the respective parts of Cone and Cylinder respectively.

Cube, Cone, Cap, Slant, and Barrel have a member vector<float> m_vertexData which stores the vertex array after create_*_vertex_data() is called in the constructor body.

For instance, create_cube_vertex_data() stores the vertex positions and its normals in the interleaving manner.

Cube, Cone, Cap, Slant, and Barrel inherit buildVAO() (which builds a VAO using m_vertexData() and draw() which can be called to draw the shape that each class name describes.

Cone is a composition of one Cap and one Slant, and Cylinder is a composition of two Caps and one Barrel.

Cone and Cylinder only overrides draw() which call draw() of their classes associated with the respective parts.

For instance, Cone::draw() calls m_cap->draw() and m_slant->draw().

Hence, I did not have to modify ShapesScene::renderGeometry().

1. Acknowledgment

I did not use insertVec3() method, but kept track of the position of the m_vertexData using the reference of the index variable ind which create_*_vertex_data() is updating.