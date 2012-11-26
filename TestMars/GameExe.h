/************************************************************************/
/* GameExe.h 
			类GameInfo为游戏的一些基本信息*/
/************************************************************************/
#include <windows.h>
class GameInfo
{
public:
	GameInfo(LPSTR gamename,bool flag);
	~GameInfo(){};
	void setGameName(LPSTR gamename);
	LPSTR getGameName();
	void setFlag(bool flag);//设置游戏运行状态
	bool getFlag();//获取游戏现在的状态

protected:
private:
	LPSTR m_GameName;//游戏名
	bool m_flag;//bool型变量，判断游戏是否在运行。为true时表示游戏正在运行，为false时游戏没有运行。

};