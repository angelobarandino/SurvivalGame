// Copyright Epic Games, Inc. All Rights Reserved.

#include "SurvivalGameCoreRuntimeModule.h"

#define LOCTEXT_NAMESPACE "FSurvivalGameCoreRuntimeModule"
void FSurvivalGameCoreRuntimeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory;
	// the exact timing is specified in the .uplugin file per-module
}

void FSurvivalGameCoreRuntimeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.
	// For modules that support dynamic reloading, we call this function before unloading the module.
}
#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSurvivalGameCoreRuntimeModule, SurvivalGameCoreRuntime)

DEFINE_LOG_CATEGORY(LogSurvivalGameCore);
