// Copyright ©  Jack Holland.
//
// DbgLOG is free software. It comes without any warranty, to the extent permitted
// by applicable law. You can redistribute it and/or modify it under the terms
// of the Do What The Fuck You Want To Public License, Version 2, as published
// by Sam Hocevar:
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                    Version 2, December 2004
//
// Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
//
// Everyone is permitted to copy and distribute verbatim or modified
// copies of this license document, and changing it is allowed as long
// as the name is changed.
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//
//  0. You just DO WHAT THE FUCK YOU WANT TO.
#pragma once
#include "dbgLog.h" // Relative path that must be updated if you move the files location.


#if KEEP_DBG_LOG
DEFINE_LOG_CATEGORY(dbgLOG);

FAutoConsoleCommand EnableCategoryCommand
	{
		TEXT("dbgLog.EnableCategory"),
		TEXT("Usage: \"dbgLog.EnableCategory SomeCategory SomeCategoryTwo\", Enables the provided category or categories (also accepts \"All\" as a way to enable all)."),
		FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Input)
		{
			if (Input.Num() == 0)
			{
				UE_LOG(dbgLOG, Warning, TEXT("Failed to enable log category as you provided no args."));
				return;
			}

			if (Input[0] == "All")
			{
				for (auto& [_, Cat] : DBG::Log::GDbgLogSingleton.CategoryMap)
				{
					Cat.bState = true;
				}
			}
			else
			{
				for (const FString& CommandStr : Input)
				{
					FName CategoryName = FName(CommandStr);
					DBG::Log::GDbgLogSingleton.SetCategoryState<true>(CategoryName, true);
				}
			}

		}),
		ECVF_Cheat
	};


FAutoConsoleCommand DisableCategoryCommand
	{
		TEXT("dbgLog.DisableCategory"),
		TEXT("Usage: \"dbgLog.DisableCategory SomeCategory SomeCategoryTwo\", Disables the provided category or categories (also accepts \"All\" as a way to enable all)."),
		FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Input)
		{
			if (Input.Num() == 0)
			{
				UE_LOG(dbgLOG, Warning, TEXT("Failed to Disable log category as you provided no args."));
				return;
			}


			if (Input[0] == "All")
			{
				for (auto& [_, Cat] : DBG::Log::GDbgLogSingleton.CategoryMap)
				{
					Cat.bState = false;
				}
			}
			else
			{
				for (const FString& CommandStr : Input)
				{
					FName CategoryName = FName(CommandStr);
					DBG::Log::GDbgLogSingleton.SetCategoryState<true>(CategoryName, false);
				}
			}
		}),
		ECVF_Cheat
	};

FAutoConsoleCommand PrintDisabledCategories
	{
		TEXT("dbgLog.PrintCategoriesStates"),
		TEXT("Prints all registered categories with their current state."),
		FConsoleCommandDelegate::CreateLambda([]()
		{
			TStringBuilder<256> CategoriesBuilder;
			int32 DisabledCategoriesNum = 0;
			int32 EnabledCategoriesNum = 0;
			for (auto& [CategoryName, Cat] : DBG::Log::GDbgLogSingleton.CategoryMap)
			{
				if (Cat.bState == false)
				{
					++DisabledCategoriesNum;
					CategoriesBuilder.Appendf(TEXT("- [Disabled] %s\n"), *CategoryName.ToString());
				}
				else
				{
					++EnabledCategoriesNum;
					CategoriesBuilder.Appendf(TEXT("- [Enabled]  %s\n"), *CategoryName.ToString());
				}
			}

			if (CategoriesBuilder.Len() > 0)
			{
				UE_LOG(dbgLOG, Display, TEXT("Categories: Enabled = %d, Disabled = %d \n%s"), EnabledCategoriesNum, DisabledCategoriesNum, CategoriesBuilder.ToString());
			}
			else
			{
				UE_LOG(dbgLOG, Display, TEXT("No categories are registered."));
			}
		}),
		ECVF_Cheat
	};



namespace DBG::Log
{
	FDbgLogSingleton GDbgLogSingleton;

	FDbgLogSingleton::FDbgLogSingleton()
	{
		// Register our default one immediately.
		GDbgLogSingleton.CategoryMap.Emplace(dbgLOG.GetCategoryName(), FRegisteredCategory{true});
	}
}


#endif

IMPLEMENT_MODULE(FDefaultModuleImpl, dbgLog)
