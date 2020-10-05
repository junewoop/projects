0. Implementation details

The seven classes Cube, Cone, Sphere, Cylinder, Cap, Slant, and Barrel inherit the Shape class. 

Cap corresponds to the cap parts of Cone and Cylinder. Slant and Barrel correspond to the respective parts of Cone and Cylinder respectively.

Cube, Cone, Cap, Slant, and Barrel have a member vector<float> m_vertexData which stores the vertex array when create_*_vertex_data() is called in the constructor body.

For instance, create_cube_vertex_data() stores the vertex positions and its normals of Cube in the interleaving manner.

Cube, Cone, Cap, Slant, and Barrel inherit Shape::buildVAO() (which builds a VAO using m_vertexData() and 

Shape::draw() which can be called to draw the shape that each class name describes.

Cone is a composition of one Cap and one Slant, and Cylinder is a composition of two Caps and one Barrel.

Cone and Cylinder only overrides Shape::draw() which call all draw() of their classes associated with the respective parts.

In other words, Cone::draw() calls m_cap->draw() and m_slant->draw(), and 

Cylinder::draw() calls m_cap_top->draw(), m_cap_bottom->draw(), and m_barrel->draw().

Hence, I did not have to modify ShapesScene::renderGeometry().