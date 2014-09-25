#include <QApplication>
#include "MainWindow.h"
#include "RK4Spring.h"
/* this code runs the basic main window and is created by the Qt Creator app */
int main(int argc, char *argv[])
{


	// make an instance of the QApplication
  QApplication a(argc, argv);
  // Create a new MainWindow
  MainWindow w;
  // show it
  w.show();
  // hand control over to Qt framework
  return a.exec();
}
