using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;

namespace UWandRW_Sender
{
	class Utility
	{
		/*!
		 * \brief
		 * モジュールディレクトリパス取得処理
		 * 
		 * \returns
		 * モジュールディレクトリパス
		 */
		static public string getModuleDirectoryPath()
		{
			string moduleFileName = Assembly.GetExecutingAssembly().Location;
			List<string> splitPath = new List<string>();
			splitPath.AddRange(moduleFileName.Split('\\'));
			splitPath.Remove(splitPath.LastOrDefault());

			string directoryPath = "";
			foreach (string path in splitPath)
			{
				directoryPath += path;
				directoryPath += "\\";
			}
			return directoryPath;
		}
	}
}
