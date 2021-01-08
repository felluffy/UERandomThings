// Fill out your copyright notice in the Description page of Project Settings.


#include "STTRequestComponent.h"
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Runtime/Online/HTTP/Public/HttpModule.h"
#include "Sound/SoundWave.h"
#include "Interfaces/IHttpRequest.h"

// Sets default values for this component's properties
USTTRequestComponent::USTTRequestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	Http = &FHttpModule::Get();
	URL = TEXT("");
	FileLocation = TEXT("");
	OAuthToken = TEXT("");
	VerbType = "POST";
}


void USTTRequestComponent::SerializeReceivedAudio_Implementation(USoundWave* SoundWave)
{
	
}

void USTTRequestComponent::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		auto it = JsonObject->GetArrayField("NBest");
		if (it.begin() != it.end())
		{
			auto JsonMid = it[0]->AsObject();
    
			float Accuracy = JsonMid->GetNumberField("Confidence");
			FString ToReturnSentence = JsonMid->GetStringField("Display");
			OnRecognizedSpeech.Broadcast(this, Accuracy, ToReturnSentence); 
		}
	}
}

void USTTRequestComponent::SendAudioRequest(FString Path, bool UseHeaderMap)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->SetURL(URL);
	Request->SetVerb(VerbType);
	if(!UseHeaderMap)
	{
		Request->OnProcessRequestComplete().BindUObject(this, &USTTRequestComponent::OnResponseReceived);
		Request->SetHeader("Content-Type", TEXT("audio/wav"));
		Request->AppendToHeader("Content-Type", TEXT("codecs=audio/pcm"));
		Request->AppendToHeader("Content-Type", TEXT("samplerate=16000"));
		Request->SetHeader(TEXT("Ocp-Apim-Subscription-Key"), OAuthToken);
		Request->SetHeader("Accept", "application/json; text/xml");
		Request->SetHeader("Expect", "100-continue");
		Request->SetHeader("Host", "centralindia.speech.microsoft.com");
	}
	else
	{
		for(auto &it: HeaderMap)
		{
			Request->SetHeader(it.Key, it.Value);
		}
	}
	bool resB = Request->SetContentAsStreamedFile(FileLocation);
	UE_LOG(LogTemp, Error, TEXT("File found %d, file loc - %s"), resB, *FileLocation);

	Request->ProcessRequest();
}

// Called when the game starts
void USTTRequestComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USTTRequestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

