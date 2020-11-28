#ifndef RAYDATA_H
#define RAYDATA_H

#endif // RAYDATA_H
#include "Scene.h"
#include <algorithm>
#include <iostream>

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


static std::vector<glm::vec4> CHARPOINTS{glm::vec4(-0.5f, 0.f, 0.f, 1.f),
            glm::vec4(0.5f, 0.f, 0.f, 1.f),
            glm::vec4(0.f, -0.5f, 0.f, 1.f),
            glm::vec4(0.f, 0.5f, 0.f, 1.f),
            glm::vec4(0.f, 0.f, -0.5f, 1.f),
            glm::vec4(0.f, 0.f, 0.5f, 1.f)};

struct AABB{
    AABB(): x_min(INFINITY), x_max(-INFINITY), y_min(INFINITY), y_max(-INFINITY), z_min(INFINITY), z_max(-INFINITY),
    prim_index(-1), visited(false) {}
    AABB(float a, float b, float c, float d, float e, float f, int i): x_min(a), x_max(b), y_min(c), y_max(d), z_min(e), z_max(f),
    prim_index(i), visited(false) {}
    AABB(glm::mat4x4 &inverse): AABB(){
        glm::vec4 p = glm::vec4();
        for (int i = 0; i < 6; i ++){
            p = inverse * CHARPOINTS[i];
            if(p.x < x_min) x_min = p.x;
            if(p.x > x_max) x_max = p.x;
            if(p.y < y_min) y_min = p.y;
            if(p.y > y_max) y_max = p.y;
            if(p.z < z_min) z_min = p.z;
            if(p.z > z_max) z_max = p.z;
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
    intsct intersectBox(ray &one_ray){
        float t = INFINITY;
        float t_tmp = t;
        if (one_ray.d.y != 0){
            t_tmp = (y_min-one_ray.p.y)/one_ray.d.y;
            if (t_tmp < t & t_tmp >= 0 &
                    t_tmp*one_ray.d.x + one_ray.p.x <= x_max + 0.000015f &
                    t_tmp*one_ray.d.x + one_ray.p.x >= x_min - 0.000015f &
                    t_tmp*one_ray.d.z + one_ray.p.z <= z_max + 0.000015f &
                    t_tmp*one_ray.d.z + one_ray.p.z >= z_min - 0.000015f)
                t = t_tmp;
            t_tmp = (y_max - one_ray.p.y)/one_ray.d.y;
            if (t_tmp < t & t_tmp >= 0 &
                    t_tmp*one_ray.d.x + one_ray.p.x <= x_max + 0.000015f &
                    t_tmp*one_ray.d.x + one_ray.p.x >= x_min - 0.000015f &
                    t_tmp*one_ray.d.z + one_ray.p.z <= z_max + 0.000015f &
                    t_tmp*one_ray.d.z + one_ray.p.z >= z_min - 0.000015f)
                t = t_tmp;
        }
        if (one_ray.d.z != 0){
            t_tmp = (z_min-one_ray.p.z)/one_ray.d.z;
            if (t_tmp < t & t_tmp >= 0 &
                    t_tmp*one_ray.d.y + one_ray.p.y <= y_max + 0.000015f &
                    t_tmp*one_ray.d.y + one_ray.p.y >= y_min - 0.000015f &
                    t_tmp*one_ray.d.x + one_ray.p.x <= x_max + 0.000015f &
                    t_tmp*one_ray.d.x + one_ray.p.x >= x_min - 0.000015f)
                t = t_tmp;
            t_tmp = (z_max - one_ray.p.z)/one_ray.d.z;
            if (t_tmp < t & t_tmp >= 0 &
                    t_tmp*one_ray.d.y + one_ray.p.y <= y_max + 0.000015f &
                    t_tmp*one_ray.d.y + one_ray.p.y >= y_min - 0.000015f &
                    t_tmp*one_ray.d.x + one_ray.p.x <= x_max + 0.000015f &
                    t_tmp*one_ray.d.x + one_ray.p.x >= x_min - 0.000015f)
                t = t_tmp;
        }
        if (one_ray.d.x != 0){
            t_tmp = (x_min-one_ray.p.x)/one_ray.d.x;
            if (t_tmp < t & t_tmp >= 0 &
                    t_tmp*one_ray.d.z + one_ray.p.z <= z_max + 0.000015f &
                    t_tmp*one_ray.d.z + one_ray.p.z >= z_min - 0.000015f &
                    t_tmp*one_ray.d.z + one_ray.p.y <= y_max + 0.000015f &
                    t_tmp*one_ray.d.z + one_ray.p.y >= y_min - 0.000015f)
                t = t_tmp;
            t_tmp = (x_max - one_ray.p.x)/one_ray.d.x;
            if (t_tmp < t & t_tmp >= 0 &
                    t_tmp*one_ray.d.z + one_ray.p.z <= z_max + 0.000015f &
                    t_tmp*one_ray.d.z + one_ray.p.z >= z_min - 0.000015f &
                    t_tmp*one_ray.d.y + one_ray.p.y <= y_max + 0.000015f &
                    t_tmp*one_ray.d.y + one_ray.p.y >= y_min - 0.000015f)
                t = t_tmp;
        }
        return intsct(t, -1, 0, 0);
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
        boxes(nullptr), boundary(0.f), numBox(0), trial(0) {}
    KDNode(float a, float b, float c, float d, float e, float f): AABB(a, b, c, d, e, f, -1),
        is_leaf(false), axis(X), child_1(nullptr), child_2(nullptr),
        boxes(nullptr), boundary(0.f), numBox(0), trial(0) {}
    bool is_leaf;
    DivideAxis axis;
    KDNode *child_1;
    KDNode *child_2;
    AABB **boxes;
    float boundary;
    int numBox;
    int trial;
};

static void recurSplitKDTree(KDNode *node){
    // std::cout << "numBox: " << node->numBox << std::endl;
    for (int i = 0; i < node->numBox; i++){
        std::cout << node->boxes[i]->prim_index << " ";
    }
    std::cout<< std::endl;
    if ((node->numBox <= 5) || (node->trial == 3)){
        node->is_leaf = true;
        return;
    }
    float c_tmp = 0.f, c_best = INFINITY;
    int i_best = -1;
    int j_1 = 0, j_2 = 0;
    AABB** left = new AABB*[node->numBox];
    AABB** right = new AABB*[node->numBox];
    float tmp_boundary;
    switch (node->axis) {
    case X: {
        std::sort(node->boxes, node->boxes + node->numBox, compareX);
        for (int i = 0; i < node->numBox; i++){
            tmp_boundary = node->boxes[i]->x_min;
            if (tmp_boundary < node->x_min)
                continue;
            c_tmp = i*(tmp_boundary - node->x_min) + (node->numBox-i)*(node->x_max - tmp_boundary);
            if (c_tmp < c_best){
                c_best = c_tmp;
                i_best = i;
            }
        }
        if (i_best == -1){
            delete[] left;
            delete[] right;
            node->axis = Y;
            node->trial++;
            recurSplitKDTree(node);
            return;
        }
        node->boundary = node->boxes[i_best]->x_min;
        for (int i = 0; i < node->numBox; i++){
            if (node->boxes[i]->x_max < node->boundary)
                left[j_1++] = node->boxes[i];
            if (node->boxes[i]->x_max > node->boundary)
                right[j_2++] = node->boxes[i];
        }
        if ((j_1 == node->numBox) || (j_2 == node->numBox)){
            delete[] left;
            delete[] right;
            node->axis = Y;
            node->trial++;
            recurSplitKDTree(node);
            return;
        }
        node->child_1 = new KDNode(node->x_min, node->boundary, node->y_min, node->y_max, node->z_min, node->z_max);
        node->child_2 = new KDNode(node->boundary, node->x_max, node->y_min, node->y_max, node->z_min, node->z_max);
        node->child_1->axis = Y;
        node->child_2->axis = Y;
        node->child_1->boxes = left;
        node->child_2->boxes = right;
        node->child_1->numBox = j_1;
        node->child_2->numBox = j_2;
        recurSplitKDTree(node->child_1);
        recurSplitKDTree(node->child_2);
        return;
    }
    case Y: {
        std::sort(node->boxes, node->boxes + node->numBox, compareY);
        for (int i = 0; i < node->numBox; i++){
            tmp_boundary = node->boxes[i]->y_min;
            if (tmp_boundary < node->y_min) continue;
            c_tmp = i*(tmp_boundary - node->y_min) + (node->numBox-i)*(node->y_max - tmp_boundary);
            if (c_tmp < c_best){
                c_best = c_tmp;
                i_best = i;
            }
        }
        if (i_best == -1){
            delete[] left;
            delete[] right;
            node->axis = Z;
            node->trial++;
            recurSplitKDTree(node);
            return;
        }
        node->boundary = node->boxes[i_best]->y_min;
        for (int i = 0; i < node->numBox; i++){
            if (node->boxes[i]->y_min < node->boundary)
                left[j_1++] = node->boxes[i];
            if (node->boxes[i]->y_max > node->boundary)
                right[j_2++] = node->boxes[i];
        }
        if ((j_1 == node->numBox) || (j_2 == node->numBox)){
            delete[] left;
            delete[] right;
            node->axis = Z;
            node->trial++;
            recurSplitKDTree(node);
            return;
        }
        node->child_1 = new KDNode(node->x_min, node->x_max, node->y_min, node->boundary, node->z_min, node->z_max);
        node->child_2 = new KDNode(node->x_min, node->x_max, node->boundary, node->y_max, node->z_min, node->z_max);
        node->child_1->axis = Z;
        node->child_2->axis = Z;
        node->child_1->boxes = left;
        node->child_2->boxes = right;
        node->child_1->numBox = j_1;
        node->child_2->numBox = j_2;
        recurSplitKDTree(node->child_1);
        recurSplitKDTree(node->child_2);
        return;
    }
    case Z: {
        std::sort(node->boxes, node->boxes + node->numBox, compareZ);
        for (int i = 0; i < node->numBox; i++){
            tmp_boundary = node->boxes[i]->z_min;
            if (tmp_boundary < node->z_min) continue;
            c_tmp = i*(tmp_boundary - node->z_min) + (node->numBox-i)*(node->z_max - tmp_boundary);
            if (c_tmp < c_best){
                c_best = c_tmp;
                i_best = i;
            }
        }
        if (i_best == -1){
            delete[] left;
            delete[] right;
            node->axis = X;
            node->trial++;
            recurSplitKDTree(node);
            return;
        }
        node->boundary = node->boxes[i_best]->z_min;
        for (int i = 0; i < node->numBox; i++){
            if (node->boxes[i]->z_min < node->boundary)
                left[j_1++] = node->boxes[i];
            if (node->boxes[i]->z_max > node->boundary)
                right[j_2++] = node->boxes[i];
        }
        if ((j_1 == 0) || (j_2 == 0) || (j_1 == node->numBox) || (j_2 == node->numBox)){
            delete[] left;
            delete[] right;
            node->axis = X;
            node->trial++;
            recurSplitKDTree(node);
            return;
        }
        node->child_1 = new KDNode(node->x_min, node->x_max, node->y_min, node->y_max, node->z_min, node->boundary);
        node->child_2 = new KDNode(node->x_min, node->x_max, node->y_min, node->y_max, node->boundary, node->z_max);
        node->child_1->axis = X;
        node->child_2->axis = X;
        node->child_1->boxes = left;
        node->child_2->boxes = right;
        node->child_1->numBox = j_1;
        node->child_2->numBox = j_2;
        recurSplitKDTree(node->child_1);
        recurSplitKDTree(node->child_2);
        return;
    }
    }
}
