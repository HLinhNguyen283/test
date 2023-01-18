using System;
using System.IO;
using System.Threading;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UWandRW_Compose_Xml
{
    static class FolderName
    {
        public const string LOG = "LOG";
        public const string FILE = "TEMP";
    }
    
    class Program
	{
		/*!
		 * \brief
		 * インク管理用XML合成コマンド(Main処理)
		 * 
		 * \param args
		 * 呼び出し側から受け取るコマンドライン引数
		 */
		static void Main(string[] args)
		{
            //for (int i = 0; i < 100; i++)
            //{
            //    Thread.Sleep(3000);
            //}

            // ファイル出力フォルダの存在を確認する(存在しない場合は作成する)
            Utility.chechFolderNotMake(Utility.getModuleDirectoryPath() + FolderName.FILE);
            // ログ出力フォルダの存在を確認する(存在しない場合は作成する)
            Utility.chechFolderNotMake(Utility.getModuleDirectoryPath() + FolderName.LOG);
            
            // 呼び出し側から受け取ったコマンドライン引数からXML文書を作成する
			string xml = createXML(args);

			// 作成したXML文書を指定されたファイルに出力する
			string filePath = Utility.getModuleDirectoryPath() + "TEMP\\" + args[0];

			System.IO.StreamWriter sw = null;
			try
			{
				sw = new System.IO.StreamWriter(
					filePath,
					true,
					System.Text.Encoding.GetEncoding("Shift_JIS"));

				Console.SetOut(sw); // 出力先（Outプロパティ）を設定

				// 作成したXML文書をファイル出力する（呼び元に渡す）
				Console.Write(xml + "\0");
			}
			catch (Exception exception)
			{
				// ファイルアクセス処理でエラーが発生した場合、ログを出力し処理終了
				var ex = null == exception.InnerException ? exception : exception.InnerException;
				string errMsg = "[ERROR] Main()\nStackTrace:" + ex.StackTrace + "\nfilePath:" + filePath + "\nErrMessage:" + ex.Message;
				OutputLog.outputLog(errMsg);
			}
			finally
			{
				if (null != sw)
				{
					sw.Dispose();
				}
			}
		}

		/*!
		 * \brief
		 * XML文書を作成する
		 * 
		 * \param inString
		 * 呼び出し側から受け取った文字列
		 * 
		 * \returns
		 * XML文書
		 */
		static private string createXML(string[] inString)
		{
			string xml = "";
			try
			{
				// 呼び出し側から受け取るコマンドライン引数からXML作成に必要なパラメータを取り出す
				Parameter param = new Parameter();
				param.setParameter(inString[1]);

				// 送信用XML文書作成
				CreateXML createXML = new CreateXML();
				xml = createXML.create(param);
			}
			catch (Exception exception)
			{
				// 何等かの原因でエラーが発生した場合、ログを出力する
				var ex = null == exception.InnerException ? exception : exception.InnerException;
				string param = System.String.Empty;
				foreach (string str in inString)
				{
					if (str.StartsWith(inString[0]))
					{
						continue;
					}

					if (!string.IsNullOrEmpty(param))
					{
						param += " ";
					}
					param += str;
				}
				string errMsg = "[ERROR] createXML()\nStackTrace:" + ex.StackTrace + "\nParameter:" + param + "\nErrMessage:" + ex.Message;
				OutputLog.outputLog(errMsg);
			}
			return xml;
		}
	}
}
