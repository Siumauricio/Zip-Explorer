#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QFileSystemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
void setModel(QAbstractItemModel *model);
void createTree();
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

void on_treeWidget_clicked(const QModelIndex &index);

void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

void on_actionopen_triggered();

void on_actionclose_triggered();

private:
Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
