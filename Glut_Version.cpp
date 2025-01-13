//
//#include <glm/ext.hpp>
//#include<iostream>
//#include "Shader.h"
//#include "Model.h"
//#include "Camera.h"
//
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <GL/glew.h>
//
//#include <assimp/Importer.hpp>      // C++ importer interface
//#include <assimp/scene.h>           // Output data structure
//#include <assimp/postprocess.h>     // Post processing flags
//
//
//const unsigned int SCR_WIDTH = 500;
//const unsigned int SCR_HEIGHT = 500;
//
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//Shader ourShader;
//Model ourModel;
//float rotateVal = 0.0f;
//float translate = 0.f;
//float factor = 0.5;
//
//void init() {
//    ourShader.InitShader("model_2.txt", "model_1.txt");
//    ourModel.InitModel("backpack/backpack.obj");
//
// 
//    ourShader.use();
//
//    // view/projection transformations
//    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//    glm::mat4 view = camera.GetViewMatrix();
//    ourShader.setMat4("projection", projection);
//    ourShader.setMat4("view", view);
//
//    // render the loaded model
//    glm::mat4 model = glm::mat4(1.0f);
//    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
//    model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));	// it's a bit too big for our scene, so scale it down
//  //  model = glm::rotate(model, 0.5f, glm::vec3(0.0, 1.0, 0.0));
//    ourShader.setMat4("model", model);
//    cout << "HERE" << endl;
//
//    
//   glEnable(GL_DEPTH_TEST);
//    glClearColor(1.0, 1.0, 1.0, 1.0);
//
//}
//
//void display(void)
//{
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//   // Clear frame buffer and depth buffer
//    ourModel.Draw(ourShader);
//    glutSwapBuffers();
//    cout << "There There" << endl;
//}
//
//
//void idle() {
//
//    rotateVal += factor;
//
//    if (translate > 5.0f) {
//        factor = -0.0005;
//    }
//    else if (translate < -5.0f) {
//        factor = 0.0005;
//    }
//
//    cout << rotateVal;
//
//    glutPostRedisplay();
//}
//
//
//
//int main(int argc, char** argv)
//{
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
//    glutInitWindowSize(600, 600);
//    glutInitContextVersion(3, 2);
//    glutInitContextProfile(GLUT_CORE_PROFILE);
//    glutCreateWindow("Sphere");
//
//
//    glewInit();
//
//    init();
//
//    glutDisplayFunc(display);
//    glutIdleFunc(idle);
//
//    glutMainLoop();
//    return 0;
//}



















//
//#include "Angel.h"
//#include <glm/ext.hpp>
//#include<iostream>
//#include "Shader.h"
//#include "Model.h"
//#include "Camera.h"
//
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <GL/glew.h>
//
//#include <assimp/Importer.hpp>      // C++ importer interface
//#include <assimp/scene.h>           // Output data structure
//#include <assimp/postprocess.h>     // Post processing flags
//
//
//
//void init(void)
//{
//    vec2 vertices[3] = {
//        vec2(-1.0, -1.0), vec2(0.0, 1.0), vec2(1.0, -1.0)
//    };
//
//    // Create a vertex array object
//    GLuint vao;
//    glGenVertexArrays(1, &vao);
//    glBindVertexArray(vao);
//
//    // Create and initialize a buffer object
//    GLuint buffer;
//    glGenBuffers(1, &buffer);
//    glBindBuffer(GL_ARRAY_BUFFER, buffer);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//
//     // Load shaders and use the resulting shader program
//    GLuint program = InitShader("vshader22.glsl", "fshader22.glsl");
//    glUseProgram(program);
//
//    // Initialize the vertex position attribute from the vertex shader    
//    GLuint loc = glGetAttribLocation(program, "vPosition");
//    glEnableVertexAttribArray(loc);
//    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0,
//        BUFFER_OFFSET(0));
//
//    glClearColor(1.0, 1.0, 1.0, 1.0); /* white background */
//}
//
//
//void display(void)
//{
//    glClear(GL_COLOR_BUFFER_BIT);
//    glDrawArrays(GL_TRIANGLES, 0, 3);
//    glFlush();
//}
//
//void idle() {
//    glutPostRedisplay();
//}
//
//
//
//int main(int argc, char** argv)
//{
//        cout << "==============";
//      glutInit(&argc, argv);
//      glutInitDisplayMode(GLUT_RGBA);
//      glutInitWindowSize(512, 512);
//      glutInitContextVersion(3, 2);
//      glutInitContextProfile(GLUT_CORE_PROFILE);
//      glutCreateWindow("Sierpinski Gasket");
//  
//      glewInit();
//  
//      init();
//  
//      glutDisplayFunc(display);
//    
//      glutMainLoop();
//      return 0;
//}
