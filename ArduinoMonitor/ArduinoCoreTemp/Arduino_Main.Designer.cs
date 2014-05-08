namespace ArduinoMonitorModule
{
    partial class Arduino_Main
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Arduino_Main));
            this.cbCom = new System.Windows.Forms.ComboBox();
            this.button1 = new System.Windows.Forms.Button();
            this.tbConsole = new System.Windows.Forms.TextBox();
            this.ACTnotf = new System.Windows.Forms.NotifyIcon(this.components);
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.SuspendLayout();
            // 
            // cbCom
            // 
            this.cbCom.FormattingEnabled = true;
            this.cbCom.Location = new System.Drawing.Point(13, 13);
            this.cbCom.Name = "cbCom";
            this.cbCom.Size = new System.Drawing.Size(121, 21);
            this.cbCom.TabIndex = 0;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(141, 12);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(131, 23);
            this.button1.TabIndex = 1;
            this.button1.Text = "Restart Com";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // tbConsole
            // 
            this.tbConsole.Location = new System.Drawing.Point(12, 48);
            this.tbConsole.Name = "tbConsole";
            this.tbConsole.Size = new System.Drawing.Size(260, 20);
            this.tbConsole.TabIndex = 2;
            // 
            // ACTnotf
            // 
            this.ACTnotf.BalloonTipIcon = System.Windows.Forms.ToolTipIcon.Info;
            this.ACTnotf.Icon = ((System.Drawing.Icon)(resources.GetObject("ACTnotf.Icon")));
            this.ACTnotf.Text = "notifyIcon1";
            this.ACTnotf.Visible = true;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(284, 24);
            this.menuStrip1.TabIndex = 3;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // Arduino_Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 75);
            this.Controls.Add(this.tbConsole);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.cbCom);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.MaximizeBox = false;
            this.Name = "Arduino_Main";
            this.Text = "Arduino Monitor";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox cbCom;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox tbConsole;
        private System.Windows.Forms.NotifyIcon ACTnotf;
        private System.Windows.Forms.MenuStrip menuStrip1;
    }
}

