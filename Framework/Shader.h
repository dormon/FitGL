#pragma once

#include<GL/glew.h>
#include<iostream>
#include<vector>

template<typename...ARGS>
GLuint compileShader(GLenum type,ARGS... sources){
  std::vector<std::string>str = {std::string(sources)...};
  std::vector<const GLchar*>ptr;
  for(auto const&x:str)ptr.push_back(x.c_str());

  //reserve shader id
  GLuint id = glCreateShader(type);

  //set shader sources
  glShaderSource(id,(GLsizei)ptr.size(),ptr.data(),nullptr);

  //compile shader
  glCompileShader(id);
  //get compilation status
  GLint status;
  glGetShaderiv(id,GL_COMPILE_STATUS,&status);
  if(status == GL_TRUE)return id;

  //get compilation log
  GLint bufferLen;
  glGetShaderiv(id,GL_INFO_LOG_LENGTH,&bufferLen);
  if(bufferLen>0){
    char*buffer = new char[bufferLen];
    glGetShaderInfoLog(id,bufferLen,nullptr,buffer);
    std::cerr<<buffer<<std::endl;
    delete[]buffer;
  }
  throw std::exception("Shader compile failed!");
  return id;
}

template<typename...ARGS>
GLuint createProgram(ARGS...args){
  //reserver program id
  GLuint id = glCreateProgram();

  //attach all shaders
  auto dummy0 = {(glAttachShader(id,args),0)...};
  (void)dummy0;

  //link program
  glLinkProgram(id);

  //get linking status
  GLint status;
  glGetProgramiv(id,GL_LINK_STATUS,&status);
  if(status == GL_TRUE){
    //mark shaders for deletion
    auto dummy1 = {(glDeleteShader(args),0)...};
    (void)dummy1;
    return id;
  }
  
  //get linking log
  GLint bufferLen;
  glGetProgramiv(id,GL_INFO_LOG_LENGTH,&bufferLen);
  if(bufferLen>0){
    char*buffer = new char[bufferLen];
    glGetProgramInfoLog(id,bufferLen,nullptr,buffer);
    std::cerr<<buffer<<std::endl;
    delete[]buffer;
    glDeleteProgram(id);
  }
  throw std::exception("Program link failed!");
  return 0;
}
