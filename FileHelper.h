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
	//当前时间转字符 并且格式化
	string static GetNowTimeToString()
	{
		//首先创建一个time_t 类型的变量nowtime
		time_t nowtime=time(NULL);
		//然后创建一个新时间结构体指针 p 
		tm p = *localtime(&nowtime);
		//使用该函数就可得到当前系统时间，使用该函数需要将传入time_t类型变量nowtime的地址值。
		
		/* 由于此时变量nowtime中的系统时间值为日历时间，
		 * 我们需要调用本地时间函数p=localtime（time_t* nowtime）将nowtime变量中
		 * 的日历时间转化为本地时间，存入到指针为p的时间结构体中。不改的话，可以参照注意事项手动改。
		 */
		return  std::to_string(p.tm_year + 1900) + "/" + ChangeTimeFormat(p.tm_mon+1) + "/" + ChangeTimeFormat(p.tm_mday) + "  " + ChangeTimeFormat(p.tm_hour) + ":" + ChangeTimeFormat(p.tm_min);
		

	}
	//支持正反斜杠的路径，将\\反斜杠统一转换为/
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
	//去除前缀空格和后缀以及换行 并返回第一个字符串
	string static RemoveSpace(string& Path)
	{
		string blanks("\f\v\r\t\n ");
		Path.erase(0, Path.find_first_not_of(blanks));
		Path.erase(Path.find_last_not_of(blanks) + 1);
		size_t pos = Path.find(' ');
		return Path.substr(0, pos);
	}

	//将命令按空格分割并去除多余空格
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
	//识别空格
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

	// 分割路径按 / 符号 C:/abc/def/gh     return  abc,def,gh
	vector<string> static GetSplitPathName(string path)
	{
		//先把所有 \\符号换为 /
		string targetpath;
		// 以 / 结尾和不以 /结尾都能识别
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
		//删除包括盘符的路径
		//res.erase(res.begin());
		return res;
	}
	//判断文件或目录是否包含非法字符 \ / : * ? " < > |
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