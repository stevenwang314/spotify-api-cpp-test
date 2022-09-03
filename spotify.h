#pragma once
#include <string>
#include <vector>
#include "spotifyRec.h"

namespace stev {
	class spotify
	{

	public:
		std::string tokenID = "";

		/// <summary>
		/// Initializes spotify API.
		/// </summary>
		/// <returns></returns>
		bool init();
		/// <summary>
		/// Creates a access token using client credentials. This token lasts 60 minutes before expiring. Note that it saves into the file so that the user can reuse the token
		/// by loading the file and getting it from there.
		/// </summary>
		/// <param name="cilent_id">The client id of the application. Obtained from Dashboard -> selected application</param>
		/// <param name="secret_id">The secret id of the application (you need to show secret id before seeing it). Obtained from Dashboard -> selected application</param>
		/// <returns>The access token id</returns>
		std::string createAccessToken_clientCredentials(std::wstring cilent_id, std::wstring secret_id);
		/// <summary>
		/// Creates a Authorization Code using your credentials. It will open a tab in a browser (default browser) and you manually need to accept it. Once so, the program will automatically get the code and create a access token.
		/// </summary>
		/// <param name="cilent_id"></param>
		/// <param name="secret_id"></param>
		/// <returns></returns>
		std::string createAccessToken_authorizationCode(std::wstring cilent_id, std::wstring secret_id);
		std::string refreshToken(std::wstring cilent_id, std::wstring secret_id, std::string refreshID);

		std::string getArtist(std::string tokenID, std::wstring albumId);
		std::string getArtistMultiple(std::string tokenID, std::vector<std::wstring> albumId);
		std::string getArtistAlbums(std::string tokenID, std::wstring albumId, int limit = 20, int offset = 0);
		std::string getArtistAlbums(std::string tokenID, std::wstring albumId, std::wstring market, int limit = 20, int offset = 0);
		std::string getArtistTopTracks(std::string tokenID, std::wstring artistID, std::wstring market);
		std::string getArtistRelatedArtists(std::string tokenID, std::wstring artistID);

		std::string getShow(std::string tokenID, std::wstring showId, std::wstring market = L"");
		std::string getShowMultiple(std::string tokenID, std::vector<std::wstring> showId, std::wstring market = L"");
		std::string getShowEpisode(std::string tokenID, std::wstring showId, int limit = 20, std::wstring market = L"", int offset = 0);
		/// <summary>
		/// Gets user's saved episodes on their profile. Must have Authorization Code required.
		/// </summary>
		/// <param name="tokenID"></param>
		/// <param name="limit"></param>
		/// <param name="market"></param>
		/// <param name="offset"></param>
		/// <returns></returns>
		std::string getUserSavedShows(std::string tokenID, int limit = 20, int offset = 0);
		std::string saveShowsForCurrentUser(std::string tokenID, std::vector<std::wstring> showsToSave);
		std::string removeUserSavedShow(std::string tokenID, std::vector<std::wstring> showsToSave, std::string market= "");
		std::string checkUserSavedShow(std::string tokenID, std::vector<std::wstring> showsToContains);

		std::string getAlbum(std::wstring albumID, std::wstring market);
		std::string getAlbumMultiple(std::vector<std::wstring> albumsList, std::wstring market);
		std::string getAlbumTracks(std::wstring albumId, std::wstring market, int limit = 20, int offset = 0);
		std::string getSavedAlbums(std::wstring market, int limit = 20, int offset = 0);
		std::string saveAlbums(std::vector<std::wstring> albumsList);
		std::string removeAlbums(std::vector<std::wstring> albumsList);
		std::string checkSavedAlbum(std::vector<std::wstring> albumsList);
		std::string getNewReleases(std::wstring market, int limit = 20, int offset = 0);

		std::string getTrack(std::wstring trackID, std::wstring market);
		std::string getTracksMultiple(std::vector<std::wstring> trackIDs, std::wstring market);
		std::string getUserSavedTracks(std::wstring market, int limit = 20, int offset = 0);
		std::string saveTrack(std::vector<std::wstring> trackIDs);
		std::string removeTrack(std::vector<std::wstring> trackIDs);
		std::string checkSavedTracks(std::vector<std::wstring> trackList);
		std::string getTrackAudioFeature(std::wstring trackID);
		std::string getTrackMultipleAudioFeature(std::vector<std::wstring> trackList);

		std::string getTrackAudioAnalysis(std::wstring trackID);
		std::string getRecommendations(spotifyRec rec);
	};

};