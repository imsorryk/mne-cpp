//=============================================================================================================
/**
 * @file     pluginmanager.cpp
 * @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
 *           Lorenz Esch <lesch@mgh.harvard.edu>
 * @since    0.1.0
 * @date     February, 2013
 *
 * @section  LICENSE
 *
 * Copyright (C) 2013, Christoph Dinh, Lorenz Esch. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 * the following conditions are met:
 *     * Redistributions of source code must retain the above copyright notice, this list of conditions and the
 *       following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
 *       the following disclaimer in the documentation and/or other materials provided with the distribution.
 *     * Neither the name of MNE-CPP authors nor the names of its contributors may be used
 *       to endorse or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @brief    Definition of the PluginManager class.
 *
 */

//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "pluginmanager.h"

#include "../Interfaces/IPlugin.h"
#include "../Interfaces/ISensor.h"
#include "../Interfaces/IAlgorithm.h"
#include "../Interfaces/IIO.h"

//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QDir>
#include <QDebug>

//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace SCSHAREDLIB;

//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

PluginManager::PluginManager(QObject *parent)
: QPluginLoader(parent)
{
}

//=============================================================================================================

PluginManager::~PluginManager()
{
}

//=============================================================================================================

void PluginManager::loadPlugins(const QString& dir)
{
#ifdef STATICBUILD
    Q_UNUSED(dir);

    const auto staticInstances = QPluginLoader::staticInstances();
    for(QObject *plugin : staticInstances) {
        // IPlugin
        if(plugin) {
            if(IPlugin* pIPlugin = qobject_cast<IPlugin*>(plugin)) {
                // plugins are always disabled when they are first loaded
                m_qVecPlugins.push_back(pIPlugin);

                IPlugin::PluginType pluginType = pIPlugin->getType();
                QString msg = "Plugin " + pIPlugin->getName() + " loaded.";

                // ISensor
                if(pluginType == IPlugin::_ISensor) {
                    if(ISensor* pSensor = qobject_cast<ISensor*>(pIPlugin)) {
                        m_qVecSensorPlugins.push_back(pSensor);
                        qDebug() << "Sensor" << pSensor->getName() << "loaded.";
                    }
                }
                // IAlgorithm
                else if(pluginType == IPlugin::_IAlgorithm) {
                    if(IAlgorithm* pAlgorithm = qobject_cast<IAlgorithm*>(pIPlugin)) {
                        m_qVecAlgorithmPlugins.push_back(pAlgorithm);
                        qDebug() << "RTAlgorithm" << pAlgorithm->getName() << "loaded.";
                    }
                }

                emit pluginLoaded(msg);
            }
        } else {
            qDebug() << "Plugin could not be instantiated!";
        }
    }
#else
    QDir PluginsDir(dir);

    foreach(QString file, PluginsDir.entryList(QDir::Files)) {
        // Exclude .exp and .lib files (only relevant for windows builds)
        if(!file.contains(".exp") && !file.contains(".lib")) {
            this->setFileName(PluginsDir.absoluteFilePath(file));
            QObject *pPlugin = this->instance();

            // IPlugin
            if(pPlugin)  {
                // plugins are always disabled when they are first loaded
                m_qVecPlugins.push_back(qobject_cast<IPlugin*>(pPlugin));

                IPlugin::PluginType pluginType = qobject_cast<IPlugin*>(pPlugin)->getType();
                QString msg = "Plugin " + qobject_cast<IPlugin*>(pPlugin)->getName() + " loaded.";

                if(pluginType == IPlugin::_ISensor) {
                    // ISensor
                    ISensor* pSensor = qobject_cast<ISensor*>(pPlugin);

                    if(pSensor) {
                        m_qVecSensorPlugins.push_back(pSensor);
                        qDebug() << "[PluginManager::loadExtension] Loading sensor plugin" << pSensor->getName() << "succeeded.";
                    } else {
                        qDebug() << "[PluginManager::loadExtension] Loading sensor plugin" << pSensor->getName() << "failed.";
                    }
                } else if(pluginType == IPlugin::_IAlgorithm) {
                    // IAlgorithm
                    IAlgorithm* pAlgorithm = qobject_cast<IAlgorithm*>(pPlugin);

                    if(pAlgorithm) {
                        m_qVecAlgorithmPlugins.push_back(pAlgorithm);
                        qDebug() << "[PluginManager::loadExtension] Loading algorithm plugin" << pAlgorithm->getName() << "succeeded.";
                    } else {
                        qDebug() << "[PluginManager::loadExtension] Loading algorithm plugin" << pAlgorithm->getName() << "failed.";
                    }
                }

                emit pluginLoaded(msg);
            }
        }
    }
#endif
}

//=============================================================================================================

int PluginManager::findByName(const QString& name)
{
    QVector<IPlugin*>::const_iterator it = m_qVecPlugins.begin();
    for(int i = 0; it != m_qVecPlugins.end(); ++i, ++it)
        if((*it)->getName() == name)
            return i;

    return -1;
}
