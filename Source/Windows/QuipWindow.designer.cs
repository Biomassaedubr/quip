namespace Quip {
  partial class QuipWindow {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose (bool disposing) {
      if (disposing && (components != null)) {
        components.Dispose();
      }
      base.Dispose(disposing);
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent () {
      this.m_statusStrip = new System.Windows.Forms.StatusStrip();
      this.m_modeStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
      this.m_statusStrip.SuspendLayout();
      this.SuspendLayout();
      // 
      // m_statusStrip
      // 
      this.m_statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
      this.m_modeStatusLabel});
      this.m_statusStrip.Location = new System.Drawing.Point(0, 708);
      this.m_statusStrip.Name = "m_statusStrip";
      this.m_statusStrip.Size = new System.Drawing.Size(1008, 22);
      this.m_statusStrip.TabIndex = 1;
      this.m_statusStrip.Text = "statusStrip1";
      // 
      // m_modeStatusLabel
      // 
      this.m_modeStatusLabel.Name = "m_modeStatusLabel";
      this.m_modeStatusLabel.Size = new System.Drawing.Size(47, 17);
      this.m_modeStatusLabel.Text = "Normal";
      // 
      // QuipForm
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(1008, 730);
      this.Controls.Add(this.m_statusStrip);
      this.Name = "QuipForm";
      this.Text = "Quip";
      this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.HandleKeyDown);
      this.m_statusStrip.ResumeLayout(false);
      this.m_statusStrip.PerformLayout();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.StatusStrip m_statusStrip;
    private System.Windows.Forms.ToolStripStatusLabel m_modeStatusLabel;
  }
}

