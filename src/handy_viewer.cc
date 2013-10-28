#include "handy_viewer.h"
#include "ui_handy_viewer.h"

HandyViewer::HandyViewer(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::HandyViewer)
{
  ui->setupUi(this);
}

HandyViewer::~HandyViewer()
{
  delete ui;
}

bool HandyViewer::init(const handy::HandyOptions& options)
{
  return true;
  // Do some stuff here to initialise shaders hand model etc
}

