#ifndef TASK_MODEL_BINDER_H
#define TASK_MODEL_BINDER_H

#include <modconfig/ModConfig.h>

#ifdef MOD_OPROS_TASK

#include "system/service/Service.h"

class TaskModelBinderProvider;

class TaskModelBinder : public opros::engine::Service
{
	TaskModelBinderProvider *mp;
public:
	TaskModelBinder();
	virtual ~TaskModelBinder();

	virtual int init();
	virtual int start();
	virtual int stop();
	virtual int destory();
};
#else // MOD_OPROS_TASK
class TaskModelBinder : public opros::engine::Service
{
};
#endif
#endif //TASK_MODEL_BINDER_H
