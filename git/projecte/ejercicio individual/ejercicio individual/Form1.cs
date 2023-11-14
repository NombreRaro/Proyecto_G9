using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Threading;

namespace ejercicio_individual
{
    public partial class Form1 : Form
    {
        Socket server;
        Thread atender;
        int codigo;
        string nombre, mssg, altura;
        public Form1()
        {
            
            InitializeComponent();
            button1.BackColor = Color.Red;
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            IPAddress newaddress = IPAddress.Parse("192.168.145.138");
            server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            IPEndPoint remoteEP = new IPEndPoint(newaddress, 9050);
            try
            {
                server.Connect(remoteEP);
                MessageBox.Show("Conectado");
            }
            catch (SocketException ex)
            {
                MessageBox.Show("Conectado");
                Console.WriteLine("No se ha podido conectar al servidor");
                Console.WriteLine(ex);
                return;
            }
            button1.BackColor = Color.Green;
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            this.codigo = 1;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            button1.BackColor = Color.Red;
            server.Shutdown(SocketShutdown.Both);
            server.Close();
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            this.codigo = 2;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.nombre = textBox1.Text;
            this.altura = textBox2.Text;
            if (textBox2.Text==null)
            {
                this.altura = "";
            }

            if (codigo == 1)
            {
                this.mssg = "1/" + this.nombre + "/" + this.altura;
                byte[] m = System.Text.Encoding.ASCII.GetBytes(mssg);
                server.Send(m);
                //Rebem la resposta del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mssg = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                MessageBox.Show("La longitud del nombre es: "+this.mssg);
            }
            else
            {
                this.mssg = "2/" + this.nombre + "/" + this.altura;
                byte[] mensaje = System.Text.Encoding.ASCII.GetBytes(mssg);
                server.Send(mensaje);
                //Rebem la resposta del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mssg = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                if (mssg == "SI")
                {
                    MessageBox.Show("Tu nombre es bonito");
                }
                else
                {
                    MessageBox.Show("Tu nombre no es bonito");
                }
            }

            textBox1.Text = "";
            textBox2.Text = "";
        }
    }
}
