#pragma once

#include <QMainWindow>
#include "handy_options.h"

namespace Ui {
  class HandyViewer;
}

class HandyViewer : public QMainWindow
{
  Q_OBJECT
  
  public:
    explicit HandyViewer(QWidget *parent = 0);
    ~HandyViewer();
    bool init(const handy::HandyOptions& options);
  
  private:
    Ui::HandyViewer *ui;
};
