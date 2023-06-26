#-----------------------------------------------------------------------------
# File: tst_ChannelReader.pro
#-----------------------------------------------------------------------------
# Project: Kactus 2
# Author: Janne Virtanen
# Date: 21.09.2015
#
# Description:
# Qt project file template for running unit tests for Channel reader.
#-----------------------------------------------------------------------------

TEMPLATE = app

TARGET = tst_ChannelReader

QT += core xml testlib
QT -= gui

CONFIG += c++11 testcase console

LIBS += -L../../../executable -lIPXACTmodels

INCLUDEPATH += $$DESTDIR
INCLUDEPATH += ../../../

DEPENDPATH += .
DEPENDPATH += ../../../

OBJECTS_DIR += $$DESTDIR

MOC_DIR += ./generatedFiles
UI_DIR += ./generatedFiles
RCC_DIR += ./generatedFiles

include(tst_ChannelReader.pri)
