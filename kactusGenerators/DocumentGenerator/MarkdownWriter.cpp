#include "MarkdownWriter.h"

#include <IPXACTmodels/Component/Component.h>
#include <IPXACTmodels/Component/MemoryMap.h>
#include <IPXACTmodels/Component/AddressBlock.h>
#include <IPXACTmodels/Component/Register.h>
#include <IPXACTmodels/Component/Field.h>
#include <IPXACTmodels/Component/BusInterface.h>
#include <IPXACTmodels/Component/FileSet.h>

#include <KactusAPI/include/ExpressionFormatter.h>
#include <KactusAPI/include/LibraryInterface.h>

#include <QFileInfo>
#include <QDateTime>
#include <QSettings>
#include <QString>

MarkdownWriter::MarkdownWriter(QSharedPointer<Component> component, ExpressionFormatter* formatter,
    LibraryInterface* libraryHandler) :
    component_(component),
    componentNumber_(0),
    expressionFormatter_(formatter),
    libraryHandler_(libraryHandler),
    DocumentationWriter(formatter)
{
    vlnvString_ = component_->getVlnv().toString();
}

MarkdownWriter::~MarkdownWriter()
{
}

void MarkdownWriter::writeHeader(QTextStream& stream)
{
    QSettings settings;
    
    // Write markdown header
    stream << "###### This document was generated by Kactus2 on " 
        << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss")
        << " by user " << settings.value("General/Username").toString()
        << "  " << Qt::endl << Qt::endl;
}

void MarkdownWriter::writeKactusAttributes(QTextStream& stream, int subHeaderNumber)
{
    writeSubHeader(stream, subHeaderNumber, "Kactus2 attributes", "attributes");

    stream << "**Product hierarchy:** " <<
        KactusAttribute::hierarchyToString(component_->getHierarchy()) << "  " << Qt::endl
        << "**Component implementation:** " <<
        KactusAttribute::implementationToString(component_->getImplementation()) << "  " << Qt::endl
        << "**Component firmness:** " <<
        KactusAttribute::firmnessToString(component_->getFirmness()) << "  " << Qt::endl << Qt::endl;
}

void MarkdownWriter::writeTableOfContents(QTextStream& stream)
{
    // Write component header
    stream << componentNumber_ << ". [" << "Component " << component_->getVlnv().toString(" - ") << "]" <<
        "(#" << vlnvString_ << ")  " << Qt::endl;

    // subHeader is running number that counts the number of sub headers for component
    int subHeader = 1;

    // Write component subheaders. Component has at least kactus attributes.
    stream << "\t" << componentNumber_ << "." << subHeader << ". " << "[Kactus2 attributes" <<
        "](#" << vlnvString_ << ".attributes)  " << Qt::endl;

    ++subHeader;

    if (component_->hasParameters())
    {
        stream << "\t" << componentNumber_ << "." << subHeader << ". " << "[General parameters]" <<
            "(#" << vlnvString_ << ".parameters)  " << Qt::endl;
        ++subHeader;
    }

    if (!component_->getMemoryMaps()->isEmpty())
    {
        stream << "\t" << componentNumber_ << "." << subHeader << ". " << "[Memory maps]" <<
            "(#" << vlnvString_ << ".memoryMaps)  " << Qt::endl;
        ++subHeader;
    }

    if (component_->hasPorts())
    {
        stream << "\t" << componentNumber_ << "." << subHeader << ". " << "[Ports]" <<
            "(#" << vlnvString_ << ".ports)  " << Qt::endl;
        ++subHeader;
    }
    
    if (component_->hasInterfaces())
    {
        stream << "\t" << componentNumber_ << "." << subHeader << ". " << "[Bus interfaces]" <<
            "(#" << vlnvString_ << ".interfaces)  " << Qt::endl;
        ++subHeader;
    }
    
    if (component_->hasFileSets())
    {
        stream << "\t" << componentNumber_ << "." << subHeader << ". " << "[File sets]" <<
            "(#" << vlnvString_ << ".fileSets)  " << Qt::endl;
        ++subHeader;
    }

    if (component_->hasViews())
    {
        stream << "\t" << componentNumber_ << "." << subHeader << ". " << "[Views]" <<
            "(#" << vlnvString_ << ".views)  " << Qt::endl;
        ++subHeader;
    }
}

void MarkdownWriter::writeParameters(QTextStream& stream, int subHeaderNumber)
{
    writeSubHeader(stream, subHeaderNumber, "General parameters", "parameters");
    
    writeTableHeader(stream, DocumentationWriter::PARAMETER_HEADERS);

    for (auto const& parameter : *component_->getParameters())
    {
        QStringList paramCells(QStringList()
            << parameter->name()
            << parameter->getType()
            << expressionFormatter_->formatReferringExpression(parameter->getValue())
            << parameter->getValueResolve()
            << expressionFormatter_->formatReferringExpression(parameter->getVectorLeft())
            << expressionFormatter_->formatReferringExpression(parameter->getVectorRight())
            << expressionFormatter_->formatReferringExpression(parameter->getArrayLeft())
            << expressionFormatter_->formatReferringExpression(parameter->getArrayRight())
            << parameter->description()
        );

        writeTableRow(stream, paramCells);
    }
}

void MarkdownWriter::writeMemoryMaps(QTextStream& stream, int subHeaderNumber)
{
    if (component_->getMemoryMaps()->isEmpty())
    {
        return;
    }
    
    writeSubHeader(stream, subHeaderNumber, "Memory maps", "memoryMaps");

    const QList<QSharedPointer<MemoryMap> > componentMemoryMaps = *component_->getMemoryMaps();
    int memoryMapNumber = 1;

    for (auto const& memoryMap : componentMemoryMaps)
    {
        QList subHeaderNumbers({
            componentNumber_,
            subHeaderNumber,
            memoryMapNumber
        });

        writeSubHeader(stream, subHeaderNumbers, memoryMap->name(), 3);

        // Memory map description and address unit bits
        if (!memoryMap->description().isEmpty())
        {
            writeDescription(stream, memoryMap->description());
        }
        
        stream << "**Address unit bits (AUB):** " << memoryMap->getAddressUnitBits() << "  " << Qt::endl << Qt::endl;

        QList<QSharedPointer <AddressBlock> > addressBlocks = getMemoryMapAddressBlocks(memoryMap);
        writeAddressBlocks(stream, addressBlocks, subHeaderNumber, memoryMapNumber);

        ++memoryMapNumber;
    }
}

void MarkdownWriter::writeAddressBlocks(QTextStream& stream, QList<QSharedPointer<AddressBlock>> addressBlocks,
    int subHeaderNumber, int memoryMapNumber)
{
    if (addressBlocks.isEmpty())
    {
        return;
    }

    int addressBlockNumber = 1;

    for (auto const& addressBlock : addressBlocks)
    {
        QList subHeaderNumbers({
            componentNumber_,
            subHeaderNumber,
            memoryMapNumber,
            addressBlockNumber
        });
        
        QStringList addressBlockTableCells(QStringList()
            << General::usage2Str(addressBlock->getUsage())
            << expressionFormatter_->formatReferringExpression(addressBlock->getBaseAddress())
            << expressionFormatter_->formatReferringExpression(addressBlock->getRange())
            << expressionFormatter_->formatReferringExpression(addressBlock->getWidth())
            << AccessTypes::access2Str(addressBlock->getAccess())
            << addressBlock->getVolatile()
        );
        
        QList <QSharedPointer <Register> > registers = getAddressBlockRegisters(addressBlock);

        writeSubHeader(stream, subHeaderNumbers, addressBlock->name(), 3);
        
        if (!addressBlock->description().isEmpty())
        {
            writeDescription(stream, addressBlock->description());
        }

        writeTableHeader(stream, DocumentationWriter::ADDRESS_BLOCK_HEADERS);
        writeTableRow(stream, addressBlockTableCells);
        writeRegisters(stream, registers, subHeaderNumber, memoryMapNumber, addressBlockNumber);

        ++addressBlockNumber;
    }
}

void MarkdownWriter::writeRegisters(QTextStream& stream, QList<QSharedPointer<Register>> registers, int subHeaderNumber, int memoryMapNumber, int addressBlockNumber)
{
    if (registers.isEmpty())
    {
        return;
    }

    int registerNumber = 1;

    for (auto const& currentRegister : registers)
    {
        QList subHeaderNumbers({
            componentNumber_,
            subHeaderNumber,
            memoryMapNumber,
            addressBlockNumber,
            registerNumber
        });

        QStringList registerInfoTableCells(QStringList()
            << expressionFormatter_->formatReferringExpression(currentRegister->getAddressOffset())
            << expressionFormatter_->formatReferringExpression(currentRegister->getSize())
            << expressionFormatter_->formatReferringExpression(currentRegister->getDimension())
            << currentRegister->getVolatile()
            << AccessTypes::access2Str(currentRegister->getAccess())
        );

        writeSubHeader(stream, subHeaderNumbers, currentRegister->name(), 3);

        if (!currentRegister->description().isEmpty())
        {
            writeDescription(stream, currentRegister->description());
        }

        writeTableHeader(stream, DocumentationWriter::REGISTER_HEADERS);
        writeTableRow(stream, registerInfoTableCells);
        writeFields(stream, currentRegister);

        ++registerNumber;
    }
}

void MarkdownWriter::writeFields(QTextStream& stream, QSharedPointer<Register> currentRegister)
{
    if (currentRegister->getFields()->isEmpty())
    {
        return;
    }

    QString headerTitle = QStringLiteral("Register ")
        + currentRegister->name()
        + QStringLiteral(" contains the following fields:");

    writeSubHeader(stream, QList <int>(), headerTitle, 4);
    writeTableHeader(stream, DocumentationWriter::FIELD_HEADERS);

    for (auto const& field : *currentRegister->getFields())
    {
        QStringList fieldTableCells(QStringList()
            << field->name() + " <a id=\"" + vlnvString_ + ".field." + field->name() + "\">"
            << expressionFormatter_->formatReferringExpression(field->getBitOffset())
            << expressionFormatter_->formatReferringExpression(field->getBitWidth())
            << field->getVolatile().toString()
            << AccessTypes::access2Str(field->getAccess())
            << getFieldResetInfo(field)
            << field->description()
        );

        writeTableRow(stream, fieldTableCells);
    }
}

void MarkdownWriter::writePorts(QTextStream& stream, int subHeaderNumber)
{
    writeSubHeader(stream, subHeaderNumber, "Ports", "ports");

    const QList<QSharedPointer<Port> > ports = *component_->getPorts();

    writePortTable(stream, ports);
}

void MarkdownWriter::writeInterfaces(QTextStream& stream, int& subHeaderNumber)
{
    writeSubHeader(stream, subHeaderNumber, "Bus interfaces", "interfaces");

    int interfaceNumber = 1;

    for (auto const& interface : *component_->getBusInterfaces())
    {        
        QList subHeaderNumbers({ componentNumber_, subHeaderNumber, interfaceNumber });

        writeSubHeader(stream, subHeaderNumbers, interface->name(), 3);

        if (!interface->description().isEmpty())
        {
            writeDescription(stream, interface->description());
        }

        stream << "**Interface mode:** " << General::interfaceMode2Str(interface->getInterfaceMode())
            << "  " << Qt::endl;

        stream << "**Ports used in this interface:** ";

        if (auto const& ports = component_->getPortsMappedInInterface(interface->name()); ports.isEmpty())
        {
            stream << "None  " << Qt::endl << Qt::endl;
        }
        else
        {
            stream << Qt::endl << Qt::endl;
            writePortTable(stream, ports);
        }

        ++interfaceNumber;
    }
}

void MarkdownWriter::writeFileSets(QTextStream& stream, int& subHeaderNumber)
{
    writeSubHeader(stream, subHeaderNumber, "File sets", "fileSets");

    const QList<QSharedPointer<FileSet> > fileSets = *component_->getFileSets();

    int fileSetNumber = 1;

    for (auto const& fileSet : fileSets)
    {
        writeSubHeader(stream, QList({ componentNumber_, subHeaderNumber, fileSetNumber }), fileSet->name(), 3);

        // description
        if (!fileSet->description().isEmpty())
        {
            writeDescription(stream, fileSet->description());
        }

        // identifiers
        writeFileSetGroupdIdentifiers(stream, fileSet);

        // Default file builders table
        writeDefaultFileBuilders(stream, fileSet);

        writeFiles(stream, fileSet, subHeaderNumber, fileSetNumber);

        ++fileSetNumber;
    }
}

void MarkdownWriter::setComponentNumber(int componentNumber)
{
    componentNumber_ = componentNumber;
}

void MarkdownWriter::writeSubHeader(QTextStream& stream, QList<int> const& subHeaderNumbers,
    QString const& title, int level) const
{
    // Writes the header level
    QString headerTag = QStringLiteral("#").repeated(level) + " ";

    QStringList subHeaderNumberParts;
    for (int number : subHeaderNumbers)
    {
        subHeaderNumberParts << QString::number(number);
    }
    
    QString headerTitle = title;

    if (!subHeaderNumberParts.isEmpty())
    {
        headerTitle = subHeaderNumberParts.join(".") + " " + title;
    }

    stream << headerTag << headerTitle << "  " << Qt::endl << Qt::endl;
}

void MarkdownWriter::writeSubHeader(QTextStream& stream, int subHeaderNumber,
    QString const& headerText, QString const& headerId) const
{
    stream << "## " << componentNumber_ << "." << subHeaderNumber << " " << headerText << " <a id=\"" <<
        vlnvString_ << "." << headerId << "\">  " << Qt::endl << Qt::endl;
}

void MarkdownWriter::writeTableRow(QTextStream& stream, QStringList const& cells) const
{
    for (auto const& cell : cells)
    {
        stream << "|" << cell;
    }

    stream << "|" << Qt::endl;
}

void MarkdownWriter::writeTableSeparator(QTextStream& stream, int columns) const
{
    QString tableSeparator(":---- ");   // :--- aligns text in cells to the left
    QStringList tableSeparators = tableSeparator.repeated(columns).split(" ", Qt::SkipEmptyParts);
    writeTableRow(stream, tableSeparators);
}

void MarkdownWriter::writeTableHeader(QTextStream& stream, QStringList const& headers) const
{
    writeTableRow(stream, headers);
    writeTableSeparator(stream, headers.length());
}

void MarkdownWriter::writePortTable(QTextStream& stream, QList<QSharedPointer<Port>> ports) const
{
    writeTableHeader(stream, DocumentationWriter::PORT_HEADERS);

    for (auto const& port : ports)
    {
        QStringList portTableCells(QStringList()
            << port->name() + " <a id=\"" + vlnvString_ + ".port." + port->name() + "\">"
            << DirectionTypes::direction2Str(port->getDirection())
            << expressionFormatter_->formatReferringExpression(port->getLeftBound())
            << expressionFormatter_->formatReferringExpression(port->getRightBound())
            << port->getTypeName()
            << port->getTypeDefinition(port->getTypeName())
            << expressionFormatter_->formatReferringExpression(port->getDefaultValue())
            << expressionFormatter_->formatReferringExpression(port->getArrayLeft())
            << expressionFormatter_->formatReferringExpression(port->getArrayRight())
            << port->description()
        );

        writeTableRow(stream, portTableCells);
    }
}

void MarkdownWriter::writeDescription(QTextStream& stream, QString const& description) const
{
    stream << "**Description:** " << description << "  " << Qt::endl << Qt::endl;
}

void MarkdownWriter::writeFileSetGroupdIdentifiers(QTextStream& stream, QSharedPointer<FileSet> fileSet) const
{
    stream << "**Identifiers:** ";

    QStringList groups = *fileSet->getGroups();
    
    stream << groups.join(", ") << "  " << Qt::endl << Qt::endl;
}

void MarkdownWriter::writeDefaultFileBuilders(QTextStream& stream, QSharedPointer<FileSet> fileSet) const
{
    const auto defaultFileBuilders = fileSet->getDefaultFileBuilders();

    if (defaultFileBuilders->isEmpty())
    {
        return;
    }

    stream << "**Default file builders:**  " << Qt::endl << Qt::endl;
    
    writeTableHeader(stream, DocumentationWriter::DEFAULT_FILE_BUILDER_HEADERS);

    for (auto const& defaultBuilder : *defaultFileBuilders)
    {
        QStringList builderCells(QStringList()
            << defaultBuilder->getFileType()
            << defaultBuilder->getCommand()
            << defaultBuilder->getFlags()
            << expressionFormatter_->formatReferringExpression(defaultBuilder->getReplaceDefaultFlags())
        );
        
        writeTableRow(stream, builderCells);
    }
}

void MarkdownWriter::writeFiles(QTextStream& stream, QSharedPointer<FileSet> fileSet,
    int subHeaderNumber, int fileSetNumber)
{
    QList<QSharedPointer<File> > files = *fileSet->getFiles();

    if (files.isEmpty())
    {
        return;
    }

    int filesSubHeaderNumber = 1;

    QList filesSubHeaderNumbers({
        componentNumber_,
        subHeaderNumber,
        fileSetNumber,
        filesSubHeaderNumber
    });

    writeSubHeader(stream, filesSubHeaderNumbers, "Files", 4);

    writeTableHeader(stream, DocumentationWriter::FILE_HEADERS);

    for (auto const& file : files)
    {
        writeSingleFile(stream, file);
    }
}

void MarkdownWriter::writeSingleFile(QTextStream& stream, QSharedPointer<File> file)
{
    QString relativeFilePath = file->name();
    QString absFilePath = General::getAbsolutePath(libraryHandler_->getPath(component_->getVlnv()), relativeFilePath);

    QFileInfo fileInfo(absFilePath);

    // get relative path from html file to the file
    QFileInfo htmlInfo(getTargetPath());
    QString pathFromDocToFile = General::getRelativePath(getTargetPath(), absFilePath);

    QSharedPointer<BuildCommand> buildCommand = file->getBuildCommand();

    QStringList fileTypes = *file->getFileTypes();

    QStringList fileTableCells(QStringList()
        << "[" + fileInfo.fileName() + "]" + "(" + pathFromDocToFile + ")"
        << file->getLogicalName()
        << (buildCommand ? buildCommand->getCommand() : QStringLiteral(""))
        << (buildCommand ? buildCommand->getFlags() : QStringLiteral(""))
        << fileTypes.join(",<br>")
        << file->getDescription()
    );

    writeTableRow(stream, fileTableCells);
}
