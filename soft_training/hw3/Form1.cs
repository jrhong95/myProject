using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WinFormExample
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        public enum ControlParseCase
        {
            None,
            Name,
            Year,
            Month,
            Date,
            Gender
        }

        public bool IsDate(string y, string m, string d)
        {
            int year = Convert.ToInt32(y);
            int month = Convert.ToInt32(m);
            int date = Convert.ToInt32(d);

            if (DateTime.Compare(new DateTime(year, month, date), DateTime.Now) > 0)
                return false;

            return year >= 0 && year <= DateTime.Now.Year 
                && month > 0 && month < 13
                && date > 0 && date <= DateTime.DaysInMonth(year, month);
        }

        private string CalculateAge(string y, string m, string d)
        {
            int year = Convert.ToInt32(y);
            int month = Convert.ToInt32(m);
            int date = Convert.ToInt32(d);

            DateTime Birthday = new DateTime(year, month, date);
            DateTime CurrentDate = DateTime.Now;

            int age = CurrentDate.Year - Birthday.Year;
            if (CurrentDate.Month < Birthday.Month) age -= 1;
            else if (CurrentDate.Month == Birthday.Month && CurrentDate.Date < Birthday.Date) age -= 1;

            return Convert.ToString(age);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            lvwPeopleInfo.View = View.Details;

            lvwPeopleInfo.Columns.Add("Name", "Name");
            lvwPeopleInfo.Columns.Add("Age", "Age");
            lvwPeopleInfo.Columns.Add("Gender", "Gender");
            lvwPeopleInfo.Columns.Add("last", "last");

            lvwPeopleInfo.AutoResizeColumns(ColumnHeaderAutoResizeStyle.HeaderSize);
            lvwPeopleInfo.Columns.RemoveByKey("last");

            lvwPeopleInfo.Columns[0].TextAlign = HorizontalAlignment.Left;
            lvwPeopleInfo.Columns["Age"].TextAlign = HorizontalAlignment.Center;
            lvwPeopleInfo.Columns[2].TextAlign = HorizontalAlignment.Right;

        }

        private void ClearControl()
        {
            txtName.Clear();
            cmbGender.SelectedItem = null;
        }

        private ControlParseCase GetControlParseCase()
        {
            if (string.Equals(txtName.Text, string.Empty)) return ControlParseCase.Name;
            else if (string.Equals(txtYear.Text, string.Empty)) return ControlParseCase.Year;
            else if (string.Equals(txtMonth.Text, string.Empty)) return ControlParseCase.Month;
            else if (string.Equals(txtDate.Text, string.Empty)) return ControlParseCase.Date;
            else if (cmbGender.SelectedItem == null) return ControlParseCase.Gender;

            return ControlParseCase.None;
        }

        private Dictionary<string, string> GetControlParseDict()
        {
            return new Dictionary<string, string>()
            {
                {"Name"  , txtName.Text },
                {"Age"   , CalculateAge(txtYear.Text, txtMonth.Text, txtDate.Text) },
                {"Gender", cmbGender.SelectedItem.ToString() }
            };
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            if (GetControlParseCase() != ControlParseCase.None) return;
            if (!IsDate(txtYear.Text, txtMonth.Text, txtDate.Text)) return;

            var lvwItem = new ListViewItem(new string[lvwPeopleInfo.Columns.Count]);

            for (int i = 0; i < lvwPeopleInfo.Columns.Count; i++)
                lvwItem.SubItems[i].Name = lvwPeopleInfo.Columns[i].Name;

            var controlParseDict = GetControlParseDict();

            foreach (string item in controlParseDict.Keys)
                lvwItem.SubItems[item].Text = controlParseDict[item];

            lvwPeopleInfo.Items.Add(lvwItem);
        }

        private void btnDelete_Click(object sender, EventArgs e)
        {
            if (lvwPeopleInfo.FocusedItem == null) return;
            lvwPeopleInfo.FocusedItem.Remove();
        }

        private void btnEdit_Click(object sender, EventArgs e)
        {
            if (GetControlParseCase() != ControlParseCase.None) return;
            if (!IsDate(txtYear.Text, txtMonth.Text, txtDate.Text)) return;

            var controlParseDict = GetControlParseDict();

            foreach(string item in controlParseDict.Keys)
            {
                var lvwItem = lvwPeopleInfo.FocusedItem;
                lvwItem.SubItems[item].Text = controlParseDict[item];
            }
        }

        private void txtAge_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (char.IsDigit(e.KeyChar) || e.KeyChar == Convert.ToChar(Keys.Back)) return;
            e.Handled = true;
        }
    }
}
