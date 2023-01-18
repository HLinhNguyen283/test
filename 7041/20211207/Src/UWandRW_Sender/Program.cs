using System;
using System.IO;
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
            OutputLog.outputLog("Start UWandRW_Sender.exe");

            // アンワインダーにXML文書を送信
            string response = sendToUnwinder(args[1]);

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
				Console.Write(response + "\0");
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
            OutputLog.outputLog("End UWandRW_Sender.exe");
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

			string response = "";
			try
			{
				// コマンドライン引数から送信するXML文書を取得する
				//string sendXML = inSendXML.Replace("\"", "");
				//sendXML = Regex.Replace(sendXML, "(?<==).*?(?=\\s|\\?|>)", "\"$0\"");	// 各パラメータの前後に"を付加

                // アンワインダーにXML文書を送信
				WebClient webClient = new WebClient();
                var sendData = Encoding.UTF8.GetBytes(inSendXML);

                // 送信データをログに出力する
                writeSendXmlLog(Encoding.UTF8.GetString(sendData));
                OutputLog.outputLog("Send Xml Data");

                var result = webClient.UploadData(url, sendData);

                // レスポンスデータをログに出力する
                writeResponseXmlLog(Encoding.UTF8.GetString(result));

				// 送信の成否＋返信されたXML文書をファイル出力する（呼び出し側に渡す）
				response = "SUCCESS" + " " + url + " " + Encoding.UTF8.GetString(result);

			}
			catch (Exception exception)
			{
				// 何らかの理由で例外が発生した場合、送信先URLのみファイル出力する（呼び出し側に渡す）
				var ex = null == exception.InnerException ? exception : exception.InnerException;
				response = "FAILURE" + " " + url + " " + ex.Message;

				// エラー内容をログに出力する
                string errMsg = "[ERROR] sendToUnwinder()StackTrace:" + ex.StackTrace + "\nURL:" + url + "\nErrMessage:" + ex.Message;
				OutputLog.outputLog(errMsg);
			}
			return response;
		}

        /*!
        * \brief
        * UWへ送信するXMLデータをログに出力する
        * 
        * \param inXmlData
        * UWへ送信するXMLデータ
        * 
        */
        static private void writeSendXmlLog(string inXmlData)
        {
            DateTime t = DateTime.Now;
            string fileName = "Send_" + t.ToString("yyyyMMddhhmmss_fff") + ".xml";
            writeXmlLog(fileName, inXmlData);
        }

        /*!
        * \brief
        * UWからのレスポンスXMLデータをログに出力する
        * 
        * \param inXmlData
        * UWからのレスポンスXML
        * 
        */
        static private void writeResponseXmlLog(string inXmlData)
        {
            DateTime t = DateTime.Now;
            t.ToString("yyyyMMddhhmmss_fff");
            string fileName = "Response_" + t.ToString("yyyyMMddhhmmss_fff") + ".xml";
            writeXmlLog(fileName, inXmlData);
        }

        /*!
         * \brief
         * 送受信のXMLデータをログに出力する
         * 
         * \param inFileName
         * ログファイル名
         * 
         * \param inXmlData
         * ログに出力するXMLデータ
         * 
         */
        static private void writeXmlLog(string inFileName, string inXmlData)
        {
            // 送信結果を指定されたファイルに出力する
            DateTime t = DateTime.Now;
            string folderPath = Utility.getModuleDirectoryPath() + @"LOG\UW_CONNECT\" + t.ToString("yyyyMMdd") + @".xml";
            if (!File.Exists(folderPath))
            {
                Directory.CreateDirectory(folderPath);
            }
            string filePath = folderPath + @"\" + inFileName;
            System.IO.StreamWriter sw = null;
            try
            {
                sw = new System.IO.StreamWriter(
                    filePath,
                    true,
                    System.Text.Encoding.GetEncoding("Shift_JIS"));

                Console.SetOut(sw); // 出力先（Outプロパティ）を設定

                // ログファイルに出力する
                Console.Write(inXmlData + "\0");
            }
            catch (Exception)
            {   // 無視
            }
            finally
            {
                if (null != sw)
                {
                    sw.Dispose();
                }
            }
        }
    }
}
