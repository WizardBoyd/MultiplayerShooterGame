// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CustomGameInstanceBase.h"
#include "JsonObjectConverter.h"
#include "Engine/World.h"

UCustomGameInstanceBase::UCustomGameInstanceBase()
{
	Http = &FHttpModule::Get();
}

const FString& UCustomGameInstanceBase::GetWebApiUrl()
{
	return WebApiUrl;
}

TArray<FServerData>& UCustomGameInstanceBase::GetServerList()
{
	return ServerList;
}

void UCustomGameInstanceBase::OnServerListRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
{
	if(Success)
	{
		FString ResponseStr = *Response->GetContentAsString();
		ResponseStr.InsertAt(0, FString("{\"Response\":"));
		ResponseStr.AppendChar('}');
		UE_LOG(LogTemp, Warning, TEXT("Response: %s"), *ResponseStr);

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseStr);

		if(FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
		{
			TArray<TSharedPtr<FJsonValue>> JsonValues = JsonObject->GetArrayField(TEXT("Response"));
			ServerList.Empty(); //Empty out irregardless
			for (TSharedPtr<FJsonValue> value: JsonValues)
			{
				FServerData ServerData = FServerData();
				TSharedPtr<FJsonObject> JsonObj = value->AsObject();
				if(FJsonObjectConverter::JsonObjectToUStruct(JsonObj.ToSharedRef(), &ServerData, 0,0))
				{
					UE_LOG(LogTemp, Warning, TEXT("Json Object to Ustruct success"));
					ServerList.Add(ServerData);
				}
			}
			OnServersReceived.Broadcast();
			// for (FServerData ServerData : ServerList)
			// {
			// 	UE_LOG(LogTemp, Warning, TEXT("ServerID: %d"), ServerData.ServerID);
			// 	UE_LOG(LogTemp, Warning, TEXT("IP: %s"), *ServerData.IPAddress);
			// 	UE_LOG(LogTemp, Warning, TEXT("ServerName: %s"), *ServerData.ServerName);
			// 	UE_LOG(LogTemp, Warning, TEXT("MapName: %s"), *ServerData.MapName);
			// 	UE_LOG(LogTemp, Warning, TEXT("CurrentPlayers: %d"), ServerData.CurrentPlayers);
			// 	UE_LOG(LogTemp, Warning, TEXT("MaxPlayers: %d"), ServerData.MaxPlayers);
			// 	UE_LOG(LogTemp, Warning, TEXT("END OF FIRST SERVER ENTRY"));
			// }
		}
	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnServerListRequestComplete Failed"));
	}
}

void UCustomGameInstanceBase::GenerateServerList()
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnServerListRequestComplete);
	
	// Request->SetURL("https://localhost:44364/api/host");
	// Request->SetVerb("GET");
	// Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	//
	// Request->ProcessRequest();
	Request->SetURL(GetWebApiUrl());
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	Request->ProcessRequest();
}

void UCustomGameInstanceBase::TravelToMap(const FString& MapUrl)
{
	UWorld* world = GetWorld();
	world->ServerTravel(MapUrl);
}
