#ifndef MOD_CONFIG_H
#define MOD_CONFIG_H
#include <modconfig/ModMonitorConfig.h>
#include <modconfig/ModTaskConfig.h>
#include <modconfig/ModServiceManagerConfig.h>
#include <modconfig/ModVerifyConfig.h>
#include <modconfig/ModAPIServiceConfig.h>
#include <modconfig/ModMWAdaptorConfig.h>

#ifdef MOD_OPROS_TASK
#define MOD_OPROS_SERVICEMANAGER_USE_TASK
#define MOD_OPROS_SYMBOLMONITOR
#endif

#ifdef MOD_OPROS_API_SERVICE
#define MOD_OPROS_SYMBOLMONITOR
#endif

#endif //MOD_CONFIG_H
