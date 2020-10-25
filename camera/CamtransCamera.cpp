/**
 * @file   CamtransCamera.cpp
 *
 * This is the perspective camera class you will need to fill in for the Camtrans lab.  See the
 * lab handout for more details.
 */

#include "CamtransCamera.h"
#include <Settings.h>

CamtransCamera::CamtransCamera() :
    m_aspectRatio(0.f),
    m_near(0.f),
    m_far(0.f),
    m_translationMatrix(glm::mat4x4()),
    m_perspectiveTransformation(glm::mat4x4()),
    m_scaleMatrix(glm::mat4x4()),
    m_rotationMatrix(glm::mat4x4()),
    m_thetaH(0.f),
    m_eye(glm::vec4()),
    m_up(glm::vec4()),
    m_u(glm::vec4()),
    m_v(glm::vec4()),
    m_w(glm::vec4())
{
    setClip(1.f, 30.f);
    setHeightAngle(60.f);
    setAspectRatio(1.f);
    orientLook(glm::vec4(2.f, 2.f, 2.f, 0.f),
               glm::vec4(-2.f, -2.f, -2.f, 0.f),
               glm::vec4(0.f, 1.f, 0.f, 0.f));
}

void CamtransCamera::setAspectRatio(float a) {
    m_aspectRatio = a;
    updateProjectionMatrix();
}

glm::mat4x4 CamtransCamera::getProjectionMatrix() const {
    return m_perspectiveTransformation*m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getViewMatrix() const {
    return m_rotationMatrix*m_translationMatrix;
}

glm::mat4x4 CamtransCamera::getScaleMatrix() const {
    return m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getPerspectiveMatrix() const {
    return m_perspectiveTransformation;
}

glm::vec4 CamtransCamera::getPosition() const {
    return m_eye;
}

glm::vec4 CamtransCamera::getLook() const {
    return -m_w;
}

glm::vec4 CamtransCamera::getUp() const {
    return m_up;
}

glm::vec4 CamtransCamera::getU() const {
    return m_u;
}

glm::vec4 CamtransCamera::getV() const {
    return m_v;
}

glm::vec4 CamtransCamera::getW() const {
    return m_w;
}

float CamtransCamera::getAspectRatio() const {
    return m_aspectRatio;
}

float CamtransCamera::getHeightAngle() const {
    return m_thetaH;
}

void CamtransCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up) {
    m_eye = eye;
    m_up = up;
    m_w = glm::normalize(-look);
    m_v = glm::normalize(up-glm::dot(m_up, m_w)*m_w);
    m_u = glm::vec4(glm::cross(glm::vec3(m_v.xyz()), glm::vec3(m_w.xyz())), 0);
    updateViewMatrix();
    updateProjectionMatrix();
}

void CamtransCamera::setHeightAngle(float h) {
    m_thetaH = glm::radians(h);
    updateProjectionMatrix();
}

void CamtransCamera::translate(const glm::vec4 &v) {
    m_eye += v;
    updateViewMatrix();
}

void CamtransCamera::rotateU(float degrees) {
    float rad = glm::radians(degrees);
    glm::vec4 tmp = cos(rad)*m_v + sin(rad)*m_w;
    m_w = -sin(rad)*m_v + cos(rad)*m_w;
    m_v = tmp;
    updateViewMatrix();
}

void CamtransCamera::rotateV(float degrees) {
    float rad = glm::radians(degrees);
    glm::vec4 tmp = cos(rad)*m_w + sin(rad)*m_u;
    m_u = -sin(rad)*m_w + cos(rad)*m_u;
    m_w = tmp;
    updateViewMatrix();
}

void CamtransCamera::rotateW(float degrees) {
    float rad = glm::radians(degrees);
    glm::vec4 tmp = cos(rad)*m_u + sin(rad)*m_v;
    m_v = -sin(rad)*m_u + cos(rad)*m_v;
    m_u = tmp;
    updateViewMatrix();
}

void CamtransCamera::setClip(float nearPlane, float farPlane) {
    m_near = nearPlane;
    m_far = farPlane;
    updateProjectionMatrix();
}

void CamtransCamera::updateProjectionMatrix(){
    updateScaleMatrix();
    updatePerspectivematrix();
}

void CamtransCamera::updateViewMatrix(){
    updateTranslationMatrix();
    updateRotationMatrix();
}

void CamtransCamera::updatePerspectivematrix(){
    float c= -m_near/m_far;
    m_perspectiveTransformation = glm::mat4x4(1, 0, 0, 0,
                                              0, 1, 0, 0,
                                              0, 0, -1/(c+1), -1,
                                              0, 0, c/(c+1), 0);
}

void CamtransCamera::updateScaleMatrix(){
    m_scaleMatrix = glm::mat4x4(1/(tan(m_thetaH/2)*m_aspectRatio*m_far), 0, 0, 0,
                                0, 1/(tan(m_thetaH/2)*m_far), 0, 0,
                                0, 0, 1/m_far, 0,
                                0, 0, 0, 1);
}

void CamtransCamera::updateRotationMatrix(){
    m_rotationMatrix = glm::mat4x4(m_u.x, m_v.x, m_w.x, 0,
                                   m_u.y, m_v.y, m_w.y, 0,
                                   m_u.z, m_v.z, m_w.z, 0,
                                   0, 0, 0, 1);

}

void CamtransCamera::updateTranslationMatrix(){
    m_translationMatrix = glm::mat4x4(1, 0, 0, 0,
                                      0, 1, 0, 0,
                                      0, 0, 1, 0,
                                      -m_eye.x, -m_eye.y, -m_eye.z, 1);
}

