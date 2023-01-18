using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace UWandRW_Compose_Xml
{
    static class AttributeName
    {
        public const string xmlns = "http://www.CIP4.org/JDFSchema_2_0";
        public const string xmlns_scr = "http://www.screen.co.jp/JDF/JDFSchema_3_0";
    }

    class CreateXML
	{
		/*!
		 * \brief
		 * XML文書作成処理
		 * 
		 * \param inParam
		 * XML文書作成用パラメータ
		 * 
		 * \returns
		 * XML文書
		 */
		public string create(Parameter inParam)
		{
			string xml = "";
			if ("QueryStatus" == inParam.type)
			{
				// QueryStatus
				xml = createQueryStatus(inParam);
			}
			else if ("QueryResource" == inParam.type)
			{
				// QueryResource
				xml = createQueryResource(inParam);
			}
			else if ("StopPersChParams" == inParam.type)
			{
				// StopPersChParams
				xml = createStopPersChParams(inParam);
			}
			else if ("CommandResource" == inParam.type)
			{
				// CommandResource
				xml = CommandResource(inParam);
			}
			else if ("CommandWakeup" == inParam.type)
			{
				// CommandWakeup
				xml = createCommandWakeup(inParam);
			}
			return xml;
		}

        /*!
         * \brief
         * XML文書作成処理（createQueryStatus）
         * 
         * \param inParam
         * XML文書作成用パラメータ
         * 
         * \returns
         * XML文書
         */

        /* ([例]XML文書)
        <XJMF xmlns="http://www.CIP4.org/JDFSchema_2_0">
　          <Header DeviceID="Truepress" ID="QS_20210720_00004" Time="2021-07-20T10:23:58+09:00" />
            <QueryStatus>
　　            <Header DeviceID="Truepress" ID="QS_20210720_00004" Time="2021-07-20T10:23:58+09:00" />
　　            <Subscription RepeatTime="30" URL="http://192.168.0.30:1234/xjmfurl"/>
　          </QueryStatus>
        </XJMF>
        */
        private string createQueryStatus(Parameter inParam)
		{
			XmlDocument document = new XmlDocument();

            XmlElement xjmfElement = document.CreateElement("XJMF");
            xjmfElement.SetAttribute("xmlns", AttributeName.xmlns);
            document.AppendChild(xjmfElement);

            XmlElement headerElement = document.CreateElement("Header");
            headerElement.SetAttribute("DeviceID", inParam.GetParameter("DeviceID"));
            headerElement.SetAttribute("ID", inParam.GetParameter("ID"));
            headerElement.SetAttribute("Time", inParam.GetParameter("Time"));
            xjmfElement.AppendChild(headerElement);

            XmlElement queryStatusElement = document.CreateElement("QueryStatus");
            xjmfElement.AppendChild(queryStatusElement);

            XmlElement headerElement2 = document.CreateElement("Header");
            headerElement2.SetAttribute("DeviceID", inParam.GetParameter("DeviceID"));
            headerElement2.SetAttribute("ID", inParam.GetParameter("ID"));
            headerElement2.SetAttribute("Time", inParam.GetParameter("Time"));
            queryStatusElement.AppendChild(headerElement2);

            XmlElement subscriptionElement = document.CreateElement("Subscription");
            subscriptionElement.SetAttribute("RepeatTime", inParam.GetParameter("RepeatTime"));
            subscriptionElement.SetAttribute("URL", inParam.GetParameter("URL"));
            queryStatusElement.AppendChild(subscriptionElement);
 
            return document.OuterXml;
		}

        /*!
         * \brief
         * XML文書作成処理（createQueryResource）
         * 
         * \param inParam
         * XML文書作成用パラメータ
         * 
         * \returns
         * XML文書
         */

        /* ([例]XML文書)
        <XJMF xmlns="http://www.CIP4.org/JDFSchema_2_0">
　          <Header DeviceID="Truepress" ID="QR_20210720_00004" Time="2021-07-20T10:23:58+09:00" />
            <QueryResource>
　　            <Header DeviceID="Truepress" ID="QR_20210720_00004" Time="2021-07-20T10:23:58+09:00" />
　　            <Subscription RepeatTime="30" ChannelMode="FireAndForget" URL="http://192.168.0.30:1234/xjmfurl"/>
                <ResourceQuParams ResourceDetails="Full" ResourceName="Media" Scope="Present" />
　          </QueryResource>
        </XJMF>
        */
        private string createQueryResource(Parameter inParam)
		{
			XmlDocument document = new XmlDocument();

            XmlElement xjmfElement = document.CreateElement("XJMF");
            xjmfElement.SetAttribute("xmlns", AttributeName.xmlns);
            document.AppendChild(xjmfElement);

            XmlElement headerElement = document.CreateElement("Header");
            headerElement.SetAttribute("DeviceID", inParam.GetParameter("DeviceID"));
            headerElement.SetAttribute("ID", inParam.GetParameter("ID"));
            headerElement.SetAttribute("Time", inParam.GetParameter("Time"));
            xjmfElement.AppendChild(headerElement);

            XmlElement queryResourceElement = document.CreateElement("QueryResource");
            xjmfElement.AppendChild(queryResourceElement);

            XmlElement headerElement2 = document.CreateElement("Header");
            headerElement2.SetAttribute("DeviceID", inParam.GetParameter("DeviceID"));
            headerElement2.SetAttribute("ID", inParam.GetParameter("ID"));
            headerElement2.SetAttribute("Time", inParam.GetParameter("Time"));
            queryResourceElement.AppendChild(headerElement2);

            XmlElement subscriptionElement = document.CreateElement("Subscription");
            subscriptionElement.SetAttribute("RepeatTime", inParam.GetParameter("RepeatTime"));
            subscriptionElement.SetAttribute("ChannelMode", inParam.GetParameter("ChannelMode"));
            subscriptionElement.SetAttribute("URL", inParam.GetParameter("URL"));
            queryResourceElement.AppendChild(subscriptionElement);

            XmlElement resourceQuParamsnElement = document.CreateElement("ResourceQuParams");
            resourceQuParamsnElement.SetAttribute("ResourceDetails", inParam.GetParameter("ResourceDetails"));
            resourceQuParamsnElement.SetAttribute("ResourceName", inParam.GetParameter("ResourceName"));
            resourceQuParamsnElement.SetAttribute("Scope", inParam.GetParameter("Scope"));
            queryResourceElement.AppendChild(resourceQuParamsnElement);

            return document.OuterXml;
		}

        /*!
         * \brief
         * XML文書作成処理（createStopPersChParams）
         * 
         * \param inParam
         * XML文書作成用パラメータ
         * 
         * \returns
         * XML文書
         */

        /* ([例]XML文書)
        <XJMF xmlns="http://www.CIP4.org/JDFSchema_2_0">
            <Header DeviceID="Truepress" ID="ST_20210720_00001" Time="2021-07-20T10:25:07+09:00" />
            <StopPersChParams ChannelID="Q1" MessageType="Media" URL="http://192.168.0.30:1234/xjmfurl" />
        </XJMF>
        */
        private string createStopPersChParams(Parameter inParam)
		{
			XmlDocument document = new XmlDocument();

            XmlElement xjmfElement = document.CreateElement("XJMF");
            xjmfElement.SetAttribute("xmlns", AttributeName.xmlns);
            document.AppendChild(xjmfElement);

            XmlElement headerElement = document.CreateElement("Header");
            headerElement.SetAttribute("DeviceID", inParam.GetParameter("DeviceID"));
            headerElement.SetAttribute("ID", inParam.GetParameter("ID"));
            headerElement.SetAttribute("Time", inParam.GetParameter("Time"));
            xjmfElement.AppendChild(headerElement);

            XmlElement stopPersChParamsElement = document.CreateElement("StopPersChParams");
            stopPersChParamsElement.SetAttribute("ChannelID", inParam.GetParameter("ChannelID"));
            stopPersChParamsElement.SetAttribute("MessageType", inParam.GetParameter("MessageType"));
            stopPersChParamsElement.SetAttribute("URL", inParam.GetParameter("URL"));
            xjmfElement.AppendChild(stopPersChParamsElement);

			return document.OuterXml;
		}

        /*!
         * \brief
         * XML文書作成処理（CommandResource）
         * 
         * \param inParam
         * XML文書作成用パラメータ
         * 
         * \returns
         * XML文書
         */

        /* ([例]XML文書)
        <XJMF xmlns="http://www.CIP4.org/JDFSchema_2_0" xmlns:scr="http://www.screen.co.jp/JDF/JDFSchema_3_0">
            <Header DeviceID="Truepress" ID="CR_20210720_00001" Time="2021-07-20T10:25:46+09:00" />
            <CommandResource>
                <Header DeviceID="Truepress" ID="CR_20210720_00001" Time="2021-07-20T10:25:46+09:00" />
                <ResourceCmdParams UpdateMethod="CompleteSet">
                    <ResourceSet Name="Media">
                        <Resource DescriptiveName="NextIJ #1" ExternalID="ID_1">
                            <Media Dimension="520.00 305.00" MediaType="NextIJ" Thickness="130" scr:UWTension="5" scr:MaxRunSpeed="120" />
                        </Resource>
                    </ResourceSet>
                </ResourceCmdParams>
            </CommandResource>
        </XJMF>
        */
        private string CommandResource(Parameter inParam)
		{
			XmlDocument document = new XmlDocument();

            XmlElement xjmfElement = document.CreateElement("XJMF");
            xjmfElement.SetAttribute("xmlns", AttributeName.xmlns);
            xjmfElement.SetAttribute("xmlns:scr", AttributeName.xmlns_scr);
            document.AppendChild(xjmfElement);

            XmlElement headerElement = document.CreateElement("Header");
            headerElement.SetAttribute("DeviceID", inParam.GetParameter("DeviceID"));
            headerElement.SetAttribute("ID", inParam.GetParameter("ID"));
            headerElement.SetAttribute("Time", inParam.GetParameter("Time"));
            xjmfElement.AppendChild(headerElement);

            XmlElement commandResourceElement = document.CreateElement("CommandResource");
            xjmfElement.AppendChild(commandResourceElement);

            XmlElement headerElement2 = document.CreateElement("Header");
            headerElement2.SetAttribute("DeviceID", inParam.GetParameter("DeviceID"));
            headerElement2.SetAttribute("ID", inParam.GetParameter("ID"));
            headerElement2.SetAttribute("Time", inParam.GetParameter("Time"));
            commandResourceElement.AppendChild(headerElement2);

            XmlElement resourceCmdParamsElement = document.CreateElement("ResourceCmdParams");
            resourceCmdParamsElement.SetAttribute("UpdateMethod", inParam.GetParameter("UpdateMethod"));
            commandResourceElement.AppendChild(resourceCmdParamsElement);

            XmlElement resourceSetElement = document.CreateElement("ResourceSet");
            xjmfElement.AppendChild(resourceSetElement);

            XmlElement resourceElement = document.CreateElement("Resource");
            resourceElement.SetAttribute("DescriptiveName", inParam.GetParameter("DescriptiveName"));
            resourceElement.SetAttribute("ExternalID", inParam.GetParameter("ExternalID"));
            resourceSetElement.AppendChild(resourceElement);

            XmlElement mediaElement = document.CreateElement("Media");
            mediaElement.SetAttribute("Dimension", inParam.GetParameter("Dimension"));
            mediaElement.SetAttribute("MediaType", inParam.GetParameter("MediaType"));
            mediaElement.SetAttribute("Thickness", inParam.GetParameter("Thickness"));
            mediaElement.SetAttribute("scr:UWTension", inParam.GetParameter("UWTension"));
            mediaElement.SetAttribute("scr:MaxRunSpeed", inParam.GetParameter("MaxRunSpeed"));
            resourceElement.AppendChild(mediaElement);

            return document.OuterXml;
		}

        /*!
         * \brief
         * XML文書作成処理（createCommandWakeup）
         * 
         * \param inParam
         * XML文書作成用パラメータ
         * 
         * \returns
         * XML文書
         */

         /* ([例]XML文書)
        <XJMF xmlns="http://www.CIP4.org/JDFSchema_2_0">
            <Header DeviceID="Truepress" ID="CW_20210720_00001" Time="2021-07-20T10:26:11+09:00" />
            <CommandWakeup>
                <Header DeviceID="Truepress" ID="CW_20210720_00001" Time="2021-07-20T10:26:11+09:00" />
            </CommandWakeup>
        </XJMF>
        */
        private string createCommandWakeup(Parameter inParam)
		{
			XmlDocument document = new XmlDocument();

            XmlElement xjmfElement = document.CreateElement("XJMF");
            xjmfElement.SetAttribute("xmlns", AttributeName.xmlns);
            document.AppendChild(xjmfElement);

            XmlElement headerElement = document.CreateElement("Header");
            headerElement.SetAttribute("DeviceID", inParam.GetParameter("DeviceID"));
            headerElement.SetAttribute("ID", inParam.GetParameter("ID"));
            headerElement.SetAttribute("Time", inParam.GetParameter("Time"));
            xjmfElement.AppendChild(headerElement);

            XmlElement commandWakeupElement = document.CreateElement("CommandWakeup");
            xjmfElement.AppendChild(commandWakeupElement);

            XmlElement headerElement2 = document.CreateElement("Header");
            headerElement2.SetAttribute("DeviceID", inParam.GetParameter("DeviceID"));
            headerElement2.SetAttribute("ID", inParam.GetParameter("ID"));
            headerElement2.SetAttribute("Time", inParam.GetParameter("Time"));
            commandWakeupElement.AppendChild(headerElement2);

            return document.OuterXml;
		}
	}
}
