#include <QMouseEvent>
#include "NGLScene.h"
#include <iostream>
#include <ngl/Transformation.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/VAOFactory.h>
#include <ngl/SimpleVAO.h>
#include <ngl/ShaderLib.h>


//----------------------------------------------------------------------------------------------------------------------
NGLScene::NGLScene(int _timer, QWidget *_parent ) : QOpenGLWidget(_parent )
{
  // set this widget to have the initial keyboard focus
  setFocus();
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  this->resize(_parent->size());
	m_timerValue=_timer;
	startSimTimer();
}

// This virtual function is called once before the first call to paintGL() or resizeGL(),
//and then once whenever the widget has been assigned a new QGLContext.
// This function should set up any required OpenGL context rendering flags, defining display lists, etc.

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::initializeGL()
{
ngl::NGLInit::initialize();
glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
// enable depth testing for drawing
 glEnable(GL_DEPTH_TEST);
 // Now we will create a basic Camera from the graphics library
 // This is a static camera so it only needs to be set once
 // First create Values for the camera position
 ngl::Vec3 from(0,0,7);
 ngl::Vec3 to(0,0,0);
 ngl::Vec3 up(0,1,0);
 
 m_view=ngl::lookAt(from,to,up);
 // set the shape using FOV 45 Aspect Ratio based on Width and Height
 // The final two are near and far clipping planes of 0.5 and 10
 m_project=ngl::perspective(45.0f,720.0f/576.0f,0.5f,150.0f);

 ngl::VAOPrimitives::createSphere("sphere",1.0,20);
 ngl::ShaderLib::use(ngl::nglDiffuseShader);
 ngl::ShaderLib::setUniform("Colour",1.0f,1.0f,0.0f,1.0f);
 ngl::ShaderLib::setUniform("lightPos",1.0f,1.0f,1.0f);
 ngl::ShaderLib::setUniform("lightDiffuse",1.0f,1.0f,1.0f,1.0f);

}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget has been resized.
// The new size is passed in width and height.
void NGLScene::resizeGL( int _w, int _h )
{
  m_project=ngl::perspective(45.0f, static_cast<float>( _w ) / _h, 0.05f, 350.0f );
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
}

void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib::use(ngl::nglDiffuseShader);
  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=m_transform.getMatrix();
  MV=m_view *m_mouseGlobalTX*M;
  MVP=m_project*MV;
  normalMatrix=MV;
  normalMatrix.inverse().transpose();
  ngl::ShaderLib::setUniform("MV",MV);
  ngl::ShaderLib::setUniform("MVP",MVP);
  ngl::ShaderLib::setUniform("normalMatrix",normalMatrix);
  ngl::ShaderLib::setUniform("M",M);
}

void NGLScene::loadMatricesToColourShader()
{
  ngl::ShaderLib::use(ngl::nglColourShader);
  ngl::Mat4 MVP;
  MVP = m_project*m_view *
        m_mouseGlobalTX*
        m_transform.getMatrix();
  ngl::ShaderLib::setUniform("MVP",MVP);

}


//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget needs to be painted.
// this is our main drawing routine
void NGLScene::paintGL()
{
  // clear the screen and depth buffer

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Rotation based on the mouse position for our global
  // transform
  // Rotation based on the mouse position for our global transform
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX(m_win.spinXFace);
  rotY.rotateY(m_win.spinYFace);
  // multiply the rotations
  m_mouseGlobalTX=rotY*rotX;
  // draw spring lines
  ngl::ShaderLib::use(ngl::nglColourShader);
  // get our position values and put in a vector
  std::vector<ngl::Vec3> points(2);
  points[0]=m_spring->getAPosition();
  points[1]=m_spring->getBPosition();
  ngl::ShaderLib::setUniform("Colour",1.0f,1.0f,1.0f,1.0f);
  m_transform.reset();
  loadMatricesToColourShader();
  // load transform stack
  std::unique_ptr<ngl::AbstractVAO> vao(ngl::VAOFactory::createVAO("simpleVAO",GL_LINES));
  vao->bind();
  vao->setData(ngl::AbstractVAO::VertexData(2*sizeof(ngl::Vec3),points[0].m_x));
  vao->setNumIndices(2);
  vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
  vao->draw();
  vao->unbind();



  ngl::ShaderLib::use(ngl::nglDiffuseShader);

  ngl::ShaderLib::setUniform("Colour",1.0f,0.0f,0.0f,1.0f);
  m_transform.setScale(0.1f,0.1f,0.1f);
  m_transform.setPosition(m_spring->getAPosition());
  loadMatricesToShader();
  // draw the cube
  ngl::VAOPrimitives::draw("cube");


  ngl::ShaderLib::setUniform("Colour",0.0f,1.0f,0.0f,1.0f);
  m_transform.setScale(0.1f,0.1f,0.1f);
  m_transform.setPosition(m_spring->getBPosition());
  loadMatricesToShader();
  // draw the sphere
  ngl::VAOPrimitives::draw("cube");
  // draw the target points
  ngl::ShaderLib::setUniform("Colour",1.0f,0.0f,0.0f,1.0f);
  m_transform.setScale(0.1f,0.1f,0.1f);
  m_transform.setPosition(m_spring->getInitialAPosition());
  loadMatricesToShader();
  // draw the cube
  ngl::VAOPrimitives::draw("sphere");
  ngl::ShaderLib::setUniform("Colour",0.0f,1.0f,0.0f,1.0f);

  m_transform.setScale(0.1f,0.1f,0.1f);
  m_transform.setPosition(m_spring->getInitialBPosition());
  loadMatricesToShader();

  // draw the sphere
  ngl::VAOPrimitives::draw("sphere");
}


void NGLScene::timerEvent( QTimerEvent *)
{
	m_spring->update();
	update();
}

NGLScene::~NGLScene()
{
}

void NGLScene::startSimTimer()
{

m_timer=startTimer(m_timerValue);
}

void NGLScene::stopSimTimer()
{
 killTimer(m_timer);
}


