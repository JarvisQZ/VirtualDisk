#pragma once
#include"head.h"

class FileHelper
{
public:
	string static ChangeTimeFormat(int time)
	{
		if (time < 10)
		{
			return "0" + std::to_string(time);
		}
		else
		{
			return std::to_string(time);
		}

	}
	//��ǰʱ��ת�ַ� ���Ҹ�ʽ��
	string static GetNowTimeToString()
	{
		//���ȴ���һ��time_t ���͵ı���nowtime
		time_t nowtime=time(NULL);
		//Ȼ�󴴽�һ����ʱ��ṹ��ָ�� p 
		tm p = *localtime(&nowtime);
		//ʹ�øú����Ϳɵõ���ǰϵͳʱ�䣬ʹ�øú�����Ҫ������time_t���ͱ���nowtime�ĵ�ֵַ��
		
		/* ���ڴ�ʱ����nowtime�е�ϵͳʱ��ֵΪ����ʱ�䣬
		 * ������Ҫ���ñ���ʱ�亯��p=localtime��time_t* nowtime����nowtime������
		 * ������ʱ��ת��Ϊ����ʱ�䣬���뵽ָ��Ϊp��ʱ��ṹ���С����ĵĻ������Բ���ע�������ֶ��ġ�
		 */
		return  std::to_string(p.tm_year + 1900) + "/" + ChangeTimeFormat(p.tm_mon+1) + "/" + ChangeTimeFormat(p.tm_mday) + "  " + ChangeTimeFormat(p.tm_hour) + ":" + ChangeTimeFormat(p.tm_min);
		

	}
	//֧������б�ܵ�·������\\��б��ͳһת��Ϊ/
	string static ExchangePath(string Path)
	{
		string res = Path;
		for (size_t i = 0; i < res.size(); i++)
		{
			if (res[i] == '\\')
			{
				res[i] = '/';
			}
		}
		return res;
	}
	//ȥ��ǰ׺�ո�ͺ�׺�Լ����� �����ص�һ���ַ���
	string static RemoveSpace(string& Path)
	{
		string blanks("\f\v\r\t\n ");
		Path.erase(0, Path.find_first_not_of(blanks));
		Path.erase(Path.find_last_not_of(blanks) + 1);
		size_t pos = Path.find(' ');
		return Path.substr(0, pos);
	}

	//������ո�ָȥ������ո�
	/*vector<string> static GetCommandPara(string CommandOrder)
	{
		int pos;
		vector<string> res;
		while (1)
		{

			string blanks("\f\v\r\t\n ");
			CommandOrder.erase(0, CommandOrder.find_first_not_of(blanks));
			CommandOrder.erase(CommandOrder.find_last_not_of(blanks) + 1);
			//cout << CommandOrder << endl;
			pos = CommandOrder.find(' ');
			//cout << CommandOrder.substr(0, pos) << "-s" << endl;
			res.emplace_back(CommandOrder.substr(0, pos));
			if (pos == string::npos)
				break;
			CommandOrder = CommandOrder.substr(pos, CommandOrder.length());
		}
		return res;
	}*/
	//ʶ��ո�
	vector<string> static GetCommandPara(string CommandOrder)
	{
		string TempCommandOrder = ExchangePath(CommandOrder);
		size_t pos;
		vector<string> res;
		while (1)
		{

			string blanks("\f\v\r\t\n ");
			TempCommandOrder.erase(0, TempCommandOrder.find_first_not_of(blanks));
			TempCommandOrder.erase(TempCommandOrder.find_last_not_of(blanks) + 1);
			//cout << "--" << CommandOrder << "--" << endl;
			if (TempCommandOrder == "")
				break;
			if (TempCommandOrder[0] == '"')
			{
				pos = TempCommandOrder.find('"', 2);
				if (pos != string::npos)
				{
					res.push_back(TempCommandOrder.substr(1, pos - 1));
					TempCommandOrder = TempCommandOrder.substr(pos + 1, TempCommandOrder.length());
				}
			}
			else
			{
				pos = TempCommandOrder.find(' ');
				//cout << TempCommandOrder.substr(0, pos) << "-s" << endl;
				res.push_back(TempCommandOrder.substr(0, pos));
				if (pos == string::npos)
					break;
				TempCommandOrder = TempCommandOrder.substr(pos, TempCommandOrder.length());
			}
		}
		return res;
	}

	// �ָ�·���� / ���� C:/abc/def/gh     return  abc,def,gh
	vector<string> static GetSplitPathName(string path)
	{
		//�Ȱ����� \\���Ż�Ϊ /
		string targetpath;
		// �� / ��β�Ͳ��� /��β����ʶ��
		if (path[path.length() - 1] == '/')
			targetpath = path.substr(0, path.length() - 1);
		else
			targetpath = path;

		RemoveSpace(targetpath);
		vector<string> res;
		size_t pos;
		while (1)
		{
			pos = targetpath.find('/');
			res.emplace_back(targetpath.substr(0, pos));
			if (pos == string::npos)
				break;
			targetpath = targetpath.substr(pos + 1, targetpath.length());
		}
		//ɾ�������̷���·��
		//res.erase(res.begin());
		return res;
	}
	//�ж��ļ���Ŀ¼�Ƿ�����Ƿ��ַ� \ / : * ? " < > |
	bool static IsContainIllegalChar(string name)
	{
		if (name.find('\\') == string::npos && name.find('/') == string::npos && name.find(':') == string::npos && name.find('*') == string::npos && name.find('?') == string::npos && name.find('"') == string::npos && name.find('<') == string::npos && name.find('>') == string::npos && name.find('|') == string::npos)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

};