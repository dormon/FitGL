#pragma once

#include<GL/glew.h>
#include<iostream>
#include<map>

class ProgramObject{
  protected:
    GLuint _id;
    std::map<std::string,GLint>_name2Uniform;
    GLint _getUniform(std::string name){
      auto ii = this->_name2Uniform.find(name);
      if(ii==this->_name2Uniform.end()){
        GLint uniform = glGetUniformLocation(this->_id,name.c_str());
        if(uniform<0)return uniform;
        this->_name2Uniform[name] = uniform;
        return uniform;
      }
      return ii->second;
    }
  public:
    ProgramObject(GLuint id){this->_id = id;}
    ~ProgramObject(){glDeleteProgram(this->_id);}
    GLuint getId()const{return this->_id;}
    void use(){glUseProgram(this->_id);}
    void set1f(std::string name,float v0                           ){glUniform1f(this->_getUniform(name),v0         );}
    void set2f(std::string name,float v0,float v1                  ){glUniform2f(this->_getUniform(name),v0,v1      );}
    void set3f(std::string name,float v0,float v1,float v2         ){glUniform3f(this->_getUniform(name),v0,v1,v2   );}
    void set4f(std::string name,float v0,float v1,float v2,float v3){glUniform4f(this->_getUniform(name),v0,v1,v2,v3);}
    void set1i(std::string name,int32_t v0                                 ){glUniform1i(this->_getUniform(name),v0         );}
    void set2i(std::string name,int32_t v0,int32_t v1                      ){glUniform2i(this->_getUniform(name),v0,v1      );}
    void set3i(std::string name,int32_t v0,int32_t v1,int32_t v2           ){glUniform3i(this->_getUniform(name),v0,v1,v2   );}
    void set4i(std::string name,int32_t v0,int32_t v1,int32_t v2,int32_t v3){glUniform4i(this->_getUniform(name),v0,v1,v2,v3);}
    void set1ui(std::string name,uint32_t v0                                    ){glUniform1ui(this->_getUniform(name),v0         );}
    void set2ui(std::string name,uint32_t v0,uint32_t v1                        ){glUniform2ui(this->_getUniform(name),v0,v1      );}
    void set3ui(std::string name,uint32_t v0,uint32_t v1,uint32_t v2            ){glUniform3ui(this->_getUniform(name),v0,v1,v2   );}
    void set4ui(std::string name,uint32_t v0,uint32_t v1,uint32_t v2,uint32_t v3){glUniform4ui(this->_getUniform(name),v0,v1,v2,v3);}
    void set1fv(std::string name,float const*v0,GLsizei count = 1){glUniform1fv(this->_getUniform(name),count,v0);}
    void set2fv(std::string name,float const*v0,GLsizei count = 1){glUniform2fv(this->_getUniform(name),count,v0);}
    void set3fv(std::string name,float const*v0,GLsizei count = 1){glUniform3fv(this->_getUniform(name),count,v0);}
    void set4fv(std::string name,float const*v0,GLsizei count = 1){glUniform4fv(this->_getUniform(name),count,v0);}
    void set1iv(std::string name,int32_t const*v0,GLsizei count = 1){glUniform1iv(this->_getUniform(name),count,v0);}
    void set2iv(std::string name,int32_t const*v0,GLsizei count = 1){glUniform2iv(this->_getUniform(name),count,v0);}
    void set3iv(std::string name,int32_t const*v0,GLsizei count = 1){glUniform3iv(this->_getUniform(name),count,v0);}
    void set4iv(std::string name,int32_t const*v0,GLsizei count = 1){glUniform4iv(this->_getUniform(name),count,v0);}
    void set1uiv(std::string name,uint32_t const*v0,GLsizei count = 1){glUniform1uiv(this->_getUniform(name),count,v0);}
    void set2uiv(std::string name,uint32_t const*v0,GLsizei count = 1){glUniform2uiv(this->_getUniform(name),count,v0);}
    void set3uiv(std::string name,uint32_t const*v0,GLsizei count = 1){glUniform3uiv(this->_getUniform(name),count,v0);}
    void set4uiv(std::string name,uint32_t const*v0,GLsizei count = 1){glUniform4uiv(this->_getUniform(name),count,v0);}
    void setMatrix4fv(std::string name,float const*v0,GLsizei count = 1,GLboolean transpose = GL_FALSE){glUniformMatrix4fv(this->_getUniform(name),count,transpose,v0);}
    void setMatrix3fv(std::string name,float const*v0,GLsizei count = 1,GLboolean transpose = GL_FALSE){glUniformMatrix3fv(this->_getUniform(name),count,transpose,v0);}
    void setMatrix2fv(std::string name,float const*v0,GLsizei count = 1,GLboolean transpose = GL_FALSE){glUniformMatrix2fv(this->_getUniform(name),count,transpose,v0);}
    void setMatrix4x3fv(std::string name,float const*v0,GLsizei count = 1,GLboolean transpose = GL_FALSE){glUniformMatrix4x3fv(this->_getUniform(name),count,transpose,v0);}
    void setMatrix4x2fv(std::string name,float const*v0,GLsizei count = 1,GLboolean transpose = GL_FALSE){glUniformMatrix4x2fv(this->_getUniform(name),count,transpose,v0);}
    void setMatrix3x4fv(std::string name,float const*v0,GLsizei count = 1,GLboolean transpose = GL_FALSE){glUniformMatrix3x4fv(this->_getUniform(name),count,transpose,v0);}
    void setMatrix3x2fv(std::string name,float const*v0,GLsizei count = 1,GLboolean transpose = GL_FALSE){glUniformMatrix3x2fv(this->_getUniform(name),count,transpose,v0);}
    void setMatrix2x4fv(std::string name,float const*v0,GLsizei count = 1,GLboolean transpose = GL_FALSE){glUniformMatrix2x4fv(this->_getUniform(name),count,transpose,v0);}
    void setMatrix2x3fv(std::string name,float const*v0,GLsizei count = 1,GLboolean transpose = GL_FALSE){glUniformMatrix2x3fv(this->_getUniform(name),count,transpose,v0);}

};
