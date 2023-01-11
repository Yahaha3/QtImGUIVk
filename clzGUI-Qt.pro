QT -= gui

QT += widgets core

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

include(projects/imgui.pri)

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


SOURCES += \
    Demo/demowidget.cpp \
    common/vulkanwindow.cpp \
    vulkanmain.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
win32:QMAKE_CXXFLAGS += /bigobj

HEADERS += \
    Demo/demowidget.h \
    common/Image.h \
    common/vulkanwindow.h
