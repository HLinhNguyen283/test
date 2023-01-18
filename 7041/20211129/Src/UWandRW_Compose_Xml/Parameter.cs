using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UWandRW_Compose_Xml
{

	class Parameter
	{
        private string m_type;	//!< コマンド種別
        public string type
        {
            get { return m_type; }
            private set { m_type = value; }
        }
        private Dictionary<string, string> m_myTable = new Dictionary<string, string>();    //!< パラメータテーブル
        public string GetParameter(string inKey)
        {
            string s;
            try
            {
                s = m_myTable[inKey];
            }
            catch (Exception exception)
            {
                s = "";
            }
            return s;
        }
        /*!
         * \brief
         * コンストラクタ
         */
		public Parameter()
		{
			type = "";
		}

        /*!
         * \brief
         * パラメータ設定処理
         * 
         * \param inString
         * 呼び出し側から受け取った文字列
         */
		public void setParameter(string inString)
		{
            string[] inName = inString.Split(' ');
            foreach (string str in inName)
            {
                if (str == "") continue;
                string key = getSplitParameter(str, "=").First();
                string val = getSplitParameter(str, "=").LastOrDefault();
                string val2 = val.Replace("@@@", " ");    // @@@はブランクに変換する
                m_myTable.Add(key, val2);
            }
            type = GetParameter("Type");
		}

        /*!
         * \brief
         * 指定した文字列でパラメータを分割する
         * 
         * \param inSrcParam
         * 対象パラメータ
         * 
         * \param inTargetString
         * 指定文字列
         * 
         * \returns
         * 分割した文字列リスト
         */
        private List<string> getSplitParameter(string inSrcParam, string inTargetString)
        {
            List<string> param = new List<string>();
            param.AddRange(inSrcParam.Split(inTargetString.ToCharArray()));
            return param;
        }

	}
}
