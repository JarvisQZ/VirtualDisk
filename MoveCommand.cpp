#pragma once
#include"Command.h"

bool MoveCommand::IsCommandCorrect()
{

	//�жϲ����Ƿ�Ϸ�

	if (this->CommandOrder.size() == 3)
	{
		this->SrcPath = this->CommandOrder[1];
		this->DstPath = this->CommandOrder[2];
	}
	else if (this->CommandOrder.size() == 4)
	{
		if (this->CommandOrder[1] != "/y")
		{
			Help::HelpMoveCommand();
			return false;
		}
		this->Cover = true;
		this->SrcPath = this->CommandOrder[2];
		this->DstPath = this->CommandOrder[3];
	}
	else
	{
		Help::HelpMoveCommand();
		return false;
	}
	return true;
}
//����һ��·���������������ڵ�Ŀ¼����   type��true��ʱ��ȡ���һ��Ŀ¼��false��ʱ��ȡ
// move path1 path2      move   a/b/c/d   e/f/g/h    ����sec��˵ a/b/c/d����Ҫ���� ���� dstpath ��˵  e/f/g ���� h ��Ҫ����� 
Dir* MoveCommand::GetPathDir(VirtualDisk* virtualdisk, string path, bool type)
{

	vector<string> parapath = FileHelper::GetSplitPathName(path);
	if (!type)
	{
		//����dstpath��˵ֻ�ж�ǰ����Ŀ¼�Ƿ����
		parapath.pop_back();
	}
	Dir* searchdir = virtualdisk->GetLocalDir();
	map<string, FileStruct*> containdirandfile = searchdir->GetFileOrDir();
	for (unsigned int i = 0; i < parapath.size(); i++)
	{
		//����·��
		if ((i == 0) && (parapath[i] == "C:" or parapath[i] == ""))    //  C:/a/b/c  ����·��  ����   /a/b/c  �� /��ͷ
		{
			if (i == parapath.size() - 1)
			{
				Error::VisitRootError();
				return nullptr;
			}
			//��ȡ��Ŀ¼����
			searchdir = VirtualDisk::GetRootDir();
			containdirandfile = searchdir->GetFileOrDir();
		}
		else if (parapath[i] == "..")  //  ../a/b/c  �� .. ��ͷ������
		{
			if (i == parapath.size() - 1)
			{
				Error::VisitRootError();
				return nullptr;
			}
			//��ȡ��ǰĿ¼��֮���Ϊ��Ŀ¼
			searchdir = searchdir->GetFatherDir();
			containdirandfile = searchdir->GetFileOrDir();
		}
		else if (parapath[i] == ".")
		{
			if (i == parapath.size() - 1)
			{
				Error::CannotVisitError();
				return nullptr;
			}
			continue;
		}
		else
		{
			//��������һ��
			if (i == parapath.size() - 1)
			{
				//Ŀ¼������,����
				if (containdirandfile.find(parapath[i]) == containdirandfile.end())
				{
					//��Ŀ¼
					if (searchdir->GetName() == "")
					{
						Error::CannotFindPathError(searchdir->GetPath() + "/" + parapath[i]);
					}
					else
					{
						Error::CannotFindPathError(searchdir->GetPath() + "/" + searchdir->GetName() + "/" + parapath[i]);

					}
					return nullptr;

				}
				//��map���ҵ������֣������ļ�����Ŀ¼����������һ��
				else
				{

					//������ļ�,ת��ΪFile��֮���ȡ��Ŀ¼
					if (containdirandfile[parapath[i]]->GetFType() == "File")
					{
						if (type)
						{
							//���ھ����ļ���Ŀ¼��ֱ�ӷ���
							return searchdir;
						}
						else
						{
							//��Ŀ¼
							if (searchdir->GetName() == "")
							{
								Error::CannotFindPathError(searchdir->GetPath() + "/" + parapath[i]);
							}
							else
							{
								Error::CannotFindPathError(searchdir->GetPath() + "/" + searchdir->GetName() + "/" + parapath[i]);

							}
							return nullptr;
						}
					}
					//�����Ŀ¼
					else
					{
						if (type)
						{
							//��ȡ��Ŀ¼������ת��ΪĿ¼����
							searchdir = dynamic_cast<Dir*>(containdirandfile[parapath[i]])->GetFatherDir();
						}
						else
						{
							searchdir = dynamic_cast<Dir*>(containdirandfile[parapath[i]]);
						}

					}
					containdirandfile = searchdir->GetFileOrDir();
					return searchdir;
				}
			}
			//�������һ����ȫ��Ŀ¼������   a/b/c/d/r/f   a,b,c,d,r ȫ������Ŀ¼
			else
			{
				//Ŀ¼������,����
				if (containdirandfile.find(parapath[i]) == containdirandfile.end())
				{
					//��Ŀ¼
					if (searchdir->GetName() == "")
					{
						Error::CannotFindPathError(searchdir->GetPath() + "/" + parapath[i]);
					}
					else
					{
						Error::CannotFindPathError(searchdir->GetPath() + "/" + searchdir->GetName() + "/" + parapath[i]);

					}
					return nullptr;

				}
				//��map���ҵ������֣������ж����ļ�����Ŀ¼
				if (containdirandfile.find(parapath[i]) != containdirandfile.end())
				{
					//������ļ�,����,˵��û�����Ŀ¼
					if (containdirandfile[parapath[i]]->GetFType() == "File")
					{
						//��Ŀ¼
						if (searchdir->GetName() == "")
						{
							Error::CannotFindPathError(searchdir->GetPath() + "/" + parapath[i]);
						}
						else
						{
							Error::CannotFindPathError(searchdir->GetPath() + "/" + searchdir->GetName() + "/" + parapath[i]);

						}
						return nullptr;
					}
					//�����Ŀ¼
					else
					{
						//��ȡ��Ŀ¼������ת��ΪĿ¼����
						searchdir = dynamic_cast<Dir*>(containdirandfile[parapath[i]]);
						containdirandfile = searchdir->GetFileOrDir();
					}
				}
			}
		}
	}
	return searchdir;
}

void MoveCommand::Execute(VirtualDisk* virtualdisk)
{
	//��ȡ
	Dir* srcdir = this->GetPathDir(virtualdisk, this->SrcPath, true);
	Dir* dstdir = this->GetPathDir(virtualdisk, this->DstPath, false);
	//���Ϊ��˵���д�
	if (srcdir == nullptr || dstdir == nullptr) { return; }

	vector<string> parapath = FileHelper::GetSplitPathName(this->SrcPath);
	//ȡ��������
	string srcname = parapath[parapath.size() - 1];
	parapath = FileHelper::GetSplitPathName(this->DstPath);
	//ȡ��������
	string dstname = parapath[parapath.size() - 1];
	//if (srcdir == dstdir ) 
	//{ 
	//	map<string, FileStruct*> dirsofsrcdir = srcdir->GetFileOrDir();
	//	if (srcname == dstname)
	//	{
	//		dirsofsrcdir[srcname]->SetModifyTime();
	//	}
	//	else {
	//		//���
	//		dstdir->FileOrDirAdd(dstname, dirsofsrcdir[srcname]);
	//		//ɾ��
	//		srcdir->RemoveFromMap(srcname);
	//	}
	//	return;
	//}

	//������� /y ���� ,������ѯ��
	if (this->Cover == true)
	{
		map<string, FileStruct*> dirsofsrcdir = srcdir->GetFileOrDir();
		map<string, FileStruct*> dirsofdstdir = dstdir->GetFileOrDir();
		//��������
		if (srcname != dstname)
		{
			//�޸�ʱ��ı�
			dirsofsrcdir[srcname]->SetModifyTime();
		}
		//�޸�·��
		if (dstdir == virtualdisk->GetRootDir())
		{
			dirsofsrcdir[srcname]->SetPath(dstdir->GetPath());
		}
		else
		{
			dirsofsrcdir[srcname]->SetPath(dstdir->GetPath() + "/" + dstdir->GetName());
		}

		//����
		dirsofsrcdir[srcname]->SetName(dstname);
		//����ƶ�����Ŀ¼����Ҫ��Ŀ¼�µ������ļ�����Ŀ¼��·�����޸�
		if (dirsofsrcdir[srcname]->GetFType() == "Dir")
		{
			Dir::ChangeAllFileAndDirPathRecursion(dynamic_cast<Dir*>(dirsofsrcdir[srcname]));
			//��Ŀ¼Ҳ���޸�
			dirsofsrcdir[srcname]->SetFatherDir(dstdir);
		}
		//���
		dstdir->FileOrDirAdd(dstname, dirsofsrcdir[srcname]);
		//ɾ��
		srcdir->RemoveFromMap(srcname);
	}
	else
	{
		map<string, FileStruct*> dirsofsrcdir = srcdir->GetFileOrDir();
		map<string, FileStruct*> dirsofdstdir = dstdir->GetFileOrDir();
		//�Ѿ�����ͬ���ļ���Ŀ¼
		string input;
		//�ƶ�����Ŀ¼����ͬ���ļ�
		if (dirsofdstdir.find(dstname) != dirsofdstdir.end())
		{
			cout << "  ����" << dirsofdstdir[dstname]->GetPath() + "/" + dstname << "��?(Yes/No):" << endl;
			getline(cin, input);
			FileHelper::RemoveSpace(input);
			if (input == "Yes" || input == "Y" || input == "y" || input == "yes")
			{
				//�޸�·��
				if (dstdir == virtualdisk->GetRootDir())
				{
					dirsofsrcdir[srcname]->SetPath(dstdir->GetPath());
				}
				else
				{
					dirsofsrcdir[srcname]->SetPath(dstdir->GetPath() + "/" + dstdir->GetName());
				}
				//����ƶ�����Ŀ¼����Ҫ��Ŀ¼�µ������ļ�����Ŀ¼��·�����޸�
				if (dirsofsrcdir[srcname]->GetFType() == "Dir")
				{
					Dir::ChangeAllFileAndDirPathRecursion(dynamic_cast<Dir*>(dirsofsrcdir[srcname]));
					//��Ŀ¼Ҳ���޸�
					dirsofsrcdir[srcname]->SetFatherDir(dstdir);
				}
				else
				{
					//�ļ�Ҫ�����ֺ�ʱ��
					dirsofsrcdir[srcname]->SetName(dstname);
					dirsofsrcdir[srcname]->SetModifyTime();
				}

				//���
				dstdir->FileOrDirAdd(dstname, dirsofsrcdir[srcname]);
				//ɾ��
				srcdir->RemoveFromMap(srcname);
			}
			else
			{
				cout << "   �����ƶ�   " << endl;
			}
		}
		//�ƶ������ļ�û��ͬ���ļ�
		else
		{

			//��������
			if (srcname != dstname)
			{
				//�޸�ʱ��ı�
				dirsofsrcdir[srcname]->SetModifyTime();
			}
			//�޸�·��
			if (dstdir == virtualdisk->GetRootDir())
			{
				dirsofsrcdir[srcname]->SetPath(dstdir->GetPath());
			}
			else
			{
				dirsofsrcdir[srcname]->SetPath(dstdir->GetPath() + "/" + dstdir->GetName());
			}
			//����
			dirsofsrcdir[srcname]->SetName(dstname);
			//����ƶ�����Ŀ¼����Ҫ��Ŀ¼�µ������ļ�����Ŀ¼��·�����޸�
			if (dirsofsrcdir[srcname]->GetFType() == "Dir")
			{
				Dir::ChangeAllFileAndDirPathRecursion(dynamic_cast<Dir*>(dirsofsrcdir[srcname]));
				//��Ŀ¼Ҳ���޸�
				dirsofsrcdir[srcname]->SetFatherDir(dstdir);
			}
			//���
			dstdir->FileOrDirAdd(dstname, dirsofsrcdir[srcname]);
			//ɾ��
			srcdir->RemoveFromMap(srcname);
		}
	}
}