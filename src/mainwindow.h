#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QStandardItemModel;
class QSortFilterProxyModel;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_p0_clicked();

    void on_p1_clicked();

    void on_lineEdit_textChanged(const QString &filterText);

    void updateTableView();

    void saveToFile(const QString &text);

    void on_ps_clicked();

private:
    void setupModel();
    void updateButtons(int row);

private:
    Ui::MainWindow *ui;
    QStandardItemModel*model;
    QSortFilterProxyModel *proxyModel;
};
#endif // MAINWINDOW_H
