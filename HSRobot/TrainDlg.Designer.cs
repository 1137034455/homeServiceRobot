namespace HSRobot
{
    partial class TrainDlg
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
            this.components = new System.ComponentModel.Container();
            this.m_imageList = new System.Windows.Forms.ImageList(this.components);
            this.label1 = new System.Windows.Forms.Label();
            this.label_name = new System.Windows.Forms.Label();
            this.button_back = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // m_imageList
            // 
            this.m_imageList.ColorDepth = System.Windows.Forms.ColorDepth.Depth8Bit;
            this.m_imageList.ImageSize = new System.Drawing.Size(16, 16);
            this.m_imageList.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("微软雅黑", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(136, 27);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(145, 25);
            this.label1.TabIndex = 1;
            this.label1.Text = "开始训练好友：";
            // 
            // label_name
            // 
            this.label_name.AutoSize = true;
            this.label_name.Font = new System.Drawing.Font("微软雅黑", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label_name.Location = new System.Drawing.Point(287, 27);
            this.label_name.Name = "label_name";
            this.label_name.Size = new System.Drawing.Size(0, 25);
            this.label_name.TabIndex = 1;
            // 
            // button_back
            // 
            this.button_back.BackColor = System.Drawing.Color.Silver;
            this.button_back.Cursor = System.Windows.Forms.Cursors.Hand;
            this.button_back.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button_back.Font = new System.Drawing.Font("微软雅黑", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button_back.Location = new System.Drawing.Point(14, 9);
            this.button_back.Name = "button_back";
            this.button_back.Size = new System.Drawing.Size(85, 61);
            this.button_back.TabIndex = 0;
            this.button_back.Text = "后退";
            this.button_back.UseVisualStyleBackColor = false;
            this.button_back.SizeChanged += new System.EventHandler(this.窗口大小变化);
            this.button_back.Click += new System.EventHandler(this.button_back_Click);
            // 
            // TrainDlg
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoScroll = true;
            this.BackColor = System.Drawing.SystemColors.ControlDark;
            this.ClientSize = new System.Drawing.Size(641, 346);
            this.Controls.Add(this.label_name);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.button_back);
            this.Name = "TrainDlg";
            this.Text = "TrainDlg";
            this.Load += new System.EventHandler(this.TrainDlg_Load);
            this.Scroll += new System.Windows.Forms.ScrollEventHandler(this.滚动条被拖动时);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button_back;
        public System.Windows.Forms.ImageList m_imageList;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label_name;
    }
}