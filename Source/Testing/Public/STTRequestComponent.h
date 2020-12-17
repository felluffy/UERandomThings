// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IHttpRequest.h"
#include "STTRequestComponent.generated.h"

//use with azure

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRecognizedSpeechSignature, USTTRequestComponent*, STTComponent, float, AccuracyScore, FString, SentenceRetreived);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTING_API USTTRequestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USTTRequestComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnRecognizedSpeechSignature OnRecognizedSpeech;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Events")
    void SerializeReceivedAudio(class USoundWave* SoundWave);

	protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	private:
	class FHttpModule* Http;
	protected:
	UPROPERTY(EditDefaultsOnly, Category = "Request Properties")
	FString URL;
	UPROPERTY(EditDefaultsOnly, Category = "Request Properties")
	FString ContentType;
	UPROPERTY(EditDefaultsOnly, Category = "Request Properties")
	FString VerbType;
	UPROPERTY(EditDefaultsOnly, Category = "Request Properties")
	FString FileLocation;
	UPROPERTY(EditDefaultsOnly, Category = "Request Properties")
	FString	OAuthToken;
	UPROPERTY(EditDefaultsOnly, Category = "Request Properties")
	TMap<FString, FString> HeaderMap;
	
	private:
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	//void SendAudioRequest(const FString& Url, const FString& RequestContent);
	UFUNCTION(BlueprintCallable)
    void SendAudioRequest(FString Path, bool UseHeaderMap = false);
};


		
};
