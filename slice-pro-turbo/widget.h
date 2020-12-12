#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtOpenGL>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class GLWidget;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_2_clicked();

    void on_checkBox_toggled(bool checked);

    void on_SliceAuto_clicked();

signals:
    void stlOpened(QFile* file);
    void toggleWireframe(bool show);
    void sliceAuto();

private:
    Ui::Widget *ui;

    GLWidget *glWidget;
};
#endif // WIDGET_H
