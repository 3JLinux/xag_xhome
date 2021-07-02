#pragma once
#ifndef __MAINBOARD_MODULES_CONTROLLER_H__
#define __MAINBOARD_MODULES_CONTROLLER_H__
#include <atomic>
#include <set>
#include <map>
#include <mutex>
#include <functional>
#include <ostream>
#include <fstream>
#include <memory>

#include "spdlog/spdlog.h"
#include "util/factory.h"
#include "common/macros.h"
#include "cyber/base/thread_pool.h"
#include "cyber/time/rate.h"
#include "cyber/cyber.h"
#include "cyber/node/node.h"
namespace xag_nav
{
namespace os
{
const int TASK_NUM = 8;
// 用于模块的基类
using xag_nav::os::Node;
class ComponentBase
{
 public:
	using NodePtr = std::unique_ptr<Node>;
	ComponentBase(const std::string &_name = "no name", double _frequency = 0) : frequency_(_frequency),
																																								component_loop_(_frequency),
																																								module_name_(_name)
	{
		node_ = CreateNode(_name);
	}
	virtual ~ComponentBase() {}
	virtual bool initialize() = 0;
	virtual bool process() = 0;
	virtual void shutdown()
	{
			if (is_shutdown_.exchange(true))
			{
					SPDLOG_INFO("module {} has been shutdown!", module_name_);
					return;
			}
			SPDLOG_INFO("Shut down module {}", module_name_);
			return;
    }

    std::string moduleName()
    {
        return module_name_;
    }

    double frequency()
    {
        return frequency_;
    }

    virtual bool ok()
    {
        return (!is_shutdown_.load());
    }

    virtual void resetLoop()
    {
        component_loop_.Reset();
    }

    virtual void sleep()
    {
        component_loop_.Sleep();
    }

protected:
    apollo::cyber::Rate component_loop_;
    std::string module_name_;
    std::atomic<bool> is_shutdown_ = {false};
    double frequency_;
    NodePtr node_;
};

class ModulesController
{
public:
    // using ComponentCreator = ComponentBase *(*)();
    using ComponentCreator = std::function<ComponentBase *()>;
    using ComponentBasePtr = std::shared_ptr<ComponentBase>;
    virtual ~ModulesController()
    {
        Clear();
    }
    static void signalProcess(int _signal);

    bool loadAllModulesAndInit(void);

    bool modulesProcess(void);

    template <typename ComponentDerive>
    bool RegisterModule(const std::string &id, const double &frequency)
    {
        std::lock_guard<std::mutex> _lock(factory_mutex_);
        if (!component_factory_.Register(id, [id, frequency]() { return new ComponentDerive(id, frequency); }))
        {
            SPDLOG_WARN("component {} failed to register!", id.c_str());
            return false;
        }
        components_name_.insert(id);
        return true;
    }

    bool Contains(const std::string &id)
    {
        std::lock_guard<std::mutex> _lock(factory_mutex_);
        return component_factory_.Contains(id);
    }

    bool UnregisterModule(const std::string &id)
    {
        std::lock_guard<std::mutex> _lock(factory_mutex_);
        if (!component_factory_.Unregister(id))
        {
            SPDLOG_WARN("component {} failed to unregister!", id.c_str());
            return false;
        }
        components_name_.erase(id);
        return true;
    }

    void Clear()
    {
        std::lock_guard<std::mutex> _lock(factory_mutex_);
        component_factory_.Clear();
        components_name_.clear();
    }

    bool Empty() const
    {
        std::lock_guard<std::mutex> _lock(factory_mutex_);
        return component_factory_.Empty();
    }

    template <typename F, typename... Args>
    auto RegisterTask(F &&f, Args &&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>
    {
        return task_pool_->Enqueue(std::forward<F>(f), std::forward<Args>(args)...);
    }

    bool isModulesInitialized(void)
    {
        return is_initialized_.load();
    }
    DECLARE_SINGLETON(ModulesController)
  private:
    void write_update_flag();
    mutable std::mutex factory_mutex_;
    util::Factory<std::string, ComponentBase, ComponentCreator> component_factory_;
    std::set<std::string> components_name_;
    static std::map<std::string, ComponentBasePtr> working_components_map_;
    std::atomic<bool> is_initialized_;
    // 模块控制器操作的线程池
    std::shared_ptr<apollo::cyber::base::ThreadPool> task_pool_;
};

} // namespace os
} // namespace xag_nav

#endif //__MAINBOARD_MODULES_CONTROLLER_H__