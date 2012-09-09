using System;
using System.Collections.Generic;
using System.Text;
using System.Timers;
using System.IO.Ports;
using GetCoreTempInfoNET;
using System.Windows.Forms;
using System.Diagnostics;

namespace ArduinoCoreTempModule
{
    public partial class Arduino_Main : Form
    {
        private System.Timers.Timer RefreshInfo;
        private CoreTempInfo CTInfo;
        private SerialPort comPort;
        private ulong totalRam = 0;
        //ram use
        private Microsoft.VisualBasic.Devices.ComputerInfo CI = new Microsoft.VisualBasic.Devices.ComputerInfo();
        //ram use
        private ulong ramCounter; 

        public ulong getAvailableRAM(){
            return CI.AvailablePhysicalMemory / 1048576; 
        } 

        private void frmMain_Resize(object sender, EventArgs e)
        {
            if (FormWindowState.Minimized == this.WindowState)
            {
                ACTnotf.Visible = true;
                ACTnotf.ShowBalloonTip(500);
                this.Hide();
            }
            else if (FormWindowState.Normal == this.WindowState)
            {
                ACTnotf.Visible = false;
            }
        }

        public Arduino_Main()
        {
            InitializeComponent();

            ACTnotf.BalloonTipText = "The Module is Still Running Find it Down Here!";
            ACTnotf.BalloonTipTitle = "Arduino CoreTemp Module";
            
            // ui

            // RAM
            
            totalRam = CI.TotalPhysicalMemory / 1048576;
            
            // Initiate CoreTempInfo class.
            CTInfo = new CoreTempInfo();

            // setup callbacks
            CTInfo.ReportError += new ErrorOccured(CTInfo_ReportError);
            ACTnotf.MouseDoubleClick += new MouseEventHandler(ACTnotf_Click);
            this.FormClosing += new FormClosingEventHandler(frm_FormClosing);
            this.Resize += new System.EventHandler(this.frmMain_Resize);

            // get stored com
            String comHold = Properties.Settings.Default.comPort;
            string[] ports = SerialPort.GetPortNames();
            
            // set the CB items
            cbCom.Items.AddRange(ports);

            // create and open com
            comPort = new SerialPort(comHold, 19200);
            try
            {
                comPort.Open();
            }
            catch (Exception expt)
            {
                // not avail main thread will deal with it
            }

            // set the combo box to the prev item
            cbCom.SelectedItem = comHold;

            // setup a timer.
            RefreshInfo = new System.Timers.Timer();
            RefreshInfo.Interval = 600;
            // setup callback
            RefreshInfo.Elapsed += new ElapsedEventHandler(RefreshInfo_Elapsed);

            // Start Main thread.
            RefreshInfo.Start();

        }
        // from the core temp demo code 
        private void RefreshInfo_Elapsed(object sender, ElapsedEventArgs e)
        {
            //Attempt to read shared memory.
            bool bReadSuccess = CTInfo.GetData();

            if (bReadSuccess)
            {
                uint index;
                float maxT = 1.0f;
                int load = 0;
                // get max and avg load for all cpu's + cores
                for (uint i = 0; i < CTInfo.GetCPUCount; i++)
                {
                    for (uint g = 0; g < CTInfo.GetCoreCount; g++)
                    {
                        index = g + (i * CTInfo.GetCoreCount);
                        if (!CTInfo.IsDistanceToTjMax)
                        {
                            if (maxT <= CTInfo.GetTemp[index])
                            {
                                maxT = CTInfo.GetTemp[index];
                            }
                            load += (int)CTInfo.GetCoreLoad[index];
                        }
                    }
                }

                //date string
                string datePatt = @"HH:mm";
                DateTime saveNow = DateTime.Now;
                DateTime myDt;
                myDt = DateTime.SpecifyKind(saveNow, DateTimeKind.Local);
                string dtString;


                // setup output string
                // Core Speed, Core Temp, Max Core Temp, Time, Average Load, 
                // #.#Ghz,#.#,##:##,###

                dtString = myDt.ToString(datePatt);
                String output = Math.Round((CTInfo.GetCPUSpeed / 1000), 1) + "Ghz" + "," + Math.Round(maxT, 1) + "," + dtString + "," + +((int)(load / CTInfo.GetCoreCount));
                //update tooltip icon
                ACTnotf.Text = "Module Running - " + output;
                
                // write com if avail if not notify
                if (comPort.IsOpen)
                {
                    comPort.WriteLine(output);
                    try
                    {
                        this.Invoke((MethodInvoker)delegate
                        { // added ram to display not to string as arduino is not updated to show it yet.
                            tbConsole.Text = output + "," + getAvailableRAM() + "," + totalRam + "," + ((getAvailableRAM() * 100 / totalRam))+"%"; // runs on UI thread 
                        });
                    }
                    catch (Exception excp)
                    {

                    }
                }
                else // no com available
                {
                    try
                    {
                        this.Invoke((MethodInvoker)delegate
                        {
                            tbConsole.Text = "Com port not available"; // runs on UI thread
                        });
                    }
                    catch (Exception excp)
                    {

                    }
                }
                            
            }
            else // coretemp not avail
            {
                this.Invoke((MethodInvoker)delegate
                {
                    tbConsole.Text = CTInfo.GetErrorMessage(CTInfo.GetLastError); // runs on UI thread
                });
                
            }

        }

        //error handler not used
        private void CTInfo_ReportError(ErrorCodes ErrCode, string ErrMsg)
        {
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine(ErrMsg);
            Console.ResetColor();
        }

        // reset com button
        private void button1_Click(object sender, EventArgs e)
        {
            Properties.Settings.Default.comPort = "" + cbCom.SelectedItem;
            Properties.Settings.Default.Save();

            if (comPort.IsOpen)
            {
                comPort.Close();
            }
            comPort = new SerialPort(("" + cbCom.SelectedItem), 19200);
            try
            {
                comPort.Open();
            }
            catch (Exception expt)
            {
                // not avail main thread will deal with it
            }
        }

        // minimise to tray
        private void ACTnotf_Click(object sender, EventArgs e)
        {
            ACTnotf.Visible = false;
            this.Show();
            this.WindowState = FormWindowState.Normal;
        }

        // minimise to tray
        private void frm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (MessageBox.Show("Are you sure you want to close?\nSerial Communications will be closed", "Arduino CoreTemp", MessageBoxButtons.YesNo) == DialogResult.No)
            {
                e.Cancel = true;
            }
            else
            {
                if (comPort.IsOpen)
                {
                    comPort.Close();
                }
                ACTnotf.Visible = false;
            }
        } 
      
    }
}
