#ifndef __JSON_MANAGER_H__
#define __JSON_MANAGER_H__

#include "json-c/json.h"
#include <string>

typedef struct DateType
{
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
} DateType;

typedef enum GameState
{
	GAME_NOT_STARTED = 0,
	GAME_IN_PROGRESS,
	GAME_FINAL
} GameState;

class JsonItem
{
public:
	virtual bool parseRequest(struct json_object* obj) = 0;
};

class JsonTeam
{
public:
	JsonTeam() : m_name(nullptr), m_current_score(0), m_wins(0), m_losses(0) {}

	bool parseRequest(struct json_object* obj);

	uint8_t GetCurrentScore() { return m_current_score; }
	uint16_t GetWins() { return m_wins; }
	uint16_t GetLosses() { return m_losses; }
	char* GetTeamName() { return m_name; }

private:
	char* m_name;
	uint8_t m_current_score;
	uint16_t m_wins;
	uint16_t m_losses;
};

class JsonVenue
{
public:
	JsonVenue() : m_id(0), m_name(nullptr), m_link(nullptr) {}

	bool parseRequest(struct json_object* obj);

	char* GetVenueName() { return m_name; }

private:
	uint8_t m_id;
	char* m_name;
	char* m_link;
};

class JsonGame
{
public:
	JsonGame() : m_gamePk(-1), m_link(nullptr) {}

	bool parseRequest(struct json_object * obj);

	std::string GetHeadline() { return m_date.month + "/" + m_date.day + '/' + m_date.year + '\n' + std::string(m_awayteam.GetTeamName()) + " vs " + std::string(m_hometeam.GetTeamName()); }
	std::string GetThumbnail() { return m_link; }
	std::string GetSummary() { return "(" + m_state + ')' + std::to_string(m_awayteam.GetCurrentScore()) + " - " + std::to_string(m_hometeam.GetCurrentScore());  }
	
	void SetPrevGame(JsonGame* game) { m_prevGame = game; }
	JsonGame* GetNextGame() { return m_prevGame; }

private:
	int m_gamePk;
	char* m_link;
	DateType m_date;
	GameState m_state;
	JsonTeam m_awayteam;
	JsonTeam m_hometeam;
	JsonVenue m_venue;
	JsonGame* m_prevGame;
};

class JsonManager
{
public:
	JsonManager() : m_totalgames(0) {}

	bool parseRequest(char* data);
	int GetTotalGames() { return m_totalgames; }
	JsonGame* GetMostRecentGame() { return m_mostrecentgame; }

private:
	JsonGame * m_mostrecentgame;
	uint16_t m_totalgames;
};

#endif