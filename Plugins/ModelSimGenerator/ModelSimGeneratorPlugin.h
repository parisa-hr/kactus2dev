//-----------------------------------------------------------------------------
// File: ModelSimGeneratorPlugin.h
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Janne Virtanen
// Date: 10.06.2016
//
// Description:
// ModelSim generator plugin.
//-----------------------------------------------------------------------------

#ifndef MODELSIMGENERATORPLUGIN_H
#define MODELSIMGENERATORPLUGIN_H

#include "modelsimgenerator_global.h"

#include <KactusAPI/include/IGeneratorPlugin.h>
#include <Plugins/PluginSystem/GeneratorPlugin/GenerationControl.h>
#include <KactusAPI/include/CLIGenerator.h>

class MODELSIMGENERATOR_EXPORT ModelSimGeneratorPlugin : public QObject, public IGeneratorPlugin,
    public CLIGenerator
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "kactus2.plugins.modelSimGenerator" FILE "modelSimGenerator.json")

    Q_INTERFACES(IPlugin)
    Q_INTERFACES(IGeneratorPlugin)

public:
    //! The default constructor.
    ModelSimGeneratorPlugin();

    //! The destructor.
    ~ModelSimGeneratorPlugin() = default;

    // Disable copying.
    ModelSimGeneratorPlugin(ModelSimGeneratorPlugin const& rhs) = delete;
    ModelSimGeneratorPlugin& operator=(ModelSimGeneratorPlugin const& rhs) = delete;

    /*!
     *  Returns the name of the plugin.
     */
    virtual QString getName() const;

    /*!
     *  Returns the version of the plugin.
     */
    virtual QString getVersion() const;

    /*!
     *  Returns the description of the plugin.
     */
    virtual QString getDescription() const;

    /*!
     *  Returns the vendor of the plugin.
     */
    virtual QString getVendor() const;

    /*!
     *  Returns the license of the plugin.
     */
    virtual QString getLicense() const;

    /*!
     *  Returns the holder of the license of the plugin.
     */
    virtual QString getLicenseHolder() const;

    /*!
     *  Returns the settings widget.
     */
    virtual QWidget* getSettingsWidget();

    /*!
     *  This is used to access the settings modified by function getSettingsWidget().
     */
    virtual PluginSettingsModel* getSettingsModel(){return NULL;}

    /*!
     *  Returns the icon for the generator.
     */
    virtual QIcon getIcon() const;
	
    /*!
     *  Checks whether the generator may run for the given component or design. 
     *
     *      @param [in] component	        The component for which to check support. If design is not null, component
	 *                                      will refer to design or designConfiguration.
     *      @param [in] design	            The design, if the generator is ran for a design.
     *      @param [in] designConfiguration The design configuration for design, if it is not null.
     *
     *      @return True, if the generator may run the given component. Otherwise false.
     */
    virtual bool checkGeneratorSupport(QSharedPointer<Component const> component,
        QSharedPointer<Design const> design,
        QSharedPointer<DesignConfiguration const> designConfiguration) const;
	 
    /*!
     *  Runs the generation, creating new files and/or modifying the IP-XACT metadata. The function has
	 *  also access to the parent window widget, so that it can show dialogs for the user to aid the generation.
     *
     *      @param [in] utility			    The plugin utility interface.
     *      @param [in] component	        The component for which to check support. If design is not null, component
     *                                      will refer to design or designConfiguration.
     *      @param [in] design	            The design, if the generator is ran for a design.
     *      @param [in] designConfiguration The design configuration for design, if it is not null.
     */
    virtual void runGenerator(IPluginUtility* utility, 
        QSharedPointer<Component> component,
        QSharedPointer<Design> design,
        QSharedPointer<DesignConfiguration> designConfiguration);

    /*!
     * Runs the generation.
     *
     *     @param [in] utility              Utilities for enabling plugin execution.
     *     @param [in] component            The component to run the generation for.
     *     @param [in] design               The design to run the generation for.
     *     @param [in] designConfiguration  The design configuration to run the generation for.
     *     @param [in] viewName             The component view name to run the generation for.
     *     @param [in] outputDirectory      The output directory for the generation results.
     */
    virtual void runGenerator(IPluginUtility* utility, QSharedPointer<Component> component, 
        QSharedPointer<Design> design, QSharedPointer<DesignConfiguration> designConfiguration,
        QString const& viewName, QString const& outputDirectory) override;

    /*!
     *  Gets the output format generated by the plugin.
     *
     *      @return The output file format generated by the plugin.
     */
    virtual QString getOutputFormat() const override;

    //! Returns the external program requirements of the plugin.
    virtual QList<IPlugin::ExternalProgramRequirement> getProgramRequirements();

private:

     //-----------------------------------------------------------------------------
     // Data.
     //-----------------------------------------------------------------------------

     //! The last values used by the generation.
     GenerationSettings settings_;
};

#endif // MODELSIMGENERATORPLUGIN_H
