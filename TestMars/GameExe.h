/************************************************************************/
/* GameExe.h 
			��GameInfoΪ��Ϸ��һЩ������Ϣ*/
/************************************************************************/
#include <windows.h>
class GameInfo
{
public:
	GameInfo(LPSTR gamename,bool flag);
	~GameInfo(){};
	void setGameName(LPSTR gamename);
	LPSTR getGameName();
	void setFlag(bool flag);//������Ϸ����״̬
	bool getFlag();//��ȡ��Ϸ���ڵ�״̬

protected:
private:
	LPSTR m_GameName;//��Ϸ��
	bool m_flag;//bool�ͱ������ж���Ϸ�Ƿ������С�Ϊtrueʱ��ʾ��Ϸ�������У�Ϊfalseʱ��Ϸû�����С�

};