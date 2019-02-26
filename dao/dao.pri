include (core/core.pri)

HEADERS += \
    $$PWD/face_constants.h \
    $$PWD/face_types.h \
    $$PWD/FaceServ.h \
    $$PWD/face.pb.h \
    $$PWD/thriftdao.h \
    $$PWD/cloudhttpdao.h \
    $$PWD/cpphttpdao.h \
    $$PWD/Videoface.pb.h

SOURCES += \
    $$PWD/face_constants.cpp \
    $$PWD/face_types.cpp \
    $$PWD/FaceServ.cpp \
    $$PWD/face.pb.cc \
    $$PWD/thriftdao.cpp \
    $$PWD/cloudhttpdao.cpp \
    $$PWD/cpphttpdao.cpp \
    $$PWD/Videoface.pb.cc
