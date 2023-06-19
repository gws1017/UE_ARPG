#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OKUI.generated.h"

DECLARE_DELEGATE(FClickBtn);

UCLASS()
class UE_ARPG_API UOKUI : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeDestruct() override;

public:

	UFUNCTION(BlueprintCallable)
		void OnClickOKBtn();
	UFUNCTION(BlueprintCallable)
		void OnClickNoBtn();

public:

	FClickBtn ClickOKBtnFunction;
	FClickBtn ClickNoBtnFunction;
};
