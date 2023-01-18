using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UWandRW_Parse_Xml
{
	class OutputLog
	{
		/*!
		 * \brief
		 * コンストラクタ
		 */
		static OutputLog()
		{
			m_logFolder = System.String.Empty;
			m_logCount = 0;
		}

		/*!
		 * \brief
		 * ログ出力処理
		 * 
		 * \param inLogMsg
		 * 出力するログ文字列
		 */
		static public void outputLog(string inLogMsg)
		{
			// ログファイル出力先パスを作成する
			if (string.IsNullOrEmpty(m_logFolder))
			{
				m_logFolder = Utility.getModuleDirectoryPath() + "Log\\";
			}
			string logFilePath = m_logFolder + getLogFileName(m_logCount) + ".txt";
			System.IO.StreamWriter writer = null;
			try
			{
				// ログファイルオープン(追記)
				writer = new System.IO.StreamWriter(
					logFilePath,
					true,
					System.Text.Encoding.GetEncoding("Shift_JIS"));

				// 出力文字列にタイムスタンプ(YYYY/MM/DD hh:mm:ss:mm)を付ける
				string msg = getLogTime() + " " + inLogMsg;

				// ログを追記
				Console.SetOut(writer);
				Console.WriteLine(msg);
			}
			catch (Exception)
			{
				// ログファイルアクセスに失敗した場合、別ファイルに出力する
				m_logCount += 1;
				outputLog(inLogMsg);
			}
			finally
			{
				if (null != writer)
				{
					// ログファイルクローズ
					writer.Dispose();
				}
				m_logCount = 0;
			}
		}

		/*!
		 * \brief
		 * ログのタイムスタンプ(YYYY/MM/DD hh:mm:ss:mm)を取得する
		 * 
		 * \returns
		 * ログのタイムスタンプ
		 */
		static private string getLogTime()
		{
			System.DateTime nowTime = System.DateTime.Now;
			string logTime = nowTime.Year.ToString() + "/"
							+ nowTime.Month.ToString() + "/"
							+ nowTime.Day.ToString() + " "
							+ nowTime.Hour.ToString() + ":"
							+ nowTime.Minute.ToString() + ":"
							+ nowTime.Second.ToString() + ":"
							+ nowTime.Millisecond.ToString();
			return logTime;
		}

		/*!
		 * \brief
		 * ログファイル名を取得する
		 * 
		 * \param inLogCount
		 * ログカウント
		 * 
		 * \returns
		 * ログファイル名
		 */
		static private string getLogFileName(int inLogCount)
		{
			// ログファイル名を作成する
			// ParseInkXml_Log_YYYYMMDD
            string fileName = "UWandRW_Parse_Xml_";

			System.DateTime time = System.DateTime.Now;
			string timeString = time.Year.ToString()
								+ time.Month.ToString("00")
								+ time.Day.ToString("00");
			fileName += timeString;

			if (0 < inLogCount)
			{
				// ログカウントが0以上(1回以上ログファイルアクセスに失敗している)場合、末尾に"_<inLogCount>"を追加
				fileName += "_" + inLogCount.ToString();
			}
			return fileName;
		}

		static private string m_logFolder;	// ログフォルダパス(\Client\Log\)
		static private int m_logCount;		// ログカウント(ログファイルにアクセス失敗時に使用する)
	}
}
