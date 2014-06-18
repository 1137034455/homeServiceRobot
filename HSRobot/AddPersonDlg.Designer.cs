namespace HSRobot
{
    partial class AddPersonDlg
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.textBox_personName = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.button_ok = new System.Windows.Forms.Button();
            this.button_cancle = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // textBox_personName
            // 
            this.textBox_personName.Location = new System.Drawing.Point(14, 55);
            this.textBox_personName.Name = "textBox_personName";
            this.textBox_personName.Size = new System.Drawing.Size(208, 21);
            this.textBox_personName.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 26);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(161, 12);
            this.label1.TabIndex = 1;
            this.label1.Text = "请输入要添加的好友的姓名：";
            // 
            // button_ok
            // 
            this.button_ok.Location = new System.Drawing.Point(179, 152);
            this.button_ok.Name = "button_ok";
            this.button_ok.Size = new System.Drawing.Size(74, 30);
            this.button_ok.TabIndex = 2;
            this.button_ok.Text = "确定";
            this.button_ok.UseVisualStyleBackColor = true;
            this.button_ok.Click += new System.EventHandler(this.button1_Click);
            // 
            // button_cancle
            // 
            this.button_cancle.Location = new System.Drawing.Point(259, 152);
            this.button_cancle.Name = "button_cancle";
            this.button_cancle.Size = new System.Drawing.Size(74, 30);
            this.button_cancle.TabIndex = 2;
            this.button_cancle.Text = "取消";
            this.button_cancle.UseVisualStyleBackColor = true;
            this.button_cancle.Click += new System.EventHandler(this.button_Cancle_Click);
            // 
            // AddPersonDlg
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(345, 194);
            this.ControlBox = false;
            this.Controls.Add(this.button_cancle);
            this.Controls.Add(this.button_ok);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBox_personName);
            this.MaximumSize = new System.Drawing.Size(361, 232);
            this.MinimumSize = new System.Drawing.Size(361, 232);
            this.Name = "AddPersonDlg";
            this.Opacity = 0.7D;
            this.Text = "添加好友";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBox_personName;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button button_ok;
        private System.Windows.Forms.Button button_cancle;
    }
}