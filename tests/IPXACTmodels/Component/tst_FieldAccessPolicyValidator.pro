#-----------------------------------------------------------------------------
# File: tst_FieldAccessPolicyValidator.pro
#-----------------------------------------------------------------------------
# Project: Kactus 2
# Author: Anton Hagqvist
# Date: 2.8s.2023
#
# Description:
# Qt project file template for running unit tests for FieldAccessPolicyValidator.
#-----------------------------------------------------------------------------

TEMPLATE = app

TARGET = tst_FieldAccessPolicyValidator

QT += core xml testlib
QT -= gui

CONFIG += c++11 testcase console

linux-g++ | linux-g++-64 | linux-g++-32 {
 LIBS += -L../../../executable \
     -lIPXACTmodels \
     -lKactusAPI

}
win64 | win32 {
 LIBS += -L../../../executable \
     -lIPXACTmodelsd \
     -lKactusAPId
}

INCLUDEPATH += $$DESTDIR
INCLUDEPATH += ../../../
INCLUDEPATH += ../../../KactusAPI/include

DEPENDPATH += .
DEPENDPATH += ../../../

OBJECTS_DIR += $$DESTDIR

MOC_DIR += ./generatedFiles
UI_DIR += ./generatedFiles
RCC_DIR += ./generatedFiles
include(tst_FieldAccessPolicyValidator.pri)