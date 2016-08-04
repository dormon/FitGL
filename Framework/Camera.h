#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>


class CameraProjection{
  public:
    virtual ~CameraProjection(){}
    virtual glm::mat4 getProjection() = 0;
};

class CameraTransform{
  public:
    virtual ~CameraTransform(){}
    virtual glm::mat4 getView() = 0;
};

class PerspectiveCamera: public CameraProjection{
  protected:
    float     _near      ;
    float     _far       ;
    float     _fovy      ;
    float     _aspect    ;
    glm::mat4 _projection;
    bool      _recompute ;
    void _compute(){
      this->_projection = glm::perspective(this->_fovy,this->_aspect,this->_near,this->_far);
    }
  public:
    PerspectiveCamera(
        float fovy   = glm::half_pi<float>(),
        float aspect = 1024.f/768.f,
        float near   = 0.1f,
        float far    = 1000.f):CameraProjection(){
      this->_fovy      = fovy  ;
      this->_near      = near  ;
      this->_far       = far   ;
      this->_aspect    = aspect;
      this->_recompute = true  ;
    }
    virtual ~PerspectiveCamera(){}
    virtual glm::mat4 getProjection(){
      if(this->_recompute){
        this->_compute();
        this->_recompute = false;
      }
      return this->_projection;
    }
    float getNear  ()const{return this->_near  ;}
    float getFar   ()const{return this->_far   ;}
    float getFovy  ()const{return this->_fovy  ;}
    float getAspect()const{return this->_aspect;}
    void setNear  (float value){this->_near   = value;this->_recompute = true;}
    void setFar   (float value){this->_far    = value;this->_recompute = true;}
    void setFovy  (float value){this->_fovy   = value;this->_recompute = true;}
    void setAspect(float value){this->_aspect = value;this->_recompute = true;}
};

class OrbitCamera: public CameraTransform{
  protected:
    float     _xAngle   ;
    float     _yAngle   ;
    float     _distance ;
    bool      _recompute;
    glm::vec3 _position ;
    glm::mat4 _view     ;
    glm::mat4 _rotation ;
    void _compute(){
      this->_rotation = 
        glm::rotate(glm::mat4(1.f),this->_xAngle,glm::vec3(1.f,0.f,0.f))*
        glm::rotate(glm::mat4(1.f),this->_yAngle,glm::vec3(0.f,1.f,0.f));
      this->_view =
        glm::translate(glm::mat4(1.f),glm::vec3(0.f,0.f,-this->_distance))*
        this->_rotation*
        glm::translate(glm::mat4(1.f),this->_position);
    }
  public:
    OrbitCamera(float distance = 1):CameraTransform(){
      this->_xAngle    = 0       ;
      this->_yAngle    = 0       ;
      this->_distance  = distance;
      this->_recompute = true    ;
    }
    virtual ~OrbitCamera(){}
    virtual glm::mat4 getView(){
      if(this->_recompute){
        this->_compute();
        this->_recompute = false;
      }
      return this->_view;
    }
    glm::mat4 getRotation(){
      if(this->_recompute){
        this->_compute();
        this->_recompute = false;
      }
      return this->_rotation;
    }
    float getXAngle  ()const{return this->_xAngle  ;}
    float getYAngle  ()const{return this->_yAngle  ;}
    float getDistance()const{return this->_distance;}
    glm::vec3 getPosition()const{return this->_position;}
    void setXAngle  (float value){
      this->_xAngle = glm::clamp(value,-glm::half_pi<float>(),glm::half_pi<float>());
      this->_recompute = true;
    }
    void setYAngle  (float value){this->_yAngle   = value;this->_recompute = true;}
    void setDistance(float value){this->_distance = value;this->_recompute = true;}
    void setPosition(glm::vec3 value){this->_position = value;this->_recompute = true;}
    void addXPosition(float dx){
      this->_position += glm::vec3(glm::row(this->_rotation,0))*dx;
      this->_recompute = true;
    }
    void addYPosition(float dy){
      this->_position += glm::vec3(glm::row(this->_rotation,1))*dy;
      this->_recompute = true;
    }

};


