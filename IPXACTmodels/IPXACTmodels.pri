# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

# This is a reminder that you are using a generated .pro file.
# Remove it when you are finished editing this file.
message("You are running qmake on a generated .pro file. This may not work!")


HEADERS += ./generaldeclarations.h \
    ./ipxactmodels_global.h \
    ./kactusExtensions/ApiDefinition.h \
    ./kactusExtensions/ApiFunction.h \
    ./kactusExtensions/ApiFunctionParameter.h \
    ./kactusExtensions/ApiInterconnection.h \
    ./kactusExtensions/ApiInterface.h \
    ./kactusExtensions/ColumnDesc.h \
    ./kactusExtensions/ComDefinition.h \
    ./kactusExtensions/ComInterconnection.h \
    ./kactusExtensions/ComInterface.h \
    ./kactusExtensions/ComProperty.h \
    ./kactusExtensions/ConnectionRoute.h \
    ./kactusExtensions/FileDependency.h \
    ./kactusExtensions/HierApiInterconnection.h \
    ./kactusExtensions/HierComInterconnection.h \
    ./kactusExtensions/InterfaceGraphicsData.h \
    ./kactusExtensions/Kactus2Array.h \
    ./kactusExtensions/Kactus2Group.h \
    ./kactusExtensions/Kactus2Placeholder.h \
    ./kactusExtensions/Kactus2Position.h \
    ./kactusExtensions/Kactus2Value.h \
    ./kactusExtensions/KactusAttribute.h \
    ./kactusExtensions/SystemView.h \
    ./kactusExtensions/validators/ApiDefinitionValidator.h \
    ./kactusExtensions/validators/ComDefinitionValidator.h \
    ./kactusExtensions/ApiDefinitionWriter.h \
    ./kactusExtensions/ApiDefinitionReader.h \
    ./kactusExtensions/ComDefinitionWriter.h \
    ./kactusExtensions/ComDefinitionReader.h \
    ./kactusExtensions/validators/SystemDesignConfigurationValidator.h \
    ./kactusExtensions/validators/SystemViewConfigurationValidator.h \
    ./common/ChoiceWriter.h \
    ./common/NameGroupWriter.h \
    ./common/TagData.h \
    ./common/AccessTypes.h \
    ./common/Array.h \
    ./common/Assertion.h \
    ./common/BooleanValue.h \
    ./common/BuildModel.h \
    ./common/CellSpecification.h \
    ./common/ClockUnit.h \
    ./common/ConfigurableElementValue.h \
    ./common/ConfigurableVLNVReference.h \
    ./common/DirectionTypes.h \
    ./common/Document.h \
    ./common/Enumeration.h \
    ./common/Extendable.h \
    ./common/FileBuilder.h \
    ./common/FileTypes.h \
    ./common/ModuleParameter.h \
    ./common/NameGroup.h \
    ./common/NameValuePair.h \
    ./common/Parameter.h \
    ./common/PartSelect.h \
    ./common/PortAlignment.h \
    ./common/PresenceTypes.h \
    ./common/Protocol.h \
    ./common/Qualifier.h \
    ./common/Range.h \
    ./common/TimingConstraint.h \
    ./common/TransactionalTypes.h \
    ./common/Vector.h \
    ./common/VendorExtension.h \
    ./common/VLNV.h \
    ./common/CommonItemsReader.h \
    ./common/CommonItemsWriter.h \
    ./common/DocumentReader.h \
    ./common/DocumentWriter.h \
    ./common/FileBuilderReader.h \
    ./common/FileBuilderWriter.h \
    ./common/GenericVendorExtension.h \
    ./common/ModuleParameterReader.h \
    ./common/ModuleParameterWriter.h \
    ./common/NameGroupReader.h \
    ./common/ParameterReader.h \
    ./common/ParameterWriter.h \
    ./common/ProtocolReader.h \
    ./common/ProtocolWriter.h \
    ./common/validators/CommonItemsValidator.h \
    ./common/validators/AssertionValidator.h \
    ./common/validators/CellSpecificationValidator.h \
    ./common/validators/ParameterValidator.h \
    ./common/validators/ProtocolValidator.h \
    ./common/validators/TimingConstraintValidator.h \
    ./common/validators/ValueFormatter.h \
    ./common/validators/namevalidator.h \
    ./common/validators/NMTokenValidator.h \
    ./BusDefinition/BusDefinition.h \
    ./BusDefinition/BusDefinitionReader.h \
    ./BusDefinition/BusDefinitionWriter.h \
    ./BusDefinition/validators/BusDefinitionValidator.h \
    ./AbstractionDefinition/AbstractionDefinition.h \
    ./AbstractionDefinition/Packet.h \
    ./AbstractionDefinition/PacketField.h \
    ./AbstractionDefinition/PacketReader.h \
    ./AbstractionDefinition/PacketWriter.h \
    ./AbstractionDefinition/PortAbstraction.h \
    ./AbstractionDefinition/TransactionalAbstraction.h \
    ./AbstractionDefinition/TransactionalAbstractionReader.h \
    ./AbstractionDefinition/TransactionalPort.h \
    ./AbstractionDefinition/WireAbstraction.h \
    ./AbstractionDefinition/WireAbstractionReader.h \
    ./AbstractionDefinition/WireAbstractionWriter.h \
    ./AbstractionDefinition/WirePort.h \
    ./AbstractionDefinition/AbstractionDefinitionReader.h \
    ./AbstractionDefinition/AbstractionDefinitionWriter.h \
    ./AbstractionDefinition/TransactionalAbstractionWriter.h \
    ./AbstractionDefinition/validators/AbstractionDefinitionValidator.h \
    ./Design/ActiveInterface.h \
    ./Design/AdHocConnection.h \
    ./Design/ComponentInstance.h \
    ./Design/Design.h \
    ./Design/HierInterface.h \
    ./Design/Interconnection.h \
    ./Design/MonitorInterconnection.h \
    ./Design/MonitorInterface.h \
    ./Design/PortReference.h \
    ./Design/ComponentInstanceReader.h \
    ./Design/ComponentInstanceWriter.h \
    ./Design/DesignReader.h \
    ./Design/DesignWriter.h \
    ./Design/validator/AdHocConnectionValidator.h \
    ./Design/validator/ComponentInstanceValidator.h \
    ./Design/validator/DesignValidator.h \
    ./Design/validator/InterconnectionValidator.h \
    ./Component/FieldReset.h \
    ./Component/MemoryBlockBaseReader.h \
    ./Component/MemoryBlockBaseWriter.h \
    ./Component/SubSpaceMap.h \
    ./Component/SubspaceMapReader.h \
    ./Component/SubSpaceMapWriter.h \
    ./Component/AddressBlock.h \
    ./Component/AddressSpace.h \
    ./Component/AlternateRegister.h \
    ./Component/BuildCommand.h \
    ./Component/BusInterface.h \
    ./Component/Channel.h \
    ./Component/Choice.h \
    ./Component/Component.h \
    ./Component/ComponentGenerator.h \
    ./Component/ComponentInstantiation.h \
    ./Component/Cpu.h \
    ./Component/DesignConfigurationInstantiation.h \
    ./Component/DesignInstantiation.h \
    ./Component/EnumeratedValue.h \
    ./Component/Field.h \
    ./Component/File.h \
    ./Component/FileSet.h \
    ./Component/Function.h \
    ./Component/IndirectInterface.h \
    ./Component/IndirectInterfaceReader.h \
    ./Component/IndirectInterfaceWriter.h \
    ./Component/MasterInterface.h \
    ./Component/MemoryBlockBase.h \
    ./Component/MemoryMap.h \
    ./Component/MemoryMapBase.h \
    ./Component/MemoryRemap.h \
    ./Component/MirroredSlaveInterface.h \
    ./Component/Model.h \
    ./Component/AbstractionType.h \
    ./Component/OtherClockDriver.h \
    ./Component/Port.h \
    ./Component/PortMap.h \
    ./Component/Register.h \
    ./Component/RegisterBase.h \
    ./Component/RegisterDefinition.h \
    ./Component/RegisterFile.h \
    ./Component/RemapPort.h \
    ./Component/RemapState.h \
    ./Component/Segment.h \
    ./Component/SlaveInterface.h \
    ./Component/Transactional.h \
    ./Component/TransparentBridge.h \
    ./Component/View.h \
    ./Component/Wire.h \
    ./Component/WireTypeDef.h \
    ./Component/WriteValueConstraint.h \
    ./Component/AddressBlockReader.h \
    ./Component/AddressBlockWriter.h \
    ./Component/AddressSpaceReader.h \
    ./Component/AddressSpaceWriter.h \
    ./Component/BusInterfaceReader.h \
    ./Component/BusInterfaceWriter.h \
    ./Component/ChannelReader.h \
    ./Component/ChannelWriter.h \
    ./Component/ComponentGeneratorReader.h \
    ./Component/ComponentGeneratorWriter.h \
    ./Component/ComponentReader.h \
    ./Component/ComponentWriter.h \
    ./Component/CPUReader.h \
    ./Component/CPUWriter.h \
    ./Component/EnumeratedValueReader.h \
    ./Component/EnumeratedValueWriter.h \
    ./Component/FieldReader.h \
    ./Component/FieldWriter.h \
    ./Component/FileReader.h \
    ./Component/FileSetReader.h \
    ./Component/FileSetWriter.h \
    ./Component/FileWriter.h \
    ./Component/InstantiationsReader.h \
    ./Component/InstantiationsWriter.h \
    ./Component/MemoryMapBaseReader.h \
    ./Component/MemoryMapBaseWriter.h \
    ./Component/MemoryMapReader.h \
    ./Component/MemoryMapWriter.h \
    ./Component/PortReader.h \
    ./Component/PortWriter.h \
    ./Component/RegisterReader.h \
    ./Component/RegisterWriter.h \
    ./Component/ChoiceReader.h \
    ./Component/OtherClockDriverReader.h \
    ./Component/OtherClockDriverWriter.h \
    ./Component/RemapStateReader.h \
    ./Component/RemapStateWriter.h \
    ./Component/ViewReader.h \
    ./Component/ViewWriter.h \
    ./Component/validators/MemoryBlockValidator.h \
    ./Component/validators/RegisterBaseValidator.h \
    ./Component/validators/RegisterFileValidator.h \
    ./Component/validators/SubspaceMapValidator.h \
    ./Component/validators/AbstractionTypeValidator.h \
    ./Component/validators/AddressBlockValidator.h \
    ./Component/validators/AddressSpaceValidator.h \
    ./Component/validators/BusInterfaceValidator.h \
    ./Component/validators/ChannelValidator.h \
    ./Component/validators/ChoiceValidator.h \
    ./Component/validators/ComponentGeneratorValidator.h \
    ./Component/validators/ComponentValidator.h \
    ./Component/validators/CPUValidator.h \
    ./Component/validators/EnumeratedValueValidator.h \
    ./Component/validators/FieldValidator.h \
    ./Component/validators/FileSetValidator.h \
    ./Component/validators/FileValidator.h \
    ./Component/validators/IndirectInterfaceValidator.h \
    ./Component/validators/InstantiationsValidator.h \
    ./Component/validators/PortMapValidator.h \
    ./Component/validators/OtherClockDriverValidator.h \
    ./Component/validators/PortTypeValidator.h \
    ./Component/validators/PortValidator.h \
    ./Component/validators/RemapStateValidator.h \
    ./Component/validators/MemoryMapBaseValidator.h \
    ./Component/validators/MemoryMapValidator.h \
    ./Component/validators/MemoryReserve.h \
    ./Component/validators/RegisterValidator.h \
    ./Component/validators/ViewValidator.h \
    ./Catalog/Catalog.h \
    ./Catalog/IpxactFile.h \
    ./Catalog/CatalogReader.h \
    ./Catalog/CatalogWriter.h \
    ./Catalog/validators/CatalogValidator.h \
    ./utilities/BusDefinitionUtils.h \
    ./utilities/XmlUtils.h \
    ./DesignConfiguration/AbstractorInstance.h \
    ./DesignConfiguration/DesignConfiguration.h \
    ./DesignConfiguration/DesignConfigurationReader.h \
    ./DesignConfiguration/DesignConfigurationWriter.h \
    ./DesignConfiguration/InterconnectionConfiguration.h \
    ./DesignConfiguration/InterfaceRef.h \
    ./DesignConfiguration/MultipleAbstractorInstances.h \
    ./DesignConfiguration/ViewConfiguration.h \
    ./DesignConfiguration/validators/DesignConfigurationValidator.h \
    ./DesignConfiguration/validators/InterconnectionConfigurationValidator.h \
    ./DesignConfiguration/validators/ViewConfigurationValidator.h \
    ./common/validators/QualifierValidator.h
SOURCES += ./generaldeclarations.cpp \
    ./kactusExtensions/ApiDefinition.cpp \
    ./kactusExtensions/ApiDefinitionReader.cpp \
    ./kactusExtensions/ApiDefinitionWriter.cpp \
    ./kactusExtensions/ApiFunction.cpp \
    ./kactusExtensions/ApiFunctionParameter.cpp \
    ./kactusExtensions/ApiInterconnection.cpp \
    ./kactusExtensions/ApiInterface.cpp \
    ./kactusExtensions/ColumnDesc.cpp \
    ./kactusExtensions/ComDefinition.cpp \
    ./kactusExtensions/ComDefinitionReader.cpp \
    ./kactusExtensions/ComDefinitionWriter.cpp \
    ./kactusExtensions/ComInterconnection.cpp \
    ./kactusExtensions/ComInterface.cpp \
    ./kactusExtensions/ComProperty.cpp \
    ./kactusExtensions/ConnectionRoute.cpp \
    ./kactusExtensions/FileDependency.cpp \
    ./kactusExtensions/HierApiInterconnection.cpp \
    ./kactusExtensions/HierComInterconnection.cpp \
    ./kactusExtensions/InterfaceGraphicsData.cpp \
    ./kactusExtensions/Kactus2Array.cpp \
    ./kactusExtensions/Kactus2Group.cpp \
    ./kactusExtensions/Kactus2Placeholder.cpp \
    ./kactusExtensions/Kactus2Position.cpp \
    ./kactusExtensions/Kactus2Value.cpp \
    ./kactusExtensions/KactusAttribute.cpp \
    ./kactusExtensions/SystemView.cpp \
    ./kactusExtensions/validators/ApiDefinitionValidator.cpp \
    ./kactusExtensions/validators/ComDefinitionValidator.cpp \
    ./kactusExtensions/validators/SystemDesignConfigurationValidator.cpp \
    ./kactusExtensions/validators/SystemViewConfigurationValidator.cpp \
    ./common/AccessTypes.cpp \
    ./common/Array.cpp \
    ./common/Assertion.cpp \
    ./common/BooleanValue.cpp \
    ./common/BuildModel.cpp \
    ./common/CellSpecification.cpp \
    ./common/ChoiceWriter.cpp \
    ./common/ClockUnit.cpp \
    ./common/CommonItemsReader.cpp \
    ./common/CommonItemsWriter.cpp \
    ./common/ConfigurableElementValue.cpp \
    ./common/ConfigurableVLNVReference.cpp \
    ./common/DirectionTypes.cpp \
    ./common/Document.cpp \
    ./common/DocumentReader.cpp \
    ./common/DocumentWriter.cpp \
    ./common/Enumeration.cpp \
    ./common/Extendable.cpp \
    ./common/FileBuilder.cpp \
    ./common/FileBuilderReader.cpp \
    ./common/FileBuilderWriter.cpp \
    ./common/FileTypes.cpp \
    ./common/GenericVendorExtension.cpp \
    ./common/ModuleParameter.cpp \
    ./common/ModuleParameterReader.cpp \
    ./common/ModuleParameterWriter.cpp \
    ./common/NameGroup.cpp \
    ./common/NameGroupReader.cpp \
    ./common/NameGroupWriter.cpp \
    ./common/NameValuePair.cpp \
    ./common/Parameter.cpp \
    ./common/ParameterReader.cpp \
    ./common/ParameterWriter.cpp \
    ./common/PartSelect.cpp \
    ./common/PortAlignment.cpp \
    ./common/PresenceTypes.cpp \
    ./common/Protocol.cpp \
    ./common/ProtocolReader.cpp \
    ./common/ProtocolWriter.cpp \
    ./common/Qualifier.cpp \
    ./common/Range.cpp \
    ./common/TimingConstraint.cpp \
    ./common/TransactionalTypes.cpp \
    ./common/Vector.cpp \
    ./common/VLNV.cpp \
    ./common/validators/AssertionValidator.cpp \
    ./common/validators/CellSpecificationValidator.cpp \
    ./common/validators/CommonItemsValidator.cpp \
    ./common/validators/namevalidator.cpp \
    ./common/validators/NMTokenValidator.cpp \
    ./common/validators/ParameterValidator.cpp \
    ./common/validators/ProtocolValidator.cpp \
    ./common/validators/TimingConstraintValidator.cpp \
    ./common/validators/ValueFormatter.cpp \
    ./BusDefinition/BusDefinition.cpp \
    ./BusDefinition/BusDefinitionReader.cpp \
    ./BusDefinition/BusDefinitionWriter.cpp \
    ./BusDefinition/validators/BusDefinitionValidator.cpp \
    ./AbstractionDefinition/AbstractionDefinition.cpp \
    ./AbstractionDefinition/AbstractionDefinitionReader.cpp \
    ./AbstractionDefinition/AbstractionDefinitionWriter.cpp \
    ./AbstractionDefinition/Packet.cpp \
    ./AbstractionDefinition/PacketField.cpp \
    ./AbstractionDefinition/PacketReader.cpp \
    ./AbstractionDefinition/PacketWriter.cpp \
    ./AbstractionDefinition/PortAbstraction.cpp \
    ./AbstractionDefinition/TransactionalAbstraction.cpp \
    ./AbstractionDefinition/TransactionalAbstractionReader.cpp \
    ./AbstractionDefinition/TransactionalAbstractionWriter.cpp \
    ./AbstractionDefinition/TransactionalPort.cpp \
    ./AbstractionDefinition/WireAbstraction.cpp \
    ./AbstractionDefinition/WireAbstractionReader.cpp \
    ./AbstractionDefinition/WireAbstractionWriter.cpp \
    ./AbstractionDefinition/WirePort.cpp \
    ./AbstractionDefinition/validators/AbstractionDefinitionValidator.cpp \
    ./Design/ActiveInterface.cpp \
    ./Design/AdHocConnection.cpp \
    ./Design/ComponentInstance.cpp \
    ./Design/ComponentInstanceReader.cpp \
    ./Design/ComponentInstanceWriter.cpp \
    ./Design/Design.cpp \
    ./Design/DesignReader.cpp \
    ./Design/DesignWriter.cpp \
    ./Design/HierInterface.cpp \
    ./Design/Interconnection.cpp \
    ./Design/MonitorInterconnection.cpp \
    ./Design/MonitorInterface.cpp \
    ./Design/PortReference.cpp \
    ./Design/validator/AdHocConnectionValidator.cpp \
    ./Design/validator/ComponentInstanceValidator.cpp \
    ./Design/validator/DesignValidator.cpp \
    ./Design/validator/InterconnectionValidator.cpp \
    ./Component/AddressBlock.cpp \
    ./Component/AddressBlockReader.cpp \
    ./Component/AddressBlockWriter.cpp \
    ./Component/AddressSpace.cpp \
    ./Component/AddressSpaceReader.cpp \
    ./Component/AddressSpaceWriter.cpp \
    ./Component/AlternateRegister.cpp \
    ./Component/BuildCommand.cpp \
    ./Component/BusInterface.cpp \
    ./Component/BusInterfaceReader.cpp \
    ./Component/BusInterfaceWriter.cpp \
    ./Component/Channel.cpp \
    ./Component/ChannelReader.cpp \
    ./Component/ChannelWriter.cpp \
    ./Component/Choice.cpp \
    ./Component/ChoiceReader.cpp \
    ./Component/Component.cpp \
    ./Component/ComponentGenerator.cpp \
    ./Component/ComponentGeneratorReader.cpp \
    ./Component/ComponentGeneratorWriter.cpp \
    ./Component/ComponentInstantiation.cpp \
    ./Component/ComponentReader.cpp \
    ./Component/ComponentWriter.cpp \
    ./Component/Cpu.cpp \
    ./Component/CPUReader.cpp \
    ./Component/CPUWriter.cpp \
    ./Component/DesignConfigurationInstantiation.cpp \
    ./Component/DesignInstantiation.cpp \
    ./Component/EnumeratedValue.cpp \
    ./Component/EnumeratedValueReader.cpp \
    ./Component/EnumeratedValueWriter.cpp \
    ./Component/Field.cpp \
    ./Component/FieldReader.cpp \
    ./Component/FieldReset.cpp \
    ./Component/FieldWriter.cpp \
    ./Component/File.cpp \
    ./Component/FileReader.cpp \
    ./Component/FileSet.cpp \
    ./Component/FileSetReader.cpp \
    ./Component/FileSetWriter.cpp \
    ./Component/FileWriter.cpp \
    ./Component/Function.cpp \
    ./Component/IndirectInterface.cpp \
    ./Component/IndirectInterfaceReader.cpp \
    ./Component/IndirectInterfaceWriter.cpp \
    ./Component/InstantiationsReader.cpp \
    ./Component/InstantiationsWriter.cpp \
    ./Component/MasterInterface.cpp \
    ./Component/MemoryBlockBase.cpp \
    ./Component/MemoryBlockBaseReader.cpp \
    ./Component/MemoryBlockBaseWriter.cpp \
    ./Component/MemoryMap.cpp \
    ./Component/MemoryMapBase.cpp \
    ./Component/MemoryMapBaseReader.cpp \
    ./Component/MemoryMapBaseWriter.cpp \
    ./Component/MemoryMapReader.cpp \
    ./Component/MemoryMapWriter.cpp \
    ./Component/MemoryRemap.cpp \
    ./Component/MirroredSlaveInterface.cpp \
    ./Component/Model.cpp \
    ./Component/AbstractionType.cpp \
    ./Component/OtherClockDriver.cpp \
    ./Component/OtherClockDriverReader.cpp \
    ./Component/OtherClockDriverWriter.cpp \
    ./Component/Port.cpp \
    ./Component/PortMap.cpp \
    ./Component/PortReader.cpp \
    ./Component/PortWriter.cpp \
    ./Component/Register.cpp \
    ./Component/RegisterBase.cpp \
    ./Component/RegisterDefinition.cpp \
    ./Component/RegisterFile.cpp \
    ./Component/RegisterReader.cpp \
    ./Component/RegisterWriter.cpp \
    ./Component/RemapPort.cpp \
    ./Component/RemapState.cpp \
    ./Component/RemapStateReader.cpp \
    ./Component/RemapStateWriter.cpp \
    ./Component/Segment.cpp \
    ./Component/SlaveInterface.cpp \
    ./Component/SubSpaceMap.cpp \
    ./Component/SubspaceMapReader.cpp \
    ./Component/SubSpaceMapWriter.cpp \
    ./Component/Transactional.cpp \
    ./Component/TransparentBridge.cpp \
    ./Component/View.cpp \
    ./Component/ViewReader.cpp \
    ./Component/ViewWriter.cpp \
    ./Component/Wire.cpp \
    ./Component/WireTypeDef.cpp \
    ./Component/WriteValueConstraint.cpp \
    ./Component/validators/AbstractionTypeValidator.cpp \
    ./Component/validators/AddressBlockValidator.cpp \
    ./Component/validators/AddressSpaceValidator.cpp \
    ./Component/validators/BusInterfaceValidator.cpp \
    ./Component/validators/ChannelValidator.cpp \
    ./Component/validators/ChoiceValidator.cpp \
    ./Component/validators/ComponentGeneratorValidator.cpp \
    ./Component/validators/ComponentValidator.cpp \
    ./Component/validators/CPUValidator.cpp \
    ./Component/validators/EnumeratedValueValidator.cpp \
    ./Component/validators/FieldValidator.cpp \
    ./Component/validators/FileSetValidator.cpp \
    ./Component/validators/FileValidator.cpp \
    ./Component/validators/IndirectInterfaceValidator.cpp \
    ./Component/validators/InstantiationsValidator.cpp \
    ./Component/validators/MemoryBlockValidator.cpp \
    ./Component/validators/PortMapValidator.cpp \
    ./Component/validators/OtherClockDriverValidator.cpp \
    ./Component/validators/PortTypeValidator.cpp \
    ./Component/validators/PortValidator.cpp \
    ./Component/validators/RegisterBaseValidator.cpp \
    ./Component/validators/RegisterFileValidator.cpp \
    ./Component/validators/RemapStateValidator.cpp \
    ./Component/validators/MemoryMapBaseValidator.cpp \
    ./Component/validators/MemoryMapValidator.cpp \
    ./Component/validators/MemoryReserve.cpp \
    ./Component/validators/RegisterValidator.cpp \
    ./Component/validators/SubspaceMapValidator.cpp \
    ./Component/validators/ViewValidator.cpp \
    ./Catalog/Catalog.cpp \
    ./Catalog/CatalogReader.cpp \
    ./Catalog/CatalogWriter.cpp \
    ./Catalog/IpxactFile.cpp \
    ./Catalog/validators/CatalogValidator.cpp \
    ./utilities/BusDefinitionUtils.cpp \
    ./utilities/XmlUtils.cpp \
    ./DesignConfiguration/AbstractorInstance.cpp \
    ./DesignConfiguration/DesignConfiguration.cpp \
    ./DesignConfiguration/DesignConfigurationReader.cpp \
    ./DesignConfiguration/DesignConfigurationWriter.cpp \
    ./DesignConfiguration/InterconnectionConfiguration.cpp \
    ./DesignConfiguration/InterfaceRef.cpp \
    ./DesignConfiguration/MultipleAbstractorInstances.cpp \
    ./DesignConfiguration/ViewConfiguration.cpp \
    ./DesignConfiguration/validators/DesignConfigurationValidator.cpp \
    ./DesignConfiguration/validators/InterconnectionConfigurationValidator.cpp \
    ./DesignConfiguration/validators/ViewConfigurationValidator.cpp \
    ./common/validators/QualifierValidator.cpp
