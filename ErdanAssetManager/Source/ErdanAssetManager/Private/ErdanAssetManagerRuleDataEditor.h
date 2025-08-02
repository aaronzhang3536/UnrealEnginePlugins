// ErdanAssetManagerRuleDataEditor.h

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "ErdanAssetManagerRuleData.h"

class UErdanAssetManagerRuleData;

class FErdanAssetManagerRuleDataEditor : public FAssetEditorToolkit, public FEditorUndoClient, public FGCObject, public FNotifyHook
{
public:
	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject* InObject);

	// FGCObject
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override { return TEXT("ErdanAssetManagerRuleDataEditor"); }

	// FAssetEditorToolkit
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

protected:
	void ExtendToolbar();
	void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;

	TSharedRef<SDockTab> SpawnDetailsTab(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnSubObjectDetailsTab(const FSpawnTabArgs& Args);

private:
	TWeakObjectPtr<UErdanAssetManagerRuleData> EditingObject;
	TSharedPtr<class IDetailsView> DetailsView;
	TSharedPtr<class IDetailsView> SubObjectDetailsView;



	static const FName DetailsTabId;
	static const FName SubObjectDetailsTabId;
};
