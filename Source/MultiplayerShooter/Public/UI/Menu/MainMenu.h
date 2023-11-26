// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Containers/Deque.h"
#include "MainMenu.generated.h"

class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

private:

	TDeque<UUserWidget>* MenuStack;

	void PutMenuOnStack(const UUserWidget& MenuToStack);
	void GoBack();
};
