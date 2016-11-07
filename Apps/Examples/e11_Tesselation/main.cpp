#include <BaseApp.h>
#include <map>

std::string vert = R".(
#version 430
void main() {
	gl_Position = vec4(0,0,0,1);
}).";

std::string tesc = R".(
#version 430
layout(vertices=1)out;
uniform float i[2];
uniform float o[4];
void main() {
  gl_TessLevelInner[0]=i[0];
  gl_TessLevelInner[1]=i[1];
  gl_TessLevelOuter[0]=o[0];
  gl_TessLevelOuter[1]=o[1];
  gl_TessLevelOuter[2]=o[2];
  gl_TessLevelOuter[3]=o[3];
}).";

std::string tese = R".(
layout(PRIMITIVE,SPACING)in;
void main() {  
#ifdef TRI
  vec2 a = vec2(0,0);
  vec2 b = vec2(1,0);
  vec2 c = vec2(0.5,0.866);  
  gl_Position = vec4((gl_TessCoord.x*a+gl_TessCoord.y*b+gl_TessCoord.z*c-0.5)*1.8,0,1);
#else
  gl_Position = vec4((gl_TessCoord.xy-0.5)*1.8,0,1);
#endif
}).";

std::string frag = R".(
#version 430
out vec4 fragColor;
void main() {
	fragColor = vec4(1,1,1,1);
}).";



int main(int /*argc*/, char ** /*argv*/) {
  BaseApp app;
  GLuint vao;
  float o[] = {4,4,4,4 };
  float i[] = { 4,4 };
  
  int currentPrimitive = 2;
  const char* primitives[] = {"isolines","triangles","quads"};
  int currentSpacing = 0;
  const char* spacing[] = { "equal_spacing","fractional_even_spacing","fractional_odd_spacing" };

  std::map<int, ProgramObject> programs;

  auto getProgram =[&]() {
    int id = currentSpacing * 3 + currentPrimitive;
    if (programs.count(id)) {
      return programs[id];
    }
    std::string defines = "#define SPACING " + std::string(spacing[currentSpacing]) +"\n";
    defines += "#define PRIMITIVE " + std::string(primitives[currentPrimitive]) + "\n";
    if(currentPrimitive==1) defines += "#define TRI\n";
    auto vs = compileShader(GL_VERTEX_SHADER, vert);
    auto tcs = compileShader(GL_TESS_CONTROL_SHADER, tesc);
    auto tes = compileShader(GL_TESS_EVALUATION_SHADER, "#version 430\n",defines, tese);
    auto fs = compileShader(GL_FRAGMENT_SHADER, frag);
    ProgramObject program = createProgram(vs, tcs, tes, fs);
    programs[id] = program;
    return program;
  };

  app.addInitCallback([&]() {
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);
  });

  app.addResizeCallback([&](int w, int h) {
    glViewport(0, 0, w, h);
  });
  
  app.addDrawCallback([&]() {
    glClearColor(0.2, 0.2, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    //bunny

    auto program = getProgram();
    program.use();
    program.set1f("o[0]", o[0]);
    program.set1f("o[1]", o[1]);
    program.set1f("o[2]", o[2]);
    program.set1f("o[3]", o[3]);
    program.set1f("i[0]", i[0]);
    program.set1f("i[1]", i[1]);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPatchParameteri(GL_PATCH_VERTICES, 1 );
    glDrawArrays(GL_PATCHES, 0, 1);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    fpsLabel();
    ImGui::Begin("Options", 0, 0);
    ImGui::PushItemWidth(-80);
    ImGui::Combo("Primitive", &currentPrimitive, primitives, 3);
    ImGui::Combo("Spacing", &currentSpacing, spacing, 3);
    
    ImGui::SliderFloat("Inner[0]", &i[0], 1, 64, "%.1f");
    ImGui::SliderFloat("Inner[1]", &i[1], 1, 64, "%.1f");
    ImGui::SliderFloat("Outer[0]", &o[0], 1, 64, "%.1f");
    ImGui::SliderFloat("Outer[1]", &o[1], 1, 64, "%.1f");
    ImGui::SliderFloat("Outer[2]", &o[2], 1, 64, "%.1f");
    ImGui::SliderFloat("Outer[3]", &o[3], 1, 64, "%.1f");
    ImGui::End();
  });
  return app.run();
}
