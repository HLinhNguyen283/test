using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace UWandRW_Parse_Xml
{
	class ParseXML
	{
		/*!
		 * \brief
		 * XML文書パース処理
		 * 
		 * \param inString
		 * 呼び出し側から受け取る文字列
		 * 
		 * \returns
		 * パース後の文字列(<属性名>=<値><スペース><属性名>=<値>...)
		 */
		public string parse(string inString)
		{
			// 受け取った文字列をXMLで読み込む
			XmlDocument document = new XmlDocument();
			document.LoadXml(inString);
            //document.Load(Utility.getModuleDirectoryPath() + FolderName.FILE + @"\TestParm.xml");
            XmlElement rootElement = document.DocumentElement;

            // 名前空間定義
            var nt = document.NameTable;
            var nsmgr = new XmlNamespaceManager(nt);
            nsmgr.AddNamespace("a", "http://www.CIP4.org/JDFSchema_2_0");
            nsmgr.AddNamespace("s", "http://www.screen.co.jp/JDF/JDFSchema_3_0");

            //var nodes = document.SelectSingleNode("//a:XJMF", nsmgr);
            //var nodes1 = document.SelectSingleNode("//a:ResponseStatus", nsmgr);
            //var nodes2 = document.SelectSingleNode("//a:Response", nsmgr);
 
            string parameter = "";
            XmlNode nodeResponseStatus = rootElement.SelectSingleNode("a:ResponseStatus", nsmgr);
            if (null != nodeResponseStatus)
            {
                parameter = parseResponseStatus(nodeResponseStatus, nsmgr);
                return parameter;
            }

            XmlNode nodeResponse = rootElement.SelectSingleNode("a:Response", nsmgr);
            if (null != nodeResponse)
            {
                parameter = parseResponse(nodeResponse, nsmgr);
                return parameter;
            }

            XmlNode subNode = rootElement.SelectSingleNode("a:SignalStatus", nsmgr);
            if (null != subNode)
            {
                parameter = parseSignalStatus(subNode, nsmgr);
                return parameter;
            }

            // 該当なし
            parameter = parseUnknown();
 			return parameter;
		}

        /*!
         * \brief
         * XML文書パース処理（ResponseStatus）
         * 
         * \param inResponseStatus
         * ノードResponseStatus
         * \param inNsmgr
         * 名前空間定義
         * 
         * \returns
         * パース後の文字列
         */
        private string parseResponseStatus(XmlNode inResponseStatus, XmlNamespaceManager inNsmgr)
        {
            // Type
            string param = "Type=ResponseStatus ";

            // ReturnCode
            param += "ReturnCode=";
            param += getAttributeValue(inResponseStatus, "ReturnCode");
            param += " ";

   			// Header
            XmlNode nodeHeader = inResponseStatus.SelectSingleNode("a:Header", inNsmgr);
            if (null != nodeHeader)
            {
                param += "refID=";
                param += getAttributeValue(nodeHeader, "refID");
            }
            return param;
        }

        /*!
          * \brief
          * XML文書パース処理（Response）
          * 
          * \param inResponseStatus
          * ノードResponseStatus
          * \param inNsmgr
          * 名前空間定義
          * 
          * \returns
          * パース後の文字列
          */
        private string parseResponse(XmlNode inResponse, XmlNamespaceManager inNsmgr)
        {
            // Type
            string param = "Type=Response ";

            // ReturnCode
            param += "ReturnCode=";
            param += getAttributeValue(inResponse, "ReturnCode");

            return param;
        }

        /*!
          * \brief
          * XML文書パース処理（SignalStatus）
          * 
          * \param inSignalStatus
          * ノードResponseStatus
          * \param inNsmgr
          * 名前空間定義
          * 
          * \returns
          * パース後の文字列
          */
        private string parseSignalStatus(XmlNode inSignalStatus, XmlNamespaceManager inNsmgr)
        {
            // Type
            string param = "Type=SignalStatus ";

            // DeviceInfo
            XmlNode nodeDeviceInfo = inSignalStatus.SelectSingleNode("a:DeviceInfo", inNsmgr);
            if (null != nodeDeviceInfo)
            {
                param += "Status=";
                param += getAttributeValue(nodeDeviceInfo, "Status");
                param += " ";
            }
            // ResourceInfo/ResourceSet/Resource/Media
            XmlNode nodeMedia = inSignalStatus.SelectSingleNode("a:ResourceInfo/a:ResourceSet/a:Resource/a:Media", inNsmgr);
            if (null != nodeMedia)
            {
                param += "Dimension=";
                param += getAttributeValue(nodeMedia, "Dimension");
                param += " ";

                param += "MediaType=";
                param += getAttributeValue(nodeMedia, "MediaType");
                param += " ";

                param += "RollDiameter=";
                param += getAttributeValue(nodeMedia, "RollDiameter");
                param += " ";

                param += "Thickness=";
                param += getAttributeValue(nodeMedia, "Thickness");
                param += " ";
            }
            return param;
        }

        /*!
          * \brief
          * XML文書パース処理（Unknown）
          * 
          * \returns
          * パース後の文字列
          */
        private string parseUnknown()
        {
            // Type
            string param = "Type=Unknown";
            return param;
        }

        /*!
		 * \brief
		 * 指定属性設定値取得処理
		 * 
		 * \param inElement
		 * 対象エレメント
		 * 
		 * \param inAttributesName
		 * 指定属性名
		 * 
		 * \returns
		 * 指定属性設定値
		 */
		private string getAttributeValue(XmlNode inElement, string inAttributesName)
		{
			XmlNode element = inElement.Attributes[inAttributesName];
			if (null == element) {
				// 指定属性が存在しない場合、空文字を返す
				return "";
			}
			return element.Value;
		}
	}
}
