// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetRuleItemStructCustomization.h"

#include "DetailWidgetRow.h"
#include "IPropertyUtilities.h"


class SWidget;

#define LOCTEXT_NAMESPACE "AssetRuleItemStructCustomization"

TSharedRef<IPropertyTypeCustomization> FAssetRuleItemStructCustomization::MakeInstance()
{
	return MakeShareable(new FAssetRuleItemStructCustomization());
}

void FAssetRuleItemStructCustomization::CustomizeRuleObjectProperty(TSharedRef<IPropertyHandle> StructPropertyHandle)
{

}
void FAssetRuleItemStructCustomization::CustomizeRegularExpressionProperty(TSharedRef<IPropertyHandle> StructPropertyHandle)
{

}
void FAssetRuleItemStructCustomization::CustomizeManagedPropertyPathProperty(TSharedRef<IPropertyHandle> StructPropertyHandle)
{

}

void FAssetRuleItemStructCustomization::CustomizeHeader( TSharedRef<IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils )
{
	TSharedPtr<IPropertyHandle> RegexProperty = StructPropertyHandle->GetChildHandle("RegularExpression");
	TSharedPtr<IPropertyHandle> RuleObjProperty = StructPropertyHandle->GetChildHandle("RuleObject");
	if (RegexProperty.IsValid() || RuleObjProperty.IsValid())
	{
		TSharedPtr<SWidget> RegexEditorEntryWidget = nullptr;

		RegexEditorEntryWidget = SAssignNew(Button_OpenRegexEditor, SButton)
			.ButtonStyle(FAppStyle::Get(), "HoverHintOnly")
			.ToolTipText(LOCTEXT("FolderComboToolTipText", "Choose a content directory"))
			//.OnClicked(this, &FAssetRuleItemStructCustomization::OnPickContent, PathProperty.ToSharedRef())
			.ContentPadding(2.0f)
			.ForegroundColor(FSlateColor::UseForeground())
			.IsFocusable(false)
			//.IsEnabled(this, &FAssetRuleItemStructCustomization::IsBrowseEnabled, StructPropertyHandle)
			[
				SNew(SImage)
					.Image(FAppStyle::GetBrush("PropertyWindow.Button_Ellipsis"))
					.ColorAndOpacity(FSlateColor::UseForeground())
			];
		HeaderRow.ValueContent()
			.MinDesiredWidth(125.0f)
			.MaxDesiredWidth(600.0f)
			[
				SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					.VAlign(VAlign_Center)
					[
						RuleObjProperty->CreatePropertyValueWidget()
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(FMargin(4.0f, 0.0f, 0.0f, 0.0f))
					.VAlign(VAlign_Center)
					[
						SNew(SButton)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							.OnClicked_Lambda([&]()
								{
									//TSharedPtr<FTabManager> TabManager = FGlobalTabmanager::Get(); // 或你自定义的TabManager

									//StructPropertyHandle->
									//FName TabId = "SubObjectDetailTab";

									//if (!TabManager->HasTabSpawner(TabId))
									//{
									//	TabManager->RegisterTabSpawner(TabId, FOnSpawnTab::CreateRaw(this, &FMyCustomization::SpawnSubObjectTab))
									//		.SetDisplayName(FText::FromString("子对象详情"))
									//		.SetGroup(WorkspaceGroup.Pin()) // 这一步是关键，让它和原来的 tab 同组
									//		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"));
									//}

									//TabManager->TryInvokeTab(TabId);
									return FReply::Handled();
								})
							.Content()
							[
								SNew(SImage)
									.Image(FAppStyle::GetBrush("LevelEditor.Tabs.Details"))
							]
					]
			]
			.NameContent()
			[
				SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Fill)
					[
						SNew(SBox)
						.HAlign(HAlign_Fill)
						[
							RegexProperty->CreatePropertyValueWidget()
						]
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(FMargin(4.0f, 0.0f, 0.0f, 0.0f))
					.VAlign(VAlign_Center)
					[
						RegexEditorEntryWidget.ToSharedRef()
					]
			];
	}
}

void FAssetRuleItemStructCustomization::CustomizeChildren( TSharedRef<IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils )
{
	CustomizeRuleObjectProperty(StructPropertyHandle);
	CustomizeRegularExpressionProperty(StructPropertyHandle);
	CustomizeManagedPropertyPathProperty(StructPropertyHandle);
}
//
//FReply FAssetRuleItemStructCustomization::OnPickContent(TSharedRef<IPropertyHandle> PropertyHandle) 
//{
//	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
//	FPathPickerConfig PathPickerConfig;
//	PropertyHandle->GetValue(PathPickerConfig.DefaultPath);
//	PathPickerConfig.bAllowContextMenu = false;
//	PathPickerConfig.OnPathSelected = FOnPathSelected::CreateSP(this, &FAssetRuleItemStructCustomization::OnPathPicked, PropertyHandle);
//
//	FMenuBuilder MenuBuilder(true, NULL);
//	MenuBuilder.AddWidget(SNew(SBox)
//		.WidthOverride(300.0f)
//		.HeightOverride(300.0f)
//		[
//			ContentBrowserModule.Get().CreatePathPicker(PathPickerConfig)
//		], FText());
//
//
//	PickerMenu = FSlateApplication::Get().PushMenu(PickerButton.ToSharedRef(),
//		FWidgetPath(),
//		MenuBuilder.MakeWidget(),
//		FSlateApplication::Get().GetCursorPos(),
//		FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu)
//		);
//
//	return FReply::Handled();
//}
//
//FReply FAssetRuleItemStructCustomization::OnPickDirectory(TSharedRef<IPropertyHandle> PropertyHandle, const bool bRelativeToGameContentDir, const bool bUseRelativePath) const
//{
//	FString Directory;
//	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
//	if (DesktopPlatform)
//	{
//
//		TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(BrowseButton.ToSharedRef());
//		void* ParentWindowHandle = (ParentWindow.IsValid() && ParentWindow->GetNativeWindow().IsValid()) ? ParentWindow->GetNativeWindow()->GetOSWindowHandle() : nullptr;
//
//		FString StartDirectory = FEditorDirectories::Get().GetLastDirectory(ELastDirectory::GENERIC_IMPORT);
//		if (bRelativeToGameContentDir && !IsValidPath(StartDirectory, bRelativeToGameContentDir))
//		{
//			StartDirectory = AbsoluteGameContentDir;
//		}
//
//		// Loop until; a) the user cancels (OpenDirectoryDialog returns false), or, b) the chosen path is valid (IsValidPath returns true)
//		for (;;)
//		{
//			if (DesktopPlatform->OpenDirectoryDialog(ParentWindowHandle, LOCTEXT("FolderDialogTitle", "Choose a directory").ToString(), StartDirectory, Directory))
//			{
//				FText FailureReason;
//				if (IsValidPath(Directory, bRelativeToGameContentDir, &FailureReason))
//				{
//					FEditorDirectories::Get().SetLastDirectory(ELastDirectory::GENERIC_IMPORT, Directory);
//
//					if (bRelativeToGameContentDir)
//					{
//						Directory.RightChopInline(AbsoluteGameContentDir.Len(), EAllowShrinking::No);
//					}
//					else if (bUseRelativePath)
//					{
//						Directory = IFileManager::Get().ConvertToRelativePath(*Directory);
//					}
//
//					PropertyHandle->SetValue(Directory);
//				}
//				else
//				{
//					StartDirectory = Directory;
//					FMessageDialog::Open(EAppMsgType::Ok, FailureReason);
//					continue;
//				}
//			}
//			break;
//		}
//	}
//
//	return FReply::Handled();
//}
//
//bool FAssetRuleItemStructCustomization::IsValidPath(const FString& AbsolutePath, const bool bRelativeToGameContentDir, FText* const OutReason) const
//{
//	if(bRelativeToGameContentDir)
//	{
//		if(!AbsolutePath.StartsWith(AbsoluteGameContentDir))
//		{
//			if(OutReason)
//			{
//				*OutReason = FText::Format(LOCTEXT("Error_InvalidRootPath", "The chosen directory must be within {0}"), FText::FromString(AbsoluteGameContentDir));
//			}
//			return false;
//		}
//	}
//
//	return true;
//}
//
//void FAssetRuleItemStructCustomization::OnPathPicked(const FString& Path, TSharedRef<IPropertyHandle> PropertyHandle)
//{
//	if (PickerMenu.IsValid())
//	{
//		PickerMenu->Dismiss();
//		PickerMenu.Reset();
//	}
//
//	PropertyHandle->SetValue(Path);
//}
//
//bool FAssetRuleItemStructCustomization::IsBrowseEnabled(TSharedRef<IPropertyHandle> PropertyHandle) const
//{
//	return PropertyHandle->IsEditable();
//}

#undef LOCTEXT_NAMESPACE
