using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;

namespace WindowsFormsApp7
{
    public partial class Form1 : Form
    {
        DataTable table = new DataTable();

        public Form1()
        {
            InitializeComponent();
            table.Columns.Add("ID", typeof(string));
            table.Columns.Add("습득 장소", typeof(string));
            table.Columns.Add("습득물", typeof(string));
            table.Columns.Add("습득 날짜", typeof(string));
            table.Columns.Add("분류", typeof(string));
            table.Columns.Add("설명", typeof(string));

            dataGridView1.DataSource = table;


        }

        private void Btn_Search_Click(object sender, EventArgs e)
        {
            XmlDocument xml = new XmlDocument();
            string cate = txt_cate.Text;
            string loca = txt_loca.Text;

            string queryurl = "http://apis.data.go.kr/1320000/LosPtfundInfoInqireService/getPtLosfundInfoAccTpNmCstdyPlace?serviceKey=QM218VcrO%2BoL6AOKYNgiEK9fQDAeHaoS3%2F8n8PWz3xVM84Dxv24YMuIAJbmrUFsk2b4HancEzvLVCyNBPsIIyw%3D%3D&pageNo=1&numOfRows=9999";
            queryurl += "&PRDT_NM=";
            queryurl += cate;
            queryurl += "&DEP_PLACE=";
            queryurl += loca;
            xml.Load(queryurl);
            XmlNodeList xnlist = xml.GetElementsByTagName("item");
            foreach(XmlNode xn in xnlist)
            {
                    string ID = xn["atcId"].InnerText;
                    string category = xn["depPlace"].InnerText;
                    string status = xn["fdPrdtNm"].InnerText;
                    string date = xn["fdYmd"].InnerText;
                    string company = xn["prdtClNm"].InnerText;
                    string location = xn["fdSbjt"].InnerText;

                    table.Rows.Add(ID,category,status,date,company,location);
            }
            dataGridView1.AutoResizeColumns(DataGridViewAutoSizeColumnsMode.AllCells);
        }

        private void dataGridView1_CellMouseDoubleClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            string id = dataGridView1.Rows[e.RowIndex].Cells[0].Value.ToString();
            string queryurl = "http://apis.data.go.kr/1320000/LosPtfundInfoInqireService/getPtLosfundDetailInfo?serviceKey=QM218VcrO%2BoL6AOKYNgiEK9fQDAeHaoS3%2F8n8PWz3xVM84Dxv24YMuIAJbmrUFsk2b4HancEzvLVCyNBPsIIyw%3D%3D&pageNo=1&numOfRows=10&FD_SN=1";
            queryurl += "&ATC_ID=";
            queryurl += id;
            XmlDocument xml = new XmlDocument();
            xml.Load(queryurl);
            XmlNodeList xnlist = xml.GetElementsByTagName("item");

            MessageBox.Show("상태 :" + xnlist[0]["csteSteNm"].InnerText+"\n"+
                            "습득장소 :" + xnlist[0]["fdPlace"].InnerText);



        }
    }
}