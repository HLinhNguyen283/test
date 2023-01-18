using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;

namespace UWandRW_Compose_Xml
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
 
        /*!
         * \brief
         * フォルダ存在確認
         * （存在しない場合は作成する）
         */
        static public void chechFolderNotMake(string path)
        {
            if (!File.Exists(path))
            {
                Directory.CreateDirectory(path);
            }
        }
    }
}
