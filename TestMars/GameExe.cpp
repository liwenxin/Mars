#include "GameExe.h"
GameInfo::GameInfo(LPSTR gamename,bool flag)
{
	m_GameName=gamename;
	m_flag=flag;
}
void GameInfo::setFlag(bool flag)
{
	m_flag=flag;
}
bool GameInfo::getFlag()
{
	return m_flag;
}
void GameInfo::setGameName(LPSTR gamename)
{
	m_GameName=gamename;
}
LPSTR GameInfo::getGameName()
{
	return m_GameName;
}