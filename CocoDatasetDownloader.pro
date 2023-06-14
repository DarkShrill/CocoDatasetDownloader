QT      += quick
CONFIG  += c++11
QT      += qml
QT      += network
QT      += quickcontrols2
QT      += widgets
QT      += core gui
QT      += multimedia

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        coco\annotationtype.cpp \
        coco\categoriestype.cpp \
        coco\cocowrapper.cpp \
        coco\imagetype.cpp \
        datasetcreator.cpp \
        main.cpp \
        coco\segmentationtype.cpp \
        radialbar.cpp

RESOURCES += qml.qrc
RESOURCES += medias.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    coco\annotationtype.h \
    coco\categoriestype.h \
    coco\cocowrapper.h \
    coco\imagetype.h \
    coco\segmentationtype.h \
    datasetcreator.h \
    radialbar.h
