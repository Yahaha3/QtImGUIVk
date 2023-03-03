QT -= gui

QT += widgets core network websockets

CONFIG += c++17
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += CURL_STATICLIB
DEFINES += DRAW_IMGUI_DEMO

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include($$_PRO_FILE_PWD_/projects/app-install-to-sdk.pri)
include(projects/imgui.pri)

include($$(AOS_KERNEL_ROOT)/projects/aos-kernel-core.pri)
include($$(AOS_KERNEL_ROOT)/projects/aos-kernel-conflux.pri)
include($$(AOS_KERNEL_ROOT)/projects/sharedmemory.pri)

include($$(EQUINOX_ROOT)/projects/eqnx-datahelper.pri)
include($$(EQUINOX_ROOT)/projects/eqnx-cflx-database.pri)
include($$(EQUINOX_ROOT)/projects/equinox-common.pri)

win32 {
    INCLUDEPATH += $$(VULKAN_SDK)/Include
#    DEFINES *= VK_USE_PLATFORM_WIN32_KHR
    CONFIG(debug, debug|release) {
        LIBS *= -L$$(VULKAN_SDK)/Lib -lvulkan-1 #-lGenericCodeGend -lMachineIndependentd -lOGLCompilerd -lOSDependentd -lglslangd -lspirv-cross-cd -lspirv-cross-cored -lspirv-cross-cppd -lspirv-cross-glsld -lspirv-cross-reflectd -lspirv-cross-utild -lSPIRVd -lSPIRV-Tools-optd -lSPIRV-Toolsd
    } else {
        LIBS *= -L$$(VULKAN_SDK)/Lib -lvulkan-1 #-lGenericCodeGen -lMachineIndependent -lOGLCompiler -lOSDependent -lglslang -lspirv-cross-c -lspirv-cross-core -lspirv-cross-cpp -lspirv-cross-glsl -lspirv-cross-reflect -lspirv-cross-util -lSPIRV -lSPIRV-Tools-opt -lSPIRV-Tools
    }
}

INCLUDEPATH += ./3rdparty
INCLUDEPATH += ./3rdparty/imgui
INCLUDEPATH += ./3rdparty/imgui/backends
INCLUDEPATH += ./3rdparty/implot

INCLUDEPATH += ./3rdparty/ffmpeg-master-latest-win64-gpl-shared/include

LIBS += -L$$PWD/3rdparty/ffmpeg-master-latest-win64-gpl-shared/lib\
    -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lswresample -lswscale


SOURCES += \
    Element/ElementContainer.cpp \
    Element/GraphicsDrawElement.cpp \
    Element/GraphicsElement.cpp \
    Element/ImageDrawElement.cpp \
    Element/Layer/GraphicsLayer.cpp \
    Element/LinestringDrawElement.cpp \
    GeoFunction/GeoFunctionUtility.cpp \
    GeoFunction/GeoMapGlobal.cpp \
    GeoFunction/LayerBackend/GeoBackendGaode.cpp \
    GeoFunction/LayerBackend/GeoBackendGoogle.cpp \
    Map/GeoMap.cpp \
    Demo/demowidget.cpp \
    GeoFunction/LayerBackend/GeoBackend.cpp \
    GeoFunction/LayerBackend/GeoBackendOSM.cpp \
    Map/MapItem/MapCopyrightItem.cpp \
    Map/MapItem/MapItem.cpp \
    Map/MapItem/MapLabelItem.cpp \
    Map/MapItem/MapScaleItem.cpp \
    Map/Projection/Projection.cpp \
    Map/Projection/ProjectionESG3857.cpp \
    Map/TileManager.cpp \
    MapSupport/ClzMapSupport.cpp \
    MapThread/MapRequestThread.cpp \
    Operator/OperatorBase.cpp \
    Operator/OperatorContainer.cpp \
    Operator/OperatorMark.cpp \
    Video/VideoDecode.cpp \
    common/Image.cpp \
    common/vulkanwindow.cpp \
    vulkanmain.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
win32:QMAKE_CXXFLAGS += /bigobj

HEADERS += \
    Element/ElementContainer.h \
    Element/GraphicsDrawElement.h \
    Element/GraphicsElement.h \
    Element/ImageDrawElement.h \
    Element/Layer/GraphicsLayer.h \
    Element/LinestringDrawElement.h \
    Element/element_include.h \
    GeoFunction/GeoFunctionUtility.h \
    GeoFunction/LayerBackend/GeoBackendGaode.h \
    GeoFunction/LayerBackend/GeoBackendGoogle.h \
    Map/GeoMap.h \
    Demo/demowidget.h \
    GeoFunction/GeoMapGlobal.h \
    GeoFunction/LayerBackend/GeoBackend.h \
    GeoFunction/LayerBackend/GeoBackendOSM.h \
    Map/MapItem/MapCopyrightItem.h \
    Map/MapItem/MapItem.h \
    Map/MapItem/MapLabelItem.h \
    Map/MapItem/MapScaleItem.h \
    Map/TileManager.h \
    Map/map_include.h \
    Map/Projection/Projection.h \
    Map/Projection/ProjectionESG3857.h \
    MapSupport/ClzMapSupport.h \
    MapThread/MapRequestThread.h \
    Operator/OperatorBase.h \
    Operator/OperatorContainer.h \
    Operator/OperatorMark.h \
    Video/VideoDecode.h \
    common/Image.h \
    common/clz_geomap_global.h \
    common/vulkanwindow.h

#copyApp(clzGUI-Qt)
