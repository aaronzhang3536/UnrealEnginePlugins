// ErdanAssetRuleDataEditor.cpp

#include "ErdanAssetManagerRuleDataEditor.h"
#include "ErdanAssetManagerRuleData.h"
#include "PropertyEditorModule.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Docking/SDockTab.h"

const FName FErdanAssetManagerRuleDataEditor::DetailsTabId(TEXT("ErdanAssetManagerRuleDataEditor_Properties"));
const FName FErdanAssetManagerRuleDataEditor::SubObjectDetailsTabId(TEXT("ErdanAssetRuleDataEditor_SubObjectDetails"));

#define LOCTEXT_NAMESPACE "FErdanAssetManagerRuleDataEditor"

void FErdanAssetManagerRuleDataEditor::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* InObject)
{
	EditingObject = Cast<UErdanAssetManagerRuleData>(InObject);

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsArgs;
	DetailsArgs.bUpdatesFromSelection = false;
	DetailsArgs.bLockable = false;
	DetailsArgs.bAllowSearch = true;
	DetailsArgs.NotifyHook = this;
	DetailsArgs.bShowPropertyMatrixButton = false;
	

	DetailsView = PropertyEditorModule.CreateDetailView(DetailsArgs);
	SubObjectDetailsView = PropertyEditorModule.CreateDetailView(DetailsArgs);
	
	DetailsView->SetObject(EditingObject.Get());
	SubObjectDetailsView->SetObject(EditingObject.Get());

	const TSharedRef<FTabManager::FLayout> StandaloneLayout = FTabManager::NewLayout("ErdanAssetRuleDataEditorLayout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.7f)
				->AddTab(DetailsTabId, ETabState::OpenedTab)->SetHideTabWell(true)
			)
			->Split
			(

				FTabManager::NewStack()
				->SetSizeCoefficient(0.3f)
				->AddTab(SubObjectDetailsTabId, ETabState::OpenedTab)->SetHideTabWell(true)
			)
		);

	InitAssetEditor(Mode, InitToolkitHost, "ErdanAssetManagerRuleDataEditor", StandaloneLayout, true, true, InObject);

	RegenerateMenusAndToolbars();
}

void FErdanAssetManagerRuleDataEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(EditingObject);
}

FName FErdanAssetManagerRuleDataEditor::GetToolkitFName() const
{
	return FName("ErdanAssetRuleDataEditor");
}

FText FErdanAssetManagerRuleDataEditor::GetBaseToolkitName() const
{
	return NSLOCTEXT("ErdanAssetRuleDataEditor", "AppLabel", "Erdan Asset Rule Data Editor");
}

FString FErdanAssetManagerRuleDataEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("RuleData");
}

FLinearColor FErdanAssetManagerRuleDataEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.5f, 0.2f, 0.6f);
}

void FErdanAssetManagerRuleDataEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_AssetManagerRuleDataEditor", "Asset Manager Rule Data Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(DetailsTabId, FOnSpawnTab::CreateSP(this, &FErdanAssetManagerRuleDataEditor::SpawnDetailsTab))
		.SetDisplayName(LOCTEXT("RuleDataDetails", "Details"))
		.SetGroup(WorkspaceMenuCategoryRef);

	InTabManager->RegisterTabSpawner(SubObjectDetailsTabId, FOnSpawnTab::CreateSP(this, &FErdanAssetManagerRuleDataEditor::SpawnSubObjectDetailsTab))
		.SetDisplayName(LOCTEXT("RuleObjectDetails", "Sub-Object Details"))
		.SetGroup(WorkspaceMenuCategoryRef);
}

void FErdanAssetManagerRuleDataEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	InTabManager->UnregisterTabSpawner(DetailsTabId);
	InTabManager->UnregisterTabSpawner(SubObjectDetailsTabId);
}

TSharedRef<SDockTab> FErdanAssetManagerRuleDataEditor::SpawnDetailsTab(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == DetailsTabId);

	return SNew(SDockTab)
		[
			DetailsView.ToSharedRef()
		];
}

TSharedRef<SDockTab> FErdanAssetManagerRuleDataEditor::SpawnSubObjectDetailsTab(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == SubObjectDetailsTabId);

	return SNew(SDockTab)
		.Label(LOCTEXT("StaticMeshProperties_TabTitle", "Details"))
		[
			SubObjectDetailsView.ToSharedRef()
		];
}

#undef LOCTEXT_NAMESPACE