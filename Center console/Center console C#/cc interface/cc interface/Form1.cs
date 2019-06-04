using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;


namespace cc_interface
{
    public partial class mainForm : Form
    {
        static SerialPort _serialPort;

        public mainForm()
        {
            InitializeComponent();
            
        }

        public void GetPorts()
        {
 
            lstBoxComports.DataSource = SerialPort.GetPortNames();
            
        }

        private void tabSettings_Click(object sender, EventArgs e)
        {

        }

        private void btComPortsRefresh_Click(object sender, EventArgs e)
        {
            GetPorts();
        }

        private void btComPortsConnect_Click(object sender, EventArgs e)
        {
            if (lstBoxComports.SelectedIndex > -1)
            {
                MessageBox.Show(String.Format("You selected port '{0}'", lstBoxComports.SelectedItem));
                Connect(lstBoxComports.SelectedItem.ToString());
            }
            else
            {
                MessageBox.Show("Please select a port first");
            }

            StartThreadMethod();
        }

        private void Connect(string portName)
        {
            _serialPort = new SerialPort(portName);
            if (!_serialPort.IsOpen)
            {
                _serialPort.BaudRate = 9600;
                _serialPort.Open();
            }
        }

        private void btComPortsTest_Click(object sender, EventArgs e)
        {
            _serialPort.Write("test");
        }

        private void _serialPort_datarecieved()
        {

        }

        private Thread checkThread;

        private void StartThreadMethod()
        {
            checkThread = new Thread(new ThreadStart(CheckSerialPort));
            checkThread.Start();
        }

        public void CheckSerialPort()
        {

            MessageBox.Show("message test");
            if (_serialPort.BytesToRead > 0)
            {
               MessageBox.Show("message incoming");
                recieveSerial();
               
            }
        }

        private void recieveSerial()
        {
            char incoming = ' ';
            char[] message = new char[256];
            while ((char)_serialPort.ReadChar() != '#') ;
            int index = 0;

            while (true)
            {
                incoming = (char)_serialPort.ReadChar();
                //MessageBox.Show(char.ToString(incoming));

                if (incoming == '$')
                {
                    break;
                    
                }
                if(incoming == '#')
                {
                    index = -1;
                }
                else
                {
                    MessageBox.Show(char.ToString( incoming));
                    message[index] = incoming;
                }

                index++;

            }

            MessageBox.Show("end message0");

            int messageCode = 0;
            int indexReading = 0;
            while (true)
            {
                if (message[indexReading] == '%')
                {
                    break;
                }
                int temp = message[indexReading] - '0';
                Console.WriteLine(temp);
                Console.WriteLine(indexReading);
                indexReading++;
                messageCode = messageCode * 10 + temp;
                Console.WriteLine(messageCode);
            }

            Console.WriteLine(messageCode);

            switch (messageCode)
            {
                case 1:
                    Console.WriteLine("Case 1");
                    break;
                case 102:
                    Console.WriteLine("Case 2");
                    break;
                case 103:
                    Console.WriteLine("Case 3");
                    break;
                case 110:
                    Console.WriteLine("Case 4");
                    break;
                case 111:
                    Console.WriteLine("Case 5");
                    break;
                case 131:
                    Console.WriteLine("Case 6");
                    break;
                case 142:
                    Console.WriteLine("Case 7");
                    break;
                case 180:
                    Console.WriteLine("Case 8");
                    break;
                case 200:
                    Console.WriteLine("Case 9");
                    break;
                case 201:
                    Console.WriteLine("Case 10");
                    break;
                default:
                    Console.WriteLine("message has unknown CAN ID");
                    break;
            }
            StartThreadMethod();
        }
       


    }
    
}
