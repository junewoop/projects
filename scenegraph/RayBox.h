#ifndef RAYBOX_H
#define RAYBOX_H

#endif // RAYBOX_H
#include "Scene.h"
#include <algorithm>
#include <utility>

struct ray {
    ray() : p(glm::vec4()), d(glm::vec4()) {}
    ray(glm::vec4 a, glm::vec4 b) : p(a), d(b){}
    glm::vec4 p;
    glm::vec4 d;
};

struct intsct{
    intsct() : t(INFINITY), i(-1), u(0.f), v(0.f){}
    intsct(float a, int b, float c, float d) : t(a), i(b), u(c), v(d){}
    float t;
    int i;
    float u;
    float v;
};

static std::vector<glm::vec4> CHARPOINTS{
    glm::vec4(0.6f, 0.6f, 0.6f, 1.f),
    glm::vec4(0.6f, 0.6f, -0.6f, 1.f),
    glm::vec4(0.6f, -0.6f, 0.6f, 1.f),
    glm::vec4(0.6f, -0.6f, -0.6f, 1.f),
    glm::vec4(-0.6f, 0.6f, 0.6f, 1.f),
    glm::vec4(-0.6f, 0.6f, -0.6f, 1.f),
    glm::vec4(-0.6f, -0.6f, 0.6f, 1.f),
    glm::vec4(-0.6f, -0.6f, -0.6f, 1.f)
};

struct AABB{
    AABB(): x_min(INFINITY), x_max(-INFINITY), y_min(INFINITY), y_max(-INFINITY), z_min(INFINITY), z_max(-INFINITY),
    prim_index(-1), visited(false) {}
    AABB(float a, float b, float c, float d, float e, float f, int i): x_min(a), x_max(b), y_min(c), y_max(d), z_min(e), z_max(f),
    prim_index(i), visited(false) {}
    AABB(glm::mat4x4 &transform): AABB(){
        glm::vec4 p = glm::vec4();
        for (int i = 0; i < 8; i ++){
            p = transform * CHARPOINTS[i];
            x_min = std::min(p.x, x_min);
            y_min = std::min(p.y, y_min);
            z_min = std::min(p.z, z_min);
            x_max = std::max(p.x, x_max);
            y_max = std::max(p.y, y_max);
            z_max = std::max(p.z, z_max);
        }
    }
    float x_min;
    float x_max;
    float y_min;
    float y_max;
    float z_min;
    float z_max;
    int prim_index;
    bool visited;
    std::pair<float, float> intersectBox(ray &one_ray){
        float t_min = INFINITY;
        float t_max = -INFINITY;
        float t_tmp;
        if (one_ray.d.y != 0){
            t_tmp = (y_min-one_ray.p.y)/one_ray.d.y;
            if (t_tmp*one_ray.d.x + one_ray.p.x <= x_max + 0.000015f &
                    t_tmp*one_ray.d.x + one_ray.p.x >= x_min - 0.000015f &
                    t_tmp*one_ray.d.z + one_ray.p.z <= z_max + 0.000015f &
                    t_tmp*one_ray.d.z + one_ray.p.z >= z_min - 0.000015f){
                t_min = std::min(t_min, t_tmp);
                t_max = std::max(t_max, t_tmp);
            }
            t_tmp = (y_max - one_ray.p.y)/one_ray.d.y;
            if (t_tmp*one_ray.d.x + one_ray.p.x <= x_max + 0.000015f &
                    t_tmp*one_ray.d.x + one_ray.p.x >= x_min - 0.000015f &
                    t_tmp*one_ray.d.z + one_ray.p.z <= z_max + 0.000015f &
                    t_tmp*one_ray.d.z + one_ray.p.z >= z_min - 0.000015f){
                t_min = std::min(t_min, t_tmp);
                t_max = std::max(t_max, t_tmp);
            }
        }
        if (one_ray.d.z != 0){
            t_tmp = (z_min-one_ray.p.z)/one_ray.d.z;
            if (t_tmp*one_ray.d.y + one_ray.p.y <= y_max + 0.000015f &
                    t_tmp*one_ray.d.y + one_ray.p.y >= y_min - 0.000015f &
                    t_tmp*one_ray.d.x + one_ray.p.x <= x_max + 0.000015f &
                    t_tmp*one_ray.d.x + one_ray.p.x >= x_min - 0.000015f){
                t_min = std::min(t_min, t_tmp);
                t_max = std::max(t_max, t_tmp);
            }
            t_tmp = (z_max - one_ray.p.z)/one_ray.d.z;
            if (t_tmp*one_ray.d.y + one_ray.p.y <= y_max + 0.000015f &
                    t_tmp*one_ray.d.y + one_ray.p.y >= y_min - 0.000015f &
                    t_tmp*one_ray.d.x + one_ray.p.x <= x_max + 0.000015f &
                    t_tmp*one_ray.d.x + one_ray.p.x >= x_min - 0.000015f){
                t_min = std::min(t_min, t_tmp);
                t_max = std::max(t_max, t_tmp);
            }
        }
        if (one_ray.d.x != 0){
            t_tmp = (x_min-one_ray.p.x)/one_ray.d.x;
            if (t_tmp*one_ray.d.z + one_ray.p.z <= z_max + 0.000015f &
                    t_tmp*one_ray.d.z + one_ray.p.z >= z_min - 0.000015f &
                    t_tmp*one_ray.d.y + one_ray.p.y <= y_max + 0.000015f &
                    t_tmp*one_ray.d.y + one_ray.p.y >= y_min - 0.000015f){
                t_min = std::min(t_min, t_tmp);
                t_max = std::max(t_max, t_tmp);
            }
            t_tmp = (x_max - one_ray.p.x)/one_ray.d.x;
            if (t_tmp*one_ray.d.z + one_ray.p.z <= z_max + 0.000015f &
                    t_tmp*one_ray.d.z + one_ray.p.z >= z_min - 0.000015f &
                    t_tmp*one_ray.d.y + one_ray.p.y <= y_max + 0.000015f &
                    t_tmp*one_ray.d.y + one_ray.p.y >= y_min - 0.000015f){
                t_min = std::min(t_min, t_tmp);
                t_max = std::max(t_max, t_tmp);
            }
        }
        if (t_max < 0) return std::make_pair(INFINITY, -INFINITY);
        return std::make_pair(std::max(t_min, 0.f), t_max);
    }
};

inline bool compareX(AABB *b_1, AABB *b_2){
    return b_1->x_min < b_2->x_min;
}

inline bool compareY(AABB *b_1, AABB *b_2){
    return b_1->y_min < b_2->y_min;
}

inline bool compareZ(AABB *b_1, AABB *b_2){
    return b_1->z_min < b_2->z_min;
}

enum DivideAxis {X, Y, Z};

struct KDNode : AABB{
    KDNode(): AABB(), is_leaf(false), axis(X), child_1(nullptr), child_2(nullptr),
        numBox(0), boxes(nullptr), boundary(0.f), trial(0) {}
    KDNode(float a, float b, float c, float d, float e, float f): AABB(a, b, c, d, e, f, -1),
        is_leaf(false), axis(X), child_1(nullptr), child_2(nullptr),
        numBox(0), boxes(nullptr), boundary(0.f), trial(0) {}
    bool is_leaf;
    DivideAxis axis;
    KDNode *child_1;
    KDNode *child_2;
    int numBox;
    AABB **boxes;
    float boundary;
    int trial;
    void recurSplitKDTree(){
        if ((numBox <= 5) || (trial == 3)){
            is_leaf = true;
            return;
        }
        float c_tmp = 0.f, c_best = INFINITY;
        int i_best = -1;
        int j_1 = 0, j_2 = 0;
        AABB** left = new AABB*[numBox];
        AABB** right = new AABB*[numBox];
        float tmp_boundary;
        switch (axis) {
        case X: {
            std::sort(boxes, boxes + numBox, compareX);
            for (int i = 0; i < numBox; i++){
                tmp_boundary = boxes[i]->x_min;
                if (tmp_boundary < x_min) continue;
                c_tmp = i*(tmp_boundary - x_min) + (numBox-i)*(x_max - tmp_boundary);
                if (c_tmp < c_best){
                    c_best = c_tmp;
                    i_best = i;
                }
            }
            if (i_best == -1){
                delete[] left;
                delete[] right;
                axis = Y;
                trial++;
                recurSplitKDTree();
                return;
            }
            boundary = boxes[i_best]->x_min;
            for (int i = 0; i < numBox; i++){
                if (boxes[i]->x_min <= boundary)
                    left[j_1++] = boxes[i];
                if (boxes[i]->x_max >= boundary)
                    right[j_2++] = boxes[i];
            }
            if ((j_1 == numBox) || (j_2 == numBox)){
                delete[] left;
                delete[] right;
                axis = Y;
                trial++;
                recurSplitKDTree();
                return;
            }
            child_1 = new KDNode(x_min, boundary, y_min, y_max, z_min, z_max);
            child_2 = new KDNode(boundary, x_max, y_min, y_max, z_min, z_max);
            child_1->axis = Y;
            child_2->axis = Y;
            child_1->boxes = left;
            child_2->boxes = right;
            child_1->numBox = j_1;
            child_2->numBox = j_2;
            child_1->recurSplitKDTree();
            child_2->recurSplitKDTree();
            return;
        }
        case Y: {
            std::sort(boxes, boxes + numBox, compareY);
            for (int i = 0; i < numBox; i++){
                tmp_boundary = boxes[i]->y_min;
                if (tmp_boundary < y_min) continue;
                c_tmp = i*(tmp_boundary - y_min) + (numBox-i)*(y_max - tmp_boundary);
                if (c_tmp < c_best){
                    c_best = c_tmp;
                    i_best = i;
                }
            }
            if (i_best == -1){
                delete[] left;
                delete[] right;
                axis = Z;
                trial++;
                recurSplitKDTree();
                return;
            }
            boundary = boxes[i_best]->y_min;
            for (int i = 0; i < numBox; i++){
                if (boxes[i]->y_min <= boundary)
                    left[j_1++] = boxes[i];
                if (boxes[i]->y_max >= boundary)
                    right[j_2++] = boxes[i];
            }
            if ((j_1 == numBox) || (j_2 == numBox)){
                delete[] left;
                delete[] right;
                axis = Z;
                trial++;
                recurSplitKDTree();
                return;
            }
            child_1 = new KDNode(x_min, x_max, y_min, boundary, z_min, z_max);
            child_2 = new KDNode(x_min, x_max, boundary, y_max, z_min, z_max);
            child_1->axis = Z;
            child_2->axis = Z;
            child_1->boxes = left;
            child_2->boxes = right;
            child_1->numBox = j_1;
            child_2->numBox = j_2;
            child_1->recurSplitKDTree();
            child_2->recurSplitKDTree();
            return;
        }
        case Z: {
            std::sort(boxes, boxes + numBox, compareZ);
            for (int i = 0; i < numBox; i++){
                tmp_boundary = boxes[i]->z_min;
                if (tmp_boundary < z_min) continue;
                c_tmp = i*(tmp_boundary - z_min) + (numBox-i)*(z_max - tmp_boundary);
                if (c_tmp < c_best){
                    c_best = c_tmp;
                    i_best = i;
                }
            }
            if (i_best == -1){
                delete[] left;
                delete[] right;
                axis = X;
                trial++;
                recurSplitKDTree();
                return;
            }
            boundary = boxes[i_best]->z_min;
            for (int i = 0; i < numBox; i++){
                if (boxes[i]->z_min <= boundary)
                    left[j_1++] = boxes[i];
                if (boxes[i]->z_max >= boundary)
                    right[j_2++] = boxes[i];
            }
            if ((j_1 == 0) || (j_2 == 0) || (j_1 == numBox) || (j_2 == numBox)){
                delete[] left;
                delete[] right;
                axis = X;
                trial++;
                recurSplitKDTree();
                return;
            }
            child_1 = new KDNode(x_min, x_max, y_min, y_max, z_min, boundary);
            child_2 = new KDNode(x_min, x_max, y_min, y_max, boundary, z_max);
            child_1->axis = X;
            child_2->axis = X;
            child_1->boxes = left;
            child_2->boxes = right;
            child_1->numBox = j_1;
            child_2->numBox = j_2;
            child_1->recurSplitKDTree();
            child_2->recurSplitKDTree();
            return;
        }
        }
    }
};
