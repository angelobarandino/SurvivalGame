// Fill out your copyright notice in the Description page of Project Settings.


#include "SGUIManagerSubsystem.h"

#include "CommonLocalPlayer.h"
#include "GameUIPolicy.h"
#include "PrimaryGameLayout.h"
#include "Components/SlateWrapperTypes.h"
#include "GameFramework/HUD.h"

USGUIManagerSubsystem::USGUIManagerSubsystem()
{
}

void USGUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	TickHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &ThisClass::Tick), 0);
}

void USGUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();

	FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
}

bool USGUIManagerSubsystem::Tick(float DeltaTime)
{
	if (const UGameUIPolicy* Policy = GetCurrentUIPolicy())
	{
		for (const ULocalPlayer* LocalPlayer : GetGameInstance()->GetLocalPlayers())
		{
			if (const APlayerController* PC = LocalPlayer->GetPlayerController(GetWorld()))
			{
				if (const AHUD* HUD = PC->GetHUD())
				{
					if (UPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CastChecked<UCommonLocalPlayer>(LocalPlayer)))
					{
						const ESlateVisibility DesiredVisibility = HUD->bShowHUD
							? ESlateVisibility::SelfHitTestInvisible
							: ESlateVisibility::Collapsed;
						
						if (DesiredVisibility != RootLayout->GetVisibility())
						{
							RootLayout->SetVisibility(DesiredVisibility);	
						}
					}
				}
			}
		}
	}
	
	return true;
}
