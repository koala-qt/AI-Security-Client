#ifndef GLIMAGEWIDGET_H
#define GLIMAGEWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QTimer>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)

class GLImageWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    GLImageWidget(QWidget *parent =0);
    ~GLImageWidget();

public slots:
    void slotShowImage(QImage);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

private:
    QOpenGLShaderProgram *program;
    QOpenGLBuffer vbo;
    QOpenGLTexture *textureImg = nullptr;
    uint videoW,videoH;
    QImage _img;
};

#endif // GLIMAGEWIDGET_H
