//��������ʵ����ƽ̨֮��Э��Ľ���
#include "BoardMessageControl.h"
#include<fstream>
int	main()
{
	//freopen("t.log","w",stdout);
	/*ofstream outfile;
	outfile.open("game.txt"); 
	outfile.close();*/
	struct Ddz tDdz, *pDdz = &tDdz;
	BoardMessageControl * pBoardControl = new BoardMessageControl();
	pBoardControl->InitTurn(pDdz);			//��ʼ������
	
	//�޶��ߣ���Ѷ�� �޶�ʱ�䣺18-7-22
	//���Ӳ��֣��ڿ���̨��ģ����ƽ̨�Ͻ�����Ϸ
	if (iFShowInConsole){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |FOREGROUND_GREEN);
	    strcpy(pDdz->sCommandIn, "DOUDIZHUVER 1.0"); //ģ��ƽ̨���Ͱ汾��Ϣ
	    pBoardControl->AnalyzeMsg(pDdz);
	    pBoardControl->OutputMsg(pDdz);
	    cout << "\n��������Ϸ�ܾ�����" << endl;
	    cin >> pDdz->iRoundTotal;
	    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	//NAME 
	}
	//

	while (pDdz->iStatus != 0)            //ѭ����ÿ�δ�ƽ̨���һ����Ϣ��Ȼ��������������Ĺ���
	{
		pBoardControl->InputMsg(pDdz);			//������Ϣ
		pBoardControl->AnalyzeMsg(pDdz);		//����������Ϣ
		pBoardControl->OutputMsg(pDdz);		    //�����Ϣ
		//pBoardControl->CalOthers(pDdz);		//������������
	}
	delete(pBoardControl);
	return 0;
}
	