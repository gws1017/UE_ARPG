#include "UI/OKUI.h"
#include "Global.h"

void UOKUI::NativeDestruct()
{
	Super::NativeDestruct();
	
	if (ClickOKBtnFunction.IsBound())
		ClickOKBtnFunction.Unbind();
	if (ClickNoBtnFunction.IsBound())
		ClickNoBtnFunction.Unbind();
}

void UOKUI::OnClickOKBtn()
{
	if (IsInViewport()) {
		RemoveFromParent();
		if (ClickOKBtnFunction.IsBound()) {
			ClickOKBtnFunction.Execute();
		}
	}
}

void UOKUI::OnClickNoBtn()
{
	if (IsInViewport()) {
		RemoveFromParent();
		if (ClickNoBtnFunction.IsBound()) {
			ClickNoBtnFunction.Execute();
		}
	}
}
