//////////////////////////////////
//////���Ʒ���///////////////
//���ܣ�����������Ϣ����////
////////////////////////////////
#ifndef _CallNum
#define _CallNum
#include <stdio.h>
#include <stdlib.h>
#include "CardsInfo.h"
#include "SplitCard.h"

class CallNum
{
public:
	CallNum();
	~CallNum();
	static CallNum* Instance();

	CardsInfo* CInfo;
	int CardForce(int iCards[]);
	int CallCardAnalyze(Ddz* pDdz);
	SplitCard *pSplitCards;						//���������
};

#endif _CallNum