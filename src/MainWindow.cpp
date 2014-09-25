#include "MainWindow.h"
#include "ui_MainWindow.h"

//----------------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), m_ui(new Ui::MainWindow)
{
  // create an openGL format and pass to the new GLWidget
  QGLFormat format;
  format.setVersion(3,2);
  format.setProfile( QGLFormat::CoreProfile);


	// setup the user interface
	m_ui->setupUi(this);
	// create our GL window for drawing the spring
	m_gl=new  GLWindow(format,m_ui->m_timerValue->value(),this);
	// add the glWindow to the UI
	m_ui->s_mainWindowGridLayout->addWidget(m_gl,0,0,4,1);
	// create a new spring using the defaul ui values
	m_spring=new RK4Spring(
                          ngl::Vec3(m_ui->m_aX->value(),m_ui->m_aY->value(),m_ui->m_aZ->value()),
                          ngl::Vec3(m_ui->m_bX->value(),m_ui->m_bY->value(),m_ui->m_bZ->value()),
													m_ui->m_length->value(),
												  m_ui->m_k->value(),
													m_ui->m_b->value()
												);
	// set some spring parameters from the ui values
	m_spring->setAFixed(m_ui->m_aFixed->checkState());
	m_spring->setBFixed(m_ui->m_bFixed->checkState());

	m_spring->setTimeStep(m_ui->m_dt->value());
	// now set the spring in the GL window as we share it
	m_gl->setSpring(m_spring);
	// now we wire up the UI components to the slots
	connect(m_ui->m_k,SIGNAL(valueChanged(double)),this,SLOT(setK(double)));
	connect(m_ui->m_b,SIGNAL(valueChanged(double)),this,SLOT(setDamping(double)));
	connect(m_ui->m_length,SIGNAL(valueChanged(double)),this,SLOT(setLength(double)));
	connect(m_ui->m_dt,SIGNAL(valueChanged(double)),this,SLOT(setDT(double)));
	connect(m_ui->m_reset,SIGNAL(clicked()),this,SLOT(reset()));

	connect(m_ui->m_aX,SIGNAL(valueChanged(double)),this,SLOT(setAPosition()));
	connect(m_ui->m_aY,SIGNAL(valueChanged(double)),this,SLOT(setAPosition()));
	connect(m_ui->m_aZ,SIGNAL(valueChanged(double)),this,SLOT(setAPosition()));


	connect(m_ui->m_bX,SIGNAL(valueChanged(double)),this,SLOT(setBPosition()));
	connect(m_ui->m_bY,SIGNAL(valueChanged(double)),this,SLOT(setBPosition()));
	connect(m_ui->m_bZ,SIGNAL(valueChanged(double)),this,SLOT(setBPosition()));
	connect(m_ui->m_aFixed,SIGNAL(clicked(bool)),this,SLOT(setAFixed(bool)));
	connect(m_ui->m_bFixed,SIGNAL(clicked(bool)),this,SLOT(setBFixed(bool)));
	connect(m_ui->m_sim,SIGNAL(clicked(bool)),this,SLOT(toggleSim(bool)));
	connect(m_ui->m_timerValue,SIGNAL(valueChanged(int)),m_gl,SLOT(setTimerDuration(int)));
}

//----------------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
		delete m_spring;
    delete m_ui;
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::setAPosition()
{
m_spring->setAPosition(
											 ngl::Vec3(
																	 m_ui->m_aX->value(),
																	 m_ui->m_aY->value(),
																	 m_ui->m_aZ->value()
																	)
											);

}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::setBPosition()
{
m_spring->setBPosition(
											 ngl::Vec3(
																	 m_ui->m_bX->value(),
																	 m_ui->m_bY->value(),
																	 m_ui->m_bZ->value()
																	)
											);

}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::toggleSim(bool _s)
{
	if(_s == true)
	{
		m_gl->startSimTimer();
	}
	else
	{
		m_gl->stopSimTimer();
	}
}
