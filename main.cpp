#pragma once
#include"head.h"
#include"VirtualDisk.h"
#include"Factory.h"
#include"Command.h"
/*
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif 
#endif  // _DEBUG

#define CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  */
//����ں����а��� _CrtDumpMemoryLeaks(); 
void test()
{
	//�����������
	VirtualDisk* Newvirtual = VirtualDisk::GetVirtualDisk();
	//��������
	string input;
	//��������
	Factory* fac = new Factory();
	//һ������
	Command* cmdtype = nullptr;
	//һ��������
	UseCommand* excucmd = new UseCommand();
	while (1)
	{
		
		cout << Newvirtual->GetLocalDir()->GetPath() +"/"+ Newvirtual->GetLocalDir()->GetName()<< ">";
		getline(cin, input);
		//ʶ������
		cmdtype=fac->GetCommand(Newvirtual->GetLocalDir(),input);
	
		if (cmdtype != nullptr)
		{
			//��������ȷ�����õ���������
			excucmd->SetCommand(cmdtype);
			
			//ִ������
			if (excucmd->SendTask(Newvirtual)) 
			{
				delete cmdtype;
				cmdtype = nullptr;
				break; 
			}
			else
			{
				delete cmdtype;
				cmdtype = nullptr;
			}
		}
		
	}


	delete Newvirtual;
	Newvirtual = nullptr;

	delete fac;
	fac = nullptr;

	delete excucmd;
	excucmd = nullptr;

	//_CrtDumpMemoryLeaks();
}
int main()
{
	test();
	return 0;
}