#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleUI.generated.h"

class UButton;
UCLASS()
class UE_ARPG_API UTitleUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;

	UPROPERTY(meta = (BindWidget))
		UButton* ContinueBtn;

	UFUNCTION(BlueprintCallable)
		void OnClickQuitButton();

	UFUNCTION(BlueprintCallable)
		void OnClickNewGameButton();

	UFUNCTION(BlueprintCallable)
		void OnClickContinueButton();
};
