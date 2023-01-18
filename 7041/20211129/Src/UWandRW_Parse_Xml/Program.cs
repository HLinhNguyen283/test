using System;
using System.IO;
using System.Threading;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace UWandRW_Parse_Xml
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
		 * インク管理用XMLパースコマンド(Main処理)
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

            // 呼び出し側から受け取ったXML文書のパース後の文字列を取得する
			string parameter = parseXML(args[1]);

            // パース後の文字列を指定されたファイルに出力する
			string filePath = Utility.getModuleDirectoryPath() + "TEMP\\" + args[0];
			System.IO.StreamWriter sw = null;
			try
			{
				sw = new System.IO.StreamWriter(
					filePath,
					true,
					System.Text.Encoding.GetEncoding("Shift_JIS"));

				Console.SetOut(sw); // 出力先（Outプロパティ）を設定

				// 取り出したパラメータをファイル出力する（呼び出し側に渡す）
				Console.Write(parameter + "\0");
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
		 * Write brief comment for parseXML here.
		 * 
		 * \param inXML
		 * 呼び出し側から受け取ったXML文書
		 * 
		 * \returns
		 * パース後の文字列
		 */
		static private string parseXML(string inXML)
		{
			string parameter = "";
			try
			{
				// 呼び出し側から受け取るコマンドライン引数(XML文書)からパラメータを取り出す
				//string xml = inXML.Replace("\"", "");
				//xml = Regex.Replace(xml, "(?<==).*?(?=\\s|\\?|>)", "\"$0\"");	// 各パラメータの前後に"を付加
				//xml = xml.Replace("\"\"\"", "\"");

				// XML文書をパースする
				ParseXML parser = new ParseXML();
                parameter = parser.parse(inXML);
			}
			catch (Exception exception)
			{
				// 何等かの原因でエラーが発生した場合、ログを出力する
				var ex = null == exception.InnerException ? exception : exception.InnerException;
                parameter = "[ERROR] parseXML()\nStackTrace:" + ex.StackTrace + "\nXMLString:" + inXML + "\nErrMessage:" + ex.Message;
                OutputLog.outputLog(parameter);
			}
			return parameter;
		}
	}
}
