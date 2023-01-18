using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace UWandRW_Sender
{
	/*!
	 * \brief
	 * SendToIMS.ini参照
	 */
	class IniSendToUnwinder
	{
		/*!
		 * \brief
		 * コンストラクタ
		 */
        public IniSendToUnwinder()
		{
            m_filePath = Utility.getModuleDirectoryPath() + "Preferences\\TP-UW_Communication.ini";
		}

		/*!
		 * \brief
		 * 送信先URL取得処理
		 * 
		 * \returns
		 * 送信先URL
		 */
		public string getURL()
		{
            string uwIpaddress = getValueString("CONNECTION", "UW_IPADDRESS", "", m_filePath);
            string uwPort = getValueString("CONNECTION", "SENDER_PORT", "", m_filePath);
            string url = "http://" + uwIpaddress + ":" + uwPort;
            return url;
        }

		/*!
		 * \brief
		 * 指定セクション、キーの設定文字列取得処理
		 * 
		 * \param inSection
		 * セクション名
		 * 
		 * \param inKey
		 * キー名
		 * 
		 * \param inDefault
		 * デフォルト値
		 * 
		 * \param inFilePath
		 * 対象iniファイルパス
		 * 
		 * \returns
		 * 設定文字列
		 */
		private string getValueString(
			string inSection,
			string inKey,
			string inDefault,
			string inFilePath)
		{
			StringBuilder strBuilder = new StringBuilder(1024);
			GetPrivateProfileString(inSection, inKey, inDefault, strBuilder, strBuilder.Capacity, inFilePath);
			return strBuilder.ToString();
		}

		// Win32APIのGetPrivateProfileString()
		[DllImport("kernel32.dll")]
		private static extern int GetPrivateProfileString(
			string inSection,
			string inKey,
			string inDefault,
			StringBuilder outString,
			int inStringSize,
			string inFilePath);

        private string m_filePath;	// TP-UW_Communication.iniパス
	}
}
