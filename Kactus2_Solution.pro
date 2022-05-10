# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = subdirs

SUBDIRS = \
    IPXACTmodels \
    KactusAPI \
    Kactus2 \
    PythonAPI \
    MCAPICodeGenerator \
    CppSourceAnalyzer \
    VHDLSourceAnalyzer \
    MemoryMapHeaderGenerator \
    ModelSimGenerator \
    AlteraBSPGenerator \
    QuartusPinImportPlugin \
    PadsPartGenerator \
    MakefileGenerator \
    VerilogGeneratorPlugin \
    VHDLGenerator \
    VHDLimport \
    VerilogImport \
    VerilogIncludeImport \
    VerilogSourceAnalyzer \
    QuartusProjectGenerator \
    MemoryViewGenerator \
    LinuxDeviceTreeGenerator \
    SVDGenerator

CONFIG(test) {
    SUBDIRS += test_all
}

IPXACTmodels.subdir = IPXACTmodels

Kactus2.file = Kactus2.pro
Kactus2.depends = IPXACTmodels
Kactus2.makefile = Makefile.Kactus2

KactusAPI.subdir = KactusAPI
KactusAPI.depends = IPXACTmodels

PythonAPI.subdir = PythonAPI
PythonAPI.depends = IPXACTmodels KactusAPI

MCAPICodeGenerator.subdir = Plugins/MCAPICodeGenerator
MCAPICodeGenerator.depends = IPXACTmodels KactusAPI

CppSourceAnalyzer.subdir = Plugins/CppSourceAnalyzer
CppSourceAnalyzer.depends = IPXACTmodels

VHDLSourceAnalyzer.file = Plugins/VHDLAnalyzer/VHDLSourceAnalyzer.pro
VHDLSourceAnalyzer.depends = IPXACTmodels KactusAPI

MemoryMapHeaderGenerator.subdir = Plugins/MemoryMapHeaderGenerator
MemoryMapHeaderGenerator.depends = IPXACTmodels KactusAPI

ModelSimGenerator.subdir = Plugins/ModelSimGenerator
ModelSimGenerator.depends = IPXACTmodels KactusAPI

AlteraBSPGenerator.subdir = Plugins/AlteraBSPGenerator
AlteraBSPGenerator.depends = IPXACTmodels

QuartusPinImportPlugin.subdir = Plugins/QuartusPinImportPlugin
QuartusPinImportPlugin.depends = IPXACTmodels

PadsPartGenerator.subdir = Plugins/PadsPartGenerator
PadsPartGenerator.depends = IPXACTmodels KactusAPI

MakefileGenerator.subdir = Plugins/MakefileGenerator
MakefileGenerator.depends = IPXACTmodels KactusAPI

VerilogGeneratorPlugin.file = Plugins/VerilogGenerator/VerilogGeneratorPlugin.pro
VerilogGeneratorPlugin.depends = IPXACTmodels KactusAPI

VHDLGenerator.subdir = Plugins/VHDLGenerator
VHDLGenerator.depends = IPXACTmodels KactusAPI

VHDLimport.subdir = Plugins/VHDLimport
VHDLimport.depends = IPXACTmodels

VerilogImport.subdir = Plugins/VerilogImport
VerilogImport.depends = IPXACTmodels KactusAPI

VerilogIncludeImport.subdir = Plugins/VerilogIncludeImport
VerilogIncludeImport.depends = IPXACTmodels

VerilogSourceAnalyzer.subdir = Plugins/VerilogSourceAnalyzer
VerilogSourceAnalyzer.depends = IPXACTmodels

QuartusProjectGenerator.subdir = Plugins/QuartusProjectGenerator
QuartusProjectGenerator.depends = IPXACTmodels

MemoryViewGenerator.subdir = Plugins/MemoryViewGenerator
MemoryViewGenerator.depends = IPXACTmodels KactusAPI

LinuxDeviceTreeGenerator.file = Plugins/LinuxDeviceTree/LinuxDeviceTreeGenerator.pro
LinuxDeviceTreeGenerator.depends = IPXACTmodels KactusAPI

SVDGenerator.file = Plugins/SVDGenerator/SVDGeneratorPlugin.pro
SVDGenerator.depends = IPXACTmodels KactusAPI

test_all.subdir = tests
test_all.depends = IPXACTmodels

