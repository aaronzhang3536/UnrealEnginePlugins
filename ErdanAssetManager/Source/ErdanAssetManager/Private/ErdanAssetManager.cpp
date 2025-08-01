// Copyright Epic Games, Inc. All Rights Reserved.

#include "ErdanAssetManager.h"
#include "ErdanAssetManagerStyle.h"
#include "ErdanAssetManagerCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "ErdanDirPathStructCustomization.h"
#include "ErdanAssetManagerRuleDataCustomization.h"
#include "ErdanAssetRuleListStructCustomization.h"
#include "AssetRuleItemStructCustomization.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "ErdanAssetManagerRuleDataAssetTypeActions.h"

#include "AssetActionListener.h"

static const FName ErdanAssetManagerTabName("ErdanAssetManager");

#define LOCTEXT_NAMESPACE "FErdanAssetManagerModule"

void FErdanAssetManagerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	// Section1: 初始化资源行为监听器
	{
		AssetActionListener = new FAssetActionListener();
		AssetActionListener->RegisterAssetEventHandlers();
	}

	FErdanAssetManagerStyle::Initialize();
	FErdanAssetManagerStyle::ReloadTextures();

	FErdanAssetManagerCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FErdanAssetManagerCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FErdanAssetManagerModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FErdanAssetManagerModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(ErdanAssetManagerTabName, FOnSpawnTab::CreateRaw(this, &FErdanAssetManagerModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FErdanAssetManagerTabTitle", "ErdanAssetManager"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);



	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout("DirPath", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FErdanDirPathStructCustomization::MakeInstance));
	PropertyModule.RegisterCustomPropertyTypeLayout("AssetRuleList", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FErdanAssetRuleListStructCustomization::MakeInstance));
	PropertyModule.RegisterCustomPropertyTypeLayout("AssetRuleItem", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FAssetRuleItemStructCustomization::MakeInstance));
	
	PropertyModule.RegisterCustomClassLayout("ErdanAssetManagerRuleData", FOnGetDetailCustomizationInstance::CreateStatic(&FErdanAssetManagerRuleDataCustomization::MakeInstance));

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	Action = MakeShared<FAssetTypeActions_ErdanAssetManagerRuleData>();
	AssetTools.RegisterAssetTypeActions(Action.ToSharedRef());
}

void FErdanAssetManagerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FErdanAssetManagerStyle::Shutdown();

	FErdanAssetManagerCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(ErdanAssetManagerTabName);


	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	AssetTools.UnregisterAssetTypeActions(Action.ToSharedRef());


	AssetActionListener->UnregisterAssetEventHandlers();
	delete AssetActionListener;
}

TSharedRef<SDockTab> FErdanAssetManagerModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(STextBlock)
			//SNew(SErdanAssetManagerView)
		];
}

void FErdanAssetManagerModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(ErdanAssetManagerTabName);
}

void FErdanAssetManagerModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FErdanAssetManagerCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FErdanAssetManagerCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}

	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomClassLayout("UErdanAssetManagerRuleData");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FErdanAssetManagerModule, ErdanAssetManager)