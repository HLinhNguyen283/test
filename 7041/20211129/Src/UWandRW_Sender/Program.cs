using System;
using System.Threading;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Text.RegularExpressions;

namespace UWandRW_Sender
{
	class Program
	{
		/*!
		 * \brief
		 * アンワインダー送信アプリ(Main処理)
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

            // アンワインダーにXML文書を送信
            string responce = sendToUnwinder(args[1]);

			// 送信結果を指定されたファイルに出力する
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
				Console.Write(responce + "\0");
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
         * アンワインダーにXML文書を送信
         * 
         * \param inSendXML
         * 送信するXML文書
         * 
         * \returns
         * 送信結果
         */
        static private string sendToUnwinder(string inSendXML)
		{
			// 送信先URLを取得
            IniSendToUnwinder ini = new IniSendToUnwinder();
			string url = ini.getURL();

			string responce = "";
			try
			{
				// コマンドライン引数から送信するXML文書を取得する
				//string sendXML = inSendXML.Replace("\"", "");
				//sendXML = Regex.Replace(sendXML, "(?<==).*?(?=\\s|\\?|>)", "\"$0\"");	// 各パラメータの前後に"を付加

                // アンワインダーにXML文書を送信
				WebClient webClient = new WebClient();
                var sendData = Encoding.UTF8.GetBytes(inSendXML);
				var result = webClient.UploadData(url, sendData);

				// 送信の成否＋返信されたXML文書をファイル出力する（呼び出し側に渡す）
				responce = "SUCCESS" + " " + url + " " + Encoding.UTF8.GetString(result);

			}
			catch (Exception exception)
			{
				// 何らかの理由で例外が発生した場合、送信先URLのみファイル出力する（呼び出し側に渡す）
				var ex = null == exception.InnerException ? exception : exception.InnerException;
				responce = "FAILURE" + " " + url + " " + ex.Message;

				// エラー内容をログに出力する
                string errMsg = "[ERROR] sendToUnwinder()StackTrace:" + ex.StackTrace + "\nURL:" + url + "\nErrMessage:" + ex.Message;
				OutputLog.outputLog(errMsg);
			}
			return responce;
		}
	}
}
