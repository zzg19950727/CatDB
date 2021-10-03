#include "state.h"
#include "log.h"

using namespace CatDB::Server;

State::ErrorStringMap State::maps_[MAX_ERROR_CODE] = {};
const State State::instance_;

State::State()
{
	#include "state.map"
}

void State::ADD_SQLSTATE(int err, const char *jdbc_state, const char *odbc_state)
{
  if (err <= 0)
  {
    maps_[err].jdbc_state = jdbc_state;
    maps_[err].odbc_state = odbc_state;
  }
}

const char *State::get_odbc_state(int err) const
{
  const char *state = "S1000";
  const char *state_succ = "00000";

  if (err >= 0)
  {
    if (err == SUCCESS)
    {
      state = state_succ;
    }
    else if (err >= MAX_ERROR_CODE)
    {
      LOG_ERR("error code out of range", K(err));
    }
    else if (NULL != maps_[err].odbc_state && maps_[err].odbc_state[0] != '\0')
    {
      state = maps_[err].odbc_state;
    }
  }
  return state;
}

const char *State::get_jdbc_state(int err) const
{
  const char *state = "HY000";
  const char *state_succ = "00000";

  if (err >= 0)
  {
    if (err == SUCCESS)
    {
      state = state_succ;
    }
    else if (err >= MAX_ERROR_CODE)
    {
      LOG_ERR("error code out of range", K(err));
    }
    else if (NULL != maps_[err].jdbc_state && maps_[err].jdbc_state[0] != '\0')
    {
      state = maps_[err].jdbc_state;
    }
  }
  return state;
}
