/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:
*
* Copyright 2023 Synamedia
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef DOBBYCONTAINER_H
#define DOBBYCONTAINER_H

#include <cstdint>
#include <memory>
#include <chrono>
#include <bitset>
#include <mutex>
#include <list>

class DobbyBundle;
class DobbyConfig;
class DobbyRootfs;
class DobbyRdkPluginManager;

class DobbyContainerImpl {
public:

    virtual ~DobbyContainerImpl() = default;

    virtual void setRestartOnCrash(const std::list<int>& files) = 0;
    virtual void clearRestartOnCrash() = 0;
};

class DobbyContainer {

protected:

    static DobbyContainerImpl* impl;

public:

    DobbyContainer(DobbyContainer&) = delete;
    DobbyContainer(DobbyContainer&&) = delete;
    friend class DobbyManager;

    DobbyContainer(): descriptor(0){}

    DobbyContainer(const std::shared_ptr<const DobbyBundle>& _bundle,const std::shared_ptr<const DobbyConfig>& _config,const std::shared_ptr<const DobbyRootfs>& _rootfs):descriptor(0){}

    DobbyContainer(const std::shared_ptr<const DobbyBundle>& _bundle,const std::shared_ptr<const DobbyConfig>& _config,const std::shared_ptr<const DobbyRootfs>& _rootfs,const std::shared_ptr<const DobbyRdkPluginManager>& _rdkPluginManager):descriptor(0){}

    ~DobbyContainer(){}

    enum class State { Starting, Running, Stopping, Paused, Unknown } state;
    pid_t containerPid;
    const int32_t descriptor;
    const std::shared_ptr<const DobbyRdkPluginManager> rdkPluginManager;
    const std::shared_ptr<const DobbyBundle> bundle;
    const std::shared_ptr<const DobbyConfig> config;
    std::list<int> mFiles;
    bool hasCurseOfDeath;
    const std::shared_ptr<const DobbyRootfs> rootfs;
    const std::list<int>& files() const;

    bool shouldRestart(int statusCode);

    std::string customConfigFilePath;

    static void setImpl(DobbyContainerImpl* newImpl)
    {
         impl = newImpl;
    }

    static DobbyContainer* getInstance()
    {
        static DobbyContainer* instance = nullptr;
        if (nullptr != instance)
        {
            instance = new DobbyContainer();
        }
        return instance;
    }

    static void setRestartOnCrash(const std::list<int>& files)
    {
        return impl->setRestartOnCrash(files);
    }

    static void clearRestartOnCrash()
    {
        return impl->clearRestartOnCrash();
    }
};

#endif // DOBBYCONTAINER_H

