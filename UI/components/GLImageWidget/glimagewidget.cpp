#include "glimagewidget.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QPainter>
#include <QDebug>

GLImageWidget::GLImageWidget(QWidget *parent):
    QOpenGLWidget(parent)
{
}

GLImageWidget::~GLImageWidget()
{
    makeCurrent();
    vbo.destroy();
    textureImg->destroy();
    doneCurrent();
}

void GLImageWidget::slotShowImage(QImage img)
{
    _img = img;
    videoW = img.width();
    videoH = img.height();
    update();
}

void GLImageWidget::initializeGL()
{
    initializeOpenGLFunctions();

    static const GLfloat vertices[]{
        //顶点坐标
        -1.0f,-1.0f,
        -1.0f,+1.0f,
        +1.0f,+1.0f,
        +1.0f,-1.0f,
        //视频上面的矩形框顶点
        -0.8f,-0.8f,
        -0.8f,+0.8f,
        +0.8f,+0.8f,
        +0.8f,-0.8f,
        //纹理坐标
        0.0f,1.0f,1.0f,
        0.0f,0.0f,1.0f,
        1.0f,0.0f,1.0f,
        1.0f,1.0f,1.0f,
        //矩形框顶点的颜色
        0.0f,1.0f,1.0f,
        0.0f,1.0f,1.0f,
        0.0f,1.0f,1.0f,
        0.0f,1.0f,1.0f,
    };


    vbo.create();
    vbo.bind();
    vbo.allocate(vertices,sizeof(vertices));

    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex,this);
    const char *vsrc =
   "attribute vec4 vertexIn; \
    attribute vec4 textureIn; \
    varying vec4 textureOut;  \
    void main(void)           \
    {                         \
        gl_Position = vertexIn; \
        textureOut = textureIn; \
    }";
    vshader->compileSourceCode(vsrc);

    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment,this);
    const char *fsrc = "varying vec4 textureOut; \
    uniform sampler2D imageTexture; \
    uniform bool drawImg; \
    void main(void) \
    { \
        if(drawImg){ \
            gl_FragColor = texture(imageTexture, textureOut.st); \
        }else{\
            gl_FragColor = textureOut; \
             }\
    }";
    fshader->compileSourceCode(fsrc);

    program = new QOpenGLShaderProgram(this);
    program->addShader(vshader);
    program->addShader(fshader);
    program->link();
    program->bind();
    program->enableAttributeArray("vertexIn");
    program->enableAttributeArray("textureIn");
    program->setUniformValue("imageTexture",0);
    program->setAttributeBuffer("vertexIn",GL_FLOAT,0,2,2*sizeof(GLfloat));
    program->setAttributeBuffer("textureIn",GL_FLOAT,16*sizeof(GLfloat),3,3*sizeof(GLfloat));

    textureImg = new QOpenGLTexture(QOpenGLTexture::Target2D);
    glClearColor(0.0,0.0,0.0,1.0);
//    glEnable(GL_DEPTH_TEST);
}

void GLImageWidget::paintGL()
{
//    QMatrix4x4 m;
//    m.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f );//透视矩阵随距离的变化，图形跟着变化。屏幕平面中心就是视点（摄像头）,需要将图形移向屏幕里面一定距离。
//    m.ortho(-2,+2,-2,+2,-10,10);//近裁剪平面是一个矩形,矩形左下角点三维空间坐标是（left,bottom,-near）,右上角点是（right,top,-near）所以此处为负，表示z轴最大为10；
                                //远裁剪平面也是一个矩形,左下角点空间坐标是（left,bottom,-far）,右上角点是（right,top,-far）所以此处为正，表示z轴最小为-10；
                                //此时坐标中心还是在屏幕水平面中间，只是前后左右的距离已限制
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    program->setUniformValue("drawImg",true);
    glActiveTexture(GL_TEXTURE0);  //激活纹理单元GL_TEXTURE0,系统里面的
    textureImg->setMinificationFilter(QOpenGLTexture::Nearest);
    textureImg->setMagnificationFilter(QOpenGLTexture::Nearest);
    textureImg->setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::ClampToEdge);
    textureImg->setWrapMode(QOpenGLTexture::DirectionT,QOpenGLTexture::ClampToEdge);
    textureImg->setData(_img);
    textureImg->bind();
//    使用顶点数组方式绘制图形
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    textureImg->destroy();

    program->setUniformValue("drawImg",false);
    glDrawArrays(GL_LINE_LOOP, 4, 4);
}
