// ErdanAssetManagerRuleDataAssetTypeActions

#include "ErdanAssetManagerRuleDataEditor.h"
#include "ErdanAssetManagerRuleData.h"
#include "AssetTypeActions_Base.h"

class FAssetTypeActions_ErdanAssetManagerRuleData : public FAssetTypeActions_Base
{
public:
	virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "ErdanAssetManagerRuleData", "Erdan Asset Manager Rule Data"); }
	virtual FColor GetTypeColor() const override { return FColor::Emerald; }
	virtual UClass* GetSupportedClass() const override { return UErdanAssetManagerRuleData::StaticClass(); }
	virtual uint32 GetCategories() override { return EAssetTypeCategories::Misc; }

	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor) override
	{
		for (UObject* Obj : InObjects)
		{
			if (UErdanAssetManagerRuleData* RuleData = Cast<UErdanAssetManagerRuleData>(Obj))
			{
				TSharedRef<FErdanAssetManagerRuleDataEditor> Editor = MakeShareable(new FErdanAssetManagerRuleDataEditor());
				Editor->InitEditor(EToolkitMode::Standalone, EditWithinLevelEditor, RuleData);
			}
		}
	}
};
