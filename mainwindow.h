#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
   MainWindow(QWidget* parent = 0, Qt::WindowFlags flags = 0);
   MainWindow(QString configFile, QWidget* parent = 0, Qt::WindowFlags flags = 0);
   ~MainWindow();
  
private slots:
   void openConfigFile();
   //void saveConfigFile();
   //void saveConfigFileAs();
   //void addItem();
   //void delItem();
   //void doReport();
  
private:
   void createLayout();
   
   QMenu *m_fileMenu;
   QMenu *m_toolMenu;
   QAction *m_openAct;
   QAction *m_saveAct;
   QAction *m_saveAsAct;
   QAction *m_exitAct;
   QAction *m_addItemAct;
   QAction *m_delItemAct;
   QAction *m_doReport;
   QString m_currentConfigFile;
};

#endif // MAINWINDOW_H
