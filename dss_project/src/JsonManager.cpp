#include "JsonManager.h"
#include <stdio.h>

int getInt(struct json_object * obj, const char * key)
{
	struct json_object * o = json_object_object_get(obj, key);
	return json_object_get_int(o);
}

const char* getString(struct json_object* obj, const char* key)
{
	struct json_object* o = json_object_object_get(obj, key);
	return json_object_get_string(o);
}

// dirty parse
void setDate(DateType &date, const char* s)
{
	char buf[8];

	memset(buf, 0, sizeof(buf));
	strncpy(buf, s, 4);
	date.year = atoi(buf);

	memset(buf, 0, sizeof(buf));
	strncpy(buf, s+5, 2);
	date.month = atoi(buf);
	
	memset(buf, 0, sizeof(buf));
	strncpy(buf, s+8, 2);
	date.day = atoi(buf);

	memset(buf, 0, sizeof(buf));
	strncpy(buf, s+11, 2);
	date.hour = atoi(buf);

	memset(buf, 0, sizeof(buf));
	strncpy(buf, s+14, 2);
	date.minute = atoi(buf);
}

bool JsonTeam::parseRequest(struct json_object* obj)
{
	m_current_score = getInt(obj, "score");

	struct json_object* record = json_object_object_get(obj, "leagueRecord");
	m_wins = getInt(record, "wins");
	m_losses = getInt(record, "losses");

	struct json_object* team = json_object_object_get(obj, "team");
	m_name = (char *)getString(team, "name");

	return true;
}

bool JsonVenue::parseRequest(struct json_object * obj)
{
	m_name = (char *)getString(obj, "name");

	return true;
}

bool JsonGame::parseRequest(struct json_object * obj)
{
	m_gamePk = getInt(obj, "gamePk");
	const char* s = getString(obj, "gameDate");
	setDate(m_date, s);

	struct json_object* teams = json_object_object_get(obj, "teams");
	m_awayteam.parseRequest(json_object_object_get(teams, "away"));
	m_hometeam.parseRequest(json_object_object_get(teams, "home"));

	m_venue.parseRequest(json_object_object_get(obj, "venue"));

	return true;
}

bool JsonManager::parseRequest(char * data)
{
	struct json_object* jobj;
	jobj = json_tokener_parse(data);
	JsonGame* prev_game = nullptr;

	m_totalgames = getInt(jobj, "totalGames");
	struct json_object* dates_arr = json_object_object_get(jobj, "dates");
	for (int i = 0; i < json_object_array_length(dates_arr); i++)
	{
		struct json_object* date_i = json_object_array_get_idx(dates_arr, i);
		struct json_object* games_arr = json_object_object_get(date_i, "games");
		for (int j = 0; j < json_object_array_length(games_arr); j++)
		{
			JsonGame * game = new JsonGame;
			game->parseRequest(json_object_array_get_idx(games_arr, i));
			if (prev_game)
				game->SetPrevGame(prev_game);
			prev_game = game;
		}
	}

	return true;
}