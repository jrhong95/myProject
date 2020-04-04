namespace WinFormExample
{
    partial class Form1
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.backgroundWorker2 = new System.ComponentModel.BackgroundWorker();
            this.backgroundWorker3 = new System.ComponentModel.BackgroundWorker();
            this.lvwPeopleInfo = new System.Windows.Forms.ListView();
            this.txtName = new System.Windows.Forms.TextBox();
            this.lblName = new System.Windows.Forms.Label();
            this.lblGender = new System.Windows.Forms.Label();
            this.btnAdd = new System.Windows.Forms.Button();
            this.btnDelete = new System.Windows.Forms.Button();
            this.btnEdit = new System.Windows.Forms.Button();
            this.cmbGender = new System.Windows.Forms.ComboBox();
            this.lblBirthday = new System.Windows.Forms.Label();
            this.txtYear = new System.Windows.Forms.TextBox();
            this.txtMonth = new System.Windows.Forms.TextBox();
            this.txtDate = new System.Windows.Forms.TextBox();
            this.backgroundWorker4 = new System.ComponentModel.BackgroundWorker();
            this.SuspendLayout();
            // 
            // lvwPeopleInfo
            // 
            this.lvwPeopleInfo.HideSelection = false;
            this.lvwPeopleInfo.Location = new System.Drawing.Point(88, 12);
            this.lvwPeopleInfo.Name = "lvwPeopleInfo";
            this.lvwPeopleInfo.Size = new System.Drawing.Size(272, 162);
            this.lvwPeopleInfo.TabIndex = 0;
            this.lvwPeopleInfo.UseCompatibleStateImageBehavior = false;
            // 
            // txtName
            // 
            this.txtName.Location = new System.Drawing.Point(144, 275);
            this.txtName.Name = "txtName";
            this.txtName.Size = new System.Drawing.Size(75, 21);
            this.txtName.TabIndex = 1;
            // 
            // lblName
            // 
            this.lblName.AutoSize = true;
            this.lblName.Location = new System.Drawing.Point(87, 278);
            this.lblName.Name = "lblName";
            this.lblName.Size = new System.Drawing.Size(51, 12);
            this.lblName.TabIndex = 4;
            this.lblName.Text = "Name : ";
            // 
            // lblGender
            // 
            this.lblGender.AutoSize = true;
            this.lblGender.Location = new System.Drawing.Point(225, 279);
            this.lblGender.Name = "lblGender";
            this.lblGender.Size = new System.Drawing.Size(54, 12);
            this.lblGender.TabIndex = 6;
            this.lblGender.Text = "Gender :";
            // 
            // btnAdd
            // 
            this.btnAdd.Location = new System.Drawing.Point(89, 337);
            this.btnAdd.Name = "btnAdd";
            this.btnAdd.Size = new System.Drawing.Size(85, 23);
            this.btnAdd.TabIndex = 8;
            this.btnAdd.Text = "Add";
            this.btnAdd.UseVisualStyleBackColor = true;
            this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
            // 
            // btnDelete
            // 
            this.btnDelete.Location = new System.Drawing.Point(180, 337);
            this.btnDelete.Name = "btnDelete";
            this.btnDelete.Size = new System.Drawing.Size(85, 23);
            this.btnDelete.TabIndex = 9;
            this.btnDelete.Text = "Delete";
            this.btnDelete.UseVisualStyleBackColor = true;
            this.btnDelete.Click += new System.EventHandler(this.btnDelete_Click);
            // 
            // btnEdit
            // 
            this.btnEdit.Location = new System.Drawing.Point(275, 337);
            this.btnEdit.Name = "btnEdit";
            this.btnEdit.Size = new System.Drawing.Size(85, 23);
            this.btnEdit.TabIndex = 10;
            this.btnEdit.Text = "Edit";
            this.btnEdit.UseVisualStyleBackColor = true;
            this.btnEdit.Click += new System.EventHandler(this.btnEdit_Click);
            // 
            // cmbGender
            // 
            this.cmbGender.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbGender.FormattingEnabled = true;
            this.cmbGender.Items.AddRange(new object[] {
            "Male",
            "Female"});
            this.cmbGender.Location = new System.Drawing.Point(285, 276);
            this.cmbGender.Name = "cmbGender";
            this.cmbGender.Size = new System.Drawing.Size(75, 20);
            this.cmbGender.TabIndex = 11;
            // 
            // lblBirthday
            // 
            this.lblBirthday.AutoSize = true;
            this.lblBirthday.Location = new System.Drawing.Point(115, 218);
            this.lblBirthday.Name = "lblBirthday";
            this.lblBirthday.Size = new System.Drawing.Size(59, 12);
            this.lblBirthday.TabIndex = 12;
            this.lblBirthday.Text = "Birthday :";
            // 
            // txtYear
            // 
            this.txtYear.Location = new System.Drawing.Point(180, 215);
            this.txtYear.Name = "txtYear";
            this.txtYear.Size = new System.Drawing.Size(66, 21);
            this.txtYear.TabIndex = 13;
            this.txtYear.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // txtMonth
            // 
            this.txtMonth.Location = new System.Drawing.Point(255, 215);
            this.txtMonth.Name = "txtMonth";
            this.txtMonth.Size = new System.Drawing.Size(38, 21);
            this.txtMonth.TabIndex = 14;
            this.txtMonth.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // txtDate
            // 
            this.txtDate.Location = new System.Drawing.Point(299, 215);
            this.txtDate.Name = "txtDate";
            this.txtDate.Size = new System.Drawing.Size(38, 21);
            this.txtDate.TabIndex = 15;
            this.txtDate.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(449, 406);
            this.Controls.Add(this.txtDate);
            this.Controls.Add(this.txtMonth);
            this.Controls.Add(this.txtYear);
            this.Controls.Add(this.lblBirthday);
            this.Controls.Add(this.cmbGender);
            this.Controls.Add(this.btnEdit);
            this.Controls.Add(this.btnDelete);
            this.Controls.Add(this.btnAdd);
            this.Controls.Add(this.lblGender);
            this.Controls.Add(this.lblName);
            this.Controls.Add(this.txtName);
            this.Controls.Add(this.lvwPeopleInfo);
            this.Name = "Form1";
            this.Text = "hw3_2015726096";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.ComponentModel.BackgroundWorker backgroundWorker1;
        private System.ComponentModel.BackgroundWorker backgroundWorker2;
        private System.ComponentModel.BackgroundWorker backgroundWorker3;
        private System.Windows.Forms.ListView lvwPeopleInfo;
        private System.Windows.Forms.TextBox txtName;
        private System.Windows.Forms.Label lblName;
        private System.Windows.Forms.Label lblGender;
        private System.Windows.Forms.Button btnAdd;
        private System.Windows.Forms.Button btnDelete;
        private System.Windows.Forms.Button btnEdit;
        private System.Windows.Forms.ComboBox cmbGender;
        private System.Windows.Forms.Label lblBirthday;
        private System.Windows.Forms.TextBox txtYear;
        private System.Windows.Forms.TextBox txtMonth;
        private System.Windows.Forms.TextBox txtDate;
        private System.ComponentModel.BackgroundWorker backgroundWorker4;
    }
}

