#include <iostream>
#include <fstream>
#include <Windows.h>
#include <winhttp.h>
#include <vector>
#include <sstream>
#include <AtlBase.h>
#include <AtlCom.h>
#include <UIAutomation.h>
#include "os_windows/swStringUtil.h"

#include "spotify.h"
#include "sBase64Encode.h"

using namespace std;
#pragma comment(lib, "Winhttp.lib")
namespace stev {
	HINTERNET accountSpotify;
	HINTERNET apiSpotify;
	std::string implementApiRequest(std::wstring verb, std::wstring apiLocation, std::vector<std::wstring> dataList) {
		HINTERNET httpRestRequest = WinHttpOpenRequest(apiSpotify, verb.c_str(), apiLocation.c_str(), L"HTTP/1.1", WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
		if (!httpRestRequest) {
			return "";
		}
		for (auto a : dataList) {
			if (!WinHttpAddRequestHeaders(httpRestRequest, a.c_str(), (DWORD)a.size(), WINHTTP_ADDREQ_FLAG_ADD)) {
				return "";
			}
		}
		if (!WinHttpSendRequest(httpRestRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, nullptr, 0, 0, 0)) {
			return "";
		}
		if (!WinHttpReceiveResponse(httpRestRequest, nullptr)) {
			return "";
		}

		DWORD data_read = 0;
		const int read_size = 1000;
		CHAR data[read_size + 1];
		//Read all the data.
		std::ostringstream os;
		do {
			if (!WinHttpQueryDataAvailable(httpRestRequest, &data_read)) {
				return "";
			}
			if (data_read == 0) { //no data to read, break.
				break;
			}
			if (!WinHttpReadData(httpRestRequest, data, read_size, &data_read)) {
				return "";
			}
			//Add a no termination 
			data[data_read] = '\0';
			os << data;

		} while (data_read);
		return os.str();
	}
	std::string implementApiRequest(std::wstring verb, std::wstring apiLocation, std::vector<std::wstring> dataList, std::wstring dataRequest) {
		std::wstring t = std::wstring(dataRequest.begin(), dataRequest.end());
		HINTERNET httpRestRequest = WinHttpOpenRequest(apiSpotify, verb.c_str(), apiLocation.c_str(), L"HTTP/1.1", WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
		if (!httpRestRequest) {
			return "";
		}
		for (auto a : dataList) {
			if (!WinHttpAddRequestHeaders(httpRestRequest, a.c_str(), (DWORD)a.size(), WINHTTP_ADDREQ_FLAG_ADD)) {
				return "";
			}
		}
		if (!WinHttpSendRequest(httpRestRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, (void*)dataRequest.c_str(), (DWORD)dataRequest.size(), (DWORD)dataRequest.size(), 0)) {
			return "";
		}
		if (!WinHttpReceiveResponse(httpRestRequest, nullptr)) {
			return "";
		}

		//Whenever we send a request, we get a result back. Let's read all that data.
		DWORD data_read = 0;
		const int read_size = 1000;
		CHAR data[read_size + 1];
		//Read all the data.
		std::ostringstream os;
		do {
			if (!WinHttpQueryDataAvailable(httpRestRequest, &data_read)) {
				return "";
			}
			if (data_read == 0) { //no data to read, break.
				break;
			}
			if (!WinHttpReadData(httpRestRequest, data, read_size, &data_read)) {
				return "";
			}
			//Add a no termination 
			data[data_read] = '\0';
			os << data;

		} while (data_read);
		return os.str();
	}
	std::string implementRequest(std::wstring verb, std::wstring apiLocation, std::vector<std::wstring> dataList, std::string dataRequest) {
		std::wstring t = std::wstring(dataRequest.begin(), dataRequest.end());
		HINTERNET httpRestRequest = WinHttpOpenRequest(accountSpotify, verb.c_str(), apiLocation.c_str(), L"HTTP/1.1", WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
		if (!httpRestRequest) {
			return "";
		}
		for (auto a : dataList) {
			if (!WinHttpAddRequestHeaders(httpRestRequest, a.c_str(), (DWORD)a.size(), WINHTTP_ADDREQ_FLAG_ADD)) {
				return "";
			}
		}
		if (!WinHttpSendRequest(httpRestRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, (void*)dataRequest.c_str(), (DWORD)dataRequest.size(), (DWORD)dataRequest.size(), 0)) {
			return "";
		}
		if (!WinHttpReceiveResponse(httpRestRequest, nullptr)) {
			return "";
		}

		//Whenever we send a request, we get a result back. Let's read all that data.
		DWORD data_read = 0;
		const int read_size = 1000;
		CHAR data[read_size + 1];
		//Read all the data.
		std::ostringstream os;
		do {
			if (!WinHttpQueryDataAvailable(httpRestRequest, &data_read)) {
				return "";
			}
			if (data_read == 0) { //no data to read, break.
				break;
			}
			if (!WinHttpReadData(httpRestRequest, data, read_size, &data_read)) {
				return "";
			}
			//Add a no termination 
			data[data_read] = '\0';
			os << data;

		} while (data_read);
		return os.str();
	}
	HINTERNET http1;
	bool spotify::init()
	{
		http1 = WinHttpOpen(L"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.0.0 Safari/537.36", WINHTTP_ACCESS_TYPE_NO_PROXY, nullptr, nullptr, WINHTTP_FLAG_ASYNC);
		if (!http1) {
			return false;
		}
		accountSpotify = WinHttpConnect(http1, L"accounts.spotify.com", INTERNET_DEFAULT_HTTPS_PORT, 0);
		if (!accountSpotify) {
			return false;
		}
		apiSpotify = WinHttpConnect(http1, L"api.spotify.com", INTERNET_DEFAULT_HTTPS_PORT, 0);
		if (!apiSpotify) {
			return false;
		}
		return true;
	}
	std::string spotify::createAccessToken_clientCredentials(std::wstring client_id, std::wstring secret_id)
	{
		//std::wstring verb = L"POST";
		//std::wstring api_request_token = L"/api/token";
		//std::vector<wstring> authorization = { L"Authorization: Basic " + stev::bit64Encode::encodeBase64Text(client_id + L":" + secret_id), L"Content-Type: application/x-www-form-urlencoded" };
		//std::string additionalDataType = "additionalDataType=client_credentials";
		//return implementRequest(verb, api_request_token, authorization, additionalDataType);

		std::wstring verb = L"POST";
		std::wstring api_request_token = L"/api/token?grant_type=client_credentials";
		std::vector<wstring> authorization = { L"Authorization: Basic " + stev::bit64Encode::encodeBase64Text(client_id + L":" + secret_id), L"Content-Type: application/x-www-form-urlencoded" };
		std::string grant_type = "";
		return implementRequest(verb, api_request_token, authorization, grant_type);
	}
	std::string spotify::createAccessToken_authorizationCode(std::wstring client_id, std::wstring secret_id)
	{
		std::vector<std::wstring> scopeList = { 
			L"ugc-image-upload",L"user-modify-playback-state",L"user-read-playback-state",L"user-read-currently-playing",L"user-follow-modify",L"user-follow-read",
			L"user-read-recently-played",L"user-read-playback-position",L"user-top-read",L"playlist-read-collaborative",L"playlist-modify-public",L"playlist-read-private",
			L"playlist-modify-private",L"app-remote-control",L"streaming",L"user-read-email",L"user-read-private",L"user-library-modify",L"user-library-read"
		};
		
		//Open the default browser using this website for the user to verify their information.
		std::wostringstream os;
		std::wstring redirectUri = L"https://spotify.com";
		os << L"http://accounts.spotify.com" << L"/authorize?client_id=" + client_id + L"&response_type=code&redirect_uri=" + redirectUri + L"&scope=" + stev::swStringUtil::appendStringW(scopeList, L"%20");
		ShellExecute(0, 0, os.str().c_str(), 0, 0, SW_SHOW);

		Sleep(2000);
		HWND hwnd = nullptr;
		std::vector<HWND> hwndList;
		do
		{
			hwnd = FindWindowEx(nullptr, hwnd, L"Chrome_WidgetWin_1", nullptr);
			if (IsWindowVisible(hwnd) && GetWindowTextLength(hwnd) > 0) {
				hwndList.push_back(hwnd);
			}
		} while (hwnd);

		CComPtr<IUIAutomation> uia;
		CComPtr<IUIAutomationElement> toolbar;
		if (SUCCEEDED(uia.CoCreateInstance(CLSID_CUIAutomation)))
		{
			for (auto a : hwndList) {
				toolbar = nullptr;
				CComPtr<IUIAutomationElement> root;
				//Get element from handle.
				if (SUCCEEDED(uia->ElementFromHandle(a, &root))) {

					CComPtr<IUIAutomationCondition> toolbar_cond;

					//Identifies the type of automation element. In this case, We want to identify the Pane control type.
					HRESULT hr2 = uia->CreatePropertyCondition(UIA_ControlTypePropertyId, CComVariant(UIA_PaneControlTypeId), &toolbar_cond);
					//Find all childrens of the element.
					CComPtr<IUIAutomationElementArray> arr;
					if FAILED(root->FindAll(TreeScope_Children, toolbar_cond, &arr))
						continue;
					//Get a list of Panes available
					int count = 0;
					arr->get_Length(&count);
					for (int i = 0; i < count; i++)
					{
						CComBSTR name;
						CComBSTR className;
				
						if SUCCEEDED(arr->GetElement(i, &toolbar)) {
							HRESULT hr = toolbar->get_CurrentClassName(&className);
							//Get the current name of the pane
							if SUCCEEDED(toolbar->get_CurrentName(&name))
								if (wcscmp(name, L"Google Chrome") == 0)
									break;
						}
						toolbar.Release();
					}

					if (!toolbar)
						continue;

				}
			}
		}
		//look for first UIA_EditControlTypeId under "Google Chrome" pane
		CComPtr<IUIAutomationElement> url;
		CComPtr<IUIAutomationCondition> url_cond;
		//Identifies the type of automation element. In this case, We want to identify the Edit control type.
		uia->CreatePropertyCondition(UIA_ControlTypePropertyId, CComVariant(UIA_EditControlTypeId), &url_cond);
		//Identifies the children and its descendants
		if FAILED(toolbar->FindFirst(TreeScope_Descendants, url_cond, &url))
			return "";
		std::string urls = "";
		while (true) {
			//get value of `url` (Identifies the Value property of the Value control pattern = Value.Value)
			CComVariant var;
			if FAILED(url->GetCurrentPropertyValue(UIA_ValueValuePropertyId, &var))
				break;
			if (!var.bstrVal)
				break;
			urls = stev::swStringUtil::convertVarToString(var);
			if (urls.find("spotify.com/us/?code=") != std::string::npos || urls.find("spotify.com/us/?error=") != std::string::npos) {
				break;
			}
			uia.Release();
		}

		//Then use the code to authenticate an account.
		std::string authCode = urls.substr(urls.find("spotify.com/us/?code=") + 21, urls.size() - (urls.rfind("spotify.com/us/?code=") + 21));

		std::wstring verb = L"POST";
		std::wstring api_request_token = L"/api/token";
		std::vector<wstring> authorization = { L"Authorization: Basic " + stev::bit64Encode::encodeBase64Text(client_id + L":" + secret_id), L"Content-Type: application/x-www-form-urlencoded" };
		std::wostringstream additionalData;
		additionalData << "?grant_type=authorization_code" << "&" << "code=" << std::wstring(authCode.begin(), authCode.end()) << "&redirect_uri=" << redirectUri;
		api_request_token += additionalData.str();
		std::string additionalDataType = "";
		return implementRequest(verb, api_request_token, authorization, additionalDataType);
	}
	std::string spotify::refreshToken(std::wstring client_id, std::wstring secret_id, std::string refreshID)
	{
		std::wstring verb = L"POST";
		std::wstring api_request_token = L"/api/token?grant_type=refresh_token&refresh_token=" + std::wstring(refreshID.begin(),refreshID.end());
		std::vector<wstring> authorization = { L"Authorization: Basic " + stev::bit64Encode::encodeBase64Text(client_id + L":" + secret_id), L"Content-Type: application/x-www-form-urlencoded" };
		std::string extraData = "";
		return implementRequest(verb, api_request_token, authorization, extraData);
	}
	std::string spotify::getArtist(std::string tokenID, std::wstring albumId)
	{
		std::wstring verb = L"GET";
		std::wstring api_request_token = L"v1/artists/" + albumId;
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	std::string spotify::getArtistMultiple(std::string tokenID, std::vector<std::wstring> albumId)
	{
		std::wstring verb = L"GET";
		std::wstring combined_id = L"";
		for (int i = 0; i < albumId.size(); i++) {
			combined_id += albumId[i];
			if (i != albumId.size() - 1) {
				combined_id += L",";
			}
		}
		std::wstring api_request_token = L"v1/artists?ids=" + combined_id;
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	std::string spotify::getArtistAlbums(std::string tokenID, std::wstring albumId, int limit, int offset)
	{
		std::wstring verb = L"GET";
		std::wstring api_request_token = L"v1/artists/" + albumId + L"/albums?limit=" + std::to_wstring(limit) + L"&offset=" + std::to_wstring(offset);
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	std::string spotify::getArtistAlbums(std::string tokenID, std::wstring albumId, std::wstring market, int limit, int offset)
	{
		std::wstring verb = L"GET";
		std::wstring api_request_token = L"v1/artists/" + albumId + L"/albums?market=" + market + L"&limit=" + std::to_wstring(limit) + L"&offset=" + std::to_wstring(offset);
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}

	std::string spotify::getArtistTopTracks(std::string tokenID, std::wstring artistID, std::wstring market)
	{
		std::wstring verb = L"GET";
		std::wstring api_request_token = L"v1/artists/" + artistID + L"/top-tracks?market=" + market;
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	std::string spotify::getArtistRelatedArtists(std::string tokenID, std::wstring artistID)
	{
		std::wstring verb = L"GET";
		std::wstring api_request_token = L"v1/artists/" + artistID + L"/related-artists";
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	std::string spotify::getShow(std::string tokenID, std::wstring showId, std::wstring market) {
		std::wstring verb = L"GET";
		std::wstring api_request_token = L"v1/shows/" + showId + L"?market=" + market;
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	std::string spotify::getShowMultiple(std::string tokenID, std::vector<std::wstring> showId, std::wstring market) {
		std::wstring verb = L"GET";
		std::wstring combined_id = L"";
		for (int i = 0; i < showId.size(); i++) {
			combined_id += showId[i];
			if (i != showId.size() - 1) {
				combined_id += L",";
			}
		}
		std::wstring api_request_token = L"v1/shows?ids=" + combined_id + L"&market=" + market;
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	std::string spotify::getShowEpisode(std::string tokenID, std::wstring showId, int limit, std::wstring market, int offset) {
		std::wstring verb = L"GET";
		std::wstring api_request_token = L"v1/shows/" + showId + L"/episodes?market=" + market + L"&limit=" + std::to_wstring(limit) + L"&offset=" + std::to_wstring(offset);
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	/// <summary>
	/// Gets user's saved episodes on their profile. Must have Authorization Code required.
	/// </summary>
	/// <param name="tokenID"></param>
	/// <param name="limit"></param>
	/// <param name="market"></param>
	/// <param name="offset"></param>
	/// <returns></returns>
	std::string spotify::getUserSavedShows(std::string tokenID, int limit, int offset) {
		std::wstring verb = L"GET";
		std::wstring api_request_token = L"v1/me/shows?limit=" + std::to_wstring(limit) + L"&offset=" + std::to_wstring(offset);
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	std::string spotify::saveShowsForCurrentUser(std::string tokenID, std::vector<std::wstring> showsToSave) {
		std::wstring verb = L"PUT";
		std::wstring combined_id = L"";
		for (int i = 0; i < showsToSave.size(); i++) {
			combined_id += showsToSave[i];
			if (i != showsToSave.size() - 1) {
				combined_id += L",";
			}
		}
		std::wstring api_request_token = L"v1/me/shows?ids=" + combined_id;
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	std::string spotify::removeUserSavedShow(std::string tokenID, std::vector<std::wstring> showsToSave, std::string market) {
		std::wstring verb = L"DELETE";
		std::wstring combined_id = L"";
		for (int i = 0; i < showsToSave.size(); i++) {
			combined_id += showsToSave[i];
			if (i != showsToSave.size() - 1) {
				combined_id += L",";
			}
		}
		std::wstring api_request_token = L"v1/me/shows?ids=" + combined_id + L"&market=" + std::wstring(market.begin(),market.end());
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	std::string spotify::checkUserSavedShow(std::string tokenID, std::vector<std::wstring> showsToContains) {
		std::wstring verb = L"GET";
		std::wstring combined_id = L"";
		for (int i = 0; i < showsToContains.size(); i++) {
			combined_id += showsToContains[i];
			if (i != showsToContains.size() - 1) {
				combined_id += L",";
			}
		}
		std::wstring api_request_token = L"v1/me/shows/contains?ids=" + combined_id;
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	std::string spotify::getAlbum(std::wstring albumID, std::wstring market)
	{
		std::wstring verb = L"GET";

		std::wstring api_request_token = L"v1/albums/" + albumID +L"?market=" + market;
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	std::string spotify::getAlbumMultiple(std::vector<std::wstring> albumsList, std::wstring market)
	{
		std::wstring verb = L"GET";
		std::wstring api_request_token = L"v1/albums?ids=" + stev::swStringUtil::appendStringW(albumsList,L",") + L"&market=" + market;
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	std::string spotify::getAlbumTracks(std::wstring albumId, std::wstring market, int limit, int offset)
	{
		std::wstring verb = L"GET";
		std::wstring api_request_token = L"v1/albums/" + albumId + L"/tracks?market=" + market + L"&limit=" + std::to_wstring(limit) + L"&offset=" + std::to_wstring(offset);
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	std::string spotify::getSavedAlbums(std::wstring market, int limit, int offset)
	{
		std::wstring verb = L"GET";
		std::wstring api_request_token = L"v1/me/albums?market=" + market + L"&limit=" + std::to_wstring(limit) + L"&offset=" + std::to_wstring(offset);
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	std::string spotify::saveAlbums(std::vector<std::wstring> albumsList)
	{
		std::wstring verb = L"PUT";
		std::wstring api_request_token = L"v1/me/albums?ids=" + stev::swStringUtil::appendStringW(albumsList, L",");
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json"};
		std::wstring dd = L"\"ids\":[]";
		return implementApiRequest(verb, api_request_token, authorization, dd);
	}
	std::string spotify::removeAlbums(std::vector<std::wstring> albumsList)
	{
		std::wstring verb = L"DELETE";
		std::wstring api_request_token = L"v1/me/albums?ids=" + stev::swStringUtil::appendStringW(albumsList, L",");
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		std::wstring dd = L"\"ids\":[]";
		return implementApiRequest(verb, api_request_token, authorization, dd);
	}
	std::string spotify::checkSavedAlbum(std::vector<std::wstring> albumsList)
	{
		std::wstring verb = L"GET";
		std::wstring api_request_token = L"v1/me/albums/contains?ids=" + stev::swStringUtil::appendStringW(albumsList, L",");
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	std::string spotify::getNewReleases(std::wstring market, int limit, int offset)
	{
		std::wstring verb = L"GET";
		std::wstring api_request_token = L"v1/browse/new-releases?market=" + market + L"&limit=" + std::to_wstring(limit) + L"&offset=" + std::to_wstring(offset);
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	std::string spotify::getTrack(std::wstring trackID, std::wstring market)
	{
		std::wstring verb = L"GET";
		std::wstring api_request_token = L"v1/tracks/" + trackID + L"?market=" + market;
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	std::string spotify::getTracksMultiple(std::vector<std::wstring> trackIDs, std::wstring market)
	{
		std::wstring verb = L"GET";
		std::wstring api_request_token = L"v1/tracks?ids=" + stev::swStringUtil::appendStringW(trackIDs, L",") + L"&market=" + market;
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	std::string spotify::getUserSavedTracks(std::wstring market, int limit, int offset)
	{
		std::wstring verb = L"GET";
		std::wstring api_request_token = L"v1/me/tracks?market=" + market + L"&limit=" + std::to_wstring(limit) + L"&offset=" + std::to_wstring(offset);
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	std::string spotify::saveTrack(std::vector<std::wstring> trackIDs)
	{
		std::wstring verb = L"PUT";
		std::wstring api_request_token = L"v1/me/tracks?ids=" + stev::swStringUtil::appendStringW(trackIDs, L",");
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		std::wstring dd = L"\"ids\":[]";
		return implementApiRequest(verb, api_request_token, authorization, dd);
	}
	std::string spotify::removeTrack(std::vector<std::wstring> trackIDs)
	{
		std::wstring verb = L"DELETE";
		std::wstring api_request_token = L"v1/me/tracks?ids=" + stev::swStringUtil::appendStringW(trackIDs, L",");
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		std::wstring dd = L"\"ids\":[]";
		return implementApiRequest(verb, api_request_token, authorization, dd);
	}
	std::string spotify::checkSavedTracks(std::vector<std::wstring> trackList)
	{
		std::wstring verb = L"GET";
		std::wstring api_request_token = L"v1/me/tracks/contains?ids=" + stev::swStringUtil::appendStringW(trackList, L",");
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	std::string spotify::getTrackAudioFeature(std::wstring trackID)
	{
		std::wstring verb = L"GET";
		std::wstring api_request_token = L"v1/audio-features/" + trackID;
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	std::string spotify::getTrackMultipleAudioFeature(std::vector<std::wstring> trackList)
	{
		std::wstring verb = L"GET";
		std::wstring api_request_token = L"v1/audio-features?ids=" + stev::swStringUtil::appendStringW(trackList, L",");
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
	std::string spotify::getTrackAudioAnalysis(std::wstring trackID)
	{
		std::wstring verb = L"GET";
		std::wstring api_request_token = L"v1/audio-analysis/" + trackID;
		std::vector<wstring> authorization = { L"Authorization: Bearer " + std::wstring(tokenID.begin(),tokenID.end()),L"Content-Type: application/json" };
		return implementApiRequest(verb, api_request_token, authorization);
	}
}
